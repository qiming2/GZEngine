#pragma once
#include <gzpch.h>

// ECSModule interface declaration
namespace GZ {
	struct ComponentRegistry;
	struct Module {

		void virtual install_into(World &world, ComponentRegistry &reg) = 0;

		void virtual uninstall_from(World &world, ComponentRegistry &reg) = 0;
	};
}