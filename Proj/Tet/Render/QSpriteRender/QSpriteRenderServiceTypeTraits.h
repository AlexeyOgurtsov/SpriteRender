#pragma once

#include "Util/ServiceContainer/ServiceTypeTraits.h"
#include "QSpriteRenderServiceTypes.h"
#include "QSpriteRender.h"

namespace Tet
{
	struct QSpriteRenderServiceInstanceType : public Eng::Svc::TServiceInstanceType<Tet::QSpriteRenderSubsystem>
	{
		QSpriteRenderServiceInstanceType() :
			Eng::Svc::TServiceInstanceType<Tet::QSpriteRenderSubsystem> { Tet::SvcName_QSpriteRender }
		{
		}
	};

	struct QSpriteRenderServiceTypeTraits : public Eng::Svc::TServiceTypeTraits<Tet::QSpriteRenderSubsystem>
	{
		QSpriteRenderServiceTypeTraits() : 
			Eng::Svc::TServiceTypeTraits<Tet::QSpriteRenderSubsystem> { Tet::SvcName_QSpriteRender }
		{
		}
	};
} // Tet