#pragma once

#include <string>
#include <d3d11.h>

namespace Dv
{
namespace Spr
{
	class SpriteRenderException
	{
	public:
		virtual ~SpriteRenderException() = default;
		SpriteRenderException(const std::string& InMessage);
	
		virtual void AppendToString(std::string* pOutString) const;

	protected:
		const std::string& GetMainMessage() const { return _message; }

	private:
		std::string _message;
	};

	class SpriteRenderHRFailure : public SpriteRenderException
	{
	public:
		SpriteRenderHRFailure(const std::string& InMessage, HRESULT InHR);

		virtual void AppendToString(std::string* pOutString) const override;

	private:
		HRESULT _hr;
	};
} // Dv::Spr
} // Dv