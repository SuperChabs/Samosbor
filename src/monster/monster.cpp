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
    int deltaX = tx - sx;
    int deltaY = ty - sy;
    int absDx = std::abs(deltaX);
    int absDy = std::abs(deltaY);
    int signX = deltaX > 0 ? 1 : (deltaX < 0 ? -1 : 0);
    int signY = deltaY > 0 ? 1 : (deltaY < 0 ? -1 : 0);

    int x = sx;
    int y = sy;

    // TODO: оптимізація — обмежити дальність огляду, якщо потрібно
    if (absDx > absDy) {
        int err = absDx / 2;
        for (int i = 0; i < absDx; ++i) {
            x += signX;
            err += absDy;
            if (err >= absDx) { err -= absDx; y += signY; }
            // Перевіряємо, що координати в межах рівня та клітинка прохідна
            if (y >= 0 && y < (int)level.size() && x >= 0 && x < (int)level[0].size()) {
                if (level[y][x] != L'.') return false; // зустріли перешкоду
            } else return false; // поза межами — не бачимо
        }
    } else {
        int err = absDy / 2;
        for (int i = 0; i < absDy; ++i) {
            y += signY;
            err += absDx;
            if (err >= absDy) { err -= absDy; x += signX; }
            if (y >= 0 && y < (int)level.size() && x >= 0 && x < (int)level[0].size()) {
                if (level[y][x] != L'.' || level[y][x] != L'^' || level[y][x] != L'v' || level[y][x] != L' ') return false;
            } else return false;
        }
    }
    return true; // нічого не завадило — лінія чиста
}

// HACK: Функція спроби переміститися в клітинку (nx,ny).
// Якщо клітинка прохідна ('.') — оновлює координати (x,y) та повертає true.
// Інакше повертає false і нічого не змінює.
static bool TryMove(int &x, int &y, int newX, int newY, const std::vector<std::wstring>& level)
{
    if (newY < 0 || newY >= (int)level.size() || newX < 0 || newX >= (int)level[0].size()) return false; // поза межами
    if (level[newY][newX] == L'.') { x = newX; y = newY; return true; }
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
        int deltaX = (targetX > x) ? 1 : ((targetX < x) ? -1 : 0);
        int deltaY = (targetY > y) ? 1 : ((targetY < y) ? -1 : 0);

        // Крок діагонально, якщо можливо
        if (deltaX != 0 && deltaY != 0) {
            if (TryMove(x, y, x + deltaX, y + deltaY, level)) return;
        }
        // Інакше горизонтально або вертикально
        if (deltaX != 0 && TryMove(x, y, x + deltaX, y, level)) return;
        if (deltaY != 0 && TryMove(x, y, x, y + deltaY, level)) return;

        // Якщо прямо в шляху стіна — пробуємо відступити на 2 клітини назад
        int backX = x - deltaX * 2;
        int backY = y - deltaY * 2;
        if (TryMove(x, y, backX, backY, level)) return;
    }

    // Якщо не бачить гравця — випадковий рух
    int directions[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    // Перемішуємо масив напрямків (рандомний порядок перевірки)
    for (int i = 0; i < 4; ++i) {
        int j = std::rand() % 4; // ! Рекомендується ініціалізувати srand у main()
        std::swap(directions[i], directions[j]);
    }

    for (int i = 0; i < 4; ++i) {
        int candidateX = x + directions[i][0];
        int candidateY = y + directions[i][1];
        if (TryMove(x, y, candidateX, candidateY, level)) return; // вдалося зробити крок
        // Якщо зустріли стіну — спробувати відступити на 2 клітини від неї
        int oppX = x - directions[i][0] * 2;
        int oppY = y - directions[i][1] * 2;
        if (TryMove(x, y, oppX, oppY, level)) return;
    }
}