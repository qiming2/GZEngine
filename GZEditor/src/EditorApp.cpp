#include <GZ_Include.h>
#include <defines.h>

#include "EditorApp.h"
#include "CommonModule.h"

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

		//// Node editor
		//ed::Config config;
		//config.SettingsFile = "Simple.json";
		//m_node_Context = ed::CreateEditor(&config);

  //      gz_info("Init flecs entity", "");
  //      
        auto ent = world.entity().set<TransformComponent>({vec3(1.0), vec3(1.0), vec3(1.0)});
        
       /* const TransformComponent *ent_transform = (ent.get<TransformComponent>());
        const TransformComponent t{vec3(1.0f), vec3(2.0f), vec3(1.0f)};
        gz_info("Entity id: {}", ent.id());*/
        //gz_info("Entity transform: {}",  (* ent_transform));
        
    }

    void EditorApp::on_update(const FrameData &frame_data) {
        // actually not much to do here
        
        // just tick system to run or stop the scene
        private_set_frame_data(frame_data);
        
        
        
    }

    void EditorApp::on_imgui_render() {
        // render editor stuff

    }


	void EditorApp::private_set_frame_data(const FrameData& frame_data)
	{
        m_cur_frame_data = frame_data;
	}

}


/////////////////////////////////// Entry point function //////////////////////////////////////////
GZ::App *GZ::create_application() {
	AppSpec spec;
	spec.headless = false;
	spec.window_height = 1080;
	spec.window_width = 1960;
    spec.is_fullscreen = false;
	spec.name = "Lost Tower Rogue Like";
    
	return new EditorApp(spec);
}
