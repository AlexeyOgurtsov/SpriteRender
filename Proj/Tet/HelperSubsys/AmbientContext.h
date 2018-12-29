#pragma once

#include "Log/ILog.h"
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
		AmbientContext(ILog* pInLog)
		:	pLog{pInLog} 
		{
			BOOST_ASSERT(pLog);
		}

		ILog* GetLog() const { return pLog; }

	private:
		ILog* pLog = nullptr;
	};
} // Tet