#pragma once

#include "ISpriteRender/ISpriteUpdater.h"
#include "ISprite/SpriteCanvasTypedefs.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

class Canvas;

struct SSpriteUpdaterInitializer
{
	Canvas* pCanvas;
	
	SSpriteUpdaterInitializer(Canvas* pInCanvas) :
		pCanvas(pInCanvas) {}
};

/**
* Implementation of the sprite updater for the given canvas.
*/
class SpriteUpdater : public Ren::ISpriteUpdater
{
public:
	SpriteUpdater(const SSpriteUpdaterInitializer& InInitializer);

	/**
	* Returns id of the canvas that is being updated.
	*/
	SpriteCanvasId GetCanvasId() const;

	/**
	* @see: interface method comment.
	*/
	virtual void CreateSprite(const Ren::SSpriteCreateCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void DeleteSprite(const Ren::SSpriteDeleteCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void HideSprite(const Ren::SSpriteHideCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void ShowSprite(const Ren::SSpriteShowCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void SetSpriteTransparency(const Ren::SSpriteSetTransparencyCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void SetSpriteGeometry(const Ren::SSpriteSetGeometryCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void SetSpriteMaterial(const Ren::SSpriteSetMaterialCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void SetSpriteZOrder(const Ren::SSpriteSetZOrderCommandInitializer& InCommand) override;

private:
	Canvas* _pCanvas;
};

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv