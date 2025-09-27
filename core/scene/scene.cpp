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
    // if (map) 
    // {
    //     ncplane_destroy(map);
    //     map = nullptr;
    // }
    // if (panel) 
    // {
    //     ncplane_destroy(panel);
    //     panel = nullptr;
    // }
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
            uint64_t channels = 0;

            switch (c) 
            {
                case L'@': ncchannels_set_fg_rgb8(&channels, 53, 62, 67); break;
                case L'%': ncchannels_set_fg_rgb8(&channels, 125, 213, 60); break;
                case L'\u2581':
                case L'\u258F': ncchannels_set_fg_rgb8(&channels, 60, 60, 60); break;
                default: ncchannels_set_fg_rgb8(&channels, 200, 200, 200); break;
            }

            ncplane_set_channels(map, channels);
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
			m->Update(player->GetX(), player->GetY());
		}
	});

	input.Bind('s', [this](){ 
		player->Move(0, 1, level); 
		for(auto& m : monsters)
        {
			m->Update(player->GetX(), player->GetY());
		}
	});

	input.Bind('a', [this](){ 
		player->Move(-1, 0, level); 
		for(auto& m : monsters)
        {
			m->Update(player->GetX(), player->GetY());
		}
	});

	input.Bind('d', [this](){ 
		player->Move(1, 0, level); 
		for(auto& m : monsters)
        {
			m->Update(player->GetX(), player->GetY());
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

void Scene::Update(ncplane *map)
{
    // player->Render(map);
    // for (auto monster : monsters) 
    // {
    //     monster->Render(map);
    // }
}

void Scene::SetupInput(){}
