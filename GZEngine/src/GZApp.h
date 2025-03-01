#ifndef GZ_APP_H
#define GZ_APP_H

namespace GZ {
	class GZApp {
		
	public:
		GZApp() = default;

		virtual ~GZApp() {};

		void virtual run();
	private:

	};
}

#endif