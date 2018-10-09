#include "ISprite/Core/SpriteRenderException.h"
#include "ISprite/IMPL/RenUtils/RenHRUtils.h"

namespace Dv
{
namespace Spr
{
	SpriteRenderException::SpriteRenderException(const std::string& InMessage) :
		_message(InMessage)
	{
	}
	
	void SpriteRenderException::AppendToString(std::string* pOutString) const
	{
		*pOutString = _message;
	}

	SpriteRenderHRFailure::SpriteRenderHRFailure(const std::string& InMessage, HRESULT InHR) :
		SpriteRenderException(InMessage)
	,	_hr(InHR)
	{
	}

	void SpriteRenderHRFailure::AppendToString(std::string* pOutString) const
	{
		pOutString->append(GetMainMessage());
		pOutString->append("\nHR=");
		pOutString->append(IMPL_ISPR::D3D::GetCommonHRESULTString(_hr));
	}
} // Dv::Spr
} // Dv