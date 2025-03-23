#include <GZ_Include.h>
#include <defines.h>


/////////////////////////////////// Entry point function //////////////////////////////////////////
GZ::App *GZ::create_application() {
	AppSpec spec;
	spec.headless = false;
	spec.window_height = 1080;
	spec.window_width = 1960;
    spec.is_fullscreen = false;
	spec.name = "Lost Tower Rogue Like";
    
	return new App(spec);
}
