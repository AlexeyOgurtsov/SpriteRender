#include "CanvasManager.h"
#include "../Subsystem/SpriteRenderSubsystemEnvironment.h"
#include "../Utils/CommonSystem.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

SCanvasManagerInitializer::SCanvasManagerInitializer(Environment* pInEnv) :
	pEnv {pInEnv}
{
	BOOST_ASSERT(pInEnv);
}

CanvasManager::CanvasManager(const SCanvasManagerInitializer& InInitializer) :
	pEnv { InInitializer.pEnv }
,	NumVisibleCanvasses{0}
{
	BOOST_ASSERT(pEnv);
}

Canvas* CanvasManager::FindById(SpriteCanvasId InId) const
{
	auto it = ById.find(InId);
	if (it == ById.cend())
	{
		return nullptr;
	}
	return it->second;
}

CanvasManager::VisibleIteratorType CanvasManager::GetIterator_CanvasByZOrder() const
{
	return SpriteCanvasListIterator { &ByZOrder, ByZOrder.begin(), ESpriteCanvasVisibilityFilter::None };
}

CanvasManager::IteratorType CanvasManager::GetIterator_VisibleCanvasByZOrder() const
{
	return SpriteCanvasListIterator { &ByZOrder, ByZOrder.begin(), ESpriteCanvasVisibilityFilter::OnlyVisible };
}


void CanvasManager::Unregister_FromZOrderList(const Canvas* pCanvas)
{
	ByZOrder.erase(pCanvas->GetIterator().It_CanvasList);
}

CanvasList::iterator CanvasManager::Register_InZOrderList(Canvas* pCanvas, SpriteCanvasId InZBeforeCanvasId)
{
	BOOST_ASSERT(pCanvas);

	CanvasList::iterator It_ZAfterCanvas;

	if (IsValidCanvasId(InZBeforeCanvasId))
	{
		T_LOG("CanvasManager: Register canvas AFTER canvas with ID=" << InZBeforeCanvasId);
		Canvas* const pZBeforeCanvas = FindById(InZBeforeCanvasId);
		BOOST_ASSERT_MSG(pZBeforeCanvas, "CanvasManager: Canvas with the given ZBeforeCanvasId must exist");
		auto It_ZBeforeCanvas = pZBeforeCanvas->GetIterator().It_CanvasList;
		BOOST_ASSERT_MSG(It_ZBeforeCanvas != ByZOrder.cend(), "The to insert after must be registered in the container of canvasses!");
		// As insert position is position "after", we must increment the iterator to insert after:
		It_ZAfterCanvas = std::next(It_ZBeforeCanvas);
	}
	else
	{
		T_LOG("CanvasManager: Registering canvas ON TOP of Z-Order");
		It_ZAfterCanvas = ByZOrder.end();

	}
	CanvasList::iterator It = ByZOrder.insert(It_ZAfterCanvas, pCanvas);
	pCanvas->BindZOrderIterator(It);
	return It;
}


void CanvasManager::CreateCanvas(const SCreateCanvasArgs& InArgs)
{
	T_LOG("CanvasManager::CreateCanvas...");
	Canvas* const pCanvas = NewCanvasObject(InArgs);
	CanvasStorage::iterator It_CanvasInStorage = Storage.emplace(Storage.end(), pCanvas);

	if (InArgs.ShouldShow())
	{
		IncrementVisibleCanvasses();
	}

	auto[ItById, bRegistered] = ById.insert(std::make_pair(InArgs.GetTargetCanvasId(), pCanvas));
	BOOST_ASSERT_MSG(bRegistered, "SpriteManager::CreateSprite: sprite with the given id must not be registered yet");

	pCanvas->BindStorageIterator(It_CanvasInStorage);
	CanvasList::iterator It_ByZOrder = Register_InZOrderList(pCanvas, InArgs.GetZBeforeCanvasId());

	T_LOG("CanvasManager::CreateCanvas DONE");
}

Canvas* CanvasManager::NewCanvasObject(const SCreateCanvasArgs& InArgs)
{
	SCanvasInitializer Initializer { GetAmbientContext(), InArgs, GetRenResources() };	
	Initializer.MaxSprites = (InArgs.GetInitialCapacityInSprites() == Ren::DEFAULT_CAPACITY_IN_SPRITES) ? GetConfig().InitialSpriteBufferCapacity : InArgs.GetInitialCapacityInSprites();
	Initializer.bAutoResizeable = InArgs.IsAutoResizeable();
	Initializer.bDebug = InArgs.ShouldDebug();
	return new Canvas(Initializer);
}

void CanvasManager::DeleteCanvas(SpriteCanvasId InId)
{
	T_LOG("CanvasManager::DeleteCanvas...");
	Canvas* const pCanvas = FindById(InId);
	BOOST_ASSERT_MSG(pCanvas, "CanvasManager::DeleteCanvas: canvas with the given Id must exist");
	if (pCanvas->IsVisible())
	{
		DecrVisibleCanvasses();
	}
	Unregister_FromZOrderList(pCanvas);
	size_t const numRemoved_FromSpritesById = ById.erase(pCanvas->GetId());
	BOOST_ASSERT_MSG(numRemoved_FromSpritesById == 1, "CanvasManager::DeleteCanvas: canvas with the given Id must be registered in the ById container");
	Storage.erase(pCanvas->GetIterator().It_CanvasStorage);
	T_LOG("CanvasManager::DeleteCanvas DONE");
}

void CanvasManager::MoveCanvasZOrderAfter(SpriteCanvasId InId, SpriteCanvasId InZBeforeCanvasId)
{
	Canvas* const pCanvas = FindById(InId);
	BOOST_ASSERT_MSG(pCanvas, "CanvasManager::MoveCanvasZOrderAfter: canvas with the given Id must exist");
	Unregister_FromZOrderList(pCanvas);
	Register_InZOrderList(pCanvas, InZBeforeCanvasId);
}

void CanvasManager::ShowCanvas(SpriteCanvasId InId)
{
	T_LOG("CanvasManager::ShowCanvas...");
	Canvas* const pCanvas = FindById(InId);
	BOOST_ASSERT_MSG(pCanvas, "CanvasManager::ShowCanvas: canvas with the given Id must exist");
	if (pCanvas->IsHidden())
	{
		pCanvas->SetVisibility(true);
		IncrementVisibleCanvasses();
	}
	T_LOG("CanvasManager::ShowCanvas DONE");
}

void CanvasManager::HideCanvas(SpriteCanvasId InId)
{
	T_LOG("CanvasManager::HideCanvas...");
	Canvas* const pCanvas = FindById(InId);
	BOOST_ASSERT_MSG(pCanvas, "CanvasManager::HideCanvas: canvas with the given Id must exist");
	if (pCanvas->IsVisible())
	{
		pCanvas->SetVisibility(false);
		DecrVisibleCanvasses();
	}
	T_LOG("CanvasManager::HideCanvas DONE");
}

void CanvasManager::DecrVisibleCanvasses()
{
	BOOST_ASSERT_MSG(NumVisibleCanvasses > 0, "CanvasManager::DecrVisibleCanvasses: at least one sprite must be visible to decrement visibility count");
	NumVisibleCanvasses--;
	if (NumVisibleCanvasses == 0)
	{
		T_LOG("Now ALL CANVASSES are HIDDEN");
	}
}

void CanvasManager::IncrementVisibleCanvasses()
{
	NumVisibleCanvasses++;
	T_LOG("Now THERE're VISIBLE canvasses");
}

std::ofstream& CanvasManager::GetLog() const
{
	return GetAmbientContext()->GetLog();
}

const SConfig& CanvasManager::GetConfig() const
{
	return GetRenResources()->GetConfig(); 
}

D3D::RenResources* CanvasManager::GetRenResources() const
{
	return pEnv->GetRenResources(); 
}

AmbientContext* CanvasManager::GetAmbientContext() const
{
	return pEnv->GetAmbientContext();
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv