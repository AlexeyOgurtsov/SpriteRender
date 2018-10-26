#pragma once

#include <d3d11.h>
#include <boost/assert.hpp>
#include <cstdlib>
#include <cstdint>
#include "MiscUtils.h"

namespace Test
{

	enum class EColorBrightnessLevel
	{
		Off = 0,
		Lowest,
		Low,
		High,
		Highest
	};

	constexpr UINT NUM_TEX_BRIGHTNESS_LEVELS = 4;
	constexpr UINT DEFAULT_MINIMAL_TEX_SIZE = 10;
	constexpr UINT BOOL_INDEX_NUM = 2;

	template<class T>
	using TArrayByColor = T[BOOL_INDEX_NUM][BOOL_INDEX_NUM][BOOL_INDEX_NUM][BOOL_INDEX_NUM][NUM_TEX_BRIGHTNESS_LEVELS];

	uint8_t ComponentCoeffFromBool(bool bInSet, EColorBrightnessLevel InBrighness);

	template<class T> T* GetArrayByColorElementPtr(TArrayByColor<T>& InArr, bool bInRed, bool bInGreen, bool bInBlue, bool bInAlpha, EColorBrightnessLevel InBrightness)
	{
		return &InArr[BoolToIndex(bInRed)][BoolToIndex(bInGreen)][BoolToIndex(bInBlue)][BoolToIndex(bInAlpha)][static_cast<UINT>(InBrightness)];
	}

	/**
	* Texel color.
	*/
	class TexelColor
	{
	public:
		TexelColor(DXGI_FORMAT InFormat, const void* InData);

		DXGI_FORMAT GetFormat() const { return Format; }
		const void* GetData() const { return data; }

		/**
		* This method is necessary for taking representation, 
		* where each component is in range 0..1.
		*
		* For examle, useful for ClearRenderTargetView.
		*/
		FLOAT* ToFloat(FLOAT OutColor[4]);

		float GetRedComponent() const;
		float GetGreenComponent() const;
		float GetBlueComponent() const;
		float GetAlphaComponent() const;
		float GetComponent(int InIndex) const;

		float GetRedFactor() const;
		float GetGreenFactor() const;
		float GetBlueFactor() const;
		float GetAlphaFactor() const;
		float GetComponentFactor(int InIndex) const;

		// ~Color Getters Begin
		static TexelColor GetRed(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel = EColorBrightnessLevel::Highest, float InAlpha = 1.0F);
		static TexelColor GetGreen(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel = EColorBrightnessLevel::Highest, float InAlpha = 1.0F);
		static TexelColor GetBlue(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel = EColorBrightnessLevel::Highest, float InAlpha = 1.0F);
		static TexelColor GetWhite(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel = EColorBrightnessLevel::Highest, float InAlpha = 1.0F);
		static TexelColor GetBlack(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel = EColorBrightnessLevel::Highest, float InAlpha = 1.0F);
		static TexelColor GetRedGreen(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel = EColorBrightnessLevel::Highest, float InAlpha = 1.0F);
		static TexelColor GetRedBlue(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel = EColorBrightnessLevel::Highest, float InAlpha = 1.0F);
		static TexelColor GetGreenBlue(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel = EColorBrightnessLevel::Highest, float InAlpha = 1.0F);
		static TexelColor GetColor(DXGI_FORMAT, float InRed, float InGreen, float InBlue, EColorBrightnessLevel InColorBrightnessLevel = EColorBrightnessLevel::Highest, float InAlpha = 1.0F);
		// ~Color Getters End

	private:
		DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;
		uint8_t data[1024];
	};

	/**
	* @See: GetMaxFormatComponent
	*/
	inline UINT GetMaxIntFormatComponent(DXGI_FORMAT InFormat)
	{
		switch (InFormat)
		{
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			BOOST_ASSERT_MSG(false, "GetMaxFormatComponent: NOT UInt format");
			return 1;

		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return static_cast<UINT>(255);

		case DXGI_FORMAT_R8G8B8A8_UINT:
			return static_cast<UINT>(255);

		default:
			break;
		}
		BOOST_ASSERT(!"GetMaxFormatComponent: Format is NOT yet impl");
		return 0;
	}

	inline float GetMaxFormatComponent(DXGI_FORMAT InFormat)
	{
		switch (InFormat)
		{
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return 1.0F;

		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return static_cast<UINT>(255);

		case DXGI_FORMAT_R8G8B8A8_UINT:
			return static_cast<UINT>(255);

		default:
			break;
		}
		BOOST_ASSERT(!"GetMaxFormatComponent: Format is NOT yet impl");
		return 0;
	}

	inline UINT GetFormatTexelSize(DXGI_FORMAT InFormat)
	{
		switch (InFormat)
		{
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return sizeof(uint32_t) * 4;

		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return sizeof(uint8_t) * 4;

		case DXGI_FORMAT_R8G8B8A8_UINT:
			return sizeof(uint8_t) * 4;

		default:
			break;
		}
		BOOST_ASSERT(!"GetFormatTexelSize: Format is NOT yet impl");
		return 0;
	}
	inline UINT GetFormatComponentSize(DXGI_FORMAT InFormat)
	{
		return GetFormatTexelSize(InFormat) / 4;
	}

	bool AreTexelsMatch(const TexelColor& InA, const TexelColor& InB, bool bInCheckAlpha = false);
	bool AreTexelsMatch(const void* pTex, DXGI_FORMAT InFormat, const void* pOtherTex, DXGI_FORMAT InOtherFormat, bool bInCheckAlpha = false);

	float GetBrightnessFactor(EColorBrightnessLevel InBrighness);
} // Test