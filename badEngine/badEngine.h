#pragma once

#include "GraphicsSys.h"
#include "NumberGenerator.h"
#include "Stopwatch.h"
#include "UniformGrid.h"

namespace badEngine {


	class badEngine
	{
	public:

		badEngine(const nlohmann::json& sys_config) 
			:gfx(sys_config)
		{

		}

		void run() {
			gfx.system_refresh();
			advance_state();
			resolve_state();
			render();
			gfx.system_present();
			prepare_next_frame();
		}
	
	private:

		void process_input();     //event reading
		void advance_state();      //updates states, moves, generally unconditional
		void resolve_state();      //handles the results of state update, like collision
		void render();            //drawing logic
		void prepare_next_frame();//cleanup, support

	private:
		//current min
		GraphicsSys gfx;
		NumberGenerator rng;
		Stopwatch frametimer;

		//probably required/important but for now idk
		UniformGrid staticObjects;
		UniformGrid dynamicObjects;
	};

}