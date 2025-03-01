#include <include/GZ_Include.h>
#include <SDL3/SDL.h>

namespace GZ {

	class EditorApp : public GZApp {
	public:
		EditorApp() {
			if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
				gz_error("SDL init failed!");
				exit(1);
			}
			
			window = SDL_CreateWindow("GZ Editor", 1960, 1080, SDL_WINDOW_RESIZABLE);
			if (window == nullptr) {
				gz_error("SDL Create window Failed!: ", SDL_GetError());
				exit(1);
			}	
			
		}

		virtual ~EditorApp() override {
			SDL_Quit();
		}

		virtual void run() override {

			SDL_Event e;
			while (is_running) {
				
				while (SDL_PollEvent(&e)) {
					switch (e.type) {

					case SDL_EVENT_QUIT:
						gz_info("Quiting...");
						is_running = false;
						break;
					case SDL_EVENT_KEY_DOWN:

						gz_info("Keydown:{}", SDL_GetKeyName(e.key.key));
						if (e.key.key == SDLK_ESCAPE) {
							is_running = false;
						}
						break;
					}
				}

				SDL_Delay(100);
				
			}
			
		}
	private:
		SDL_Window *window = nullptr;
		bool is_running = true;
	};
}

GZ::GZApp *GZ::create_application() {
	return new EditorApp();
}