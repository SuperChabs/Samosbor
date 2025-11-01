#pragma once

#include <vector>
#include <string>

namespace MapUtils 
{
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

        int draw_left = std::max(0, left);
        int draw_top = std::max(0, top);
        int draw_right = std::min(mapw_i - 1, right);
        int draw_bottom = std::min(rows_i - 1, bottom);

        if (draw_left > draw_right || draw_top > draw_bottom) return; 

        for (int y = draw_top; y <= draw_bottom; ++y) 
        {
            for (int x = draw_left; x <= draw_right; ++x) 
            {
                bool on_border = (y == draw_top || y == draw_bottom || x == draw_left || x == draw_right);
                if (fill) level[y][x] = ch;
                else if (on_border) level[y][x] = ch;
            }
        }
    }


    using Room = std::vector<std::wstring>;

    inline Room MakeRectRoom(int height, int width, wchar_t border = L'\u2501', wchar_t floor = L'.', bool filled = false)
    {
        Room room;
        if (height <= 0 || width <= 0) return room;
        room.resize(height, std::wstring(width, floor));

        for (int x = 0; x < width; ++x) 
        {
            room[0][x] = border;
            room[height - 1][x] = border;
        }
        for (int y = 0; y < height; ++y) 
        {
            room[y][0] = border;
            room[y][width - 1] = border;
        }

        if (filled) 
            for (int y = 0; y < height; ++y) 
                for (int x = 0; x < width; ++x) 
                    room[y][x] = border;
                
        return room;
    }

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

        for (int ty = 0; ty < tpl_h; ++ty) 
        {
            int y = top + ty;

            if (y < 0 || y >= rows_i) continue;

            for (int tx = 0; tx < tpl_w; ++tx) 
            {
                int x = left + tx;

                if (x < 0 || x >= mapw_i) continue;

                wchar_t c = tpl[ty][tx];

                if (!overwrite && c == L' ') continue;

                level[y][x] = c;
            }
        }
    }
} // namespace MapUtils
