#pragma once

#include "ISprite/SpriteMaterialTypedefs.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{

namespace IMPL
{
	class IMaterial;
}

/**
* Exposes material operations available to the client of the sprite render subsystem library.
*/
class ISpriteMaterialManager
{
public:
	virtual ~ISpriteMaterialManager() = default;

	/**
	* Registers the given material, so it's to be available to the FindById commands.
	* Id is taken from the material itself.
	* The material will be owned and must be created with the C++ new operator.
	*/
	virtual void Register(IMPL::IMaterial* pInMaterial) = 0;

	/**
	* Tries to find the given material by id.
	* Throws the exception if material with the given id is NOT registered.
	*/
	virtual IMPL::IMaterial* FindById(SpriteMaterialId InId) const = 0;
};

} // Dv::Spr::Ren
} // Dv::Spr
} // Dv