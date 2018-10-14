#include "TexelColor.h"
#include "../IMPL/Utils/RenTextureUtils.h"
#include "TestUtils.h"

namespace Test
{
	TexelColor::TexelColor(DXGI_FORMAT InFormat, const void* InData) :
		Format{InFormat}
	{
		BOOST_ASSERT(Format != DXGI_FORMAT_UNKNOWN);	
		size_t FormatSize = GetFormatTexelSize(InFormat);
		std::memcpy(data, InData, FormatSize);
	}

	TexelColor TexelColor::GetRed(DXGI_FORMAT InFormat, float InAlpha)
	{
		return GetColor(InFormat, 1.0F, 0.0F, 0.0F, InAlpha);
	}

	TexelColor TexelColor::GetGreen(DXGI_FORMAT InFormat, float InAlpha)
	{
		return GetColor(InFormat, 0.0F, 1.0F, 0.0F, InAlpha); 
	}

	TexelColor TexelColor::GetBlue(DXGI_FORMAT InFormat, float InAlpha)
	{
		return GetColor(InFormat, 0.0F, 0.0F, 1.0F, InAlpha);
	}

	TexelColor TexelColor::GetWhite(DXGI_FORMAT InFormat, float InAlpha)
	{
		return GetColor(InFormat, 1.0F, 1.0F, 1.0F, InAlpha);
	}

	TexelColor TexelColor::GetBlack(DXGI_FORMAT InFormat, float InAlpha)
	{
		return GetColor(InFormat, 0.0F, 0.0F, 0.0F, InAlpha);
	}

	void SetTextureComponent(void* pOutTexel, int InIndex, DXGI_FORMAT InFormat, float InCoeff)
	{
		BOOST_ASSERT(InFormat != DXGI_FORMAT_UNKNOWN);
		UINT const ComponentSize = GetFormatComponentSize(InFormat);
		void* const pOutComponent = static_cast<uint8_t*>(pOutTexel) + ComponentSize * InIndex;
		
		uint8_t* const pOutByte = static_cast<uint8_t*>(pOutComponent);
		float* const pOutFloat = static_cast<float*>(pOutComponent);

		switch (InFormat)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM: // fall-through
		case DXGI_FORMAT_R8G8B8A8_UINT:
			// We doing extra checks for corner cases for components with values ZERO or ONE, 
			// because this values are often used in test as references.
			if (InCoeff <= 0)
			{
				*pOutByte = 0;
			}
			else if (InCoeff >= 1.0F)
			{
				*pOutByte = 255;
			}
			else
			{
				*pOutByte = static_cast<uint8_t>(255 * InCoeff);
			}
			return;

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			*pOutFloat = InCoeff;
			return;

		default:
			break;
		}
		BOOST_ASSERT_MSG(false, "SetTextureComponent: unknown texture format!");
	}

	TexelColor TexelColor::GetColor(DXGI_FORMAT InFormat, float InRed, float InGreen, float InBlue, float InAlpha)
	{		
		uint8_t Data[1024];
		SetTextureComponent(Data, 0, InFormat, InRed);
		SetTextureComponent(Data, 1, InFormat, InGreen);
		SetTextureComponent(Data, 2, InFormat, InBlue);
		SetTextureComponent(Data, 3, InFormat, InAlpha);
		return TexelColor(InFormat, Data);
	}

	bool AreTexelsMatch(const void* pTex, DXGI_FORMAT InFormat, const void* pOtherTex, DXGI_FORMAT InOtherFormat)
	{
		BOOST_ASSERT(pTex);
		BOOST_ASSERT(pOtherTex);
		BOOST_ASSERT(InFormat != DXGI_FORMAT_UNKNOWN);
		BOOST_ASSERT(InOtherFormat != DXGI_FORMAT_UNKNOWN);

		BOOST_ASSERT_MSG(InFormat == InOtherFormat, "AreTexelsMatch: This implementation works only if both texels are of the same format");
		size_t texelSize = GetFormatTexelSize(InFormat);
		int cmpResult = std::memcmp(pTex, pOtherTex, texelSize);
		return (0 == cmpResult);
	}

	bool operator==(const TexelColor& A, const TexelColor& B)
	{
		return AreTexelsMatch(A.GetData(), A.GetFormat(), B.GetData(), B.GetFormat());
	}

	bool operator!=(const TexelColor& A, const TexelColor& B) 
	{
		return !(A == B); 
	}
} // Test