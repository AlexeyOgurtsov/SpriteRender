#pragma once

#include "../Utils/TestUtils.h"
#include "../SprRenHelper/SprRenHelper_MaterialTypes.h"
#include "../IMPL/Utils/TextureElement.h"

namespace Test
{
	using TextureElement = IMPL::TextureElement;

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
		TextureElement& GetRedTexture_10_10();
		TextureElement& GetGreenTexture_10_10();
		TextureElement& GetBlueTexture_10_10();
		TextureElement& GetWhiteTexture_10_10();
		// ~Textures with default format End

		TextureElement& GetRedTexture_10_10(DXGI_FORMAT InFormat);
		TextureElement& GetGreenTexture_10_10(DXGI_FORMAT InFormat);
		TextureElement& GetBlueTexture_10_10(DXGI_FORMAT InFormat);
		TextureElement& GetWhiteTexture_10_10(DXGI_FORMAT InFormat);
		// ~ Textures End

		// ~ Materials Begin
		Handle_SprMaterialInstance GetMatInst_Red_10_10();
		Handle_SprMaterialInstance GetMatInst_Green_10_10();
		Handle_SprMaterialInstance GetMatInst_Blue_10_10();
		Handle_SprMaterialInstance GetMatInst_White_10_10();

		Handle_SprMaterialInstance CreateMatInst_Default(const char* pInName, const TextureElement& InElem);
		Handle_SprMaterialInstance CreateMatInst_Default_Red_10_10(const char* pInName = "Red");
		Handle_SprMaterialInstance CreateMatInst_Default_Green_10_10(const char* pInName = "Green");
		Handle_SprMaterialInstance CreateMatInst_Default_Blue_10_10(const char* pInName = "Blue");
		Handle_SprMaterialInstance CreateMatInst_Default_White_10_10(const char* pInName = "White");
		// ~ Materials End

	private:
		DXGI_FORMAT TextureFormat = DXGI_FORMAT_UNKNOWN;

		/**
		* WARNING! We may NOT use GetLog() name here, because we may have "ambigious access"
		* (@see class details)
		*/
		std::ofstream& GetMyLog() const;

		Handle_SprMaterialInstance pMatInst_Red_10_10;
		Handle_SprMaterialInstance pMatInst_Green_10_10;
		Handle_SprMaterialInstance pMatInst_Blue_10_10;
		Handle_SprMaterialInstance pMatInst_White_10_10;
	};
} // Test