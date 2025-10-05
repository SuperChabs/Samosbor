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
    
    level.resize(rows, std::wstring(mapWidth, L'.'));
}

Scene::~Scene()
{

}

void Scene::Render()
{
    ncplane_erase(map);
    ncplane_erase(panel);

    DrawMap();

    for (int y = 0; y < rows; y++) 
    {
        for (int x = 0; x < mapWidth; x++) 
        {
            wchar_t c = level[y][x];
            if (c == '@') ncplane_set_fg_rgb8(map, 1, 4, 88);
            else if (c == '%') 
                ncplane_set_fg_rgb8(map, 125, 213, 60);
            else if (c == L'\u2501' || c == L'\u2503' || 
                     c == L'\u2517' || c == L'\u251B' ||
                     c == L'\u250F' || c == L'\u2513') 
                ncplane_set_fg_rgb8(map, 61, 240, 60);   
            else if (c == L'░') 
                ncplane_set_fg_rgb8(map, 107, 63, 105);
            else if (c == L'v' || c == L'^')
                ncplane_set_fg_rgb8(map, 250, 177, 47);
            else if (c == L'¢') 
                ncplane_set_fg_rgb8(map, 31, 69, 41);
            else 
                ncplane_set_fg_rgb8(map, 60, 60, 60);

            ncplane_putwc_yx(map, y, x, c);
        }
    }

    Update(map);

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
}

void Scene::DrawMap()
{
}

void Scene::PanelDraw()
{
}

