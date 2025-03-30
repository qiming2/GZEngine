#pragma once
#include <array>

#define MAX_SCOPE_PROFILER 100
#define MAX_PROFILER_FRAMES_IN_FLIGHT 2
namespace GZ {

	// Don't change these default!
	// This is a crude estimation of per frame time
	// since it includes a lot of unrelated execution and code path
	// that probably won't get into the actually game like imgui
	struct PerframeProfilerData {
		u64 measured_time = 1; // initialize as a non 0 val for bootstrap profiler
		b8 is_started = false;
	};

	// This is a more accurate data that users can use
	struct PerScopeProfilerData {
		std::string name;
		u64 measured_time = 1;
		b8 is_started = false;
	};

	// We just hard code a number since we can just increase this when we need to
	// we don't expose the core profiler, we just expose ScopedProfiler to user
	
	// Once end_frame is called, all statistics would be available for
	// use
	struct Profiler {
		
		static Profiler g_profiler_instance;
		static void init();
		void begin_frame();
		void end_frame();
		
		GZ_API const PerframeProfilerData & get_last_per_frame_data() const;
		GZ_API void walk_last_perscope_frame_data(std::function<void(const PerScopeProfilerData &)> func) const;
		GZ_API const std::span<PerScopeProfilerData> get_last_perscope_frame_data_span();
	private:
		size_t start_scope_frame(const std::string& name);
		void end_scope_frame(size_t);
		friend class ScopedProfiler;
	private:
		std::array<PerScopeProfilerData, MAX_SCOPE_PROFILER> m_perscope_profiler_data[MAX_PROFILER_FRAMES_IN_FLIGHT];
		PerframeProfilerData m_perframe_profiler_data[MAX_PROFILER_FRAMES_IN_FLIGHT];
	private:
		size_t m_slot_index[MAX_PROFILER_FRAMES_IN_FLIGHT] = {0, 0};
		size_t m_cur_frame_index = 1; // start with 1
	};
	
	struct ScopedProfiler {
		ScopedProfiler(const std::string &scope_name);
		~ScopedProfiler();
	private:
		size_t m_given_slot;
	};
	
}
