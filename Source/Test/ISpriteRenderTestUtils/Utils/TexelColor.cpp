#include "TexelColor.h"
#include "../IMPL/Utils/RenTextureUtils.h"
#include "TestUtils.h"
#include "MathUtils.h"

namespace Test
{
	namespace
	{
		uint32_t GetUIntComponentByFactor(DXGI_FORMAT InFormat, float InFactor)
		{
			return ClampLerp(0U, GetMaxIntFormatComponent(InFormat), InFactor);
		}

		uint8_t GetComponentOffset(int InIndex, DXGI_FORMAT InFormat)
		{
			BOOST_ASSERT(InIndex < 4);

			UINT const ComponentSize = GetFormatComponentSize(InFormat);
			return ComponentSize * InIndex;
		}

		template<class PtrType>
		PtrType GetComponentPtr(PtrType Ptr, int InIndex, DXGI_FORMAT InFormat)
		{
			return const_cast<PtrType>(static_cast<const void*>(static_cast<const uint8_t*>(Ptr) + GetComponentOffset(InIndex, InFormat)));
		}

		void SetTextureComponent(void* pOutTexel, int InIndex, DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel, float InCoeff)
		{
			BOOST_ASSERT(InIndex < 4);

			BOOST_ASSERT(InFormat != DXGI_FORMAT_UNKNOWN);
			UINT const ComponentSize = GetFormatComponentSize(InFormat);
			void* const pOutComponent = GetComponentPtr(pOutTexel, InIndex, InFormat);

			uint8_t* const pOutByte = static_cast<uint8_t*>(pOutComponent);
			float* const pOutFloat = static_cast<float*>(pOutComponent);

			// Enough for any UINT format
			float BrightnessFactor = GetBrightnessFactor(InColorBrightnessLevel);
			uint32_t const UIntComp = GetUIntComponentByFactor(InFormat, InCoeff) * BrightnessFactor;

			switch (InFormat)
			{
			case DXGI_FORMAT_R8G8B8A8_UNORM: // fall-through
			case DXGI_FORMAT_R8G8B8A8_UINT:
				*pOutByte = static_cast<uint8_t>(UIntComp);
				return;

			case DXGI_FORMAT_R32G32B32A32_FLOAT:
				*pOutFloat = InCoeff * BrightnessFactor;
				return;

			default:
				break;
			}
			BOOST_ASSERT_MSG(false, "SetTextureComponent: unknown texture format!");
		}
	}

	TexelColor::TexelColor(DXGI_FORMAT InFormat, const void* InData) :
		Format{InFormat}
	{
		BOOST_ASSERT(Format != DXGI_FORMAT_UNKNOWN);	
		size_t FormatSize = GetFormatTexelSize(InFormat);
		std::memcpy(data, InData, FormatSize);
	}

	FLOAT* TexelColor::ToFloat(FLOAT OutColor[4])
	{
		OutColor[0] = GetRedFactor();
		OutColor[1] = GetGreenFactor();
		OutColor[2] = GetBlueFactor();
		OutColor[3] = GetAlphaFactor();
		return OutColor;
	}


	float TexelColor::GetRedComponent() const
	{
		return GetComponent(0);
	}

	float TexelColor::GetGreenComponent() const
	{
		return GetComponent(1);
	}

	float TexelColor::GetBlueComponent() const
	{
		return GetComponent(2);
	}

	float TexelColor::GetAlphaComponent() const
	{
		return GetComponent(3);
	}

	float TexelColor::GetComponent(int InIndex) const
	{
		const void* const pComponent = GetComponentPtr(GetData(), InIndex, Format);
		BOOST_ASSERT(pComponent);

		switch(Format)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM: // fall-through
		case DXGI_FORMAT_R8G8B8A8_UINT:		
			return static_cast<float>(*static_cast<const uint8_t*>(pComponent));

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return static_cast<float>(*static_cast<const float*>(pComponent));

		default:
			break;
		}
		BOOST_ASSERT_MSG(false, "TexelColor::GetComponent: Format: not yet impl");
		return 0;
	}

	float TexelColor::GetComponentFactor(int InIndex) const
	{
		float MaxComponent = GetMaxFormatComponent(Format);
		return ClampLerpFactor(0.0F, MaxComponent, GetComponent(InIndex));
	}

	float TexelColor::GetRedFactor() const
	{
		return GetComponentFactor(0);
	}

	float TexelColor::GetGreenFactor() const
	{
		return GetComponentFactor(1);
	}

	float TexelColor::GetBlueFactor() const
	{
		return GetComponentFactor(2);
	}

	float TexelColor::GetAlphaFactor() const
	{
		return GetComponentFactor(3);
	}
	

	TexelColor TexelColor::GetRed(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel, float InAlpha)
	{
		return GetColor(InFormat, 1.0F, 0.0F, 0.0F, InColorBrightnessLevel, InAlpha);
	}

	TexelColor TexelColor::GetGreen(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel, float InAlpha)
	{
		return GetColor(InFormat, 0.0F, 1.0F, 0.0F, InColorBrightnessLevel, InAlpha);
	}

	TexelColor TexelColor::GetBlue(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel, float InAlpha)
	{
		return GetColor(InFormat, 0.0F, 0.0F, 1.0F, InColorBrightnessLevel, InAlpha);
	}

	TexelColor TexelColor::GetWhite(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel, float InAlpha)
	{
		return GetColor(InFormat, 1.0F, 1.0F, 1.0F, InColorBrightnessLevel, InAlpha);
	}

	TexelColor TexelColor::GetBlack(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel, float InAlpha)
	{
		return GetColor(InFormat, 0.0F, 0.0F, 0.0F, InColorBrightnessLevel, InAlpha);
	}

	TexelColor TexelColor::GetRedGreen(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel, float InAlpha)
	{
		return GetColor(InFormat, 1.0F, 1.0F, 0.0F, InColorBrightnessLevel, InAlpha);
	}

	TexelColor TexelColor::GetRedBlue(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel, float InAlpha)
	{
		return GetColor(InFormat, 1.0F, 0.0F, 1.0F, InColorBrightnessLevel, InAlpha);
	}

	TexelColor TexelColor::GetGreenBlue(DXGI_FORMAT InFormat, EColorBrightnessLevel InColorBrightnessLevel, float InAlpha)
	{
		return GetColor(InFormat, 0.0F, 1.0F, 1.0F, InColorBrightnessLevel, InAlpha);
	}

	TexelColor TexelColor::GetColor(DXGI_FORMAT InFormat, float InRed, float InGreen, float InBlue, EColorBrightnessLevel InColorBrightnessLevel, float InAlpha)
	{		
		uint8_t Data[1024];
		SetTextureComponent(Data, 0, InFormat, InColorBrightnessLevel, InRed);
		SetTextureComponent(Data, 1, InFormat, InColorBrightnessLevel, InGreen);
		SetTextureComponent(Data, 2, InFormat, InColorBrightnessLevel, InBlue);
		SetTextureComponent(Data, 3, InFormat, EColorBrightnessLevel::Highest, InAlpha);
		return TexelColor(InFormat, Data);
	}

	bool AreTexelsMatch(const void* pTex, DXGI_FORMAT InFormat, const void* pOtherTex, DXGI_FORMAT InOtherFormat, bool bInCheckAlpha)
	{
		BOOST_ASSERT(pTex);
		BOOST_ASSERT(pOtherTex);
		BOOST_ASSERT(InFormat != DXGI_FORMAT_UNKNOWN);
		BOOST_ASSERT(InOtherFormat != DXGI_FORMAT_UNKNOWN);

		BOOST_ASSERT_MSG(InFormat == InOtherFormat, "AreTexelsMatch: This implementation works only if both texels are of the same format");		
		size_t texelSize = GetFormatTexelSize(InFormat);
		if (bInCheckAlpha)
		{
			int cmpResult = std::memcmp(pTex, pOtherTex, texelSize);
			return (0 == cmpResult);
		}
		else
		{
			size_t componentSize = GetFormatComponentSize(InFormat);
			int cmpResult = std::memcmp(pTex, pOtherTex, texelSize - componentSize);
			return (0 == cmpResult);
		}
	}

	bool AreTexelsMatch(const TexelColor& InA, const TexelColor& InB, bool bInCheckAlpha)
	{
		return AreTexelsMatch(InA.GetData(), InA.GetFormat(), InB.GetData(), InB.GetFormat(), bInCheckAlpha);
	}

	float GetBrightnessFactor(EColorBrightnessLevel InBrighness)
	{
		return (static_cast<float>(InBrighness)) / (NUM_TEX_BRIGHTNESS_LEVELS);
	}

	uint8_t ComponentCoeffFromBool(bool bInSet, EColorBrightnessLevel InBrighness)
	{
		BOOST_ASSERT(static_cast<float>(InBrighness) > 0);
		if (bInSet)
		{
			float const BrightnessCoeff = GetBrightnessFactor(InBrighness);
			return static_cast<uint8_t>(255 * BrightnessCoeff);
		}
		else
		{
			return 0;
		}
	}
} // Test