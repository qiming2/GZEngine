#include <test.h>
#include <windows.h>

namespace GZEngine {
	__declspec(dllimport) void Print();
}

 int main ( int argc, char* argv[] ) {
	GZEngine::Print();
	return 0;
}