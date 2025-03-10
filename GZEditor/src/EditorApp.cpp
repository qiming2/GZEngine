#include <GZ_include.h>
#include <defines.h>

namespace ed = ax::NodeEditor;


namespace GZ {

	struct EditorApp : public App {
	public:
		explicit EditorApp(const AppSpec &spec):
			App(spec)
		{
			// App init is done
			// Editor setup

			// push any layer

			// layer should do system and other init			
		}

		virtual ~EditorApp() override {
			
		}

	private:
		// Most of these setup should be handled by engine
		
		
	};
}

GZ::App *GZ::create_application() {
	AppSpec spec;
	spec.headless = false;
	spec.window_height = 1960;
	spec.window_width = 1080;
	spec.name = "New App";
	return new EditorApp(spec);
}
