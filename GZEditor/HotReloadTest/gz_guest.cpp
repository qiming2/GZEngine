#include <cr.h>
#include <stdio.h>
#include <imgui.h>
#include <Log.h>

#include <App.h>

using namespace GZ;

namespace GZ {
    void gz_plugin_load(PluginData *data) {
        ImGui::SetCurrentContext(data->imgui_ctx);
    }
}

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation) {
    //assert(ctx);

    
    PluginData *data = (PluginData *)(ctx->userdata);
    switch (operation) {
        case CR_LOAD:  gz_plugin_load(data); return true; // loading back from a reload

        case CR_UNLOAD: return true; // preparing to a new reload
        case CR_CLOSE: return true; // the plugin will close and not reload anymore
    }
    
    /*gz_info("hello from the other side!");*/
    gz_info("hello but this time we are going to try imgui!");
    // no step, just give the function pointer
    // CR_STEP
    
	ImGui::Begin("Another Window1");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImGui::Text("Hello from another window!");
    ImGui::Text("Hello from another window second edition!");
	if (ImGui::Button("Tick Me"))
		gz_info("Oh yeah!");

    static vec3 color;
    ImGui::ColorEdit3("Some phacy stuff", &color[0]);
    ImGui::TextColored({0.7, 0.7, 0.1, 1.0}, "Yo yo how about some more fancy stuff");
    ImGui::TextColored({0.2, 0.5, 0.1, 1.0}, "Yo yo how about this fancy stuff");
	ImGui::End();

    
    return true;
}