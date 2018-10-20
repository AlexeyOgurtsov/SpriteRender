#include "UniformBufferFixture.h"

namespace Test
{
namespace ISpr
{
namespace QSpr
{
namespace IMPL
{
	UniformBufferFixture::UniformBufferFixture()
	{
		T_LOG("UniformBufferFixture: ctor...");
		Initializer.bDebug = true;
		Initializer.Name = "TestUniformBuffer";
		Initializer.pLog = &GetLog();
		Initializer.pDev = GetDev();
		Initializer.pDevCon = GetDevCon();
		Initializer.Usage = D3D11_USAGE_STAGING;
		Initializer.BindFlags = 0; // For staging resource BindFlags must be always zero!
		Initializer.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		Initializer.CapacityInSlots = UB_DEFAULT_CAPACITY;
		Initializer.SlotSize = UB_DEFAULT_SLOT_SIZE;
		Initializer.bAutoResizeable = false;
		UniformBufferNS::CheckUniformBufferInitializer(Initializer);
		T_LOG("UniformBufferFixture: ctor DONE");
	}

	void UniformBufferFixture::InitSlotSize(UINT InSlotSize)
	{
		Initializer.SlotSize = InSlotSize;
		UniformBufferNS::CheckUniformBufferInitializer(Initializer);
	}

	void UniformBufferFixture::InitCapacityInSlots(UINT InNumSlots)
	{
		Initializer.CapacityInSlots = InNumSlots;
		UniformBufferNS::CheckUniformBufferInitializer(Initializer);
	}

	UniformBufferFixture::~UniformBufferFixture()
	{
		T_LOG("UniformBufferFixture: ~DTOR...");
		T_LOG("UniformBufferFixture: ~DTOR DONE");
	}

	void UniformBufferFixture::ReCreateBuffer()
	{
		Buffer.reset(new UB{ Initializer });
	}

	void UniformBufferFixture::UserPostSetup()
	{
		ReCreateBuffer();
	}
} // Test::ISpr::QSpr::IMPL
} // Test::ISpr::QSpr
} // Test::ISpr
} // Test