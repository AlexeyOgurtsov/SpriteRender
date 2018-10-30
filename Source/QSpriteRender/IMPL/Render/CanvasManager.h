#pragma once

#include <fstream>
#include <map>
#include "../Canvas/Canvas.h"
#include "CanvasManagerContainerTypes.h"
#include <climits>

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
} // D3D

class Environment;

struct SCanvasManagerInitializer
{
	Environment* pEnv = nullptr;

	SCanvasManagerInitializer(Environment* pInEnv);
};

class CanvasManager
{
public:
	using VisibleIteratorType = SpriteCanvasListIterator;
	using IteratorType = SpriteCanvasListIterator;

	CanvasManager(const SCanvasManagerInitializer& InInitializer);

	void CreateCanvas(const SCreateCanvasArgs& InArgs);
	void DeleteCanvas(SpriteCanvasId InCanvasId);

	void ShowCanvas(SpriteCanvasId InCanvasId);
	void HideCanvas(SpriteCanvasId InCanvasId);

	void MoveCanvasZOrderAfter(SpriteCanvasId InCanvasId, SpriteCanvasId InZBeforeCanvasId);

	/**
	* Tries to find canvas by id.
	*
	* @returns: nullptr if canvas is NOT found.
	*/
	Canvas* FindById(SpriteCanvasId InId) const;
	
	/**
	* Returns iterator for iterating all canvasses in the Z Order.
	*/
	VisibleIteratorType GetIterator_CanvasByZOrder() const;

	/**
	* Returns iterator for iterating all visible canvasses in the Z Order.
	*/
	IteratorType GetIterator_VisibleCanvasByZOrder() const;

	/**
	* Limit of canvasses that can be created simultaneosly.
	*/
	int GetMaxCanvasLimit() const { return INT_MAX; }

	/**
	* Total number of all canvasses (both visible and hidden).
	*/
	int GetNumCanvasses() const { return Storage.size(); }

	/**
	* Number of canvasses marked as visible right now.
	*/
	int GetNumVisibleCanvasses() const { return NumVisibleCanvasses; }

private:
	std::ofstream& GetLog() const;
	const SConfig& GetConfig() const;
	D3D::RenResources* GetRenResources() const;
	AmbientContext* GetAmbientContext() const;

	Environment* pEnv = nullptr;

	/**
	* Registers canvas in the ZOrder list.
	* Automatically binds the.
	*/
	CanvasList::iterator Register_InZOrderList(Canvas* pCanvas, SpriteCanvasId InZBeforeCanvasId);

	/**
	* Unregisters the given sprite from the SpritesByZOrder list
	*/
	void Unregister_FromZOrderList(const Canvas* pCanvas);

	Canvas* NewCanvasObject(const SCreateCanvasArgs& InArgs);

	void DecrVisibleCanvasses();
	void IncrementVisibleCanvasses();

	int NumVisibleCanvasses;

	CanvasStorage Storage;
	std::map<SpriteCanvasId, Canvas*> ById;
	CanvasList ByZOrder;
};

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv