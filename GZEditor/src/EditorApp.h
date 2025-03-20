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
        
        virtual void on_update(const FrameData &frame_data) override;
        
        void on_imgui_render();
	private:
		void private_set_frame_data(const FrameData &frame_data);

	private: // Transient data
		FrameData m_cur_frame_data;

	private:
		// Most of these setup should be handled by engine
		b8 m_show_demo_window = true;
		b8 m_show_another_window = true;
		b8 m_show_node_editor = true;
		b8 m_show_main_scene = true;

		u32 m_main_view_w = 0, m_main_view_h = 0;

		vec4 m_clear_color = vec4(0.18f, 0.18f, 0.18f, 1.00f);

		ed::EditorContext* m_node_Context = nullptr;
	};
}
