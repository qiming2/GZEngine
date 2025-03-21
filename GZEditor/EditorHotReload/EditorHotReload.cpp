#include <cr.h>
#include <stdio.h>
#include <imgui.h>
#include <Log.h>

#include <CommonModule.h>
#include <App.h>

namespace GZ {
    void editor_plugin_load(const PluginData *data) {
        ImGui::SetCurrentContext(data->imgui_ctx);

        // Trying init some component and system
        
    }

    //static CommonInterface<Component> *comp_interface = new CommonInterface<TransformComponent>();
	void editor_plugin_update(const PluginData *data) {
       
        ImGui::Begin("Another Window1");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        ImGui::Text("Hello from another window second edition!");
        if (ImGui::Button("Tick Me"))
            gz_info("Oh yeah asd sa!");

        auto e1 = data->world->lookup("Hello");
        
		/*const char* a = GZ_COMPONENT_TYPE_IMPL_DRAW(Hello);
		gz_info(a);*/
        //gz_info(e1.id());

        World *world = data->world;
        ComponentRegistry *reg = data->reg;
        //ComponentID vec3_id = world->component<vec3>().id();
        ///*IDrawComponentInterface *compInter = new DrawComponentImplTransformComponent();*/
        //std::shared_ptr<IDrawComponentInterfaceName> compInter = reg->draw_component_interfaces[vec3_id];
        TransformComponent *transform = e1.get_mut<TransformComponent>();
        ImGui::TextColored({0.5, 0.7, 0.5, 1.0}, "Hello");
        ImGui::TextColored({0.5, 0.7, 0.5, 1.0}, "2123");
        //draw_component_imgui_ui_transform(transform);
        DrawComponentContext ctx;
        ////ctx.name = "position";
        //compInter->draw_imgui(&transform->p, reg, world, &ctx);

        ctx.name = "Model Matrix";
        reg->get_draw_interface(world->component<TransformComponent>().id())->draw_imgui(transform, reg, world, &ctx);
        //comp_interface->draw_second_stuff(transform);
        
        static vec3 color;
        ImGui::ColorEdit3("Some phacy stuff", &color[0]);
        ImGui::TextColored({0.5, 0.7, 0.5, 1.0}, "Yo yo how about some more fancy stuff");
        ImGui::TextColored({0.2, 0.3, 0.1, 1.0}, "Yo yo how about this fancy stuff");
        ImGui::TextColored({0.2, 0.3, 0.1, 1.0}, "Yo yo this fancy stuff");



        ImGui::End();
    }
}

using namespace GZ;

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation) {
    //assert(ctx);

    
    PluginData *data = (PluginData *)(ctx->userdata);
    switch (operation) {
        case CR_LOAD:  editor_plugin_load(data); return true; // loading back from a reload

        case CR_STEP:
            editor_plugin_update(data);
            return true;
        case CR_UNLOAD: return true; // preparing to a new reload
        case CR_CLOSE: return true; // the plugin will close and not reload anymore
    }

    
    return true;
}
