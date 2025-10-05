#include "gamescene.hpp"

#include <memory>
#include <random>
#include <algorithm>

#include "../map_utils.hpp"

GameScene::GameScene(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input)
                : Scene(nc, stdn, rows, cols, input) 
{
    level.resize(rows, std::wstring(mapWidth, L'.'));

    GenerateAutoDungeon(5);
    InitEntitys();
}

void GameScene::InitEntitys()
{
    // Очищаємо список монстрів перед (повторним) розміщенням
    monsters.clear();
    // Розміщуємо гравця у центрі першої згенерованої кімнати (якщо вона є)
    if (!roomCenters.empty()) {
        auto c = roomCenters.front();
        player = std::make_shared<Player>(c.first, c.second); // center.x, center.y
    } else {
        // якщо кімнат не знайдено — ставимо в центр екрану
        player = std::make_shared<Player>(cols / 2, rows / 2);
    }

    // Налаштовуємо генератор випадкових чисел для розміщення монстрів
    std::mt19937 rng((unsigned)time(nullptr));
    // pickRoom вибирає індекс кімнати з масиву roomCenters
    std::uniform_int_distribution<int> pickRoom(0, std::max(0, (int)roomCenters.size()-1));

    int monstersToPlace = 5; // бажана кількість монстрів
    int attempts = 0; // лічильник спроб (щоб не застрягнути у циклі)
    while ((int)monsters.size() < monstersToPlace && attempts < 200) 
    {
        ++attempts;
        if (roomCenters.empty()) 
            break; // немає кімнат — нічого не робимо
        int ri = pickRoom(rng); // вибираємо випадкову кімнату
        int cx = roomCenters[ri].first; // центр вибраної кімнати (x)
        int cy = roomCenters[ri].second; // центр (y)
        // беремо невеликий випадковий офсет від центру, щоб монстри не були точно в центрі
        int ox = (rng()%5) - 2; // -2..+2
        int oy = (rng()%3) - 1; // -1..+1
        int mx = cx + ox;
        int my = cy + oy;
        // перевірка меж і того, що клітинка прохідна ('.') перед розміщенням монстра
        if (my >= 0 && my < (int)rows && mx >= 0 && mx < (int)mapWidth && level[my][mx] == L'.') 
        {
            monsters.push_back(std::make_shared<Monster>(mx, my));
        }
    }
}


// -----------------------------------------------------------------------------
// Метод генерує випадковий рівень (підземелля) з кімнатами та коридорами.
// Алгоритм:
//   1) Очищаємо карту (заповнюємо пробілами)
//   2) Випадково створюємо кімнати (з перевіркою на перекриття)
//   3) Малюємо кімнати на карті
//   4) Створюємо граф зв’язків (відстані між кімнатами)
//   5) Будуємо MST (мінімальне остовне дерево) за Крускалом
//   6) Прокладаємо коридори між кімнатами
//   7) Додаємо кілька випадкових "додаткових" коридорів для циклів
//   8) Малюємо зовнішню рамку карти
//   9) Ставимо сходи (вхід/вихід)
//   10) Додаємо дрібний декор (точки, коми)
// -----------------------------------------------------------------------------
void GameScene::GenerateAutoDungeon(int roomCount)
{
    // -------------------------------
    // 1) Початкова очистка карти
    // -------------------------------
    for (int y = 0; y < (int)rows; ++y) {
        for (int x = 0; x < (int)mapWidth; ++x) {
            level[y][x] = L' '; // порожній простір
        }
    }

    // -------------------------------
    // 2) RNG і розподіли для кімнат
    // -------------------------------
    std::mt19937 rng((unsigned)time(nullptr)); // генератор випадкових чисел
    std::uniform_int_distribution<int> rw(5, 20); // ширина кімнат
    std::uniform_int_distribution<int> rh(4, 8);  // висота кімнат
    std::uniform_int_distribution<int> rx(2, std::max(2, (int)mapWidth - 14)); // координата X
    std::uniform_int_distribution<int> ry(2, std::max(2, (int)rows - 10));     // координата Y

    // Структура кімнати
    struct Room {
        int top, left; // верхній лівий кут
        int h, w;      // висота і ширина
        int cx, cy;    // центр кімнати
    };

    std::vector<Room> rooms;

    // -------------------------------
    // 3) Створення кімнат
    // -------------------------------
    int attempts = 0;
    while ((int)rooms.size() < roomCount && attempts < roomCount * 8) {
        ++attempts;

        // випадкові параметри кімнати
        int w   = rw(rng);
        int h   = rh(rng);
        int left= rx(rng);
        int top = ry(rng);

        // якщо кімната виходить за межі карти → пропускаємо
        if (left + w + 2 >= (int)mapWidth || top + h + 2 >= (int)rows)
            continue;

        // перевірка на перекриття з існуючими кімнатами (додаємо зазор 2)
        bool ok = true;
        for (auto &r : rooms) {
            bool noOverlap = (left + w + 2 < r.left || r.left + r.w + 2 < left ||
                              top + h + 2 < r.top  || r.top  + r.h + 2 < top);
            if (!noOverlap) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        // створюємо кімнату та додаємо до списку
        Room rr { top, left, h, w, left + w / 2, top + h / 2 };
        rooms.push_back(rr);
    }

    // -------------------------------
    // 4) Малювання кімнат
    // -------------------------------
    roomCenters.clear(); // очищаємо список центрів кімнат
    for (auto &r : rooms) {
        // генеруємо "шаблон" кімнати (стелі + підлога)
        auto tpl = MapUtils::MakeRectRoom(r.h, r.w, L'░', L'.', false);

        // прикрашаємо кути Unicode-рамкою
        tpl[0][0]         = L'┏'; tpl[0][r.w-1]     = L'┓';
        tpl[r.h-1][0]     = L'┗'; tpl[r.h-1][r.w-1] = L'┛';

        // верхня/нижня стінка
        for (int x=1; x<r.w-1; ++x) {
            tpl[0][x] = L'━'; 
            tpl[r.h-1][x] = L'━';
        }

        // ліва/права стінка
        for (int y=1; y<r.h-1; ++y) {
            tpl[y][0] = L'┃'; 
            tpl[y][r.w-1] = L'┃';
        }

        // вставляємо кімнату в основну карту
        MapUtils::InsertTemplate(level, rows, mapWidth, tpl, r.top, r.left);

        // зберігаємо центр кімнати
        roomCenters.emplace_back(r.cx, r.cy);
    }

    // -------------------------------
    // 5) Створюємо граф кімнат (ребра між центрами)
    // -------------------------------
    struct Edge { int a, b; double d; };
    std::vector<Edge> edges;

    for (int i=0; i<(int)rooms.size(); ++i) {
        for (int j=i+1; j<(int)rooms.size(); ++j) {
            int dx = rooms[i].cx - rooms[j].cx;
            int dy = rooms[i].cy - rooms[j].cy;
            double dist = std::sqrt((double)dx*dx + (double)dy*dy);
            edges.push_back({i, j, dist});
        }
    }

    // сортуємо ребра для Крускала
    std::sort(edges.begin(), edges.end(),
              [](const Edge &a, const Edge &b){ return a.d < b.d; });

    // -------------------------------
    // 6) Union-Find для MST
    // -------------------------------
    std::vector<int> parent(rooms.size());
    for (int i=0; i<(int)rooms.size(); ++i) parent[i] = i;

    std::function<int(int)> findp = [&](int x) {
        return (parent[x] == x) ? x : parent[x] = findp(parent[x]);
    };

    // -------------------------------
    // 7) Хелпери для коридорів
    // -------------------------------
    auto hcorr = [&](int y, int x1, int x2) {
        if (x1 > x2) std::swap(x1, x2);
        for (int x=x1; x<=x2; ++x)
            if (y>0 && y<(int)rows && x>0 && x<(int)mapWidth)
                level[y][x] = L'.';
    };

    auto vcorr = [&](int x, int y1, int y2) {
        if (y1 > y2) std::swap(y1, y2);
        for (int y=y1; y<=y2; ++y)
            if (y>0 && y<(int)rows && x>0 && x<(int)mapWidth)
                level[y][x] = L'.';
    };

    // -------------------------------
    // 8) Будуємо MST і прокладаємо коридори
    // -------------------------------
    for (auto &e : edges) {
        int pa = findp(e.a);
        int pb = findp(e.b);

        if (pa != pb) {
            parent[pa] = pb; // об’єднуємо кімнати в одну компоненту

            int x1 = rooms[e.a].cx, y1 = rooms[e.a].cy;
            int x2 = rooms[e.b].cx, y2 = rooms[e.b].cy;

            // робимо коридор L-подібної форми
            if (rng()%2) {
                hcorr(y1, x1, x2);
                vcorr(x2, y1, y2);
            } else {
                vcorr(x1, y1, y2);
                hcorr(y2, x1, x2);
            }
        }
    }

    // -------------------------------
    // 9) Додаємо кілька додаткових коридорів (цикли)
    // -------------------------------
    for (int k=0; k<(int)rooms.size()/3; ++k) {
        int i = rng()%rooms.size();
        int j = rng()%rooms.size();
        if (i == j) continue;

        int x1 = rooms[i].cx, y1 = rooms[i].cy;
        int x2 = rooms[j].cx, y2 = rooms[j].cy;

        hcorr((y1+y2)/2, std::min(x1, x2), std::max(x1, x2));
    }

    // -------------------------------
    // 10) Малюємо рамку навколо карти
    // -------------------------------
    for (int x=0; x<(int)mapWidth; ++x) {
        level[0][x]        = L'━';
        level[rows-1][x]   = L'━';
    }
    for (int y=0; y<(int)rows; ++y) {
        level[y][0]             = L'┃';
        level[y][mapWidth-1]    = L'┃';
    }

    level[0][0]                = L'┏';
    level[0][mapWidth-1]       = L'┓';
    level[rows-1][0]           = L'┗';
    level[rows-1][mapWidth-1]  = L'┛';

    // -------------------------------
    // 11) Сходи (вхід/вихід)
    // -------------------------------
    if (roomCenters.size() >= 2) {
        level[roomCenters.front().second][roomCenters.front().first] = L'^'; // вгору
        level[roomCenters.back().second][roomCenters.back().first]   = L'v'; // вниз
    }

    // -------------------------------
    // 12) Декоративні деталі
    // -------------------------------

    for (auto &center : roomCenters) 
    {
        int coinCount = rng() % 3 + 1; // 1-3 монетки на кімнату
        for (int i = 0; i < coinCount; ++i) 
        {
            int dx = rng() % 3 - 1; // випадкове зміщення по X (-1,0,1)
            int dy = rng() % 3 - 1; // випадкове зміщення по Y (-1,0,1)

            int mx = center.first + dx;
            int my = center.second + dy;

            // ставимо символ лише на підлозі та в межах карти
            if (my > 0 && my < (int)rows && mx > 0 && mx < (int)mapWidth && level[my][mx] == L'.') 
            {
                level[my][mx] = L'¢'; // символ монетки
            }
        }
    }
}

//void GameScene::ChangeLevel()
//{
    // bool validPlacement = false;

    // while (!validPlacement)
    // {
    //     GenerateAutoDungeon(2);  
    //     InitEntitys();            

    //     int px = player->GetX();
    //     int py = player->GetY();

    //     if (level[py][px] == L'v')
    //     {
    //         validPlacement = true;
    //         debugText = "Level must been changed";
    //     }
    // } 
//}

void GameScene::DrawMap()
{
    return;
}

void GameScene::PanelDraw()
{
    ncplane_set_fg_rgb8(panel, 255, 0, 255);
    ncplane_putstr_yx(panel, 1, 1, "Bro just typing shit😂😂😂");

    hud = "Coins = " + std::to_string(player->GetCoinsValue());
    ncplane_set_fg_rgb8(panel, 255, 200, 0);
    ncplane_putstr_yx(panel, 3, 1, hud.c_str());

}

void GameScene::Update(ncplane *map)
{
    player->Render(map);
    for (auto monster : monsters) 
    {
        monster->Render(map);
    }

    int px = player->GetX();
    int py = player->GetY();

    if (level[py][px] == L'v') 
    {
        GenerateAutoDungeon(10);
        InitEntitys();
    }
}