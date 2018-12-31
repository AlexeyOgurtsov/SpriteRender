#pragma once

#include "QSpriteRender/INTERFACE/SpriteRenderSubsystemInitializer.h"

namespace Eng::Svc
{
	class IServiceContainer;
} // Eng::Svc

namespace Tet
{
	void RegSvc_QSpriteRender(const ::Dv::Spr::QRen::SSpriteRenderSubsystemInitializer& InInitializer, Eng::Svc::IServiceContainer* pInSvcContainer);
} // Tet