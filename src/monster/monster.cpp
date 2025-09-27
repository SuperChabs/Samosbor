#include "monster.hpp"
#include <cstdlib>
#include <algorithm>

// TODO: Better Comments markers are supported by your extension. Use
// TODO: or NOTE: to get colored highlights.

Monster::Monster(int x, int y) : Entity(x, y, 'M', 200, 0, 0) {}

// NOTE: Функція перевірки прямої видимості (line-of-sight)
// Використовує простий підхід, схожий на алгоритм Брезенхема.
// Повертає true, якщо між (sx,sy) і (tx,ty) немає перешкод (тобто усі клітинки 
// по лінії є прохідними '.'), і false якщо зустрічається стіна чи вихід за межі.
static bool HasLineOfSight(int sx, int sy, int tx, int ty, const std::vector<std::wstring>& level)
{
    int dx = tx - sx;
    int dy = ty - sy;
    int nx = std::abs(dx);
    int ny = std::abs(dy);
    int sign_x = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
    int sign_y = dy > 0 ? 1 : (dy < 0 ? -1 : 0);

    int x = sx;
    int y = sy;

    // TODO: оптимізація — обмежити дальність огляду, якщо потрібно
    if (nx > ny) {
        int px = nx / 2;
        for (int i = 0; i < nx; ++i) {
            x += sign_x;
            px += ny;
            if (px >= nx) { px -= nx; y += sign_y; }
            // Перевіряємо, що координати в межах рівня та клітинка прохідна
            if (y >= 0 && y < (int)level.size() && x >= 0 && x < (int)level[0].size()) {
                if (level[y][x] != L'.') return false; // зустріли перешкоду
            } else return false; // поза межами — не бачимо
        }
    } else {
        int py = ny / 2;
        for (int i = 0; i < ny; ++i) {
            y += sign_y;
            py += nx;
            if (py >= ny) { py -= ny; x += sign_x; }
            if (y >= 0 && y < (int)level.size() && x >= 0 && x < (int)level[0].size()) {
                if (level[y][x] != L'.') return false;
            } else return false;
        }
    }
    return true; // нічого не завадило — лінія чиста
}

// HACK: Функція спроби переміститися в клітинку (nx,ny).
// Якщо клітинка прохідна ('.') — оновлює координати (x,y) та повертає true.
// Інакше повертає false і нічого не змінює.
static bool TryMove(int &x, int &y, int nx, int ny, const std::vector<std::wstring>& level)
{
    if (ny < 0 || ny >= (int)level.size() || nx < 0 || nx >= (int)level[0].size()) return false; // поза межами
    if (level[ny][nx] == L'.') { x = nx; y = ny; return true; }
    return false; // стіна або інша непрохідна річ
}

// MAIN: оновлення логіки монстра
// - Якщо монстр бачить гравця (HasLineOfSight) — намагається рухатись у його бік,
//   спочатку намагається діагонально, потім по горизонталі/вертикалі.
// - Якщо шлях заблокований — пробує відступити на 2 клітини у протилежний бік.
// - Якщо не бачить гравця — випадково блукає (рандом серед 4-х напрямків).
//   При зіткненні зі стіною також пробує відступити на 2 клітини.
void Monster::Update(int targetX, int targetY, const std::vector<std::wstring>& level)
{
    if (!alive) return; // якщо мрець — не робимо нічого

    // Якщо монстр бачить гравця — пересуваємось до нього
    if (HasLineOfSight(x, y, targetX, targetY, level)) {
        int dx = (targetX > x) ? 1 : ((targetX < x) ? -1 : 0);
        int dy = (targetY > y) ? 1 : ((targetY < y) ? -1 : 0);

        // Крок діагонально, якщо можливо
        if (dx != 0 && dy != 0) {
            if (TryMove(x, y, x + dx, y + dy, level)) return;
        }
        // Інакше горизонтально або вертикально
        if (dx != 0 && TryMove(x, y, x + dx, y, level)) return;
        if (dy != 0 && TryMove(x, y, x, y + dy, level)) return;

        // Якщо прямо в шляху стіна — пробуємо відступити на 2 клітини назад
        int backx = x - dx * 2;
        int backy = y - dy * 2;
        if (TryMove(x, y, backx, backy, level)) return;
    }

    // Якщо не бачить гравця — випадковий рух
    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    // Перемішуємо масив напрямків (рандомний порядок перевірки)
    for (int i = 0; i < 4; ++i) {
        int j = std::rand() % 4; // ! Рекомендується ініціалізувати srand у main()
        std::swap(dirs[i], dirs[j]);
    }

    for (int i = 0; i < 4; ++i) {
        int nx = x + dirs[i][0];
        int ny = y + dirs[i][1];
        if (TryMove(x, y, nx, ny, level)) return; // вдалося зробити крок
        // Якщо зустріли стіну — спробувати відступити на 2 клітини від неї
        int oppx = x - dirs[i][0] * 2;
        int oppy = y - dirs[i][1] * 2;
        if (TryMove(x, y, oppx, oppy, level)) return;
    }
}