#include "../ISprPerTestFixture_SingleCanvas_SmokePick.h"

namespace Test::ISpr
{
	ISprPerTestFixture_SingleCanvas_SmokePick::ISprPerTestFixture_SingleCanvas_SmokePick()
	{
		T_LOG("ISprPerTestFixture_SingleCanvas_SmokePick: CTOR...");
		SetTextureFormat(GetSprRen_DefaultTextureFormat_Diffuse());
		SetPickProps(MySpr::SCanvasPickProps::Enabled);
		EnablePick();
		T_LOG("ISprPerTestFixture_SingleCanvas_SmokePick: CTOR DONE");
	}
	ISprPerTestFixture_SingleCanvas_SmokePick::~ISprPerTestFixture_SingleCanvas_SmokePick()
	{
		T_LOG("ISprPerTestFixture_SingleCanvas_SmokePick: ~DTOR...");
		DisablePick();
		T_LOG("ISprPerTestFixture_SingleCanvas_SmokePick: ~DTOR DONE");
	}

	MySprRen::SPickResult ISprPerTestFixture_SingleCanvas_SmokePick::PickSprite(const IFrameCheckContextHandle& pChecker, SpriteHandle InSprite)
	{
		return Test::PickSprite(pChecker.get(), GetCanvHandle(), InSprite);
	}
	
	MySprRen::SPickResult ISprPerTestFixture_SingleCanvas_SmokePick::PickSprite_AtCanvasPoint(const IFrameCheckContextHandle& pChecker, SpriteHandle InSprite, const SprVec2& InSpriteLBPosition_AsCanvasPoint)
	{
		return Test::PickSprite_AtCanvasPoint(pChecker.get(), GetCanvHandle(), InSprite, InSpriteLBPosition_AsCanvasPoint);
	}

	bool ISprPerTestFixture_SingleCanvas_SmokePick::SpritePickedAs_AtCanvasPoint(const IFrameCheckContextHandle& pChecker, SpriteHandle InSprite, MySpr::PickObjectId InObjId, const SprVec2& InSpriteLBPosition_AsCanvasPoint)
	{
		return Test::SpritePickedAs_AtCanvasPoint(pChecker.get(), GetCanvHandle(), InSprite, InObjId, InSpriteLBPosition_AsCanvasPoint);
	}

	bool ISprPerTestFixture_SingleCanvas_SmokePick::SpritePickedAs(const IFrameCheckContextHandle& pChecker, SpriteHandle InHandle, MySpr::PickObjectId InObjectId)
	{		
		return Test::SpritePickedAs(pChecker.get(), GetCanvHandle(), InHandle, InObjectId);
	}

	bool ISprPerTestFixture_SingleCanvas_SmokePick::SpriteNotPicked(const IFrameCheckContextHandle& pChecker, SpriteHandle InHandle)
	{
		return Test::SpriteNotPicked(pChecker.get(), GetCanvHandle(), InHandle);
	}

	bool ISprPerTestFixture_SingleCanvas_SmokePick::SpriteNotPickedAt(const IFrameCheckContextHandle& pChecker, SpriteHandle InSprite, const SprVec2& InSpriteLBPosition_AsCanvasPoint)
	{
		return Test::SpriteNotPickedAt(pChecker.get(), GetCanvHandle(), InSprite, InSpriteLBPosition_AsCanvasPoint);
	}

	int ISprPerTestFixture_SingleCanvas_SmokePick::PreparePickSprite
	(
		TSSpriteVector* pOutSprites,
		MySpr::PickObjectId InObjectId,
		const TSMaterial& InMaterial,
		const MySprMath::SVec2& InPosition, const MySprMath::SSize& InSize,
		const MySprMath::SVec2& InOrigin,
		const SpriteTransparencyMode& InTransparencyMode,
		float InRotationAngle,
		MySpr::ESpritePickMode InPickMode,
		bool bInShow
	)
	{
		return PrepareSprite
		(
			pOutSprites, 
			MySpr::SSpritePickProps{InObjectId, InPickMode },
			InTransparencyMode,
			InMaterial,
			InPosition, InSize,
			InOrigin,			
			InRotationAngle,
			bInShow
		);
	}

	void ISprPerTestFixture_SingleCanvas_SmokePick::DisableSpritePick(SpriteHandle InHandle)
	{
		T_LOG("Fixture: SingleCanvas: DisableSpritePick, Id=" << InHandle->GetId() << "...");
		InHandle->DisablePick(GetUpdater());
		T_LOG("Fixture: SingleCanvas: DisableSpritePick << " << InHandle->GetId() << ": Picking disabled");
	}

	void ISprPerTestFixture_SingleCanvas_SmokePick::DisableSpritePick(const TSSprite& InSprite)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		DisableSpritePick(InSprite.GetHandle());
	}

	void ISprPerTestFixture_SingleCanvas_SmokePick::EnableSpritePick(SpriteHandle InHandle)
	{
		T_LOG("Fixture: SingleCanvas: EnableSpritePick, Id=" << InHandle->GetId() << "...");
		InHandle->EnablePick(GetUpdater());
		T_LOG("Fixture: SingleCanvas: EnableSpritePick << " << InHandle->GetId() << ": Picking enabled");
	}

	void ISprPerTestFixture_SingleCanvas_SmokePick::EnableSpritePick(const TSSprite& InSprite)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		EnableSpritePick(InSprite.GetHandle());
	}

	void ISprPerTestFixture_SingleCanvas_SmokePick::SetSpriteObjectId(const TSSprite& InSprite, MySpr::PickObjectId InObjectId)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		SetSpriteObjectId(InSprite.GetHandle(), InObjectId);
	}

	void ISprPerTestFixture_SingleCanvas_SmokePick::SetSpriteObjectId(SpriteHandle InHandle, MySpr::PickObjectId InObjectId)
	{
		T_LOG("Fixture: SingleCanvas: SetSpriteObjectId, Id=" << InHandle->GetId() << "...");
		InHandle->SetPickObjectId(GetUpdater(), InHandle->GetId(), InObjectId);
		T_LOG("Fixture: SingleCanvas: SetSpriteObjectId << " << InHandle->GetId() << ": object id changed");
	}

	boost::test_tools::predicate_result ISprPerTestFixture_SingleCanvas_SmokePick::CheckPickability(const IFrameCheckContextHandle& ContextHandle, const TSSpriteVector& InSprites)
	{
		for (int SpriteIndex = 0; SpriteIndex < static_cast<int>(InSprites.size()); SpriteIndex++)
		{
			const TSSprite* pS = &(InSprites[SpriteIndex]);

			if (pS->GetHandle()->GetPickProps().CanBePicked())
			{
				MySpr::PickObjectId ObjId = pS->GetHandle()->GetObjectId();
				if (false == SpritePickedAs(ContextHandle, pS->GetHandle(), ObjId))
				{
					boost::test_tools::predicate_result res{ false };
					res.message() << "Sprite " << SpriteIndex << "must be picked with ObjectId=" << ObjId;
					return res;
				}
			}
			else
			{
				// At this point the sprite must be NON-pickable
				if (false == SpriteNotPicked(ContextHandle, pS->GetHandle()))
				{
					boost::test_tools::predicate_result res{ false };
					res.message() << "Sprite " << SpriteIndex << "must be non-pickable";
					return res;
				}
			}
		}
		return true;
	}

	boost::test_tools::predicate_result ISprPerTestFixture_SingleCanvas_SmokePick::CheckAllPickable(const IFrameCheckContextHandle& ContextHandle, const TSSpriteVector& InSprites)
	{
		for (int SpriteIndex = 0; SpriteIndex < static_cast<int>(InSprites.size()); SpriteIndex++)
		{
			const TSSprite* pS = &(InSprites[SpriteIndex]);
			if (false == SpritePickedAs(ContextHandle, pS->GetHandle(), pS->GetHandle()->GetObjectId()))
			{
				boost::test_tools::predicate_result res{ false };
				res.message() << "Sprite " << SpriteIndex << "must be picked";
				return res;
			}
		}
		return true;
	}

	boost::test_tools::predicate_result ISprPerTestFixture_SingleCanvas_SmokePick::CheckAllNotPickable(const IFrameCheckContextHandle& ContextHandle, const TSSpriteVector& InSprites)
	{		
		for (int SpriteIndex = 0; SpriteIndex < static_cast<int>(InSprites.size()); SpriteIndex++)
		{
			const TSSprite* pS = &(InSprites[SpriteIndex]);
			if (false == SpriteNotPicked(ContextHandle, pS->GetHandle()))
			{
				boost::test_tools::predicate_result res{ false };
				res.message() << "Sprite " << SpriteIndex << "must be NON-picked";
				return res;
			}
		}
		return true;
	}

	void ISprPerTestFixture_SingleCanvas_SmokePick::AssignSpriteObjectIds(TSSpriteVector& Sprites, MySpr::PickObjectId InStartObjectId)
	{
		MySpr::PickObjectId ObjId = InStartObjectId;
		for (int i = 0; i < static_cast<int>(Sprites.size()); i++)
		{
			SetSpriteObjectId(Sprites[i], ObjId);
			++ObjId;
		}
	}
} // Test::ISpr