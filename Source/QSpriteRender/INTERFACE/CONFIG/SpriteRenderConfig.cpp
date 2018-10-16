#include "SpriteRenderConfig.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{

SConfig::SConfig(const SMinimalConfig& InMinimalConfig) :
	Log{InMinimalConfig.pLog}
,	Device{ InMinimalConfig.pDev, InMinimalConfig.pDevCon }
,	RenderTarget{ InMinimalConfig.RTWidth, InMinimalConfig.RTHeight, InMinimalConfig.pRTView, InMinimalConfig.pDepthStencilView }
,	Shaders{ InMinimalConfig.ShaderOutputDirectory }
,	InitialSpriteBufferCapacity{ INITIAL_SPRITE_BUFFER_CAPACITY }
{
}

} // Dv::Spr::QRen
} // DV::Spr
} // Dv