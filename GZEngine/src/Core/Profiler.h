#pragma once
#include <array>

#define SCOPE_PROFILER_MAX 100
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
	struct Profiler {
		
		static std::shared_ptr<Profiler> g_profiler_instance;
		static void init();
		void start_frame();
		void end_frame();
		size_t start_scope_frame(const std::string &name);
		void end_scope_frame(size_t);
		const PerframeProfilerData & get_per_frame_data() const;
		void get_perscope_frame_data(std::function<void(const PerScopeProfilerData &)> func) const;
		const std::span<PerScopeProfilerData> get_perscope_frame_data_span();

		// This is very important for scoped data
		void clear();
	private:
		std::array<PerScopeProfilerData, SCOPE_PROFILER_MAX> m_perscope_profiler_data;
		PerframeProfilerData m_perframe_profiler_data;
	private:
		size_t m_slot_index = 0;
	};
	
	struct ScopedProfiler {
		ScopedProfiler(const std::string &scope_name);
		~ScopedProfiler();
	private:
		size_t m_given_slot;
	};
	
}
