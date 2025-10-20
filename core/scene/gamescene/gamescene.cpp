#include "gamescene.hpp"

#include <memory>
#include <random>
#include <algorithm>

#include "settings.hpp" 
#include "itemregistry.hpp"
#include "panelmanager.hpp"

#include "../map_utils.hpp"

GameScene::GameScene(struct notcurses* nc, struct ncplane* stdn, unsigned int rows, unsigned int cols, InputManager& input, PanelManager& panelManager)
                : Scene(nc, stdn, rows, cols, input), panelManager(panelManager) 
{
    ncplane_erase(map);
    ncplane_erase(panel);

    level.resize(rows, std::wstring(mapWidth, L' '));

    GenerateAutoDungeon(10);
    InitEntitys();

    panelManager.SetPanel(panel);
}

void GameScene::InitEntitys()
{
    monsters.clear();

    if (player) 
    { 
        if (!roomCenters.empty()) 
        {
            auto c = roomCenters.front();
            player->SetPosition(c.first, c.second);
        } 
        else 
        {
            player->SetPosition(this->mapWidth / 2, this->rows / 2);
        }
    }
    else if (!player) 
    {
        if (!roomCenters.empty()) 
        {
            auto c = roomCenters.front();
            player = std::make_shared<Player>(c.first, c.second);
        } 
        else 
        {
            player = std::make_shared<Player>(this->mapWidth / 2, this->rows / 2); 
        }
    }


    std::mt19937 rng((unsigned)time(nullptr));

    std::uniform_int_distribution<int> pickRoom(0, std::max(0, (int)roomCenters.size()-1));

    int monstersToPlace = 5; 
    int attempts = 0;
    while ((int)monsters.size() < monstersToPlace && attempts < 200) 
    {
        ++attempts;
        if (roomCenters.empty()) 
            break;
        int ri = pickRoom(rng); 
        int cx = roomCenters[ri].first; 
        int cy = roomCenters[ri].second; 

        int ox = (rng()%5) - 2;
        int oy = (rng()%3) - 1;
        int mx = cx + ox;
        int my = cy + oy;

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
    for (int y = 0; y < (int)rows; ++y) 
        for (int x = 0; x < (int)mapWidth; ++x) 
            level[y][x] = L' ';

    // -------------------------------
    // 2) RNG і розподіли для кімнат
    // -------------------------------
    std::mt19937 rng((unsigned)time(nullptr));
    std::uniform_int_distribution<int> rw(5, 20); 
    std::uniform_int_distribution<int> rh(4, 8);  
    std::uniform_int_distribution<int> rx(2, std::max(2, (int)mapWidth - 14)); 
    std::uniform_int_distribution<int> ry(2, std::max(2, (int)rows - 10));    


    struct Room 
    {
        int top, left; 
        int h, w;      
        int cx, cy;    
    };

    std::vector<Room> rooms;

    // -------------------------------
    // 3) Створення кімнат
    // -------------------------------
    int attempts = 0;
    while ((int)rooms.size() < roomCount && attempts < roomCount * 8) 
    {
        ++attempts;

        int w   = rw(rng);
        int h   = rh(rng);
        int left= rx(rng);
        int top = ry(rng);

        if (left + w + 2 >= (int)mapWidth || top + h + 2 >= (int)rows)
            continue;

        bool ok = true;
        for (auto &r : rooms) 
        {
            bool noOverlap = (left + w + 2 < r.left || r.left + r.w + 2 < left ||
                              top + h + 2 < r.top  || r.top  + r.h + 2 < top);
            if (!noOverlap) 
            {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        Room rr { top, left, h, w, left + w / 2, top + h / 2 };
        rooms.push_back(rr);
    }

    // -------------------------------
    // 4) Малювання кімнат
    // -------------------------------
    roomCenters.clear(); 
    for (auto &r : rooms) 
    {

        auto tpl = MapUtils::MakeRectRoom(r.h, r.w, L'░', L'.', false);

        tpl[0][0]         = L'┏'; tpl[0][r.w-1]     = L'┓';
        tpl[r.h-1][0]     = L'┗'; tpl[r.h-1][r.w-1] = L'┛';

        for (int x=1; x<r.w-1; ++x) 
        {
            tpl[0][x] = L'━'; 
            tpl[r.h-1][x] = L'━';
        }

        for (int y=1; y<r.h-1; ++y) 
        {
            tpl[y][0] = L'┃'; 
            tpl[y][r.w-1] = L'┃';
        }

        MapUtils::InsertTemplate(level, rows, mapWidth, tpl, r.top, r.left);

        roomCenters.emplace_back(r.cx, r.cy);
    }

    // -------------------------------
    // 5) Створюємо граф кімнат (ребра між центрами)
    // -------------------------------
    struct Edge { int a, b; double d; };
    std::vector<Edge> edges;

    for (int i=0; i<(int)rooms.size(); ++i) 
    {
        for (int j=i+1; j<(int)rooms.size(); ++j) 
        {
            int dx = rooms[i].cx - rooms[j].cx;
            int dy = rooms[i].cy - rooms[j].cy;
            double dist = std::sqrt((double)dx*dx + (double)dy*dy);
            edges.push_back({i, j, dist});
        }
    }

    std::sort(edges.begin(), edges.end(),
              [](const Edge &a, const Edge &b){ return a.d < b.d; });

    // -------------------------------
    // 6) Union-Find для MST
    // -------------------------------
    std::vector<int> parent(rooms.size());
    for (int i=0; i<(int)rooms.size(); ++i) parent[i] = i;

    std::function<int(int)> findp = [&](int x) 
    {
        return (parent[x] == x) ? x : parent[x] = findp(parent[x]);
    };

    // -------------------------------
    // 7) Хелпери для коридорів
    // -------------------------------
    auto hcorr = [&](int y, int x1, int x2) 
    {
        if (x1 > x2) std::swap(x1, x2);
        for (int x=x1; x<=x2; ++x)
            if (y>0 && y<(int)rows && x>0 && x<(int)mapWidth)
                level[y][x] = L'.';
    };

    auto vcorr = [&](int x, int y1, int y2) 
    {
        if (y1 > y2) std::swap(y1, y2);
        for (int y=y1; y<=y2; ++y)
            if (y>0 && y<(int)rows && x>0 && x<(int)mapWidth)
                level[y][x] = L'.';
    };

    // -------------------------------
    // 8) Будуємо MST і прокладаємо коридори
    // -------------------------------
    for (auto &e : edges) 
    {
        int pa = findp(e.a);
        int pb = findp(e.b);

        if (pa != pb)
        {
            parent[pa] = pb; 

            int x1 = rooms[e.a].cx, y1 = rooms[e.a].cy;
            int x2 = rooms[e.b].cx, y2 = rooms[e.b].cy;


            if (rng()%2) 
            {
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
    for (int k=0; k<(int)rooms.size()/3; ++k) 
    {
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
    for (int x=0; x<(int)mapWidth; ++x) 
    {
        level[0][x]        = L'━';
        level[rows-1][x]   = L'━';
    }
    for (int y=0; y<(int)rows; ++y) 
    {
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
    if (roomCenters.size() >= 2) 
    {
        level[roomCenters.front().second][roomCenters.front().first] = L'^'; // вгору
        level[roomCenters.back().second][roomCenters.back().first]   = L'v'; // вниз
    }

    // -------------------------------
    // 12) Декоративні деталі
    // -------------------------------

    for (auto &center : roomCenters) 
    {
        int coinCount = rng() % 3 + 1; 
        for (int i = 0; i < coinCount; ++i) 
        {
            int dx = rng() % 3 - 1; 
            int dy = rng() % 3 - 1; 

            int mx = center.first + dx;
            int my = center.second + dy;

            if (my > 0 && my < (int)rows && mx > 0 && mx < (int)mapWidth && level[my][mx] == L'.') 
            {
                level[my][mx] = L'¢';
            }
        }
    }

    for (auto &center : roomCenters) 
    {
        int ix = center.first;
        int iy = center.second;

        if (iy > 0 && iy < (int)rows && ix > 0 && ix < (int)mapWidth && level[iy][ix] == L'.') 
        {
            auto it = ItemRegistry::GetItem("key");
            if (it) level[iy][ix] = it->GetSymbol();
        }
    }
}

void GameScene::DrawMap()
{
    return;
}

void GameScene::PanelDraw()
{
    panelManager.Clear();

    std::string coinText = "Монети: " + std::to_string(player->GetCoinsValue());
    panelManager.SetHUDLine(1, coinText, 255, 200, 0);

    std::string healthText = "HP: " + std::to_string(player->GetHealth()) + "/" + std::to_string(player->GetMaxHealth());
    panelManager.SetHUDLine(2, healthText, 255, 100, 100);

    std::string levelText = "Рівень: " + std::to_string(Settings::Instance().GetCurrentLevel());
    panelManager.SetHUDLine(3, levelText, 150, 150, 255);

    panelManager.SetHUDLine(5, "Інвентар ([/] вибір, F використати):", 255, 255, 255);

    auto& inv = player->GetInventory();
    const auto& slots = inv.GetSlots();
    int selectedIdx = inv.GetSelectedIndex();

    std::vector<std::string> lines;
    lines.reserve(slots.size());
    for (const auto& slot : slots) 
        if (slot.item) 
        {
            wchar_t sym = slot.item->GetSymbol();
            std::string name = slot.item->GetName();
            char buf[128];
            snprintf(buf, sizeof(buf), "%lc %s x%d", (wint_t)sym, name.c_str(), slot.count);
            lines.emplace_back(buf);
        } else 
            lines.emplace_back("");
    
    panelManager.RenderInventory(lines, selectedIdx);
}


void GameScene::Update()
{
    player->Render(GetMap());
    for (auto monster : monsters) 
    {
        monster->Render(GetMap());
    }

    int px = player->GetX();
    int py = player->GetY();
    
    wchar_t tile = level[py][px];
    if (tile == L'♿' || tile == L'⚿' || tile == L'☠' || tile == L'⚠' || tile == L'♯' || tile == L'‣') 
    {
        auto& items = ItemRegistry::GetAllItems(); 

        for (auto& [id, item] : items) 
            if (item->GetSymbol() == tile) 
            {
                if (player->GetInventory().AddItem(item, 1)) 
                    level[py][px] = L'.'; 
                break;
            }
    }

    if (level[py][px] == L'v') 
    {
        Settings::Instance().SetCurrentLevel(Settings::Instance().GetCurrentLevel() + 1);
        Settings::Instance().SaveProgress();
        
        GenerateAutoDungeon(10);
        InitEntitys();
    }
}

ncplane *GameScene::GetMap()
{
    return map;
}

ncplane *GameScene::GetPanel()
{
    return panel;
}
