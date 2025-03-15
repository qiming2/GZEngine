#pragma once
#include <GZ_Include.h>
#include <defines.h>

namespace ed = ax::NodeEditor;


namespace GZ {

	struct EditorApp : public App {
	public:
		explicit EditorApp(const AppSpec &spec);

		virtual ~EditorApp() override;

        virtual void on_init() override;
        
        virtual void on_update() override;
        
        virtual void on_imgui_render() override;

	private:
		// Most of these setup should be handled by engine
		
		
	};
}
