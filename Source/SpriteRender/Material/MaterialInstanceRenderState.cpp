#include "MaterialInstanceRenderState.h"
#include "SpriteMaterial.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

MaterialInstanceRenderState::MaterialInstanceRenderState(IMaterial* pInMaterial) :
	SpriteMaterialInstanceRenderState(pInMaterial->GetId())
,	_pMaterial(pInMaterial)
{
	BOOST_ASSERT(pInMaterial);
}

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv