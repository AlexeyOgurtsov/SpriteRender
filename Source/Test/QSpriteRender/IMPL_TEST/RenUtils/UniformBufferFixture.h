#pragma once

#include "../ImplD3DFixture.h"
#include "QSpriteRender/IMPL/RenUtils/UniformBuffer.h"
#include "../ImplTestHelper.h"
#include <vector>
#include <algorithm> // std::fill

namespace Test
{
namespace ISpr
{
namespace QSpr
{
namespace IMPL
{
	using UB_DEFAULT_SLOT_TYPE = UINT;

	constexpr UINT UB_DEFAULT_CAPACITY = 100;
	constexpr size_t UB_DEFAULT_SLOT_SIZE = sizeof(UB_DEFAULT_SLOT_TYPE);

	namespace UniformBufferNS = Dv::Spr::QRen::IMPL::D3D;
	using UBAlloc = UniformBufferNS::BufferAlloc;
	using UB = UniformBufferNS::UniformBuffer;
	using UBHandle = std::unique_ptr<UB>;

	class UniformBufferFixture : public D3DFixture
	{
	public:
		UniformBufferFixture();
		~UniformBufferFixture();

		UB* GetBuffer() const { return Buffer.get(); }

		/**
		* To be called before SetupTest.
		*/
		void InitSlotSize(UINT InSlotSize);

		/**
		* To be called before SetupTest.
		*/
		void InitCapacityInSlots(UINT InNumSlots);

	protected:
		virtual void UserPostSetup() override;
		void ReCreateBuffer();

	private:
		UBHandle Buffer;
		UniformBufferNS::SUniformBufferInitializer Initializer;
	};

	template<class SlotTypeArg>
	class TUniformBufferFixture : public UniformBufferFixture
	{
	public:
		using SlotType = SlotTypeArg;
		using SlotDynamicBuffer = std::vector<SlotType>;

		SlotDynamicBuffer GetFilledBuffer(UINT InSizeInSlots, SlotType InValue) const
		{
			BOOST_ASSERT(InSizeInSlots > 0);
			SlotDynamicBuffer buf;
			buf.resize(InSizeInSlots);
			std::fill(buf.begin(), buf.end(), InValue);
			return buf;
		}
		UBAlloc AllocBuffer(const SlotDynamicBuffer& InBuf)
		{
			return GetBuffer()->Alloc(InBuf.data(), InBuf.size() * sizeof(SlotType));
		}
		void OverwriteBufferAlloc(const UBAlloc& InAlloc, const SlotDynamicBuffer& InBuf)
		{
			return GetBuffer()->OverwriteAlloc(InAlloc, InBuf.data(), InBuf.size() * sizeof(SlotType));
		}
		bool BufferEquals(const SlotDynamicBuffer& InBuf, const void* pInReferenceData) const
		{
			BOOST_ASSERT(pInReferenceData);
			size_t const SizeInBytes = InBuf.size() * sizeof(SlotType);
			auto const pBufferData = static_cast<const uint8_t*>(static_cast<const void*>(InBuf.data()));
			return (0 == std::memcmp(pBufferData, pInReferenceData, SizeInBytes));
		}
		bool BufferEqualsPart(const SlotDynamicBuffer& InBuf, const void* pInReferenceData, UINT InSlotIndex)
		{
			const void* pRefDataPart = static_cast<const uint8_t*>(pInReferenceData) + InSlotIndex * sizeof(SlotType);
			return BufferEquals(InBuf, pRefDataPart);
		}
		bool BufferEqualsPart(const SlotDynamicBuffer& InBuf, const void* pInReferenceData, const UBAlloc& InAlloc)
		{
			BOOST_ASSERT(InAlloc.IsValid());
			BOOST_ASSERT(InBuf.size() == InAlloc.NumSlots);
			return BufferEqualsPart(InBuf, pInReferenceData, InAlloc.OffsetInSlots);
		}

		/**
		* Returns true if the given alloc's content is equal to the contents of the buffer and stored in RAM.
		*/
		bool BufferAlloc_InRAM(const SlotDynamicBuffer& InBuf, const UBAlloc& InAlloc)
		{
			return BufferEqualsPart(InBuf, GetBuffer()->GetData(), InAlloc);
		}

		/**
		* Returns true if the given alloc's content is equal to the contents of the buffer and stored in D3D buffer itself.
		*/
		bool BufferAlloc_InD3DBuffer(const SlotDynamicBuffer& InBuf, const UBAlloc& InAlloc)
		{
			BOOST_ASSERT_MSG(D3D11_USAGE_STAGING == GetBuffer()->GetUsage(), "BufferAlloc_InD3DBuffer: this check is implemented only for buffers with D3D11_USAGE_STAGING");
			BOOST_ASSERT_MSG(D3D11_CPU_ACCESS_READ & GetBuffer()->GetCpuAccessFlags(), "BufferAlloc_InD3DBuffer: this check is implemented only for buffers with D3D11_CPU_ACCESS_READ flag set");
			
			ID3D11Buffer* const pBuffer = GetBuffer()->GetBuffer();
			D3D11_MAPPED_SUBRESOURCE Map;
			HRESULT const hr = GetDevCon()->Map(pBuffer, 0, D3D11_MAP_READ, 0, &Map);
			BOOST_ASSERT_MSG(SUCCEEDED(hr), "BufferAlloc_InD3DBuffer: Map must succeeed!");
			bool const bEquals = BufferEqualsPart(InBuf, Map.pData, InAlloc);
			GetDevCon()->Unmap(pBuffer, 0);
			return bEquals;
		}

	protected:
		virtual void UserPostSetup() override
		{
			UniformBufferFixture::UserPostSetup();
			InitSlotSize(sizeof(SlotType));
			UniformBufferFixture::ReCreateBuffer();
		}

	private:
	};
} // Test::ISpr::QSpr::IMPL
} // Test::ISpr::QSpr
} // Test::ISpr
} // Test