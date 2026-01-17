#include "SLList.h"
#include "Stopwatch.h"
#include "GraphicsSys.h"
#include "Validate_data.h"
#include <thread>
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <conio.h>

#include "Sprite.h"

#include "UniformGrid.h"
#include "Canvas.h"
#include "Sequence.h"
#include "load_data.h"

#include "NumberGenerator.h"

#include "Color.h"
#include "Camera.h"
#include "Scripts.h"
#include "Ray.h"
#include "TextureLoader.h"

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
        nlohmann::json window_conf;
        nlohmann::json texture_loader_config;
        load_and_validate_JSON(window_conf, "../Configs/system_config.json");
        load_and_validate_JSON(texture_loader_config, "../Configs/textures.json");
        validate_GraphicsSys_manifest(window_conf, "sys_config");
        validate_texture_manifest(texture_loader_config, "texture_set1");
        auto loaded = load_GraphicsSys_data(window_conf, "sys_config");
        //init SDL system, can throw
        GraphicsSys renManager(loaded);

        TextureLoader loader(texture_loader_config.at("texture_set1"), renManager.get_render());

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


        AABB wasdBox = AABB(100, 100, 69, 69);   // outside, top-left
        AABB mouseBox = AABB(420, 420, 16, 16);  // outside, bottom-right
        
        Texture txt("C:/Users/ADMIN/Desktop/badEngine/Fonts/font_32x3.png", renManager.get_render());
        Texture animtxt("C:/Users/ADMIN/Desktop/badEngine/Textures/player.png", renManager.get_render());
        //TargetTexture ttxt(500,500, renManager);

        Font font(txt, 32, 3);
        Canvas canv(960,540, renManager.get_render());
        Animation anim(animtxt, 32,32);
        font.set_text("yey or nay");

        canv.start_drawing(renManager.get_render());
        anim.set_pos(float2(200, 200));
        renManager.draw_sprite(anim);
        renManager.draw_shape(mouseBox, Colors::Red);
        renManager.draw_shape(wasdBox, Colors::Blue);
        canv.end_drawing(renManager.get_render());

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
            }
            renManager.draw_texture(canv.get());
            renManager.system_present();
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}
