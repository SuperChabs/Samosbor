#pragma once

#include <notcurses/notcurses.h>
#include <string>
#include <vector>

class PanelManager
{
public:
    PanelManager() = default;
    explicit PanelManager(struct ncplane* panel) : panel_(panel) {}

    void SetPanel(struct ncplane* panel) { panel_ = panel; }

    void Clear();

    void PutStr(unsigned int y, unsigned int x, const std::string& s);

    void PutWChar(unsigned int y, unsigned int x, wchar_t wc);

    void SetHUDLine(unsigned int y, const std::string& text, unsigned char r=255, unsigned char g=255, unsigned char b=255);

    void RenderInventory(const std::vector<std::string>& lines, int selectedIdx);

private:
    struct ncplane* panel_ = nullptr;
};
