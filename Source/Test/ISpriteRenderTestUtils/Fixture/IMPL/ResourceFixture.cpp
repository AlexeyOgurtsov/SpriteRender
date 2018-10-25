#include "../ResourceFixture.h"
#include "../../IMPL/Environment.h"
#include "../../IMPL/Utils/RenHRUtils.h"

namespace Test
{
	namespace {
		/**
		* Helper to get the environment within the FixtureBase
		*/
		IMPL::Environment* GetEnv() { return IMPL::Environment::Get(); }
		ISpriteRenderSubsystemManager* GetSprRenManager() { return GetEnv()->GetSpriteRenderManager(); }
		IMPL::Resources* GetResources() { return GetEnv()->GetResources(); }
		IMPL::Textures* GetTextures() { return GetResources()->GetTextures(); }
		IMPL::TextureLib* GetTexLib() { return GetResources()->GetTextureLib(); }
	}

	ResourceFixture::ResourceFixture()
	{
		T_LOG_TO(GetMyLog(), "ResourceFixture CTOR...");
		T_LOG_TO(GetMyLog(), "ResourceFixture CTOR DONE");
	}

	ResourceFixture::~ResourceFixture()
	{
		T_LOG_TO(GetMyLog(), "ResourceFixture ~DTOR...");
		T_LOG_TO(GetMyLog(), "ResourceFixture ~DTOR DONE");
	}

	DXGI_FORMAT ResourceFixture::GetTextureFormat() const
	{
		BOOST_ASSERT_MSG(TextureFormat != DXGI_FORMAT_UNKNOWN, "ResourceFixture::GetTextureFormat: Texture format must be already set!");
		return TextureFormat;
	}

	void ResourceFixture::SetTextureFormat(DXGI_FORMAT InFormat, bool bInForceOverwrite)
	{
		T_LOG_TO(GetMyLog(), "ResourceFixture: Setting texture format...");
		if (TextureFormat != DXGI_FORMAT_UNKNOWN)
		{
			T_LOG_TO(GetMyLog(), "ResourceFixture: Format already set!");
			BOOST_ASSERT_MSG(bInForceOverwrite, "ResourceFixture::SetTextureFormat: WARNING!!! bInForceOverwrite to be passed to overwrite the already set texture format!");
		}
		T_LOG_TO(GetMyLog(), "New format: " << IMPL::GetFormatString(InFormat));
		TextureFormat = InFormat;
		T_LOG_TO(GetMyLog(), "ResourceFixture: Setting texture format DONE");
	}

	TextureElement& ResourceFixture::GetRedTexture_10_10()
	{
		return GetRedTexture_10_10(GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetGreenTexture_10_10()
	{
		return GetGreenTexture_10_10(GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetBlueTexture_10_10()
	{
		return GetBlueTexture_10_10(GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetWhiteTexture_10_10()
	{
		return GetWhiteTexture_10_10(GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetRedTexture_10_10(DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetRedTexture_10_10(InFormat);
	}

	TextureElement& ResourceFixture::GetGreenTexture_10_10(DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetGreenTexture_10_10(InFormat);
	}

	TextureElement& ResourceFixture::GetBlueTexture_10_10(DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetBlueTexture_10_10(InFormat);
	}

	TextureElement& ResourceFixture::GetWhiteTexture_10_10(DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetWhiteTexture_10_10(InFormat);
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default(const char* pInName, const TextureElement& InElem)
	{
		return GetSprRenManager()->CreateMatInst_Default(pInName, InElem.GetShaderResourceView());
	}

	std::ofstream& ResourceFixture::GetMyLog() const
	{
		return GetEnv()->GetMainLog();
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst_Red_10_10()
	{
		if (nullptr == pMatInst_Red_10_10)
		{
			pMatInst_Red_10_10 = CreateMatInst_Default_Red_10_10();
		}
		return pMatInst_Red_10_10;
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst_Green_10_10()
	{
		if (nullptr == pMatInst_Green_10_10)
		{
			pMatInst_Green_10_10 = CreateMatInst_Default_Green_10_10();
		}
		return pMatInst_Green_10_10;
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst_Blue_10_10()
	{
		if (nullptr == pMatInst_Blue_10_10)
		{
			pMatInst_Blue_10_10 = CreateMatInst_Default_Blue_10_10();
		}
		return pMatInst_Blue_10_10;
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst_White_10_10()
	{
		if (nullptr == pMatInst_White_10_10)
		{
			pMatInst_White_10_10 = CreateMatInst_Default_White_10_10();
		}
		return pMatInst_White_10_10;
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_Red_10_10(const char* pInName)
	{
		return CreateMatInst_Default(pInName, GetRedTexture_10_10());
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_Green_10_10(const char* pInName)
	{
		return CreateMatInst_Default(pInName, GetGreenTexture_10_10());
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_Blue_10_10(const char* pInName)
	{
		return CreateMatInst_Default(pInName, GetBlueTexture_10_10());
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_White_10_10(const char* pInName)
	{
		return CreateMatInst_Default(pInName, GetWhiteTexture_10_10());
	}
} // Test