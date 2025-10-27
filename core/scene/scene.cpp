#include "scene.hpp"
#include "monster.hpp"
#include "player.hpp"

#include <notcurses/notcurses.h>

Scene::Scene(notcurses *nc, ncplane *stdn, unsigned int rows, unsigned int cols, InputManager& input)
        : nc(nc), rows(rows), cols(cols), input(input)
{
    panelWidth = cols / 4;
    mapWidth   = cols - panelWidth;

    struct ncplane_options panel_opts = 
    { 
        .y=0, 
        .x=(int)mapWidth, 
        .rows=rows, 
        .cols=panelWidth 
    };
    panel = ncplane_create(stdn, &panel_opts);

    struct ncplane_options map_opts = 
    {
         .y=0, 
         .x=0, 
         .rows=rows, 
         .cols=mapWidth 
    };
    map = ncplane_create(stdn, &map_opts);
    
    level.resize(rows, std::wstring(mapWidth, L' '));
}

Scene::~Scene()
{
}

void Scene::Render()
{
    ncplane_erase(map);
    ncplane_erase(panel);

    DrawMap();

    ncplane_set_styles(map, NCSTYLE_BOLD);

    for (int y = 0; y < rows; y++) 
    {
        for (int x = 0; x < mapWidth; x++) 
        {
            wchar_t c = level[y][x];
            uint8_t r=60, g=60, b=60; 

            if (c == '@') 
                r=1, g=4, b=88;
            else if (c == '%') 
                r=125, g=213, b=60;
            else if (c == L'\u2501' || c == L'\u2503' || 
                     c == L'\u2517' || c == L'\u251B' ||
                     c == L'\u250F' || c == L'\u2513') 
                r=61, g=240, b=60;
            else if (c == L'░') 
                r=107, g=63, b=105;
            else if (c == L'v' || c == L'^')
                r=250, g=177, b=47;
            else if (c == L'¢') 
                r=31, g=69, b=41;
            else if (c == L'♥' || c == L'⚿' || c == L'☠' || c == L'⚠' || c == L'♯' || c == L'‣')
                r=183, g=116, b=102; 

            ncplane_set_fg_rgb8(map, r, g, b);

            ncplane_putwc_yx(map, y, x, c);
        }
    }

    Update();

    PanelDraw();


    notcurses_render(nc);
}

void Scene::HandleInput()
{
	input.Bind('w', [this](){ 
		player->Move(0, -1, level); 
        for(auto& m : monsters)
        {
            m->Update(player->GetX(), player->GetY(), level);
        }
	});

	input.Bind('s', [this](){ 
		player->Move(0, 1, level); 
        for(auto& m : monsters)
        {
            m->Update(player->GetX(), player->GetY(), level);
        }
	});

	input.Bind('a', [this](){ 
		player->Move(-1, 0, level); 
        for(auto& m : monsters)
        {
            m->Update(player->GetX(), player->GetY(), level);
        }
	});

	input.Bind('d', [this](){ 
		player->Move(1, 0, level); 
        for(auto& m : monsters)
        {
            m->Update(player->GetX(), player->GetY(), level);
        }
	});

	input.Bind('e', [this](){ 
		for(auto& m : monsters)
        {
			int dx = abs(player->GetX() - m->GetX());
			int dy = abs(player->GetY() - m->GetY());
			if(m->IsAlive() && dx + dy == 1){
				m->Kill();
			}
		}
	});

    input.Bind('[', [this](){
        if (player) {
            player->GetInventory().SelectPrevious();
        }
    });
    
    input.Bind(']', [this](){
        if (player) {
            player->GetInventory().SelectNext();
        }
    });
    
    input.Bind('f', [this](){
        if (player) {
            player->GetInventory().UseSelectedItem(player.get());
        }
    });
}

void Scene::Show()
{
    if (map) ncplane_move_top(map);
    if (panel) ncplane_move_top(panel);
    BindInput();
}

void Scene::Hide()
{
    if (map) ncplane_move_bottom(map);
    if (panel) ncplane_move_bottom(panel);
    UnbindInput();
}


void Scene::BindInput()
{
    if (inputBound) return;
    HandleInput();
    inputBound = true;
}

void Scene::UnbindInput()
{
    inputBound = false;
}

ncplane* Scene::GetMap()
{
    return map;
}

ncplane *Scene::GetPanel()
{
    return panel;
}
