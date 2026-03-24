#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <conio.h>
#include <iostream>

#include "badCore.h"
#include "GFX_INIT.h"
#include "Geometric.h"

#include "MakeShape.h"
#include "Validate_data.h"
#include "load_data.h"
#include "MouseCameraController.h"
#include "EngineUtils.h"
#include "FreeDraw.h"
#include "CollisionRoutines.h"


// TODO:: test uniform grid again

int main() {

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

    {
        bad::GFX_INIT sdl_sys(SDL_INIT_VIDEO);

        //using namespace badEngine;
        bad::validate_json_file("../badEngine/Configs/system_config.json", bad::expected_file_type::WINDOW_JSON);
        nlohmann::json window_conf = bad::load_json("../badEngine/Configs/system_config.json");
        auto windowContextData = bad::create_window_description(window_conf, "sys_config");

        bad::Window window = bad::make_window(windowContextData.heading, windowContextData.width, windowContextData.height, windowContextData.flags);
        bad::Renderer renderer = bad::make_renderer(window.get(), {});

        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 

        bad::UniformGrid grid(bad::Rect{ 0, 0, 960, 540 }, 10, 10);


        const auto& grid_bounds = grid.get_grid_bounds();
        const auto cell_width = grid.get_cell_width();
        const auto cell_height = grid.get_cell_height();

        const auto cell_count_width = grid_bounds.get_width() / cell_width;
        const auto cell_count_heigth = grid_bounds.get_height() / cell_height;

        bad::Sequence<bad::Rect> cells(cell_count_width * cell_count_heigth);

        int index = 0;
        for (int row = 0; row < cell_count_heigth; ++row)
        {
            for (int col = 0; col < cell_count_width; ++col)
            {
                int x = grid_bounds.min.x + col * cell_width;
                int y = grid_bounds.min.y + row * cell_height;

                cells[index++] = bad::Rect{ (float)x, (float)y, cell_width, cell_height };
            }
        }


        struct something
        {
            bad::Point p0{ 0,0 };
            bad::Point p1{ 200,200 };
        } line;

        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################

        //main loop
        bool GAME_RUNNING = true;

        //this whole main loop is badly bad but engine class in the future so fuck it for now
        bad::Stopwatch steper;
        while (GAME_RUNNING) {
            SDL_Event EVENT;
            float dt = steper.dt_float();
            //CLEAR RENDERING
            SDL_RenderClear(renderer.get());

            //LISTEN TO EVENTS
            while (SDL_PollEvent(&EVENT)) {

                switch (EVENT.type) {
                case SDL_EVENT_QUIT:
                    GAME_RUNNING = false;
                    break;

                case SDL_EVENT_KEY_DOWN:

                    if (EVENT.key.key == SDLK_W) {
                        line.p0.y -= 10;
                    }
                    if (EVENT.key.key == SDLK_S) {
                        line.p0.y += 10;
                    }
                    if (EVENT.key.key == SDLK_A) {
                        line.p0.x -= 10;
                    }
                    if (EVENT.key.key == SDLK_D) {
                        line.p0.x += 10;
                    }
                    break;

                default:
                    break;
                }
            }

            float mx, my;
            SDL_GetMouseState(&mx, &my);
            line.p1 = { mx,my };


            bad::Sequence<int> colliding_cells;
            colliding_cells.reserve(cells.size());
            const bad::LineSegment line_segment(line.p0, line.p1);
            grid.query_ray(line_segment, colliding_cells);

            auto* pr = renderer.get();
            auto default_transform = bad::Mat3::identity();
            //first draw grid
            for (auto cell : cells)
            {
                bad::draw_rect_lines_transformed(pr, cell, default_transform, bad::Colors::Red);
            }

            //then do basic fill rect

            SDL_SetRenderDrawColor(pr, 255,255,0,255);

            for (int index : colliding_cells)
            {
                const auto& rect = cells[index];
                SDL_FRect to_sdl_rect{rect.min.x, rect.min.y, rect.get_width(), rect.get_height()};
                SDL_RenderRect(pr, &to_sdl_rect);
            }

            bad::draw_line_transformed(pr, line.p0, line.p1, default_transform, bad::Colors::Magenta);

            SDL_SetRenderTarget(renderer.get(), nullptr);//reminder
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);//reset to black ONCE before the end
            SDL_RenderPresent(renderer.get());
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}
