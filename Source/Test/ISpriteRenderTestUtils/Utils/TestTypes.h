#pragma once

#include "../SprRenHelper/SprRenHelper_System.h"
#include "../SprRenHelper/SprRenHelper_ISpriteRenderFwd.h"
#include "../SprRenHelper/SprRenHelper_Sprite.h"
#include "TexelColor.h"
#include <boost/assert.hpp>
#include <optional>
#include <vector>

namespace Test
{
	namespace IMPL
	{
		class TextureLib;
	} // Test::IMPL

	/**
	* TSMaterial:
	*/
	struct TSMaterial
	{
	public:
		TSMaterial
		(
			const TexelColor& InUniColor,
			const Handle_SprMaterialInstance& InMatInst
		);

		bool IsUniColor() const { return UniColor.has_value(); }
		const TexelColor& GetInitUniColor() const { return UniColor.value(); }
		const Handle_SprMaterialInstance& GetInitMatInst() const { return InitMatInst; }

	private:	
		std::optional<TexelColor> UniColor;
		Handle_SprMaterialInstance InitMatInst;
	};
	using TSMaterialVector = std::vector<TSMaterial>;

	/**
	* TSMaterial generation flags.
	*/	
	constexpr UINT TSMAT_GEN_ONLY_ONE_COMPONENT = 1 << 0;
	
	/**
	* Never use colors that are scaled with the brightness coeffs - always use maximal brighness for each component;
	*/
	constexpr UINT TSMAT_GEN_NO_BRIGHT_COEFF = 1 << 1;

	/**
	* NEVER add White color
	*/
	constexpr UINT TSMAT_GEN_NO_WHITE = 1 << 2;

	/**
	* NEVER add Black color
	*/
	constexpr UINT TSMAT_GEN_NO_BLACK = 1 << 3;
	
	/**
	* NEVER add White or Black color
	*/
	constexpr UINT TSMAT_GEN_NO_WHITE_BLACK = 1 << 4;

	/**
	* Flags to be set almost for any props
	*/
	constexpr UINT TMAT_GET_DEFAULT_FLAGS = TSMAT_GEN_NO_WHITE_BLACK;

	/**
	* Properties of material generation
	*/
	struct TSMaterialGenProps
	{
		/**
		* See description above.
		*/
		UINT GenFlags = 0;

		/**
		* If strict is true, then never generate color prohibited by the flags,
		* rather generate duplicated colors!
		*/
		bool bStrict = false;

		TSMaterialGenProps(UINT InGenFlags, bool bInStrict = false) :
			GenFlags{ InGenFlags }
		,	bStrict{ bInStrict } {}

		/**
		* Default: Red/Green, Green/Blue, Red, Green, Blue are allowed, never use colors with lesser brighness.
		* White and black are prohibited.
		*/
		static const TSMaterialGenProps StrictDefault;

		/**
		* May use bright coeff.
		* White and black are prohibited.
		*/
		static const TSMaterialGenProps StrictWithBrightCoeff;

		/**
		* Only Red/Green/Blue components are allowed
		*/
		static const TSMaterialGenProps StrictOnlyMain;
	};	

	/**
	* Sprite with some additional testing into.	
	*/
	struct TSSprite
	{
	public:
		TSSprite
		(
			TSSprite* InZBeforeSpriteId,
			const TSMaterial& InMaterial,
			const MySprMath::SSize& InInitSize,
			const MySprMath::SVec2& InInitPos,
			MySpr::ESpriteTransparency InInitTransparency = MySpr::ESpriteTransparency::Opaque
		);
		TSSprite
		(
			const TSMaterial& InMaterial,
			const MySprMath::SSize& InInitSize,
			const MySprMath::SVec2& InInitPos,
			MySpr::ESpriteTransparency InInitTransparency = MySpr::ESpriteTransparency::Opaque
		);
		void SetHandle(const SpriteHandle& InHandle);
		const SpriteHandle& GetHandle() const { return Handle; }

		/**
		* If nullptr, then initially sprite was added on top of the ZOrder.
		*/
		TSSprite* GetInitZBeforeSprite() const { return InZBeforeSpriteId; }
		SpriteHandle GetInitZBeforeSpriteHandle() const;
		const TSMaterial& GetInitMaterial() const { return InitMaterial; }		

		bool IsUniColor() const { return GetInitMaterial().IsUniColor(); }
		const TexelColor& GetInitUniColor() const { return GetInitMaterial().GetInitUniColor(); }
		const Handle_SprMaterialInstance& GetInitMatInst() const { return GetInitMaterial().GetInitMatInst(); }

		const MySprMath::SSize& GetInitSize() const { return InitSize; }
		const MySprMath::SVec2& GetInitPos() const { return InitPos; }
		const MySpr::ESpriteTransparency& GetInitTransparency() const { return InitTransparency; }

	
		bool IsCreated() const { return Handle.get(); }
		bool IsNotCreated() const { return false == IsCreated(); }

	private:
		mutable SpriteHandle Handle;
	
		TSSprite* InZBeforeSpriteId = nullptr;
		TSMaterial InitMaterial;
		MySprMath::SSize InitSize;
		MySprMath::SVec2 InitPos;
		MySpr::ESpriteTransparency InitTransparency;
	};

	using TSSpriteVector = std::vector<TSSprite>;	

} // Test