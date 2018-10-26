#pragma once

#include "../Utils/TestUtils.h"
#include "../SprRenHelper/SprRenHelper_MaterialTypes.h"
#include "../IMPL/Utils/TextureElement.h"
#include "../Utils/TestTypes.h"
#include "../Utils/TexelColor.h"

namespace Test
{
	using TextureElement = IMPL::TextureElement;
	using MatInstArrayByColor = TArrayByColor<Handle_SprMaterialInstance>;

	/**
	* The fixture provides simple access to the default resource library of the tester
	* and to the common resource subsystems of the sprite render.
	*	
	* WARNING 1: It does NOT initialize or shutdown resources!
	* WARNING 2: Typically used as additional base class.
	*
	*/
	class ResourceFixture
	{
	public:
		ResourceFixture();
		~ResourceFixture();

		// ~ Textures Begin

		/**
		* Gets ALREADY set texture format!
		*/
		DXGI_FORMAT GetTextureFormat() const;

		/**
		* Sets texture format to be used by default.
		* If overwrite is NOT specified, then NOT allowed to overwrite already set format!
		*/
		void SetTextureFormat(DXGI_FORMAT InFormat, bool bInForceOverwrite = false);

		// ~Textures with default format Begin
		TextureElement& GetTexture_10_10(bool bInRed, bool bInGreen, bool bInBlue, bool bInAlpha, EColorBrightnessLevel InBrightness);
		TextureElement& GetRedTexture_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TextureElement& GetGreenTexture_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TextureElement& GetBlueTexture_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TextureElement& GetWhiteTexture_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TextureElement& GetRedGreenTexture_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TextureElement& GetRedBlueTexture_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TextureElement& GetGreenBlueTexture_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		// ~Textures with default format End

		TextureElement& GetTexture_10_10(bool bInRed, bool bInGreen, bool bInBlue, bool bInAlpha, EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat);
		TextureElement& GetRedTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat);
		TextureElement& GetGreenTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat);
		TextureElement& GetBlueTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat);
		TextureElement& GetWhiteTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat);
		TextureElement& GetRedGreenTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat);
		TextureElement& GetRedBlueTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat);
		TextureElement& GetGreenBlueTexture_10_10(EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat);
		// ~ Textures End

		// ~ Materials Begin
		Handle_SprMaterialInstance GetMatInst(bool bInRed, bool bInGreen, bool bInBlue, bool bInAlpha, EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		Handle_SprMaterialInstance GetMatInst_Red_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		Handle_SprMaterialInstance GetMatInst_Green_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		Handle_SprMaterialInstance GetMatInst_Blue_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		Handle_SprMaterialInstance GetMatInst_RedBlue_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		Handle_SprMaterialInstance GetMatInst_RedGreen_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		Handle_SprMaterialInstance GetMatInst_GreenBlue_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		Handle_SprMaterialInstance GetMatInst_White_10_10(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);

		Handle_SprMaterialInstance CreateMatInst_Default(const char* pInName, const TextureElement& InElem);
		Handle_SprMaterialInstance CreateMatInst_Default_10_10(bool bInRed, bool bInGreen, bool bInBlue, bool bInAlpha, EColorBrightnessLevel InBrightness, const char* pInName = "Custom");
		Handle_SprMaterialInstance CreateMatInst_Default_Red_10_10(EColorBrightnessLevel InBrightness, const char* pInName = "Red");
		Handle_SprMaterialInstance CreateMatInst_Default_Green_10_10(EColorBrightnessLevel InBrightness, const char* pInName = "Green");
		Handle_SprMaterialInstance CreateMatInst_Default_Blue_10_10(EColorBrightnessLevel InBrightness, const char* pInName = "Blue");
		Handle_SprMaterialInstance CreateMatInst_Default_White_10_10(EColorBrightnessLevel InBrightness, const char* pInName = "White");
		Handle_SprMaterialInstance CreateMatInst_Default_RedBlue_10_10(EColorBrightnessLevel InBrightness, const char* pInName = "RedBlue");
		Handle_SprMaterialInstance CreateMatInst_Default_RedGreen_10_10(EColorBrightnessLevel InBrightness, const char* pInName = "RedGreen");
		Handle_SprMaterialInstance CreateMatInst_Default_GreenBlue_10_10(EColorBrightnessLevel InBrightness, const char* pInName = "GreenBlue");
		// ~ Materials End

		// ~TSMaterial Begin
		TSMaterial GetTestMat(bool bInRed, bool bInGreen, bool bInBlue, bool bInAlpha, EColorBrightnessLevel InBrightness);
		TSMaterial GetTestMat_Red(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TSMaterial GetTestMat_Green(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TSMaterial GetTestMat_Blue(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TSMaterial GetTestMat_White(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TSMaterial GetTestMat_RedGreen(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TSMaterial GetTestMat_RedBlue(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);
		TSMaterial GetTestMat_GreenBlue(EColorBrightnessLevel InBrightness = EColorBrightnessLevel::Highest);

		TSMaterialVector GetTSMaterials(UINT InCount, const TSMaterialGenProps& InProps = TSMaterialGenProps::StrictDefault);
		// ~TSMaterial End

	private:
		bool Append_TSMaterial_MainColors(TSMaterialVector* pOutResult, UINT InCount, EColorBrightnessLevel InBrightness);
		bool Append_TSMaterial_TwoColorCombis(TSMaterialVector* pOutResult, UINT InCount, EColorBrightnessLevel InBrightness);
		TSMaterialVector Generate_TSMaterialVector(UINT InCount, const TSMaterialGenProps& InProps);

		DXGI_FORMAT TextureFormat = DXGI_FORMAT_UNKNOWN;

		/**
		* WARNING! We may NOT use GetLog() name here, because we may have "ambigious access"
		* (@see class details)
		*/
		std::ofstream& GetMyLog() const;

		MatInstArrayByColor pMatInstances;
	};
} // Test