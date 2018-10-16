#include "MaterialManager.h"
#include "SpriteMaterial.h"
#include "ISprite/Core/SpriteRenderException.h"
#include "QSpriteRender/IMPL/Environment/AmbientContext.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

MaterialManager::MaterialManager(const SMaterialManagerInitializer& InInitializer) :
	_pConfig{ InInitializer.pConfig}
,	_pAmbientContext{ InInitializer.pAmbientContext }
,	_pDev { InInitializer.pDevice }
,	_ShaderCompileEnv
	{
		InInitializer.pDevice, 
		&InInitializer.pAmbientContext->GetLog(), 
		InInitializer.pConfig->CompileFlags, 
		InInitializer.pConfig->OutputDirectory.c_str()
	}
{
	BOOST_ASSERT(_pConfig);
	BOOST_ASSERT(_pAmbientContext);
	BOOST_ASSERT(_pDev);
}

void MaterialManager::Register(IMPL::IMaterial* pInMaterial)
{
	BOOST_ASSERT(pInMaterial);
	_materials.emplace(std::make_pair(pInMaterial->GetId(), std::unique_ptr<IMPL::IMaterial>(pInMaterial)));
}

IMPL::IMaterial* MaterialManager::FindById(SpriteMaterialId InId) const
{
	auto it = _materials.find(InId);
	if(it == _materials.end())
	{
		throw SpriteRenderException("MaterialManager: material with the given id is not registered");
	}
	return it->second.get();
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv