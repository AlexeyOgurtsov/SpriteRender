#include "QPC.h"
#include "../../Utils/TestUtils.h"

namespace Test::IMPL
{
	void LogLargeInteger(std::ofstream& InLog, const char* InLegend, const LARGE_INTEGER& InValue)
	{
		T_LOG_TO(InLog, "LargeInteger " << InLegend << ": High=" << InValue.HighPart << " Low=" << InValue.LowPart);
	}


	void DoQueryPerfCounter(std::ofstream& InLog, const char* pInLegend, LARGE_INTEGER* pOutInteger)
	{
		BOOL bSucc = QueryPerformanceCounter(pOutInteger);
		LogLargeInteger(InLog, pInLegend, *pOutInteger);
		BOOST_ASSERT_MSG(bSucc, "QueryPerformanceCounter: Must succeed");
	}

	float ComputeQPCDeltaSeconds(std::ofstream& InLog, const char* pInLegend, const LARGE_INTEGER* pLast, const LARGE_INTEGER& InFrequency)
	{
		BOOST_ASSERT(pLast);
		BOOST_ASSERT(pInLegend);

		LARGE_INTEGER QPCNow;
		DoQueryPerfCounter(InLog, "QPCNow", &QPCNow);

		float DeltaCounter = static_cast<float>(QPCNow.LowPart - pLast->LowPart);
		float DeltaSeconds = DeltaCounter / InFrequency.LowPart;
		T_LOG_TO(InLog, pInLegend << ": " << DeltaSeconds);

		return DeltaSeconds;
	}

	float QPC::GetDeltaSeconds() const 
	{
		BOOST_ASSERT_MSG(bInitialized, "QPC::GetDeltaSeconds: must be initialized");

		return DeltaSeconds; 
	}

	void QPC::Reset(std::ofstream& InLog)
	{
		T_LOG_TO(InLog, "QPC::Reset...");

		BOOST_ASSERT_MSG( ! bPaused, "QPC::Reset: unable to reset in pause state");

		BOOL bFreqOk = QueryPerformanceFrequency(&Frequency);
		BOOST_ASSERT_MSG(bFreqOk, "QueryPerformanceFrequency: must succeed");
		LogLargeInteger(InLog, "Frequency", Frequency);
		
		FixLast(InLog);
		DeltaSeconds = 0.0F;
		TotalPausedDeltaSeconds = 0.0F;

		bInitialized = true;

		T_LOG_TO(InLog, "QPC::Reset DONE");	
	}

	void QPC::FixLast(std::ofstream& InLog)
	{
		DoQueryPerfCounter(InLog, "FixLast", &Last);
	}

	void QPC::Pause(std::ofstream& InLog)
	{
		T_LOG_TO(InLog, "QPC::Pause...");
		BOOST_ASSERT_MSG( bInitialized, "QPC::Pause: Must be initialized");
		BOOST_ASSERT_MSG( ! bPaused, "QPC::Pause: already paused!");

		DoQueryPerfCounter(InLog, "PauseTime", &PauseTime);
		bPaused = true;
		
		T_LOG_TO(InLog, "QPC::Pause DONE");
	}

	void QPC::Resume(std::ofstream& InLog)
	{
		T_LOG_TO(InLog, "QPC::Resume...");
		BOOST_ASSERT_MSG( bInitialized, "QPC::Resume: Must be initialized");
		BOOST_ASSERT_MSG( bPaused, "QPC::Resume: Must be paused to resume!");
		
		float PausedDeltaSeconds = ComputeQPCDeltaSeconds(InLog, "PausedDeltaSeconds", &Last, Frequency);
		TotalPausedDeltaSeconds += PausedDeltaSeconds;
		T_LOG_TO(InLog, "TotalPausedDeltaSeconds: " << TotalPausedDeltaSeconds);

		bPaused = false;

		T_LOG_TO(InLog, "QPC::Resume DONE");
	}

	void QPC::Fix(std::ofstream& InLog)
	{
		T_LOG_TO(InLog, "QPC::Fix...");
		BOOST_ASSERT_MSG( bInitialized, "QPC::Pause: Must be initialized");
		BOOST_ASSERT_MSG( ! bPaused, "QPC::Pause: Must not be paused!");

		float RealTimeDeltaSeconds = ComputeQPCDeltaSeconds(InLog, "RealTimeDeltaSeconds", &PauseTime, Frequency);
		float SimDeltaSeconds = RealTimeDeltaSeconds - TotalPausedDeltaSeconds;
		T_LOG_TO(InLog, "TotalPausedDeltaSeconds: " << TotalPausedDeltaSeconds);
		T_LOG_TO(InLog, "SimDeltaSeconds: " << SimDeltaSeconds);

		TotalPausedDeltaSeconds = 0.0F;

		FixLast(InLog);

		T_LOG_TO(InLog, "QPC::Fix DONE");
	}
} // Test::IMPL