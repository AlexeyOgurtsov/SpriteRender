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

	TextureElement& ResourceFixture::GetTexture_10_10(bool bInRed, bool bInGreen, bool bInBlue, bool bInAlpha, EColorBrightnessLevel InBrightness)
	{
		return GetTexture_10_10(bInRed, bInGreen, bInBlue, bInAlpha, InBrightness, GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetRedTexture_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetRedTexture_10_10(InBrightness, GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetGreenTexture_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetGreenTexture_10_10(InBrightness, GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetBlueTexture_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetBlueTexture_10_10(InBrightness, GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetWhiteTexture_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetWhiteTexture_10_10(InBrightness, GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetRedGreenTexture_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetRedGreenTexture_10_10(InBrightness, GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetRedBlueTexture_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetRedBlueTexture_10_10(InBrightness, GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetGreenBlueTexture_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetGreenBlueTexture_10_10(InBrightness, GetTextureFormat());
	}

	TextureElement& ResourceFixture::GetTexture_10_10(bool bInRed, bool bInGreen, bool bInBlue, bool bInAlpha, EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetTexture_10_10(bInRed, bInGreen, bInBlue, bInAlpha, InBrightness, InFormat);
	}

	TextureElement& ResourceFixture::GetRedTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetRedTexture_10_10(InFormat, InBrightness);
	}

	TextureElement& ResourceFixture::GetGreenTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetGreenTexture_10_10(InFormat, InBrightness);
	}

	TextureElement& ResourceFixture::GetBlueTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetBlueTexture_10_10(InFormat, InBrightness);
	}

	TextureElement& ResourceFixture::GetWhiteTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetWhiteTexture_10_10(InFormat, InBrightness);
	}

	TextureElement& ResourceFixture::GetRedGreenTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetRedGreenTexture_10_10(InFormat, InBrightness);
	}

	TextureElement& ResourceFixture::GetRedBlueTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetRedBlueTexture_10_10(InFormat, InBrightness);
	}

	TextureElement& ResourceFixture::GetGreenBlueTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat)
	{
		return GetTexLib()->GetGreenBlueTexture_10_10(InFormat, InBrightness);
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default(const char* pInName, const TextureElement& InElem)
	{
		return GetSprRenManager()->CreateMatInst_Default(pInName, InElem.GetShaderResourceView());
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_10_10(bool bInRed, bool bInGreen, bool bInBlue, bool bInAlpha, EColorBrightnessLevel InBrightness, const char* pInName)
	{
		return GetSprRenManager()->CreateMatInst_Default(pInName, GetTexture_10_10(bInRed, bInGreen, bInBlue, bInAlpha, InBrightness).GetShaderResourceView());
	}

	std::ofstream& ResourceFixture::GetMyLog() const
	{
		return GetEnv()->GetMainLog();
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst_Red_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetMatInst(true, false, false, false, InBrightness);
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst_Green_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetMatInst(false, true, false, false, InBrightness);
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst_Blue_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetMatInst(false, false, true, false, InBrightness);
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst_RedBlue_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetMatInst(true, false, true, false, InBrightness);
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst_RedGreen_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetMatInst(true, true, false, false, InBrightness);;
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst_GreenBlue_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetMatInst(false, true, true, false, InBrightness);;
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst_White_10_10(EColorBrightnessLevel InBrightness)
	{
		return GetMatInst(true, true, true, false, InBrightness);
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_Red_10_10(EColorBrightnessLevel InBrightness, const char* pInName)
	{
		return CreateMatInst_Default(pInName, GetRedTexture_10_10(InBrightness));
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_Green_10_10(EColorBrightnessLevel InBrightness, const char* pInName)
	{
		return CreateMatInst_Default(pInName, GetGreenTexture_10_10(InBrightness));
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_Blue_10_10(EColorBrightnessLevel InBrightness, const char* pInName)
	{
		return CreateMatInst_Default(pInName, GetBlueTexture_10_10(InBrightness));
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_White_10_10(EColorBrightnessLevel InBrightness, const char* pInName)
	{
		return CreateMatInst_Default(pInName, GetWhiteTexture_10_10(InBrightness));
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_RedBlue_10_10(EColorBrightnessLevel InBrightness, const char* pInName)
	{
		return CreateMatInst_Default(pInName, GetRedBlueTexture_10_10(InBrightness));
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_RedGreen_10_10(EColorBrightnessLevel InBrightness, const char* pInName)
	{
		return CreateMatInst_Default(pInName, GetRedGreenTexture_10_10(InBrightness));
	}

	Handle_SprMaterialInstance ResourceFixture::CreateMatInst_Default_GreenBlue_10_10(EColorBrightnessLevel InBrightness, const char* pInName)
	{
		return CreateMatInst_Default(pInName, GetGreenBlueTexture_10_10(InBrightness));
	}

	Handle_SprMaterialInstance ResourceFixture::GetMatInst(bool bInRed, bool bInGreen, bool bInBlue, bool bInAlpha, EColorBrightnessLevel InBrightness)
	{
		Handle_SprMaterialInstance* pCachedMatInst = GetArrayByColorElementPtr(pMatInstances, bInRed, bInGreen, bInBlue, bInAlpha, InBrightness);
		if (nullptr == *pCachedMatInst)
		{
			*pCachedMatInst = CreateMatInst_Default_10_10(bInRed, bInGreen, bInBlue, bInAlpha, InBrightness);
		}
		return *pCachedMatInst;
	}

	TSMaterial ResourceFixture::GetTestMat(bool bInRed, bool bInGreen, bool bInBlue, bool bInAlpha, EColorBrightnessLevel InBrightness)
	{
		return TSMaterial
		(
			TexelColor::GetColor(GetTextureFormat(), bInRed, bInGreen, bInBlue, InBrightness, bInAlpha), 
			GetMatInst(bInRed, bInGreen, bInBlue, bInAlpha, InBrightness)
		);
	}

	TSMaterial ResourceFixture::GetTestMat_Red(EColorBrightnessLevel InBrightness)
	{
		return TSMaterial(TexelColor::GetRed(GetTextureFormat()), GetMatInst_Red_10_10(InBrightness));
	}

	TSMaterial ResourceFixture::GetTestMat_Green(EColorBrightnessLevel InBrightness)
	{
		return TSMaterial(TexelColor::GetGreen(GetTextureFormat()), GetMatInst_Green_10_10(InBrightness));
	}

	TSMaterial ResourceFixture::GetTestMat_Blue(EColorBrightnessLevel InBrightness)
	{
		return TSMaterial(TexelColor::GetBlue(GetTextureFormat()), GetMatInst_Blue_10_10(InBrightness));
	}

	TSMaterial ResourceFixture::GetTestMat_White(EColorBrightnessLevel InBrightness)
	{
		return TSMaterial(TexelColor::GetWhite(GetTextureFormat()), GetMatInst_White_10_10(InBrightness));
	}

	TSMaterial ResourceFixture::GetTestMat_RedGreen(EColorBrightnessLevel InBrightness)
	{
		return TSMaterial(TexelColor::GetRedGreen(GetTextureFormat()), GetMatInst_RedGreen_10_10(InBrightness));
	}

	TSMaterial ResourceFixture::GetTestMat_RedBlue(EColorBrightnessLevel InBrightness)
	{
		return TSMaterial(TexelColor::GetRedBlue(GetTextureFormat()), GetMatInst_RedBlue_10_10(InBrightness));
	}

	TSMaterial ResourceFixture::GetTestMat_GreenBlue(EColorBrightnessLevel InBrightness)
	{
		return TSMaterial(TexelColor::GetGreenBlue(GetTextureFormat()), GetMatInst_GreenBlue_10_10(InBrightness));
	}

	bool ResourceFixture::Append_TSMaterial_MainColors(TSMaterialVector* pOutResult, UINT InCount, EColorBrightnessLevel InBrightness)
	{
		BOOST_ASSERT(pOutResult);
		if (pOutResult->size() < InCount)
		{
			pOutResult->emplace_back(GetTestMat_Red(InBrightness));
		}
		else
		{
			return true;
		}
		if (pOutResult->size() < InCount)
		{
			pOutResult->emplace_back(GetTestMat_Green(InBrightness));
		}
		else
		{
			return true;
		}
		if (pOutResult->size() < InCount)
		{
			pOutResult->emplace_back(GetTestMat_Blue(InBrightness));
		}
		else
		{
			return true;
		}
		return false;
	}
	bool ResourceFixture::Append_TSMaterial_TwoColorCombis(TSMaterialVector* pOutResult, UINT InCount, EColorBrightnessLevel InBrightness)
	{
		BOOST_ASSERT(pOutResult);
		if (pOutResult->size() < InCount)
		{
			pOutResult->emplace_back(GetTestMat_RedGreen(InBrightness));
		}
		else
		{
			return true;
		}
		if (pOutResult->size() < InCount)
		{
			pOutResult->emplace_back(GetTestMat_RedBlue(InBrightness));
		}
		else
		{
			return true;
		}
		if (pOutResult->size() < InCount)
		{
			pOutResult->emplace_back(GetTestMat_GreenBlue(InBrightness));
		}
		else
		{
			return true;
		}
		return false;
	}

	TSMaterialVector ResourceFixture::Generate_TSMaterialVector(UINT InCount, const TSMaterialGenProps& InProps)
	{
		BOOST_ASSERT_MSG(InProps.bStrict, "ResourceFixture::Generate_TSMaterialVector: At this time only strict generation is supported");
		BOOST_ASSERT_MSG(InProps.GenFlags & TSMAT_GEN_NO_BRIGHT_COEFF, "ResourceFixture::Generate_TSMaterialVector: using bright coeff at this time will cause precision errors whn comparing (so tests will fail). Eliminate the issue, then this feature will be allowed");

		TSMaterialVector Result;
		Result.reserve(InCount);

		/**
		* Here we will use the same colors again if the existings colors are not sufficient.
		*/
		while (true)
		{
			if (Append_TSMaterial_MainColors(&Result, InCount, EColorBrightnessLevel::Highest))
			{
				return Result;
			}

			if (0 == (InProps.GenFlags & TSMAT_GEN_ONLY_ONE_COMPONENT))
			{
				if (Append_TSMaterial_TwoColorCombis(&Result, InCount, EColorBrightnessLevel::Highest))
				{
					return Result;
				}
			}

			// @TODO: 
			if (0 == (InProps.GenFlags & TSMAT_GEN_NO_BRIGHT_COEFF))
			{
				if (Append_TSMaterial_MainColors(&Result, InCount, EColorBrightnessLevel::Low))
				{
					return Result;
				}

				if (0 == (InProps.GenFlags & TSMAT_GEN_ONLY_ONE_COMPONENT))
				{
					if (Append_TSMaterial_TwoColorCombis(&Result, InCount, EColorBrightnessLevel::Low))
					{
						return Result;
					}
				}
			}
		}

		BOOST_ASSERT(Result.size() == InCount);

		return Result;
	}

	TSMaterialVector ResourceFixture::GetTSMaterials(UINT InCount, const TSMaterialGenProps& InProps)
	{
		return Generate_TSMaterialVector(InCount, InProps);
	}
} // Test