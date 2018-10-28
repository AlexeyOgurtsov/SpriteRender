#pragma once

#include "ISpriteRenderSubsystemManager_PublicTypes.h"
#include "SprRenHelper/SprRenHelper_ISpriteRenderFwd.h"
#include "SprRenHelper/SprRenHelper_ISpriteUpdaterFwd.h"
#include "SprRenHelper/SprRenHelper_Transparency.h"
#include <fstream>
#include <windows.h> // HWND

namespace Test
{
	struct TesterConfig;

	namespace IMPL
	{
		class D3DDevice;
	} // IMPL
	using D3DDevice = IMPL::D3DDevice;

	/**
	* Abstract interface to the underlying sprite render subsystem.
	*/
	class ISpriteRenderSubsystemManager
	{
	public:
		virtual ~ISpriteRenderSubsystemManager() = default;

		/**
		* Name of the manager.
		*/
		virtual const char* GetName() const = 0;

		/**
		* Gets the deployed sprite render interface (or nullptr if NOT yet deployed).
		*/
		virtual ISpriteRender* GetSpriteRender() const = 0;

		/**
		* Initializes the subsystem with current settings
		* (WARNING!!! The subsystem must be shutted down already).
		*/
		virtual void Initialize() = 0;

		/**
		* Shutdown the subsystem completely
		*/
		virtual void Shutdown() = 0;

		/**
		* Tick function (To be called each frame).
		*/
		virtual void Tick(float InDeltaSeconds) = 0;

		/**
		* BeginFrame function (To be called before starting any frame).
		*/
		virtual void BeginFrame(float InDeltaSeconds) = 0;

		/**
		* EndFrame function (To be called after starting any frame).
		*/
		virtual void EndFrame(float InDeltaSeconds) = 0;

		/**
		* Returns true if any settings of the sprite render are updated,
		* but the current sprite render instance does NOT account the new settings
		* (does NOT make sense if there's no existing sprite render instance at the moment).
		*/
		virtual bool AreSettingsUpdated() const = 0;

		/**
		* To be called each time DefaultTesterConfig is updated
		* To be called from the Fixtures automatically.
		*/
		virtual void OnDefaultTesterConfigUpdated(const TesterConfig& InDefautConfig) = 0;

		/**
		* To be called each time D3DDevice or any of its COM objects (render target etc.) created or destroyed.
		*
		* WARNING!!! The device pointer may be passed as nullptr.
		*/
		virtual void OnD3DUpdated(const D3DDevice* pInDev) = 0;

		/**
		* To be called each time the viewport window is changed.
		*/
		virtual void OnViewportWindowChanged(HWND hWnd) = 0;
		
		/**
		* To be called each time the new log is to be used.
		*/
		virtual void SetLog(std::ofstream* pInLog) = 0;
		virtual std::ofstream& GetLog() const = 0;

		// ~Canvas operations Begin
		CanvasHandle CreateCanvas(CanvasId InId, unsigned int InWidth, unsigned int InHeight);
		void DeleteCanvas(CanvasHandle InHandle);
		void ShowCanvas(CanvasHandle InHandle);
		void HideCanvas(CanvasHandle InHandle);
		// ~Canvas operations End

		// ~Sprite operations Begin
		SpriteHandle CreateSprite
		(
			SprId InId,
			MySprRen::ISpriteUpdater* pInUpdater,
			CanvasHandle pInCanvas,
			const MySprMath::SVec2& InPosition,
			float InWidth, float InHeight,
			MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
			const SpriteTransparencyMode& InTransparencyMode = SpriteTransparencyMode::Opaque,
			const MySprMath::SVec2& InOrigin = { 0.0F, 0.0F },
			float InRotationAngle = 0.0F
		);
		SpriteHandle CreateSprite_ZOrderAfter
		(
			SprId InId,
			SprId InZBeforeSpriteId,
			MySprRen::ISpriteUpdater* pInUpdater,
			CanvasHandle pInCanvas,
			const MySprMath::SVec2& InPosition,
			float InWidth, float InHeight,
			MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
			const SpriteTransparencyMode& InTransparencyMode = SpriteTransparencyMode::Opaque,
			const MySprMath::SVec2& InOrigin = { 0.0F, 0.0F },
			float InRotationAngle = 0.0F
		);
		void DeleteSprite(MySprRen::ISpriteUpdater* pInUpdater, SpriteHandle InHandle);
		void ShowSprite(MySprRen::ISpriteUpdater* pInUpdater, SprId InId);
		void HideSprite(MySprRen::ISpriteUpdater* pInUpdater, SprId InId);
		void SetSpriteTransparency(MySprRen::ISpriteUpdater* pInUpdater, SprId InId, MySpr::ESpriteTransparency InTransparency);
		void SetSpriteGeometry(MySprRen::ISpriteUpdater* pInUpdater, SprId InId, const MySpr::SSpriteGeometryProps& InGeometry);
		void SetMatInst(MySprRen::ISpriteUpdater* pInUpdater, SprId InId, MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState);
		void MoveSpriteZOrderAfter(MySprRen::ISpriteUpdater* pInUpdater, SprId InId, SprId InZBeforeSpriteId);
		// ~Sprite operations End

		// ~Info getters Begin
		/**
		* Description of coordinate system of the entire screen.
		*/
		virtual ScreenCoordSystemDesc GetScreenCoordSystem() const = 0;

		/**
		* Description of the coordinate system of the given canvas.
		*/
		virtual ScreenCoordSystemDesc GetCanvasCoordSystem(CanvasId InCanvasId) const = 0;

		virtual DXGI_FORMAT GetDefaultTextureFormat_Diffuse() const = 0;
		// ~Info getters End

		// ~ Materials Begin
		virtual Handle_SprMaterialInstance CreateMatInst_Default(const char* pInName, ID3D11ShaderResourceView* pInTexture2D) const = 0;
		// ~ Materials End

	private:
	};
} // Test