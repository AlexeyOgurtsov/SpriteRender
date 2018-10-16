#include "MaterialInstanceRenderState.h"
#include "SpriteMaterial.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

MaterialInstanceRenderState::MaterialInstanceRenderState(IMaterial* pInMaterial) :
	Ren::SpriteMaterialInstanceRenderState(pInMaterial->GetId())
,	_pMaterial(pInMaterial)
{
	BOOST_ASSERT(pInMaterial);
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv