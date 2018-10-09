#pragma once

#include "ISpriteRender/SpriteMaterialInstanceRenderState.h"
#include <memory>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

class IMaterial;

/**
* This particular class is to be used as a material instance render state
* anywhere in the SpriteRender code.
*/
class MaterialInstanceRenderState : public SpriteMaterialInstanceRenderState
{
public:
	MaterialInstanceRenderState(IMaterial* pInMaterial);

	/**
	* Get the material, associated with the given material state
	*/
	IMaterial* GetMaterial() const { return _pMaterial; }

private:
	IMaterial* _pMaterial;
};
using MaterialInstanceRenderStateHandle = std::shared_ptr<MaterialInstanceRenderState>;

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv