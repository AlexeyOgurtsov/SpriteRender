#pragma once

#include "ISpriteRender/ISpriteUpdater.h"
#include "ISprite/SpriteCanvasTypedefs.h"

namespace Dv
{
namespace Spr
{
namespace Ren
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
class SpriteUpdater : public ISpriteUpdater
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
	virtual void CreateSprite(const SSpriteCreateCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void DeleteSprite(const SSpriteDeleteCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void HideSprite(const SSpriteHideCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void ShowSprite(const SSpriteShowCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void SetSpriteTransparency(const SSpriteSetTransparencyCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void SetSpriteGeometry(const SSpriteSetGeometryCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void SetSpriteMaterial(const SSpriteSetMaterialCommandInitializer& InCommand) override;

	/**
	* @see: interface method comment.
	*/
	virtual void SetSpriteZOrder(const SSpriteSetZOrderCommandInitializer& InCommand) override;

private:
	Canvas* _pCanvas;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv