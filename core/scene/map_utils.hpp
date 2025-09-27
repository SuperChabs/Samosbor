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
