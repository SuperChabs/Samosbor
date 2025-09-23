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

    player = std::make_shared<Player>(10, 30);

    monsters.push_back(std::make_shared<Monster>(10, 10));
    monsters.push_back(std::make_shared<Monster>(15, 7));

    level.resize(rows, std::wstring(mapWidth, L'.'));

}

void Scene::Render()
{
    ncplane_erase(map);
    ncplane_erase(panel);

    Draw();

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < mapWidth; x++) {
            wchar_t c = level[y][x];
            if (c == '@') ncplane_set_fg_rgb8(map, 53, 62, 67);
            else if (c == L'%') ncplane_set_fg_rgb8(map, 125, 213, 60);
            else if (c == L'\u2581') ncplane_set_fg_rgb8(map, 60, 60, 60);
            else if (c == L'\u258F') ncplane_set_fg_rgb8(map, 60, 60, 60);
            else ncplane_set_fg_rgb8(map, 60, 60, 60);

            ncplane_putwc_yx(map, y, x, c);
        }
    }

    Update(map);

    PanelDraw();

    notcurses_render(nc);
}

void Scene::HandleInput()
{
	input.Bind('w', [&](){ 
		player->Move(0, -1, level); 
		for(auto& m : monsters){
			m->Update(player->GetX(), player->GetY());
		}
	});

	input.Bind('s', [&](){ 
		player->Move(0, 1, level); 
		for(auto& m : monsters){
			m->Update(player->GetX(), player->GetY());
		}
	});

	input.Bind('a', [&](){ 
		player->Move(-1, 0, level); 
		for(auto& m : monsters){
			m->Update(player->GetX(), player->GetY());
		}
	});

	input.Bind('d', [&](){ 
		player->Move(1, 0, level); 
		for(auto& m : monsters){
			m->Update(player->GetX(), player->GetY());
		}
	});

	input.Bind('e', [&](){ 
		for(auto& m : monsters){
			int dx = abs(player->GetX() - m->GetX());
			int dy = abs(player->GetY() - m->GetY());
			if(m->IsAlive() && dx + dy == 1){
				m->Kill();
			}
		}
	});

}

void Scene::Draw()
{
    for(int y = 28; y < 29; y++) {
        for(int x = 0; x < mapWidth; x++) {
            level[y][x] = L'@';
        }
    }

    for(int y = 31; y < 32; y++) {
        for(int x = 2; x < mapWidth; x++) {
            level[y][x] = L'@';
        }
    }

    for (int x = 0; x < mapWidth; x++) 
    {
        level[0][x] = L'\u2581';
        level[rows-1][x] = L'\u2581'; 
    }

    for (int y = 0; y < rows; y++) 
    {
        level[y][0] = L'\u258F';          
        level[y][mapWidth-1] = L'\u258F'; 
    }
}

void Scene::PanelDraw()
{
    ncplane_set_fg_rgb8(panel, 255, 255, 255);
    ncplane_putstr_yx(panel, 1, 1, "== Панель ==");
    ncplane_putstr_yx(panel, 3, 1, "HP: 100");
    ncplane_putstr_yx(panel, 5, 1, "Інвентар:");
    ncplane_putstr_yx(panel, 6, 3, "- Меч");
    ncplane_putstr_yx(panel, 8, 1, "Коментар:");
    if(false /*!monster.IsAlive()*/)
        ncplane_putstr_yx(panel, 9, 3, "Монстр переможений!");
    else
        ncplane_putstr_yx(panel, 9, 3, "Монстр поруч...");
}

void Scene::Update(ncplane *map)
{
    player->Render(map);
    for (auto monster : monsters) 
    {
        monster->Render(map);
    }
}
