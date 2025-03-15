#include <GZ_Include.h>
#include <defines.h>

#include "EditorApp.h"

namespace ed = ax::NodeEditor;


namespace GZ {


    EditorApp::EditorApp(const AppSpec &spec):
        App(spec)
    {
        // App init is done
        // Editor setup
        
    }

    EditorApp::~EditorApp() {
        gz_info("Tearing down Editor App");
    }
    
    void EditorApp::on_init() {
        gz_info("Init Editor App");
        // Should register any system
    }
    
    void EditorApp::on_update() {
        // actually not much to do here
        
        // just tick system to run or stop the scene
    }
    
    void EditorApp::on_imgui_render() {
        // render editor stuff
    }


}

GZ::App *GZ::create_application() {
	AppSpec spec;
	spec.headless = false;
	spec.window_height = 1080;
	spec.window_width = 1960;
	spec.name = "New App";
	return new EditorApp(spec);
}
