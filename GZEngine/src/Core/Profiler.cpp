#include <gzpch.h>
#include <SDL3/SDL.h>

#include "Log.h"
#include "Profiler.h"

namespace GZ {

	//Ex:
	/*
	* 
	* {
			ScopedProfiler("SomeFunc time");
			SomeFunc();
	  }
	* 
	m_profiler_instance_ref->get_perscope_frame_data([&](const PerScopeProfilerData& data) {
			gz_info("{} spent {} ", data.name, get_s_from_ns(data.measured_time));
		});
	*/

	Profiler Profiler::g_profiler_instance;
	 
	void Profiler::init()
	{
		Profiler::g_profiler_instance = Profiler();
		g_profiler_instance.m_last_profiling_time = SDL_GetTicksNS();
	}

	void Profiler::is_profiling_cur_frame() {
		if (m_is_profiling_everyframe) {
			m_is_profiling_cur_frame = true;
			return;
		}
		u64 cur_profiling_time = SDL_GetTicksNS();
		u64 elapsed_time = cur_profiling_time - m_last_profiling_time + m_accumulated_time;
		if (elapsed_time > m_wait_time) {
			m_accumulated_time = elapsed_time - m_wait_time;
			m_last_profiling_time = cur_profiling_time;
			m_is_profiling_cur_frame = true;
		}

	}

	void Profiler::begin_frame()
	{
		is_profiling_cur_frame();
		if (!m_is_profiling_cur_frame) return;
		// Using double buffer
		m_cur_frame_index = (m_cur_frame_index + 1) % MAX_PROFILER_FRAMES_IN_FLIGHT;
		PerframeProfilerData &cur_frame_profiler_data = m_perframe_profiler_data[m_cur_frame_index];
		gz_core_assert(cur_frame_profiler_data.measured_time > 0 && !cur_frame_profiler_data.is_started, "Did you call Profiler::g_profiler_instance::start_frame() twice or you forgot to close last start frame with end_frame() call?");
		cur_frame_profiler_data.is_started = true;
		cur_frame_profiler_data.measured_time = SDL_GetTicksNS();

		// Reset slot index
		m_slot_index[m_cur_frame_index] = 0;

		// Reset name map
		m_profile_name_to_index.clear();
	}
	
	void Profiler::end_frame()
	{
		if (!m_is_profiling_cur_frame) return;

		PerframeProfilerData &cur_frame_profiler_data = m_perframe_profiler_data[m_cur_frame_index];
		gz_core_assert(cur_frame_profiler_data.is_started, "Did you call Profiler::g_profiler_instance::start_frame() ?");
		cur_frame_profiler_data.is_started = false;
		cur_frame_profiler_data.measured_time = SDL_GetTicksNS() - cur_frame_profiler_data.measured_time;

		// no need to check for end of scoped profiler since only scoped profiler
		// can access the scope profiler func which should be implemented correctly
		m_is_profiling_cur_frame = false;
	}

	size_t Profiler::start_scope_frame(const std::string &name)
	{
		if (!m_is_profiling_cur_frame) return 0;
		std::array<PerScopeProfilerData, MAX_SCOPE_PROFILER> &cur_frame_perscope_profiler_data = m_perscope_profiler_data[m_cur_frame_index];
		gz_core_assert(cur_frame_perscope_profiler_data.size() > m_slot_index[m_cur_frame_index], "Not Enough Scope profiler data slot!");
		
		auto it = m_profile_name_to_index.find(name);
		size_t slot = 0;
		if (it == m_profile_name_to_index.end()) {
			slot = m_slot_index[m_cur_frame_index]++;
			m_profile_name_to_index[name] = slot;
			cur_frame_perscope_profiler_data[slot].measured_time = 0;
			cur_frame_perscope_profiler_data[slot].name = name;
		}
		else {
			slot = it->second;
		}

		gz_core_assert(cur_frame_perscope_profiler_data[slot].new_scope_start_time > 0 && !cur_frame_perscope_profiler_data[slot].is_started, "Did you call Profiler::g_profiler_instance::start_frame() twice or you forgot to close last start frame with end_frame() call?");
		
		cur_frame_perscope_profiler_data[slot].is_started = true;
		cur_frame_perscope_profiler_data[slot].new_scope_start_time = SDL_GetTicksNS();

		return slot;
	}

	void Profiler::end_scope_frame(size_t slot)
	{
		if (!m_is_profiling_cur_frame) return;
		std::array<PerScopeProfilerData, MAX_SCOPE_PROFILER> &cur_frame_perscope_profiler_data = m_perscope_profiler_data[m_cur_frame_index];
		gz_core_assert(cur_frame_perscope_profiler_data.size() > slot, "Index out of bound!");
		gz_core_assert(cur_frame_perscope_profiler_data[slot].is_started, "Did you call start_scope_frame() ?");
		cur_frame_perscope_profiler_data[slot].is_started = false;
		cur_frame_perscope_profiler_data[slot].measured_time += SDL_GetTicksNS() - cur_frame_perscope_profiler_data[slot].new_scope_start_time;
		cur_frame_perscope_profiler_data[slot].new_scope_start_time = 1;
	}
	
	GZ_FORCE_INLINE size_t get_last_frame_index(size_t p_cur_index) {
		return ((p_cur_index % MAX_PROFILER_FRAMES_IN_FLIGHT + MAX_PROFILER_FRAMES_IN_FLIGHT - 1) % MAX_PROFILER_FRAMES_IN_FLIGHT);
	}

	const PerframeProfilerData &Profiler::get_last_per_frame_data() const
	{
		size_t last_frame_index = get_last_frame_index(m_cur_frame_index);
		const PerframeProfilerData &last_per_frame_data = m_perframe_profiler_data[last_frame_index];
		gz_core_assert(last_per_frame_data.measured_time > 0 && !last_per_frame_data.is_started, "Did you call Profiler::g_profiler_instance::start_frame() twice or you forgot to close last start frame with end_frame() call?");
		return last_per_frame_data;
	}

	void Profiler::walk_last_perscope_frame_data(std::function<void(const PerScopeProfilerData &)> func) const
	{
		size_t last_frame_index = get_last_frame_index(m_cur_frame_index);
		const auto &last_perscope_profiler_data = m_perscope_profiler_data[last_frame_index];
		for (size_t i = 0; i < m_slot_index[last_frame_index]; ++i) {
			func(last_perscope_profiler_data[i]);
		}
	}

	const std::span<PerScopeProfilerData> Profiler::get_last_perscope_frame_data_span()
	{
		size_t last_frame_index = get_last_frame_index(m_cur_frame_index);
		auto &last_perscope_profiler_data = m_perscope_profiler_data[last_frame_index];
		std::span<PerScopeProfilerData> slice(last_perscope_profiler_data.data(), m_slot_index[last_frame_index]);
		return slice;
	}

	ScopedProfiler::ScopedProfiler(const std::string& scope_name)
	{
		m_given_slot = Profiler::g_profiler_instance.start_scope_frame(scope_name);
	}

	ScopedProfiler::~ScopedProfiler()
	{
		Profiler::g_profiler_instance.end_scope_frame(m_given_slot);
	}

}
