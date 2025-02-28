#include <stdio.h>
#include <GZ_Include.h>

namespace GZ {

	class EditorApp : public GZApp {
	public:
		EditorApp() {

		}
		
		virtual ~EditorApp() override {

		}

		virtual void run() override {
			printf("Hello From Run!\n");
		}
	};
}

GZ::GZApp *GZ::create_application() {
	return new EditorApp();
}