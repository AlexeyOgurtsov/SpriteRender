#pragma once

#include "SpriteRender/INTERFACE/Material/ISpriteMaterialManager.h"
#include "SpriteRender/IMPL/RenUtils/ShaderCompileUtils.h"
#include "SpriteRender/INTERFACE/CONFIG/SpriteRenderShadersConfig.h"
#include <unordered_map>
#include <memory>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

class AmbientContext;
struct SMaterialManagerInitializer
{
	const SShadersConfig* pConfig;
	AmbientContext* pAmbientContext;
	ID3D11Device* pDevice;

	SMaterialManagerInitializer(const SShadersConfig *pInConfig, AmbientContext* pInAmbientContext, ID3D11Device* pInDevice) :
		pConfig{ pInConfig }
	,	pAmbientContext{ pInAmbientContext }
	,	pDevice{ pInDevice } {}
};

/**
* Implementation of the sprite material manager.
*/
class MaterialManager : public ISpriteMaterialManager
{
public:
	MaterialManager(const SMaterialManagerInitializer& InInitializer);

	const D3D::SShaderCompileEnv* GetShaderCompileEnv() const { return &_ShaderCompileEnv; }

	virtual void Register(IMPL::IMaterial* pInMaterial) override;
	virtual IMPL::IMaterial* FindById(SpriteMaterialId InId) const override;

private:
	const SShadersConfig* _pConfig = nullptr;
	AmbientContext* _pAmbientContext = nullptr;
	ID3D11Device* _pDev = nullptr;
	D3D::SShaderCompileEnv _ShaderCompileEnv;
	std::unordered_map<SpriteMaterialId, std::unique_ptr<IMPL::IMaterial>> _materials;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv