#pragma once

#include <d3d11.h>
#include <boost/assert.hpp>
#include <cstdlib>
#include <cstdint>

namespace Test
{
	/**
	* Texel color.
	*/
	class TexelColor
	{
	public:
		TexelColor(DXGI_FORMAT InFormat, const void* InData);

		DXGI_FORMAT GetFormat() const { return Format; }
		const void* GetData() const { return data; }

		// ~Color Getters Begin
		static TexelColor GetRed(DXGI_FORMAT InFormat, float InAlpha = 1.0F);
		static TexelColor GetGreen(DXGI_FORMAT InFormat, float InAlpha = 1.0F);
		static TexelColor GetBlue(DXGI_FORMAT InFormat, float InAlpha = 1.0F);
		static TexelColor GetWhite(DXGI_FORMAT InFormat, float InAlpha = 1.0F);
		static TexelColor GetBlack(DXGI_FORMAT InFormat, float InAlpha = 1.0F);
		static TexelColor GetColor(DXGI_FORMAT, float InRed, float InGreen, float InBlue, float InAlpha = 1.0F);
		// ~Color Getters End

	private:
		DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;
		uint8_t data[1024];
	};
	bool operator==(const TexelColor& A, const TexelColor& B);
	bool operator!=(const TexelColor& A, const TexelColor& B);

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
		BOOST_ASSERT(!"Should NOT get here");
		return 0;
	}
	inline UINT GetFormatComponentSize(DXGI_FORMAT InFormat)
	{
		return GetFormatTexelSize(InFormat) / 4;
	}

	bool AreTexelsMatch(const void* pTex, DXGI_FORMAT InFormat, const void* pOtherTex, DXGI_FORMAT InOtherFormat);
} // Test