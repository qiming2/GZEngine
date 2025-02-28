#ifndef GZ_APP_H
#define GZ_APP_H

#include "Core.h"
namespace GZ {
	class GZ_API GZApp {
		
	public:
		GZApp() = default;

		virtual ~GZApp() {};

		void virtual run();
	private:

	};
}

#endif