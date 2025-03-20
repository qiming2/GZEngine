#include <cstdint>


#if GZ_MAKE_DLL
#   ifdef _MSC_VER
#       define GZ_API __declspec(dllexport)
#   else
#       define GZ_API __attribute__((visibility("default")))
#   endif
#else
#   ifdef _MSC_VER
#       define GZ_API __declspec(dllimport)
#   else
#       define GZ_API
#   endif
#endif
namespace GZ {
	// unsigned int
	using u64 = std::uint64_t;
	using u32 = std::uint32_t;
	using u16 = std::uint16_t;
	using u8 = std::uint8_t;

	// singed int
	using i64 = std::int64_t;
	using i32 = std::int32_t;
	using i16 = std::int16_t;
	using i8 = std::int8_t;

	// bool
	using b8 = bool;

	// floating point
	using f32 = float;
	using f64 = double;

	static_assert(sizeof(u64) == 8, "u64 should be 8 byte(s)");
	static_assert(sizeof(u32) == 4, "u32 should be 4 byte(s)");
	static_assert(sizeof(u16) == 2, "u16 should be 2 byte(s)");
	static_assert(sizeof(u8)  == 1, "u8  should be 1 byte(s)");

	static_assert(sizeof(i64) == 8, "i64 should be 8 byte(s)");
	static_assert(sizeof(i32) == 4, "i32 should be 4 byte(s)");
	static_assert(sizeof(i16) == 2, "i16 should be 2 byte(s)");
	static_assert(sizeof(i8)  == 1,  "i8  should be 1 byte(s)");

	static_assert(sizeof(b8)  == 1,  "b8  should 1 byte(s)");
	static_assert(sizeof(f32) == 4, "f32 should 4 byte(s)");
	static_assert(sizeof(f64) == 8, "f64 should 8 byte(s)");
}
