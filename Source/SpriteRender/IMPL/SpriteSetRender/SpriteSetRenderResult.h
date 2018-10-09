#pragma once

#include "SpriteRender/IMPL/RenUtils/RenCommandListTypes.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

/**
* Result of rendering a set of sprites with the SpriteSetRender.
*/
class SpriteSetRenderResult
{
public:
	SpriteSetRenderResult() :
		_pCommandList(nullptr) {}
	SpriteSetRenderResult(D3D::CommandListHandle&& pInCommandList) :
		_pCommandList(std::move(pInCommandList)) {}

	bool IsRelevant() const { return GetCommandList() != nullptr; }
	ID3D11CommandList* GetCommandList() const { return _pCommandList.get(); }

	void Invalidate()
	{
		_pCommandList.reset();
	}

private:
	D3D::CommandListHandle _pCommandList;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv