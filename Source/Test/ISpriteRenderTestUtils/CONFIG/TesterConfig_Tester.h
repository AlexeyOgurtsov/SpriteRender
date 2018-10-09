#pragma once

namespace Test
{
	enum class ETestPresenation
	{
		// Do NOT present anything - test as quickly as you can
		NonStop,

		// Delay after each test for the time specified in the settings
		Delay
	};

	/**
	* Config of the testing process itself.
	* Should we delay, show test messages etc.?
	*/
	struct TesterConfig_Tester
	{
		/**
		* Presentation method.
		*/
		ETestPresenation Presentation = ETestPresenation::Delay;

		/**
		* Time to stop for each test (@see ETestPresenation)
		*/
		float DelaySeconds = 5.0F;

		/**
		* Should we show message before presenting each test?
		*/
		bool bShowMessageBeforeTest = true;
	};
} // Test