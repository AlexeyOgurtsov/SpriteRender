#include "SpriteManager.h"
#include "../Utils/CommonSystem.h"
#include "../Environment/AmbientContext.h"
#include "../RenUtils/Geometry/SpriteGeometryVertex.h"
#include "ISprite/Math/MathUtils.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
	namespace IMPL
	{
		SSpriteManagerInitializer::SSpriteManagerInitializer(AmbientContext* pInAmbientContext, ID3D11Device* pInDev, ID3D11DeviceContext* pInDevCon, bool bInDebug) :
			pAmbientContext{ pInAmbientContext }
			, pDev{ pInDev }
			, pDevCon{ pInDevCon }
			, bDebug{ bInDebug }
		{
			BOOST_ASSERT(pAmbientContext);
			BOOST_ASSERT(pDev);
			BOOST_ASSERT(pDevCon);
		}

		SpriteManager::SpriteManager(const SSpriteManagerInitializer& InInitializer) :
			bDebug{ InInitializer.bDebug }
		,	pAmbientContext{ InInitializer.pAmbientContext }
		,	pDev{ InInitializer.pDev }
		,	pDevCon{ InInitializer.pDevCon }
		{
			InitializeVB(InInitializer.MaxSprites, InInitializer.bAutoResizeable);
		}

		std::ofstream& SpriteManager::GetLog() const
		{
			return pAmbientContext->GetLog();
		}

		void SpriteManager::InitializeVB(UINT InMaxSprites, bool bInAutoResizeable)
		{
			T_LOG("SpriteManager::InitializeVB...");
			BOOST_ASSERT_MSG(nullptr == pVB.get(), "SpriteManager::InitializeVB: to be called only once");
			D3D::SUniformBufferInitializer Initializer;
			Initializer.pLog = &GetLog();
			Initializer.bDebug = bDebug;
			Initializer.Name = "VertexBuffer";
			Initializer.pDev = pDev;
			Initializer.pDevCon = pDevCon;
			Initializer.SlotSize = D3D::SPRITE_GEOMETRY_SIZE;
			Initializer.bAutoResizeable = bInAutoResizeable;
			Initializer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			Initializer.Usage = D3D11_USAGE_DYNAMIC;
			Initializer.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE;
			Initializer.CapacityInSlots = InMaxSprites;
			pVB.reset(new D3D::UniformBuffer(Initializer));
			T_LOG("SpriteManager::InitializeVB Done");
		}

		void SpriteManager::FlushD3D()
		{
			return pVB->Flush();
		}

		ID3D11Buffer *SpriteManager::GetVB() const
		{
			return pVB->GetBuffer();
		}

		UINT SpriteManager::GetVBStride() const
		{
			return D3D::SPRITE_VERTEX_SIZE;
		}

		void SpriteManager::SetVB(ID3D11DeviceContext* pInDevCon, UINT InVBSlot)
		{
			ID3D11Buffer* const pVB = GetVB();
			UINT const Stride = GetVBStride();
			UINT const Offset = 0;
			pInDevCon->IASetVertexBuffers(InVBSlot, 1, &pVB, &Stride, &Offset);
		}

		SpriteManager::SpriteIteratorType SpriteManager::Iterator_SpriteByZOrder() const
		{
			return SpriteListIterator{ &SpritesByZOrder, SpritesByZOrder.begin(), ESpriteVisibilityFilter::None };
		}

		SpriteManager::VisibleSpriteIteratorType SpriteManager::Iterator_VisibleSpriteByZOrder() const
		{
			return SpriteListIterator{ &SpritesByZOrder, SpritesByZOrder.begin(), ESpriteVisibilityFilter::OnlyVisible };
		}

		size_t FillSpriteGeometry_ReturnSizeInBytes(D3D::SD3D11SpriteGeometryVertex* pOutVertices, const SSpriteGeometryData& InGeometry)
		{
			Math::SVec2 Vertices[4];
			Math::CalculateRectVertices
			(
				Vertices,
				Math::SVec2{ InGeometry.PositionX, InGeometry.PositionY },
				Math::SSize{ InGeometry.Width, InGeometry.Height },
				Math::SVec2{ InGeometry.OriginOffset[0], InGeometry.OriginOffset[1] },
				InGeometry.AngleDegs
			);

			// Top 
			D3D::SetD3D11SpriteVertex(&pOutVertices[0], Vertices[0].X, Vertices[0].Y, InGeometry.Z, InGeometry.TexCoordX[0], InGeometry.TexCoordY[0]);
			// Top right
			D3D::SetD3D11SpriteVertex(&pOutVertices[1], Vertices[1].X, Vertices[1].Y, InGeometry.Z, InGeometry.TexCoordX[1], InGeometry.TexCoordY[1]);
			// Bottom
			D3D::SetD3D11SpriteVertex(&pOutVertices[2], Vertices[2].X, Vertices[2].Y, InGeometry.Z, InGeometry.TexCoordX[2], InGeometry.TexCoordY[2]);
			// Bottom right
			D3D::SetD3D11SpriteVertex(&pOutVertices[3], Vertices[3].X, Vertices[3].Y, InGeometry.Z, InGeometry.TexCoordX[3], InGeometry.TexCoordY[3]);
			return (sizeof(D3D::SD3D11SpriteGeometryVertex) * 4);
		}

		D3D::BufferAlloc AllocSprite(D3D::UniformBuffer* pVB, const SSpriteGeometryData& InGeometry)
		{
			D3D::SD3D11SpriteGeometryVertex Vertices[4];
			const size_t SZ = FillSpriteGeometry_ReturnSizeInBytes(Vertices, InGeometry);
			return pVB->Alloc(Vertices, SZ);
		}

		void ReallocSprite(D3D::UniformBuffer* pVB, const D3D::BufferAlloc& InVBAlloc, const SSpriteGeometryData& InGeometry)
		{
			D3D::SD3D11SpriteGeometryVertex Vertices[4];
			const size_t SZ = FillSpriteGeometry_ReturnSizeInBytes(Vertices, InGeometry);
			return pVB->OverwriteAlloc(InVBAlloc, Vertices, SZ);
		}

		Sprite* NewSpriteObject(const D3D::BufferAlloc& InVBAlloc, const SCreateSpriteArgs& InArgs)
		{
			SSpriteInitializer Initializer;
			Initializer.bVisible = InArgs.ShouldShow();
			Initializer.Geometry = InArgs.GetRenderLayerGeometryData();
			Initializer.Id = InArgs.GetTargetSpriteId();
			Initializer.pMaterialRenderState = InArgs.GetRenderState();
			Initializer.RenderMode = InArgs.GetRenderMode();
			Initializer.VBAlloc = InVBAlloc;
			return new Sprite(Initializer);
		}

	void SpriteManager::CreateSprite(const SCreateSpriteArgs& InArgs)
	{
		T_LOG("SpriteManager::CreateSprite...");
		D3D::BufferAlloc const Alloc = AllocSprite(pVB.get(), InArgs.GetRenderLayerGeometryData());

		Sprite* const pSpr = NewSpriteObject(Alloc, InArgs);
		SpriteStorageList::iterator It_SpriteInStorage = Sprites.emplace(Sprites.end(), pSpr);

		if (InArgs.ShouldShow())
		{
			IncrementVisibleSprites();
		}

		auto [ItById, bRegistered] = SpritesById.insert(std::make_pair(InArgs.GetTargetSpriteId(), pSpr));
		BOOST_ASSERT_MSG(bRegistered, "SpriteManager::CreateSprite: sprite with the given id must not be registered yet");
				
		SpriteList::iterator It_ByZOrder = Register_InZOrderList(pSpr, InArgs.GetZBeforeSpriteId());

		SpriteIterator SpriteIt;
		SpriteIt.It_Storage = It_SpriteInStorage;
		SpriteIt.It_ByZOrder = It_ByZOrder;
		pSpr->BindIterator(SpriteIt);

		T_LOG("SpriteManager::CreateSprite DONE");
	}

	SpriteList::iterator SpriteManager::Register_InZOrderList(Sprite* pSprite, SpriteId InZBeforeSpriteId)
	{
		BOOST_ASSERT(pSprite);

		SpriteList::iterator It_ZAfterSprite;

		if (IsValidSpriteId(InZBeforeSpriteId))
		{
			T_LOG("SpriteManager: Register sprite AFTER sprite with ID=" << InZBeforeSpriteId);
			Sprite* const pZBeforeSprite = GetSpriteById(InZBeforeSpriteId);
			BOOST_ASSERT_MSG(pZBeforeSprite, "SpriteManager: Sprite with the given ZBeforeSpriteId must exist");
			auto It_ZBeforeSprite = pZBeforeSprite->GetIterator().It_ByZOrder;
			BOOST_ASSERT_MSG(It_ZBeforeSprite != SpritesByZOrder.cend(), "The to insert after must be registered in the container of sprites!");
			// As insert position is position "after", we must increment the iterator to insert after:
			It_ZAfterSprite = std::next(It_ZBeforeSprite);
		}
		else
		{
			T_LOG("SpriteManager: Registering sprite ON TOP of Z-Order");
			It_ZAfterSprite = SpritesByZOrder.end();
			
		}
		return SpritesByZOrder.insert(It_ZAfterSprite, pSprite);
	}

	Sprite* SpriteManager::GetSpriteById(SpriteId InId) const
	{
		auto it = SpritesById.find(InId);
		if (it == SpritesById.cend())
		{
			return nullptr;
		}
		return it->second;
	}

	void SpriteManager::DeleteSprite(SpriteId InId)
	{
		T_LOG("SpriteManager::DeleteSprite...");
		Sprite* const pSprite = GetSpriteById(InId);
		BOOST_ASSERT_MSG(pSprite, "SpriteManager::DeleteSprite: sprite with the given Id must exist");
		D3D::BufferAlloc Alloc = pSprite->GetVBAlloc();
		pVB->FreeAlloc(/*Out*/Alloc);
		if (pSprite->IsVisible())
		{
			DecrVisibleSprites();
		}
		Unregister_FromZOrderList(pSprite);
		size_t const numRemoved_FromSpritesById = SpritesById.erase(pSprite->GetId());
		BOOST_ASSERT_MSG(numRemoved_FromSpritesById == 1, "SpriteManager::DeleteSprite: sprite with he given Id must be registered in the SpritesById container");
		Sprites.erase(pSprite->GetIterator().It_Storage);
		T_LOG("SpriteManager::DeleteSprite DONE");
	}
	
	void SpriteManager::SetSpriteTransparency(SpriteId InId, ESpriteTransparency InTransparency)
	{
		Sprite* const pSprite = GetSpriteById(InId);
		BOOST_ASSERT_MSG(pSprite, "SpriteManager::SetSpriteTransparency: sprite with the given Id must exist");
		pSprite->SetTransparency(InTransparency);
	}

	
	void SpriteManager::SetMaterialInstance(SpriteId InId, MaterialInstanceRenderStateHandle pInRenderState)
	{
		Sprite* const pSprite = GetSpriteById(InId);
		BOOST_ASSERT_MSG(pSprite, "SpriteManager::SetSpriteTransparency: sprite with the given Id must exist");
		pSprite->SetMaterialRenderState(pInRenderState);
	}


	void SpriteManager::SetSpriteGeometry(SpriteId InId, const SSpriteGeometryData& InGeometry)
	{
		Sprite* const pSprite = GetSpriteById(InId);
		BOOST_ASSERT_MSG(pSprite, "SpriteManager::SetSpriteGeometry: sprite with the given Id must exist");
		D3D::BufferAlloc Alloc = pSprite->GetVBAlloc();
		ReallocSprite(pVB.get(), /*Out*/Alloc, InGeometry);
		pSprite->SetVBAlloc(Alloc);
		pSprite->SetGeometry(InGeometry);
	}

	void SpriteManager::MoveSpriteZOrderAfter(SpriteId InId, SpriteId InZBeforeSpriteId)
	{
		Sprite* const pSprite = GetSpriteById(InId);
		BOOST_ASSERT_MSG(pSprite, "SpriteManager::MoveSpriteZOrderAfter: sprite with the given Id must exist");
		Unregister_FromZOrderList(pSprite);
		Register_InZOrderList(pSprite, InZBeforeSpriteId);
	}

	void SpriteManager::Unregister_FromZOrderList(const Sprite* pSprite)
	{
		SpritesByZOrder.erase(pSprite->GetIterator().It_ByZOrder);
	}

	void SpriteManager::ShowSprite(SpriteId InId)
	{
		T_LOG("SpriteManager::ShowSprite...");
		Sprite* const pSprite = GetSpriteById(InId);
		BOOST_ASSERT_MSG(pSprite, "SpriteManager::ShowSprite: sprite with the given Id must exist");
		pSprite->SetVisibility(true);
		IncrementVisibleSprites();
		T_LOG("SpriteManager::ShowSprite DONE");
	}
	
	void SpriteManager::HideSprite(SpriteId InId)
	{
		T_LOG("SpriteManager::HideSprite...");
		Sprite* const pSprite = GetSpriteById(InId);
		BOOST_ASSERT_MSG(pSprite, "SpriteManager::HideSprite: sprite with the given Id must exist");
		pSprite->SetVisibility(false);
		DecrVisibleSprites();
		T_LOG("SpriteManager::HideSprite DONE");
	}

	void SpriteManager::DecrVisibleSprites()
	{
		BOOST_ASSERT_MSG(NumVisibleSprites > 0, "SpriteManager::DecrVisibleSprites: at least one sprite must be visible to decrement visibility count");
		NumVisibleSprites--;
		if (NumVisibleSprites == 0)
		{
			T_LOG("Now ALL SPRITES are HIDDEN");
		}
	}

	void SpriteManager::IncrementVisibleSprites()
	{
		NumVisibleSprites++;
		T_LOG("Now THERE're VISIBLE sprites");
	}
} // Dv::Spr::QRen::IMPL

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv