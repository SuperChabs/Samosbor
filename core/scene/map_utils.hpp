#pragma once

#include <vector>
#include <string>

namespace MapUtils {

// Draw a rectangle into a level buffer (vector of wstrings).
// - level: reference to level grid (rows x mapWidth)
// - rows, mapWidth: dimensions of the level
// - top, left: top-left corner (can be negative or outside - function will clip)
// - width, height: size (if <=0 nothing is drawn)
// - ch: wchar_t to draw
// - fill: if true, fill the interior with ch; otherwise draw only outline
inline void DrawRect(std::vector<std::wstring>& level,
                     unsigned int rows,
                     unsigned int mapWidth,
                     int top, int left,
                     int width, int height,
                     wchar_t ch,
                     bool fill = false)
{
    if (width <= 0 || height <= 0) return;

    int rows_i = static_cast<int>(rows);
    int mapw_i = static_cast<int>(mapWidth);

    int right = left + width - 1;
    int bottom = top + height - 1;

    // clip rectangle to grid bounds
    int draw_left = std::max(0, left);
    int draw_top = std::max(0, top);
    int draw_right = std::min(mapw_i - 1, right);
    int draw_bottom = std::min(rows_i - 1, bottom);

    if (draw_left > draw_right || draw_top > draw_bottom) return; // fully outside

    // draw filled or outline
    for (int y = draw_top; y <= draw_bottom; ++y) 
    {
        // safe row exists
        for (int x = draw_left; x <= draw_right; ++x) 
        {
            bool on_border = (y == draw_top || y == draw_bottom || x == draw_left || x == draw_right);
            if (fill) level[y][x] = ch;
            else if (on_border) level[y][x] = ch;
        }
    }
}

} // namespace MapUtils

// --- Template helpers ---
namespace MapUtils {

// A small alias for a room/template: rows of wchar_t
using Room = std::vector<std::wstring>;

// Create a rectangular room template of given height/width.
// - border: character used for the walls
// - floor: character used for the interior
// - filled: if true, interior will also be filled with border; otherwise with floor
inline Room MakeRectRoom(int height, int width, wchar_t border = L'\u2501', wchar_t floor = L'.', bool filled = false)
{
    Room room;
    if (height <= 0 || width <= 0) return room;
    room.resize(height, std::wstring(width, floor));

    for (int x = 0; x < width; ++x) {
        room[0][x] = border;
        room[height - 1][x] = border;
    }
    for (int y = 0; y < height; ++y) {
        room[y][0] = border;
        room[y][width - 1] = border;
    }

    if (filled) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                room[y][x] = border;
            }
        }
    }

    return room;
}

// Insert a template into the main level buffer at (top,left).
// If overwrite==false, characters in the template that are L' ' will be treated as transparent.
inline void InsertTemplate(std::vector<std::wstring>& level,
                           unsigned int rows,
                           unsigned int mapWidth,
                           const Room& tpl,
                           int top, int left,
                           bool overwrite = true)
{
    if (tpl.empty()) return;
    int tpl_h = static_cast<int>(tpl.size());
    int tpl_w = static_cast<int>(tpl[0].size());

    int rows_i = static_cast<int>(rows);
    int mapw_i = static_cast<int>(mapWidth);

    for (int ty = 0; ty < tpl_h; ++ty) {
        int y = top + ty;
        if (y < 0 || y >= rows_i) continue;
        for (int tx = 0; tx < tpl_w; ++tx) {
            int x = left + tx;
            if (x < 0 || x >= mapw_i) continue;
            wchar_t c = tpl[ty][tx];
            if (!overwrite && c == L' ') continue;
            level[y][x] = c;
        }
    }
}

} // namespace MapUtils
