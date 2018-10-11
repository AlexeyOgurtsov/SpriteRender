#pragma once

#include "Exception.h"
#include "Com.h"
#include <d3d11.h>
#include <memory>
#include <list>
#include <boost/assert.hpp>

namespace Test::IMPL
{
	using TextureHandle = std::unique_ptr<ID3D11Texture2D, ComReleaser>;
	using ShaderResourceViewHandle = std::unique_ptr<ID3D11ShaderResourceView, ComReleaser>;
	using RenderTargetViewHandle = std::unique_ptr<ID3D11RenderTargetView, ComReleaser>;

	using TextureData = D3D11_SUBRESOURCE_DATA;
	using RealTextureData = std::unique_ptr<uint8_t[]>;

	/**
	* Holds the source data of the texture.
	* Does NOT initializes the source data.
	*/
	class TextureSourceData
	{
	public:
		TextureSourceData(size_t InWidth, size_t InHeight, size_t InTexelSize, size_t InNumMips = 0) :
			_width(InWidth)
			, _height(InHeight)
			, _texelSize(InTexelSize)
			, _numMips(InNumMips)
		{
			BOOST_ASSERT(_width > 0);
			BOOST_ASSERT(_height > 0);
			BOOST_ASSERT(_texelSize > 0);
			BOOST_ASSERT_MSG(_numMips <= 1, "TextureSourceData: only up to one mip-mapping levels are supported now");

			_pTextureData.reset(new uint8_t[GetFullSize()]);

			std::memset(&_subresourceData, 0, sizeof(_subresourceData));
			_subresourceData.SysMemPitch = GetRowSize();
			_subresourceData.pSysMem = _pTextureData.get();
			_subresourceData.SysMemSlicePitch = 0;
		}

		size_t GetNumMips() const { return _numMips; }

		size_t GetTexelSize() const { return _texelSize; }
		int GetWidth() const { return _width; }
		int GetHeight() const { return _height; }

		size_t GetRowSize() const { return _width * GetTexelSize(); }
		size_t GetFullSize() const { return GetRowSize() * GetHeight(); }

		const D3D11_SUBRESOURCE_DATA& GetSubresourceData() const { return _subresourceData; }

		void SetTexel(int32_t InRowIndex, int32_t InX, const void* pInSource, size_t InSize)
		{
			BOOST_ASSERT(pInSource);
			void* pTexel = GetTexelPtr(InRowIndex, InX);
			std::memcpy(pTexel, pInSource, InSize);
		}

		void Fill(const void* pInSource, size_t InTexelSize)
		{
			for (int i = 0; i < GetHeight(); i++)
			{
				for (int j = 0; j < GetWidth(); j++)
				{
					SetTexel(i, j, pInSource, InTexelSize);
				}
			}
		}

		void* GetTexelPtr(int32_t InRowIndex, int32_t InX) { return const_cast<void*>(const_cast<const TextureSourceData*>(this)->GetTexelPtr(InRowIndex, InX)); }
		const void* GetTexelPtr(int32_t InRowIndex, int32_t InX) const { return _pTextureData.get() + (InRowIndex * GetRowSize()) + (InX * GetTexelSize()); }

	private:
		size_t _texelSize = 0;
		uint32_t _width = 0, _height = 0;
		size_t _numMips = 0;
		RealTextureData _pTextureData;
		D3D11_SUBRESOURCE_DATA _subresourceData;
	};

	struct TextureElement;
	using TextureList = std::list<TextureElement>;

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


	struct STexturesInitializer
	{
		ID3D11Device* pDev = nullptr;
		ID3D11DeviceContext* pDevCon = nullptr;

		STexturesInitializer(ID3D11Device* pInDev, ID3D11DeviceContext* pInDevCon) :
			pDev(pInDev)
			, pDevCon(pInDevCon) {}
	};

	/**
	* Initializer of a 2D texture
	*/
	struct STexInitializer
	{
		STextureProps Props;
		SD3D11MemoryClass MemoryClass;
		UINT BindFlags = D3D11_BIND_SHADER_RESOURCE;

		/**
		* If mip levels is zero, then the mips are to be generated automatically
		*/
		UINT MipLevels = 0;

		explicit STexInitializer(const STextureProps& InProps) :
			Props(InProps)
			, BindFlags(D3D11_BIND_SHADER_RESOURCE)
			, MipLevels(0) {}
		STexInitializer(const STextureProps& InProps, const SD3D11MemoryClass& InMemoryClass) :
			Props(InProps)
			, MemoryClass(InMemoryClass)
			, BindFlags(D3D11_BIND_SHADER_RESOURCE)
			, MipLevels(0) {}
		STexInitializer(const STextureProps& InProps, UINT InBindFlags, const SD3D11MemoryClass& InMemoryClass) :
			Props(InProps)
			, MemoryClass(InMemoryClass)
			, BindFlags(InBindFlags)
			, MipLevels(0) {}
	};

	/**
	* - Holds a set of textures.
	* - Generates mips for textures
	*/
	class Textures
	{
	public:
		Textures(const STexturesInitializer& InInitializer);

		/**
		* Adds a texture.
		* Never returns nullptr (throw exception on failure).
		*/
		TextureElement* AddTexture(const STexInitializer& InInitializer, const TextureData& InTextureData);

	private:
		ID3D11Device * _pDev = nullptr;
		ID3D11DeviceContext* _pDevCon = nullptr;
		TextureList _textures;
	};

	/**
	* Adds a texture of the uniform color and the given size.
	* @argument: InNumMips: count of mip levels (if zero, auto mip-map generation is used);
	*/
	TextureElement* AddUniformTexture(Textures* pTextures, const void* pInColor, size_t InTexSize, const STextureProps& InTexProps, UINT InNumMips = 0);

	/**
	* Adds a texture of uniform color and the given size and R8G8B8A8_UNORM format.
	*/
	inline TextureElement* AddUniformTexture_R8G8B8A8_UNORM(Textures* pTextures, const uint8_t InColor[4], size_t InWidth, size_t InHeight, UINT InNumMips = 0)
	{
		return AddUniformTexture(pTextures, InColor, sizeof(uint8_t) * 4, STextureProps{ InWidth, InHeight, DXGI_FORMAT_R8G8B8A8_UNORM }, InNumMips);
	}

	/**
	* Adds a texture of uniform color and the given size and R32G32B32A32_FLOAT format.
	*/
	inline TextureElement* AddUniformTexture_R32G32B32A32_FLOAT(Textures* pTextures, const float InColor[4], size_t InWidth, size_t InHeight, UINT InNumMips = 0)
	{
		return AddUniformTexture(pTextures, InColor, sizeof(float) * 4, STextureProps{ InWidth, InHeight, DXGI_FORMAT_R32G32B32A32_FLOAT }, InNumMips);
	}

	inline TextureElement* AddUniformTexture(Textures* pTextures, const void* pInColor, size_t InTexSize, const STextureProps& InTexProps, UINT InNumMips)
	{
		BOOST_ASSERT(pTextures);
		STexInitializer texInitializer{ InTexProps };
		texInitializer.MipLevels = InNumMips;

		TextureSourceData sourceData{ InTexProps.Width, InTexProps.Height, InTexSize, InNumMips };
		sourceData.Fill(pInColor, InTexSize);
		return pTextures->AddTexture(texInitializer, sourceData.GetSubresourceData());
	}

	TextureHandle CreateStagingTexture2D_Uninitialized
	(
		ID3D11Device* pInDev,
		UINT InWidth, UINT InHeight,
		DXGI_FORMAT InFormat,
		UINT BindFlags, UINT InCpuAccessFlags,
		UINT InMipLevels = 1
	);
} // Test::IMPL