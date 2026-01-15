#include "SLList.h"
#include "Stopwatch.h"
#include "GraphicsSys.h"
#include "Config_JSON.h"
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

1) make engine class for on user create and run loop sepparation


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
        Config_JSON windowConfig("../Configs/system_config.json");

        //init SDL system, can throw
        GraphicsSys renManager(windowConfig);

        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 
        AABB gridBounds = AABB(0, 0, 256, 256); // exact square
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

        UniformGrid muhGrid(gridBounds, 8.0f, 8.0f);
        muhGrid.insert(myABBS.begin(), myABBS.end(), 0ll);


        AABB wasdBox = AABB(-64, -64, 8, 8);   // outside, top-left
        AABB mouseBox = AABB(256, 256, 8, 8);  // outside, bottom-right

        std::string path = "C:/Users/ADMIN/Desktop/badEngine/Fonts/font_32x3.png";
        std::string path2 = "C:/Users/ADMIN/Desktop/badEngine/Textures/player.png";
        
        Texture txt(path, renManager);
        Texture animtxt(path2, renManager);
        //TargetTexture ttxt(500,500, renManager);

        Font font(txt, 32, 3);
        //Canvas canv(ttxt);
        Animation anim(animtxt, 32,32);
        font.set_text("yey or nay");

        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################

        //main loop
        bool GAME_RUNNING = true;
        SDL_Event EVENT;
        //this whole main loop is badly bad but engine class in the future so fuck it for now
        Stopwatch steper;
        while (GAME_RUNNING) {
            float dt = steper.dt_float();
            //CLEAR RENDERING
            renManager.system_refresh();

            //LISTEN TO EVENTS
            while (SDL_PollEvent(&EVENT)) {
                if (EVENT.type == SDL_EVENT_QUIT) {
                    GAME_RUNNING = false;
                    continue;
                }
                //update wasdBox
                if (EVENT.key.key == SDLK_W) {
                    wasdBox.y -= 1;
                }
                if (EVENT.key.key == SDLK_A) {
                    wasdBox.x -= 1;
                }
                if (EVENT.key.key == SDLK_S) {
                    wasdBox.y += 1;
                }
                if (EVENT.key.key == SDLK_D) {
                    wasdBox.x += 1;
                }
                if (EVENT.key.key == SDLK_E) {
                    anim.set_scale(anim.get_scale() + 1);
                }
                if (EVENT.key.key == SDLK_J) {
                    font.set_scale(font.get_scale() + 1);
                    font.update();
                }
                if (EVENT.key.key == SDLK_K) {
                    font.set_scale(font.get_scale() - 1);
                    font.update();
                }
            }

            anim.update(dt);

            //update mouseBox
            float x, y;
            SDL_GetMouseState(&x, &y);
            anim.set_pos(float2(x, y));
            font.set_pos(float2(x, y));
            font.update();
            mouseBox.x = x;
            mouseBox.y = y;
            //first draw the whole grid to yellow
            renManager.draw_shape(muhGrid.get_grid_bounds(), Colors::Yellow);
            //draw mouse and wasd box
            renManager.draw_shape(mouseBox, Colors::Red);
            renManager.draw_shape(wasdBox, Colors::Blue);
            //create and draw a line segment
            float2 lineStart = float2(wasdBox.x, wasdBox.y);
            float2 lineEnd = float2(mouseBox.x, mouseBox.y);
           // renManager.render_line(lineStart,lineEnd, Colors::White);
            Sequence<int> cells;
            muhGrid.query_ray(lineStart, lineEnd, cells);
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

                renManager.draw_shape(cellBox, Colors::Green);
            }
            renManager.draw_shape(lineStart, lineEnd, 4, Colors::Green);

            anim.draw(renManager);
            font.draw(renManager);
            renManager.system_present();
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}
