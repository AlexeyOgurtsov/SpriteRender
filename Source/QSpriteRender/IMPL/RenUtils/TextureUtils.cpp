#pragma once

#include "TextureUtils.h"
#include "../Utils/CommonSystem.h"
#include "RenHRUtils.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{
namespace D3D
{
	TextureHandle CreateTexture2D
	(
		ID3D11Device* pDev, 
		UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat,
		UINT InBindFlags,
		D3D11_USAGE InUsage,
		UINT InCpuAccessFlags,
		UINT NumMips
	)
	{
		D3D11_TEXTURE2D_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.Width = InWidth;
		Desc.Height = InHeight;
		Desc.Format = InFormat;
		Desc.Usage = InUsage;
		Desc.BindFlags = InBindFlags;
		Desc.CPUAccessFlags = InCpuAccessFlags;
		Desc.ArraySize = 1;
		Desc.MipLevels = NumMips;
		
		ID3D11Texture2D* pTex = nullptr;
		HRESULT hr = pDev->CreateTexture2D(&Desc, nullptr, &pTex);
		if(FAILED(hr))
		{
			std::string Message = 
			 	std::string("QSpriteRender: Failed to create texture: ")
				+ std::string("HR=") + GetDevceHRESULTString(hr)
				+ std::string("; Width=") + std::to_string(InWidth)
				+ std::string("; Height=") + std::to_string(InHeight)
				+ std::string("; Format=") + GetFormatString(InFormat);
			throw SpriteRenderException(Message);
		}
		return TextureHandle(pTex);
	}
} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL 
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv