#pragma once

#include "Exception.h"
#include "Com.h"
#include <d3d11.h>
#include <memory>
#include <fstream>
#include <boost/assert.hpp>

namespace Test::IMPL
{
	using TextureHandle = std::unique_ptr<ID3D11Texture2D, ComReleaser>;
	using ShaderResourceViewHandle = std::unique_ptr<ID3D11ShaderResourceView, ComReleaser>;
	using RenderTargetViewHandle = std::unique_ptr<ID3D11RenderTargetView, ComReleaser>;

	/**
	* Properties of a 2D texture.
	*/
	struct STextureProps
	{
		uint32_t Width = 0, Height = 0;
		DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		STextureProps() :
			Width(0)
			, Height(0)
			, Format(DXGI_FORMAT_R8G8B8A8_UNORM) {}
		STextureProps(uint32_t InWidth, uint32_t InHeight, DXGI_FORMAT InFormat = DXGI_FORMAT_R8G8B8A8_UNORM) :
			Width(InWidth)
			, Height(InHeight)
			, Format(InFormat) {}
	};

	/**
	* Class of the D3D11 memory
	*/
	struct SD3D11MemoryClass
	{
	public:
		SD3D11MemoryClass() :
			_usage(D3D11_USAGE_DEFAULT)
			, _CPUAccessFlags(0) {}
		explicit SD3D11MemoryClass(D3D11_USAGE InUsage) :
			_usage(InUsage)
			, _CPUAccessFlags(0) {}

		D3D11_USAGE GetUsage() const { return _usage; }
		UINT GetCPUAccessFlags() const { return _CPUAccessFlags; }

	private:
		D3D11_USAGE _usage;
		UINT _CPUAccessFlags;
	};

	/**
	* Element of 2D texture, managed by the textures subsystem.
	*/
	struct TextureElement
	{
		TextureElement() = default;
		TextureElement(TextureHandle&& pInTexture, ShaderResourceViewHandle&& pInSRV, const STextureProps& InProps, const SD3D11MemoryClass& InMemoryClass) :
			_pTexture(std::move(pInTexture))
			, _pSRV(std::move(pInSRV))
			, _props(InProps)
			, _memoryClass(InMemoryClass) {}

		ID3D11Texture2D* GetTexture() const { return _pTexture.get(); }
		ID3D11ShaderResourceView* GetShaderResourceView() const { return _pSRV.get(); }
		const STextureProps& GetProps() const { return _props; }
		const SD3D11MemoryClass& GetMemoryClass() const { return _memoryClass; }

	private:
		TextureHandle _pTexture;
		ShaderResourceViewHandle _pSRV;
		STextureProps _props;
		SD3D11MemoryClass _memoryClass;
	};
} // Test::IMPL