#pragma once

#include "SprRenHelper_MaterialTypes.h"
#include "SprRenHelper_Canvas.h"
#include "ISpriteRender/SpriteCommandInitializers.h"
#include "SprRenHelper_ISpriteUpdaterFwd.h"
#include "SprRenHelper_MathTypes.h"
#include <memory>

namespace Test
{
	class ISpriteRenderSubsystemManager;

	/**
	* Sprite, registered in the system.
	*/
	class Sprite
	{
	public:
		Sprite(ISpriteRenderSubsystemManager* pInSubsys, CanvasHandle pInCanvas, const MySprRen::SSpriteCreateCommandInitializer& InInitializer);

		CanvasHandle GetCanvas() const { return pCanvas; }

		SprId GetId() const;

		void UpdatePickProps(const MySpr::SSpritePickProps& InProps);
		const MySpr::SSpritePickProps& GetPickProps() const;
		bool CanBePicked() const;

		const Handle_SprMaterialInstance& GetMatInst();
		const MySpr::SSpriteProps& GetProps() const;
		const MySpr::SSpriteGeometryProps& GetGeometry() const;
		const MySprMath::SSpriteTransform& GetTransform() const;
		/**
		* Position that sprite origin point has in the coord system of the canvas.
		*/
		const MySprMath::SVec2& GetOriginPosition() const;

		/**
		* Position of the left bottom point position (no matter where the origin point is located).
		*/
		MySprMath::SVec2 GetLBPosition() const;

		const MySprMath::SSize& GetSize() const;
		const MySprMath::SVec2 GetOrigin() const;
		MySprMath::SVec2 GetCenter() const;
		const MySpr::ESpriteTransparency& GetTransparency() const;

		const Handle_SprMaterialInstance& GetInitMatInst();
		const MySpr::SSpriteProps& GetInitProps() const;

		void SetGeometry(MySprRen::ISpriteUpdater* pInUpdater, const MySpr::SSpriteGeometryProps& InGeometry);
		void Resize(MySprRen::ISpriteUpdater* pInUpdater, float InNewWidth, float InNewHeight);
		void SetPosition(MySprRen::ISpriteUpdater* pInUpdater, const SprVec2& InPosition);
		void SetRotationAngle(MySprRen::ISpriteUpdater* pInUpdater, float InAngleDegs);
		float GetRotationAngle() const;

		/**
		* Origin point of the sprite from the left bottom point of the sprite.
		*/
		void SetOrigin(MySprRen::ISpriteUpdater* pInUpdater, const SprVec2& InNewOrigin);
		void SetOriginToLeftBottom(MySprRen::ISpriteUpdater* pInUpdater);
		void SetOriginToCenter(MySprRen::ISpriteUpdater* pInUpdater);
		void SetMatInst(MySprRen::ISpriteUpdater* pInUpdater, const Handle_SprMaterialInstance& InInst);
		bool IsVisible() const;

		void Show(MySprRen::ISpriteUpdater* pInUpdater);
		void Hide(MySprRen::ISpriteUpdater* pInUpdater);
		void SetTransparency(MySprRen::ISpriteUpdater* pInUpdater, MySpr::ESpriteTransparency InTransparency);

	private:	
		ISpriteRenderSubsystemManager* pSubsys = nullptr;
		CanvasHandle pCanvas = nullptr;

		MySprRen::SSpriteCreateCommandInitializer Initializer;

		/**
		* WARNING! Updated only if you call methods on the Sprite itself!
		*/
		MySprRen::SSpriteCreateCommandInitializer CurrInitializer;
	};
	using SpriteHandle = std::shared_ptr<Sprite>;
} // Test