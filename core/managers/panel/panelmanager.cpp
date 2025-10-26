#include "panelmanager.hpp"

#include <notcurses/notcurses.h>
#include <cstring>

void PanelManager::Clear()
{
    if (!panel_) return;

    unsigned int h, w;

    ncplane_dim_yx(panel_, &h, &w);
    ncplane_set_bg_rgb8(panel_, 0, 0, 0);

    for (unsigned int y = 0; y < h; ++y)
        for (unsigned int x = 0; x < w; ++x)
            ncplane_putchar_yx(panel_, y, x, ' ');
}


// Вставлення Рядку у панель
void PanelManager::PutStr(unsigned int y, unsigned int x, const std::string& s)
{
    if (!panel_) return;
    ncplane_putstr_yx(panel_, y, x, s.c_str());
}

// Вставлення символу у панель
void PanelManager::PutWChar(unsigned int y, unsigned int x, wchar_t wc)
{
    if (!panel_) return;
    ncplane_putwc_yx(panel_, y, x, wc);
}

//хз
void PanelManager::SetHUDLine(unsigned int y, const std::string& text, unsigned char r, unsigned char g, unsigned char b)
{
    if (!panel_) return;
    ncplane_set_fg_rgb8(panel_, r, g, b);
    PutStr(y, 1, text);
}

void PanelManager::RenderInventory(const std::vector<std::string>& lines, int selectedIdx)
{
    if (!panel_) return;

    unsigned int h, w;
    ncplane_dim_yx(panel_, &h, &w);

    int startY = 7;
    int maxDisplay = std::min((int)lines.size(), 10);

    for (int i = 0; i < maxDisplay; ++i) 
    {
        int y = startY + i;

        if (i == selectedIdx) 
        {
            ncplane_set_fg_rgb8(panel_, 0, 255, 0);
            PutStr(y, 1, ">");
        } else 
            PutStr(y, 1, " ");

        if (!lines[i].empty()) 
        {
            if (i == selectedIdx)
                ncplane_set_fg_rgb8(panel_, 0, 255, 0);
            else
                ncplane_set_fg_rgb8(panel_, 200, 200, 200);

            PutStr(y, 3, lines[i]);
        } else 
        {
            ncplane_set_fg_rgb8(panel_, 60, 60, 60);
            PutStr(y, 3, "[порожньо]");
        }
    }
}
