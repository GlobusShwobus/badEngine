#pragma once

#include "load_data.h"
#include "WindowContext.h"
#include "RandomNum.h"
#include "Stopwatch.h"
#include "UniformGrid.h"

namespace badEngine {

	using namespace badCore;
	using namespace badWindow;

	class badEngine
	{
	public:

		badEngine(const WindowContextDescription& window)
			:window(window.heading.c_str(), window.width, window.height, window.flags)
		{

		}

		void run() {
			window.system_refresh();
			advance_state();
			resolve_state();
			render();
			window.system_present();
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
		WindowContext window;
		RandomNum rng;
		Stopwatch frametimer;

		//probably required/important but for now idk
		
		//UniformGrid staticObjects;
		//UniformGrid dynamicObjects;
	};

}