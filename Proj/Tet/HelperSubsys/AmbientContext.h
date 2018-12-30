#pragma once

#include "Log/ILog.h"
#include "ConfigManager/ConfigManager.h"
#include <boost/assert.hpp>

namespace Tet
{
	/**
	* Contains all environment objects typically to be accessible from almost any subsystem.
	*/
	class AmbientContext
	{
	public:
		AmbientContext() = default;
		AmbientContext
		(
			ILog* pInLog,
			CfgManager* pInCfgManager
		)
		:	pLog{pInLog} 
		,	pCfgManager{pInCfgManager}
		{
			BOOST_ASSERT(pLog);
			BOOST_ASSERT(pCfgManager);
		}

		ILog* GetLog() const { return pLog; }
		CfgManager* GetCfgManager() const { return pCfgManager; }

	private:
		ILog* pLog = nullptr;
		CfgManager* pCfgManager = nullptr;
	};
} // Tet