#pragma once
#include "CommonInclude.h"

namespace ME {
	class Time
	{
	public:
		static void Intialize();
		static void Update();
		static void Render();

		__forceinline static float DeltaTime() { return DeltaTimeValue; }

	private:
		static LARGE_INTEGER CpuFrequency;
		static LARGE_INTEGER PrevFrequency;
		static LARGE_INTEGER CurrentFrequency;
		static float DeltaTimeValue;
	};
}

