#pragma once

#include "ISpriteRender/ISpriteRender.h"
#include "CanvasManager.h"
#include <fstream>

namespace Dv
{
namespace Spr
{
namespace QRen
{

struct SConfig;

namespace IMPL
{

namespace D3D
{
	class RenResources;
}

class Environment;
class AmbientContext;
class SpriteGeometryBuffer;
class Canvas;
class SpriteUpdater;

/**
* Implementor of the ISpriteRender.
* Performs entire rendering of the sprite render.
*/
class SpriteRender : public Ren::ISpriteRender
{
public:
	virtual ~SpriteRender();
	SpriteRender(Environment* pInEnv);

	/**
	* @see: interface method comment
	*/
	virtual void BeginFrame() override;

	/**
	* @see: interface method comment
	*/
	virtual void EndFrame() override;

	/**
	* @see: interface method comment
	*/
	virtual void RenderAllVisibleCanvasses() override;

	/**
	* @see: interface method comment
	*/
	virtual void RenderCanvas(SpriteCanvasId InCanvasId) override;

	/**
	* @see: interface method comment
	*/
	virtual void BeginPickFrame() override;

	/**
	* @see: interface method comment
	*/
	virtual void EndPickFrame() override;

	/**
	* @see: interface method comment
	*/
	virtual void RenderAllPickableCanvasses() override;

	/**
	* @see: interface method comment
	*/
	virtual void RenderPickableCanvas(SpriteCanvasId InCanvasId) override;

	/**
	* @see: interface method comment
	*/
	virtual void EnableCanvasPicking(SpriteCanvasId InId) override;

	/**
	* @see: interface method comment
	*/
	virtual void DisableCanvasPicking(SpriteCanvasId InId) override;

	/**
	* @see: interface method comment
	*/
	virtual Ren::SPickResult PickAtScreen(const Math::SVec2& InPoint) override;

	/**
	* @see: interface method comment
	*/
	virtual Ren::SPickResult PickAtCanvasPoint(SpriteCanvasId InCanvasId, const Math::SVec2& InCanvasPoint) override;

	/**
	* @see: interface method comment
	*/
	virtual void BeginUpdates() override;

	/**
	* @see: interface method comment
	*/
	virtual void EndUpdates() override;

	/**
	* @see: interface method comment
	*/
	virtual Ren::ISpriteUpdater* BeginCanvasUpdate(SpriteCanvasId InCanvasId) override;

	/**
	* @see: interface method comment
	*/
	virtual void EndCanvasUpdate(Ren::ISpriteUpdater* pInUpdater) override;

	/**
	* Makes the given canvas visible.
	* Can be called ever during the update and during the rendering.
	*/
	virtual void ShowCanvas(SpriteCanvasId InCanvasId) override;

	/**
	* Makes the given canvas hidden.
	* Can be called ever during the update and during the rendering.
	*/
	virtual void HideCanvas(SpriteCanvasId InCanvasId) override;

	/**
	* Registers a canvas.
	* Calling during rendering exhibits an undefined behaviour.
	*/
	virtual void CreateCanvas(const Ren::SSpriteCanvasCreateCommandInitializer& InInitializer) override;

	/**
	* Deletes a canvas.
	* Calling during rendering exhibits an undefined behaviour.
	*/
	virtual void DeleteCanvas(SpriteCanvasId InCanvasId) override;

	/**
	* Moves the canvas right after the given one in the ZOrder.
	* Calling during rendering the rendering exhibits an undefined behaviour.
	*
	* @argument InCanvasId: already registered canvas id of the canvas to be moved.
	* @argument InZBeforeSpriteId: already registered canvas id of the sprite, that must preceed the given canvas in the ZOrder
	* (if null, the canvas is to be moved on top of the ZOrder).
	*/
	virtual void MoveCanvasZOrderAfter(SpriteCanvasId InCanvasId, SpriteCanvasId InZBeforeCanvasId) override;

	/**
	* Capacity of the canvas in sprites.
	*/
	virtual int GetCapacityInSprites_ForCanvas(SpriteCanvasId InCanvasId) const override;

	/**
	* Total number of sprites (both visible and hidden) for the given canvas.
	*/
	virtual int GetNumSprites_ForCanvas(SpriteCanvasId InCanvasId) const override;

	/**
	* Number of visible sprites for the given canvas
	*/
	virtual int GetNumVisibleSprites_ForCanvas(SpriteCanvasId InCanvasId) const override;

	/**
	* Max count of canvasses.
	*/
	virtual int GetMaxCanvasLimit() const override;

	/**
	* Count of visible canvasses
	*/
	virtual int GetNumCanvasses() const override;

	/**
	* Count of visible canvasses
	*/
	virtual int GetNumVisibleCanvasses() const override;

private:
	std::ofstream& GetLog();
	AmbientContext* GetAmbientContext();
	SpriteGeometryBuffer* GetSpriteGeometryBuffer();
	D3D::RenResources* GetRenResources();
	ID3D11DeviceContext* GetDevCon();
	const SConfig& GetConfig() const;
	CanvasManager* GetCanvasManager() const { return pCanvasManager.get(); }

	std::unique_ptr<CanvasManager> pCanvasManager;

	Environment* _pEnv;
	bool _bRenderingStarted;
	bool IsUpdating() const;
	bool IsUpdatingCanvas() const;
	SpriteUpdater* GetCanvasUpdater() const;
	bool _bUpdating;
	std::unique_ptr<SpriteUpdater> _pCanvasUpdater;
};

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv