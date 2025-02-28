#ifdef GZ_PLATFORM_WINDOWS
	#ifdef GZ_BUILD_DLL
		#define GZ_API __declspec(dllexport)
	#else
		#define GZ_API __declspec(dllimport)
	#endif
#else
	#error "Platform not supported yet!"
#endif