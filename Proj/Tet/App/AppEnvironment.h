#pragma once

#include "../HelperSubsys/AmbientContext.h"

namespace Tet
{
	/**
	* Contains references to all environment objects of the application.
	*
	* Typically should be used only inside the root-level application objects,
	* and should be narrowed for concrete application subsystems.
	*/
	class AppEnvironment
	{
	public:
		AmbientContext *GetAmbient() { return &Ambient; }
		void SetAmbient(const AmbientContext& InAmbient) { Ambient = InAmbient; }

	private:
		AmbientContext Ambient;
	};
} // Tet