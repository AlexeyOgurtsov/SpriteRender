#pragma once

#include "../SprRenHelper/SprRenHelper_System.h"
#include "../SprRenHelper/SprRenHelper_ISpriteRenderFwd.h"
#include "../SprRenHelper/SprRenHelper_Sprite.h"
#include "TexelColor.h"
#include <boost/assert.hpp>
#include <optional>

namespace Test
{
	/**
	* TestMaterial:
	*/
	struct TestMaterial
	{
	public:
		TestMaterial
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

	/**
	* Sprite with some additional testing into.
	*/
	// @TODO: Move to utils/header
	struct TSSprite
	{
	public:
		TSSprite
		(
			const TestMaterial& InMaterial,
			const MySprMath::SSize& InInitSize,
			const MySprMath::SVec2& InInitPos
		);
		void SetHandle(const SpriteHandle& InHandle);
		const SpriteHandle& GetHandle() const { return Handle; }

		const TestMaterial& GetInitMaterial() const { return InitMaterial; }		

		bool IsUniColor() const { return GetInitMaterial().IsUniColor(); }
		const TexelColor& GetInitUniColor() const { return GetInitMaterial().GetInitUniColor(); }
		const Handle_SprMaterialInstance& GetInitMatInst() const { return GetInitMaterial().GetInitMatInst(); }

		const MySprMath::SSize& GetInitSize() const { return InitSize; }
		const MySprMath::SVec2& GetInitPos() const { return InitPos; }

	
		bool IsCreated() const { return Handle.get(); }
		bool IsNotCreated() const { return false == IsCreated(); }

	private:
		SpriteHandle Handle;
	
		TestMaterial InitMaterial;
		MySprMath::SSize InitSize;
		MySprMath::SVec2 InitPos;
	};
} // Test