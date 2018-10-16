#include "MaterialInstanceFactory.h"
#include "ISprite/SpriteMaterialSystemConstants.h"
#include "QSpriteRender/Material/DefaultMaterialInstanceRS.h"
#include "QSpriteRender/Material/DefaultSpriteMaterial.h"
#include "QSpriteRender/INTERFACE/ISpriteRenderSubsystem.h"
#include "ISpriteMaterialManager.h"
#include <boost/assert.hpp>


namespace Dv
{
namespace Spr
{
namespace QRen
{

std::shared_ptr<Ren::SpriteMaterialInstanceRenderState> CreateMatInst_Default(ISpriteRenderSubsystem* pInRenderSubsystem, ID3D11ShaderResourceView* pInTexture2D)
{
	auto const pMaterial = dynamic_cast<IMPL::DefaultSpriteMaterial*>(pInRenderSubsystem->GetMaterials()->FindById(DEFAULT_MATERIAL_ID));

	IMPL::DefaultShaderConfigId ShaderConfigId = PrepareShader_ForDefaultMaterialInstance(pMaterial, pInTexture2D);

	return std::make_shared<IMPL::DefaultMaterialInstanceRS>
	(
		IMPL::SDefaultMaterialInstanceRSInitializer{pMaterial, pInTexture2D, ShaderConfigId }
	);
}

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv