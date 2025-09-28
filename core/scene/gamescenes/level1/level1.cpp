#include "level1.hpp"

#include <memory>
// <random> використовується для процедурної генерації (рандомні кімнати, коридори, розміщення монстрів)
#include <random>
// <algorithm> потрібен для сортування ребер та інших допоміжних операцій
#include <algorithm>
#include "../../map_utils.hpp"

Level1::Level1(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input)
                : Scene(nc, stdn, rows, cols, input) 
{
    // prepare base level buffer and entities
    level.resize(rows, std::wstring(mapWidth, L'.'));
    // Викликаємо процедурний генератор рівня: число кімнат передається як параметр.
    // Змінюйте аргумент, щоб отримати більше/менше кімнат.
    GenerateAutoDungeon(3);
    InitEntitys();
}

void Level1::InitEntitys()
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

    int monstersToPlace = 10; // бажана кількість монстрів
    int attempts = 0; // лічильник спроб (щоб не застрягнути у циклі)
    while ((int)monsters.size() < monstersToPlace && attempts < 200) {
        ++attempts;
        if (roomCenters.empty()) break; // немає кімнат — нічого не робимо
        int ri = pickRoom(rng); // вибираємо випадкову кімнату
        int cx = roomCenters[ri].first; // центр вибраної кімнати (x)
        int cy = roomCenters[ri].second; // центр (y)
        // беремо невеликий випадковий офсет від центру, щоб монстри не були точно в центрі
        int ox = (rng()%5) - 2; // -2..+2
        int oy = (rng()%3) - 1; // -1..+1
        int mx = cx + ox;
        int my = cy + oy;
        // перевірка меж і того, що клітинка прохідна ('.') перед розміщенням монстра
        if (my >= 0 && my < (int)rows && mx >= 0 && mx < (int)mapWidth && level[my][mx] == L'.') {
            monsters.push_back(std::make_shared<Monster>(mx, my));
        }
    }
}


// Procedural dungeon generator: random rooms + MST to connect them
void Level1::GenerateAutoDungeon(int roomCount)
{
    // 1) Очистка карти — заповнюємо пробілами (пізніше будемо прописувати підлогу в кімнатах і коридорах)
    for (int y = 0; y < (int)rows; ++y) for (int x = 0; x < (int)mapWidth; ++x) level[y][x] = L' ';

    // 2) Ініціалізація RNG та розподілів для розмірів і позицій кімнат
    std::mt19937 rng((unsigned)time(nullptr)); // сид по часу
    std::uniform_int_distribution<int> rw(5, 12); // ширина кімнат
    std::uniform_int_distribution<int> rh(4, 8);  // висота кімнат
    // допустимі координати лівого верхнього кута кімнати (з урахуванням бордерів)
    std::uniform_int_distribution<int> rx(2, std::max(2, (int)mapWidth-14));
    std::uniform_int_distribution<int> ry(2, std::max(2, (int)rows-10));

    struct Room { int top,left,h,w; int cx,cy; };
    std::vector<Room> rooms;

    // 3) Розміщення кімнат випадково, уникаючи перекриття
    int attempts = 0;
    while ((int)rooms.size() < roomCount && attempts < roomCount*8) {
        ++attempts;
        int w = rw(rng);
        int h = rh(rng);
        int left = rx(rng);
        int top = ry(rng);
        // пропускаємо, якщо кімната виходить за межі карти
        if (left + w + 2 >= (int)mapWidth || top + h + 2 >= (int)rows) continue;
        // перевірка на перекриття з раніше створеними кімнатами (додаємо невеликі зазори)
        bool ok = true;
        for (auto &r : rooms) {
            if (!(left + w + 2 < r.left || r.left + r.w + 2 < left || top + h + 2 < r.top || r.top + r.h + 2 < top)) { ok = false; break; }
        }
        if (!ok) continue; // якщо перекривається — пробуємо іншу позицію
        Room rr{top,left,h,w,left + w/2, top + h/2};
        rooms.push_back(rr);
    }

    // 4) Малюємо кімнати у level та зберігаємо їх центри (roomCenters)
    roomCenters.clear();
    for (auto &r : rooms) {
        auto tpl = MapUtils::MakeRectRoom(r.h, r.w, L'░', L'.', false);
        // покращуємо вигляд стін (юнікод-рамка)
        tpl[0][0]=L'┏'; tpl[0][r.w-1]=L'┓'; tpl[r.h-1][0]=L'┗'; tpl[r.h-1][r.w-1]=L'┛';
        for (int x=1;x<r.w-1;++x) tpl[0][x]=L'━', tpl[r.h-1][x]=L'━';
        for (int y=1;y<r.h-1;++y) tpl[y][0]=L'┃', tpl[y][r.w-1]=L'┃';
        MapUtils::InsertTemplate(level, rows, mapWidth, tpl, r.top, r.left);
        roomCenters.emplace_back(r.cx, r.cy); // зберігаємо центр кімнати для з'єднань/розстановки
    }

    // 5) Побудова графа кімнат: всі пари центрів з відстанями (ребра)
    struct Edge { int a,b; double d; };
    std::vector<Edge> edges;
    for (int i=0;i<(int)rooms.size();++i) for (int j=i+1;j<(int)rooms.size();++j) {
        int dx = rooms[i].cx - rooms[j].cx;
        int dy = rooms[i].cy - rooms[j].cy;
        edges.push_back({i,j, std::sqrt((double)dx*dx + (double)dy*dy)}); // евклідова відстань
    }
    // сортуємо ребра за зростанням довжини — підготовка до MST (Крускал)
    std::sort(edges.begin(), edges.end(), [](const Edge&a,const Edge&b){return a.d<b.d;});

    // 6) Union-find (для Kruskal): масив батьків для з'єднання компонентів
    std::vector<int> parent(rooms.size()); for (int i=0;i<(int)rooms.size();++i) parent[i]=i;
    std::function<int(int)> findp = [&](int x){ return parent[x]==x?x:parent[x]=findp(parent[x]); };

    // 7) Хелпери для карвінгу горизонтальних та вертикальних коридорів (роблять підлогу '.')
    auto hcorr = [&](int y, int x1, int x2){ if (x1>x2) std::swap(x1,x2); for (int x=x1; x<=x2; ++x) if (y>0 && y<(int)rows && x>0 && x<(int)mapWidth) level[y][x]=L'.'; };
    auto vcorr = [&](int x, int y1, int y2){ if (y1>y2) std::swap(y1,y2); for (int y=y1; y<=y2; ++y) if (y>0 && y<(int)rows && x>0 && x<(int)mapWidth) level[y][x]=L'.'; };

    // 8) Прокладка MST: для кожного ребра в порядку зростання, якщо вузли з різних компонент — з'єднати
    int connections = 0;
    for (auto &e : edges) {
        int pa = findp(e.a), pb = findp(e.b);
        if (pa != pb) {
            parent[pa] = pb; // об'єднуємо компоненти
            // вирізаємо L-подібний коридор між центрами: спочатку по горизонталі потім по вертикалі або навпаки
            int x1 = rooms[e.a].cx, y1 = rooms[e.a].cy;
            int x2 = rooms[e.b].cx, y2 = rooms[e.b].cy;
            if (rng()%2) { hcorr(y1, x1, x2); vcorr(x2, y1, y2); } else { vcorr(x1, y1, y2); hcorr(y2, x1, x2); }
            ++connections;
        }
    }

    // 9) Додаємо кілька випадкових додаткових коридорів, щоб створити петлі і зробити карту цікавішою
    for (int k=0;k< (int)rooms.size()/3; ++k) {
        int i = rng()%rooms.size();
        int j = rng()%rooms.size(); if (i==j) continue;
        int x1=rooms[i].cx, y1=rooms[i].cy; int x2=rooms[j].cx, y2=rooms[j].cy;
        // прокладаємо горизонтальну вставку посередині між двома центрами
        hcorr((y1+y2)/2, std::min(x1,x2), std::max(x1,x2));
    }

    // 10) Малюємо зовнішню рамку карти
    for (int x=0;x<(int)mapWidth;++x) { level[0][x]=L'\u2501'; level[rows-1][x]=L'\u2501'; }
    for (int y=0;y<(int)rows;++y) { level[y][0]=L'\u2503'; level[y][mapWidth-1]=L'\u2503'; }
    level[0][0]=L'\u250F'; level[0][mapWidth-1]=L'\u2513'; level[rows-1][0]=L'\u2517'; level[rows-1][mapWidth-1]=L'\u251B';

    // 11) Сходи — ставимо в центрі першої і останньої кімнат
    if (roomCenters.size()>=2) {
        level[roomCenters.front().second][roomCenters.front().first] = L'^';
        level[roomCenters.back().second][roomCenters.back().first] = L'v';
    }

    // 12) Дрібні декоративні деталі (точки/коми) на підлозі
    for (int i=0;i< (int)roomCenters.size()*2; ++i) {
        int rx = rng() % (int)mapWidth;
        int ry = rng() % (int)rows;
        if (level[ry][rx]==L'.') level[ry][rx] = (i%4==0?L'•':L',');
    }
}

void Level1::DrawMap()
{
    return;
}

void Level1::PanelDraw()
{
    ncplane_set_fg_rgb8(panel, 255, 0, 255);
    ncplane_putstr_yx(panel, 1, 1, "Bro just typing shit😂😂😂");
}

void Level1::Update(ncplane *map)
{
    player->Render(map);
    for (auto monster : monsters) 
    {
        monster->Render(map);
    }
}