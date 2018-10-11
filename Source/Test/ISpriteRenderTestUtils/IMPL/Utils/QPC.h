#pragma once

#include <Windows.h>
#include <fstream>

namespace Test::IMPL
{
	class QPC
	{
	public:
		float GetDeltaSeconds() const;
		void Reset(std::ofstream& InLog);
		void Pause(std::ofstream& InLog);
		void Resume(std::ofstream& InLog);
		void Fix(std::ofstream& InLog);
		bool IsPaused() const { return bPaused; }
		bool IsInitialized() const { return bInitialized; }
		const LARGE_INTEGER& GetFrequency() const { return Frequency; }

	private:
		void FixLast(std::ofstream& InLog);

		LARGE_INTEGER Frequency;

		/**
		* Time of the last "Fix" call.
		*/
		LARGE_INTEGER Last;

		/**
		* Time of the last "Pause" call.
		*/

		LARGE_INTEGER PauseTime;
		float DeltaSeconds = 0.0F;
		float TotalPausedDeltaSeconds = 0.0F;
		bool bPaused = false;
		bool bInitialized = false;		
	};
} // Test::IMPL