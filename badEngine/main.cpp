#include "SLList.h"
#include "Stopwatch.h"
#include "GraphicsSys.h"
#include "Configs.h"
#include <thread>
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <conio.h>

#include "Sprite.h"

#include "UniformGrid.h"

#include "Sequence.h"

#include "NumberGenerator.h"

#include "Color.h"
#include "Camera.h"
#include "Scripts.h"
#include "Ray.h"

#include <iostream>
/*
PRIORITY: constexpr for SLList and then correct signatures for quad/BHV tree

1) revist quadtree (though not a lot of justification), mainly create a hash grid instead
2) documentation and visit <functional>
2) make engine class for on user create and run loop sepparation

*/

int main() {

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    {
        using namespace badEngine;
        //using namespace badEngine;
        //configs
        Configs windowConfig("../Configs/system_config.json");

        //init SDL system, can throw
        GraphicsSys renManager(windowConfig.get());

        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 
        AABB gridBounds = AABB(2 * 32, 2 * 32, 22*32,14*32);
        NumberGenerator gen;
        Sequence<AABB> myABBS;
        myABBS.set_capacity(10000);
        for (int i = 0; i < myABBS.capacity(); i++) {
            myABBS.emplace_back(
                gen.random_float(gridBounds.x, gridBounds.x+ gridBounds.w-1),
                gen.random_float(gridBounds.y, gridBounds.y+ gridBounds.h-1),
                16,
                16
            );
        }

        UniformGrid muhGrid(gridBounds, 32.0f, 32.0f);
        muhGrid.insert(myABBS.begin(), myABBS.end(), 0ll);


        AABB wasdBox = AABB(32, 32, 8, 8);
        AABB mouseBox = AABB(0, 0, 8, 8);
        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################

        //main loop
        bool GAME_RUNNING = true;
        SDL_Event EVENT;
        //this whole main loop is badly bad but engine class in the future so fuck it for now

        while (GAME_RUNNING) {
            Stopwatch fps;
            //CLEAR RENDERING
            renManager.renderer_refresh();

            //LISTEN TO EVENTS
            while (SDL_PollEvent(&EVENT)) {
                if (EVENT.type == SDL_EVENT_QUIT) {
                    GAME_RUNNING = false;
                    continue;
                }
                //update wasdBox
                if (EVENT.key.key == SDLK_W) {
                    wasdBox.y -= 10;
                }
                if (EVENT.key.key == SDLK_A) {
                    wasdBox.x -= 10;
                }
                if (EVENT.key.key == SDLK_S) {
                    wasdBox.y += 10;
                }
                if (EVENT.key.key == SDLK_D) {
                    wasdBox.x += 10;
                }
            }
            //update mouseBox
            float x, y;
            SDL_GetMouseState(&x, &y);
            mouseBox.x = x;
            mouseBox.y = y;
            //first draw the whole grid to yellow
            renManager.render_rectangle(muhGrid.get_grid_bounds(), Colors::Yellow);
            //draw mouse and wasd box
            renManager.render_rectangle(mouseBox, Colors::Red);
            renManager.render_rectangle(wasdBox, Colors::Blue);
            //create and draw a line segment
            float2 lineStart = float2(wasdBox.x, wasdBox.y);
            float2 lineEnd = float2(mouseBox.x, mouseBox.y);
           // renManager.render_line(lineStart,lineEnd, Colors::White);
            Sequence<int> cells;
            muhGrid.query_ray3(lineStart, lineEnd, cells);
            //draw all the cells captured
            for (int idx : cells)
            {
                int cols = (int)(muhGrid.get_grid_bounds().w / muhGrid.get_cell_width());
                int gx = idx % cols;
                int gy = idx / cols;

                float cellW = muhGrid.get_cell_width();
                float cellH = muhGrid.get_cell_height();
                const AABB& gridBounds = muhGrid.get_grid_bounds();

                AABB cellBox;
                cellBox.x = gridBounds.x + gx * cellW;
                cellBox.y = gridBounds.y + gy * cellH;
                cellBox.w = cellW;
                cellBox.h = cellH;

                renManager.render_rectangle(cellBox, Colors::Green);
            }
            renManager.render_line(lineStart, lineEnd, Colors::White);
            renManager.renderer_present();
            std::cout << "fps: " << 1 / fps.dt_float() << '\n';
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}
