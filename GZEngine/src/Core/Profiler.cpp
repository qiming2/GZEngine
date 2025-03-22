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

	std::shared_ptr<Profiler> Profiler::g_profiler_instance;
	 
	void Profiler::init()
	{
		Profiler::g_profiler_instance = std::make_shared<Profiler>();
	}

	void Profiler::start_frame()
	{
		gz_core_assert(m_perframe_profiler_data.measured_time > 0 && !m_perframe_profiler_data.is_started, "Did you call Profiler::g_profiler_instance::start_frame() twice or you forgot to close last start frame with end_frame() call?");
		m_perframe_profiler_data.is_started = true;
		m_perframe_profiler_data.measured_time = SDL_GetTicksNS();
	}
	
	void Profiler::end_frame()
	{
		gz_core_assert(m_perframe_profiler_data.is_started, "Did you call Profiler::g_profiler_instance::start_frame() ?");
		m_perframe_profiler_data.is_started = false;
		m_perframe_profiler_data.measured_time = SDL_GetTicksNS() - m_perframe_profiler_data.measured_time;
	}
	
	void Profiler::clear() {
		m_slot_index = 0;
	}

	size_t Profiler::start_scope_frame(const std::string &name)
	{
		gz_core_assert(m_perscope_profiler_data.size() > m_slot_index, "Not Enough Scope profiler data slot!");

		size_t slot = m_slot_index++;
		gz_core_assert(m_perscope_profiler_data[slot].measured_time > 0 && !m_perscope_profiler_data[slot].is_started, "Did you call Profiler::g_profiler_instance::start_frame() twice or you forgot to close last start frame with end_frame() call?");
		
		m_perscope_profiler_data[slot].is_started = true;
		m_perscope_profiler_data[slot].measured_time = SDL_GetTicksNS();
		m_perscope_profiler_data[slot].name = name;

		return slot;
	}

	void Profiler::end_scope_frame(size_t slot)
	{
		gz_core_assert(m_perscope_profiler_data.size() > slot, "Index out of bound!");
		gz_core_assert(m_perscope_profiler_data[slot].is_started, "Did you call start_scope_frame() ?");
		m_perscope_profiler_data[slot].is_started = false;
		m_perscope_profiler_data[slot].measured_time = SDL_GetTicksNS() - m_perscope_profiler_data[slot].measured_time;
	}

	const PerframeProfilerData &Profiler::get_per_frame_data() const
	{
		gz_core_assert(m_perframe_profiler_data.measured_time > 0 && !m_perframe_profiler_data.is_started, "Did you call Profiler::g_profiler_instance::start_frame() twice or you forgot to close last start frame with end_frame() call?");
		return m_perframe_profiler_data;
	}

	void Profiler::get_perscope_frame_data(std::function<void(const PerScopeProfilerData &)> func) const
	{
		for (size_t i = 0; i < m_slot_index; ++i) {
			func(m_perscope_profiler_data[i]);
		}
	}

	const std::span<PerScopeProfilerData> Profiler::get_perscope_frame_data_span()
	{
		std::span<PerScopeProfilerData> slice(m_perscope_profiler_data.data(), m_slot_index);
		return slice;
	}

	ScopedProfiler::ScopedProfiler(const std::string& scope_name)
	{
		m_given_slot = Profiler::g_profiler_instance->start_scope_frame(scope_name);
	}

	ScopedProfiler::~ScopedProfiler()
	{
		Profiler::g_profiler_instance->end_scope_frame(m_given_slot);
	}

}