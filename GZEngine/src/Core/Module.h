#pragma once
#include <gzpch.h>

// ECSModule interface declaration
namespace GZ {
	struct Module {

		void virtual install_into(World &world) = 0;

		void virtual uninstall_from(World &world) = 0;
	};
}