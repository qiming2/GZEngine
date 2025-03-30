#pragma once
#include <imgui.h>
namespace GZ {
// Add another style maybe
	static void SetupImGuiStyle()
	{
		// Primary style from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 1.0f;
		style.WindowPadding = ImVec2(10.0f, 10.0f);
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 0.0f;
		style.WindowMinSize = ImVec2(20.0f, 20.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_None;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.PopupRounding = 3.0f;
		style.PopupBorderSize = 0.0f;
		style.FramePadding = ImVec2(10.0f, 5.0f);
		style.FrameRounding = 3.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(6.0f, 6.0f);
		style.ItemInnerSpacing = ImVec2(6.0f, 3.0f);
		style.CellPadding = ImVec2(6.0f, 10.0f);
		style.IndentSpacing = 20.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 12.0f;
		style.ScrollbarRounding = 2.0f;
		style.GrabMinSize = 7.0f;
		style.GrabRounding = 1.0f;
		style.TabRounding = 3.0f;
		style.TabBorderSize = 0.0f;
		style.TabCloseButtonMinWidthUnselected = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

#if 1
		// in srgb space
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.249111220240593f, 0.3344880044460297f, 0.4206008315086365f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.07058823853731155f, 0.07058823853731155f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.1094144284725189f, 0.1320907771587372f, 0.1416308879852295f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1176470592617989f, 0.1176470592617989f, 0.1176470592617989f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3882353007793427f, 0.1522491276264191f, 0.1522491276264191f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.6784313917160034f, 0.2660515010356903f, 0.2660515010356903f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.04291844367980957f, 0.04236584529280663f, 0.04236584529280663f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9882352948188782f, 0.3875432312488556f, 0.3875432312488556f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3882353007793427f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.6784313917160034f, 0.2666666805744171f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3882353007793427f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.6784313917160034f, 0.2666666805744171f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.3882353007793427f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.6784313917160034f, 0.2666666805744171f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3882353007793427f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.6784313917160034f, 0.2666666805744171f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.4134539067745209f, 0.7467811107635498f, 0.7339057922363281f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.7669611573219299f, 0.7882353067398071f, 0.4265743792057037f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9847311973571777f, 1.0f, 0.7124463319778442f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.6784313917160034f, 0.2666666805744171f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 0.501960813999176f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.9882352948188782f, 0.3882353007793427f, 0.3882353007793427f, 0.501960813999176f);
#else
		// in linear space
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.051269f, 0.089437f, 0.141630f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.005497f, 0.005497f, 0.005497f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.007768f, 0.008856f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.007768f, 0.008856f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.012038f, 0.016807f, 0.019349f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.007768f, 0.008856f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.012038f, 0.012038f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.117647f, 0.020661f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.007768f, 0.008856f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.009766f, 0.010768f, 0.013856f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.002130f, 0.002089f, 0.002089f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.020661f, 0.024771f, 0.033104f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.970085f, 0.117647f, 0.117647f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.970085f, 0.117647f, 0.117647f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.117647f, 0.020661f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.117647f, 0.020661f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.020661f, 0.027451f, 0.050980f, 1.0f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.020661f, 0.027451f, 0.050980f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.020661f, 0.027451f, 0.050980f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.117647f, 0.020661f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.117647f, 0.020661f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.007768f, 0.008856f, 0.012038f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.141630f, 0.478431f, 0.466667f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.003130f, 0.960784f, 0.960784f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.537254f, 0.564706f, 0.152941f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.970085f, 1.0f, 0.466667f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.003906f, 0.004764f, 0.007768f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.012038f, 0.015686f, 0.020661f, 1.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.009766f, 0.010768f, 0.013856f, 1.0f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.385499f, 0.054902f, 0.054902f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.970085f, 0.117647f, 0.117647f, 1.0f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.970085f, 0.117647f, 0.117647f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.970085f, 0.117647f, 0.117647f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.970085f, 0.117647f, 0.117647f, 0.501961f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.970085f, 0.117647f, 0.117647f, 0.501961f);
#endif

		// Custom ones that are not exported from Imthemes
		style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
		style.ImageBorderSize = 3.0f;
		style.TabBarOverlineSize = 0.0f;
		style.TabBarBorderSize = 1.0f;
	}
}