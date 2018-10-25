#pragma once

#include <boost/test/included/unit_test.hpp>
#include "UniformBufferFixture.h"

namespace Test
{
namespace ISpr
{
namespace QSpr
{
namespace IMPL
{
	using UniformBufferFixtureInt = TUniformBufferFixture<int>;

	BOOST_AUTO_TEST_SUITE(TestSuite_QRen)
	BOOST_AUTO_TEST_SUITE(TestSuite_Impl)
	BOOST_AUTO_TEST_SUITE(TestSuite_RenUtils)
	BOOST_AUTO_TEST_SUITE(TestSuite_UniformBuffer)
	BOOST_FIXTURE_TEST_CASE(Test_Smoke, UniformBufferFixtureInt)
	{
		SetupTest("Smoke");

		BOOST_TEST_CHECKPOINT("Init checks");
		{
			BOOST_REQUIRE(false == GetBuffer()->IsStoring());
			BOOST_REQUIRE_EQUAL(0, GetBuffer()->GetNumAllocs());
			BOOST_REQUIRE_EQUAL(0, GetBuffer()->GetNumOccupiedSlots());
			BOOST_REQUIRE_EQUAL(GetBuffer()->GetNumSlots(), GetBuffer()->GetNumFreeSlots());
		}

		BOOST_TEST_CHECKPOINT("Getting buffer must always return valid buffer");
		{
			BOOST_REQUIRE(GetBuffer()->GetBuffer()); 
		}

		BOOST_TEST_CHECKPOINT("Storing test");
		{
			BOOST_REQUIRE_NO_THROW(GetBuffer()->StartStore());
			BOOST_REQUIRE(GetBuffer()->IsStoring());
			BOOST_REQUIRE_NO_THROW(GetBuffer()->EndStore());
		}

		BOOST_TEST_CHECKPOINT("First");
		{
			constexpr UINT AllocOne_SlotCount = 5;
			constexpr UINT AllocOne_Val = 1;
			SlotDynamicBuffer const Buf_One = GetFilledBuffer(AllocOne_SlotCount, AllocOne_Val);
			UBAlloc Alloc_One;

			BOOST_TEST_CHECKPOINT("FirstAlloc");
			{
				BOOST_REQUIRE_NO_THROW(GetBuffer()->StartStore());

				BOOST_REQUIRE_NO_THROW(Alloc_One = AllocBuffer(Buf_One));
				BOOST_REQUIRE(Alloc_One.IsValid());
				BOOST_REQUIRE_EQUAL(Alloc_One.NumSlots, Buf_One.size());

				BOOST_REQUIRE(BufferAlloc_InRAM(Buf_One, Alloc_One));

				BOOST_REQUIRE_NO_THROW(GetBuffer()->EndStore());
			}

			BOOST_TEST_CHECKPOINT("FlushFirstAlloc");
			{
				BOOST_REQUIRE_NO_THROW(GetBuffer()->Flush());

				BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_One, Alloc_One));
			}

			BOOST_TEST_CHECKPOINT("FlushFirstAgain");
			{
				BOOST_REQUIRE_NO_THROW(GetBuffer()->Flush());

				BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_One, Alloc_One));
			}

			BOOST_TEST_CHECKPOINT("FirstAndSecond");
			{
				constexpr UINT AllocSecond_SlotCount = 3;
				constexpr UINT AllocSecond_Val = 2;
				SlotDynamicBuffer const Buf_Second = GetFilledBuffer(AllocSecond_SlotCount, AllocSecond_Val);
				UBAlloc Alloc_Second;

				BOOST_TEST_CHECKPOINT("SecondAlloc");
				{
					BOOST_REQUIRE_NO_THROW(GetBuffer()->StartStore());

					BOOST_REQUIRE_NO_THROW(Alloc_Second = AllocBuffer(Buf_Second));
					BOOST_REQUIRE(Alloc_Second.IsValid());
					BOOST_REQUIRE_EQUAL(Alloc_Second.NumSlots, Buf_Second.size());

					BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Second, Alloc_Second));
					BOOST_REQUIRE(BufferAlloc_InRAM(Buf_One, Alloc_One));

					BOOST_REQUIRE_NO_THROW(GetBuffer()->EndStore());
				}

				BOOST_TEST_CHECKPOINT("FlushSecondAlloc");
				{
					BOOST_REQUIRE_NO_THROW(GetBuffer()->Flush());

					BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Second, Alloc_Second));
				}

				BOOST_TEST_CHECKPOINT("FirstSecondThird");
				{
					constexpr UINT AllocThird_SlotCount = 10;
					constexpr UINT AllocThird_Val = 3;
					SlotDynamicBuffer const Buf_Third = GetFilledBuffer(AllocThird_SlotCount, AllocThird_Val);
					UBAlloc Alloc_Third;

					BOOST_TEST_CHECKPOINT("ThirdAlloc");
					{
						BOOST_REQUIRE_NO_THROW(GetBuffer()->StartStore());

						BOOST_REQUIRE_NO_THROW(Alloc_Third = AllocBuffer(Buf_Third));
						BOOST_REQUIRE(Alloc_Third.IsValid());
						BOOST_REQUIRE_EQUAL(Alloc_Third.NumSlots, Buf_Third.size());

						BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Third, Alloc_Third));
						BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Second, Alloc_Second));
						BOOST_REQUIRE(BufferAlloc_InRAM(Buf_One, Alloc_One));

						BOOST_REQUIRE_NO_THROW(GetBuffer()->EndStore());
					}

					BOOST_TEST_CHECKPOINT("FlushThirdAlloc");
					{
						BOOST_REQUIRE_NO_THROW(GetBuffer()->Flush());

						BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Third, Alloc_Third));
					}

					BOOST_TEST_CHECKPOINT("FreeSecondAlloc");
					{
						BOOST_REQUIRE_NO_THROW(GetBuffer()->FreeAlloc(Alloc_Second));

						BOOST_REQUIRE(false == Alloc_Second.IsValid());

						BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Third, Alloc_Third));
						BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Third, Alloc_Third));

						BOOST_REQUIRE(BufferAlloc_InRAM(Buf_One, Alloc_One));
						BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_One, Alloc_One));
					};

					BOOST_TEST_CHECKPOINT("FirstThirdFourthAlloc");
					{
						constexpr UINT AllocFourth_SlotCount = 1;
						constexpr UINT AllocFourth_Val = 4;
						SlotDynamicBuffer const Buf_Fourth = GetFilledBuffer(AllocFourth_SlotCount, AllocFourth_Val);
						UBAlloc Alloc_Fourth;

						BOOST_TEST_CHECKPOINT("FourthAlloc");
						{
							BOOST_REQUIRE_NO_THROW(GetBuffer()->StartStore());

							BOOST_REQUIRE_NO_THROW(Alloc_Fourth = AllocBuffer(Buf_Fourth));
							BOOST_REQUIRE(Alloc_Fourth.IsValid());
							BOOST_REQUIRE_EQUAL(Alloc_Fourth.NumSlots, Buf_Fourth.size());

							BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Fourth, Alloc_Fourth));
							BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Third, Alloc_Third));
							BOOST_REQUIRE(BufferAlloc_InRAM(Buf_One, Alloc_One));

							BOOST_REQUIRE_NO_THROW(GetBuffer()->EndStore());
						}

						BOOST_TEST_CHECKPOINT("FlushFourthAlloc");
						{
							BOOST_REQUIRE_NO_THROW(GetBuffer()->Flush());

							BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Fourth, Alloc_Fourth));
						}

						BOOST_TEST_CHECKPOINT("FirstThirdFourthFifthAllocs");
						{
							constexpr UINT AllocFifth_SlotCount = 20;
							constexpr UINT AllocFifth_Val = 5;
							SlotDynamicBuffer const Buf_Fifth = GetFilledBuffer(AllocFifth_SlotCount, AllocFifth_Val);
							UBAlloc Alloc_Fifth;


							BOOST_TEST_CHECKPOINT("FifthAlloc");
							{
								BOOST_REQUIRE_NO_THROW(GetBuffer()->StartStore());

								BOOST_REQUIRE_NO_THROW(Alloc_Fifth = AllocBuffer(Buf_Fifth));
								BOOST_REQUIRE(Alloc_Fifth.IsValid());
								BOOST_REQUIRE_EQUAL(Alloc_Fifth.NumSlots, Buf_Fifth.size());

								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Fifth, Alloc_Fifth));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Fourth, Alloc_Fourth));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Third, Alloc_Third));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_One, Alloc_One));

								BOOST_REQUIRE_NO_THROW(GetBuffer()->EndStore());
							}


							BOOST_TEST_CHECKPOINT("FlushFifthAlloc");
							{
								BOOST_REQUIRE_NO_THROW(GetBuffer()->Flush());

								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Fifth, Alloc_Fifth));
								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Fourth, Alloc_Fourth));
								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Third, Alloc_Third));
								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_One, Alloc_One));
							}

							SlotDynamicBuffer const Buf_Fifth_Altered = GetFilledBuffer(AllocFifth_SlotCount, 555);
							BOOST_TEST_CHECKPOINT("OverwriteFifthAlloc");
							{
								BOOST_REQUIRE_NO_THROW(GetBuffer()->StartStore());

								BOOST_REQUIRE_NO_THROW(OverwriteBufferAlloc(Alloc_Fifth, Buf_Fifth_Altered));
								BOOST_REQUIRE(Alloc_Fifth.IsValid());
								BOOST_REQUIRE_EQUAL(Alloc_Fifth.NumSlots, Buf_Fifth.size());

								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Fifth_Altered, Alloc_Fifth));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Fourth, Alloc_Fourth));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Third, Alloc_Third));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_One, Alloc_One));

								BOOST_REQUIRE_NO_THROW(GetBuffer()->EndStore());
							}

							BOOST_TEST_CHECKPOINT("FlushFifthAllocOverwrite");
							{
								BOOST_REQUIRE_NO_THROW(GetBuffer()->Flush());

								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Fifth_Altered, Alloc_Fifth));
								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Fourth, Alloc_Fourth));
								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Third, Alloc_Third));
								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_One, Alloc_One));
							}

							BOOST_TEST_CHECKPOINT("TryResetCapacityToOne");
							{
								BOOST_REQUIRE_NO_THROW(GetBuffer()->ResetCapacity(1));

								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Fifth_Altered, Alloc_Fifth));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Fourth, Alloc_Fourth));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Third, Alloc_Third));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_One, Alloc_One));
							}

							BOOST_TEST_CHECKPOINT("TryResetCapacityToLesser");
							{
								UINT const NewCapacity = GetBuffer()->GetNumSlots() - 10;
								UINT const InitiallyAllocatedSlotCount = Alloc_One.NumSlots + Alloc_Second.NumSlots + Alloc_Third.NumSlots + Alloc_Fourth.NumSlots + Alloc_Fifth.NumSlots;
								BOOST_ASSERT_MSG(NewCapacity >= InitiallyAllocatedSlotCount, "For this test NewCapacity must be greater or equal the initially allocated slot count to minimize chance to cut the allocated data by resize");
								BOOST_REQUIRE_NO_THROW(GetBuffer()->ResetCapacity(NewCapacity));

								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Fifth_Altered, Alloc_Fifth));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Fourth, Alloc_Fourth));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Third, Alloc_Third));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_One, Alloc_One));
							}

							BOOST_TEST_CHECKPOINT("TryResetCapacityToGreater");
							{
								BOOST_REQUIRE_NO_THROW(GetBuffer()->ResetCapacity(GetBuffer()->GetNumSlots() * 2));

								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Fifth_Altered, Alloc_Fifth));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Fourth, Alloc_Fourth));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_Third, Alloc_Third));
								BOOST_REQUIRE(BufferAlloc_InRAM(Buf_One, Alloc_One));
							}

							BOOST_TEST_CHECKPOINT("FlushBufferAfterReset");
							{
								BOOST_REQUIRE_NO_THROW(GetBuffer()->Flush());

								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Fifth_Altered, Alloc_Fifth));
								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Fourth, Alloc_Fourth));
								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_Third, Alloc_Third));
								BOOST_REQUIRE(BufferAlloc_InD3DBuffer(Buf_One, Alloc_One));
							}
						}
					}
				}
			}
		}

		BOOST_TEST_CHECKPOINT("Clear");
		{
			BOOST_REQUIRE_NO_THROW(GetBuffer()->Clear());
			BOOST_REQUIRE_EQUAL(0, GetBuffer()->GetNumOccupiedSlots());
			BOOST_REQUIRE_EQUAL(GetBuffer()->GetNumSlots(), GetBuffer()->GetNumFreeSlots());
			BOOST_REQUIRE_EQUAL(0, GetBuffer()->GetNumAllocs());
		}
	}
	BOOST_FIXTURE_TEST_CASE(Test_Second, UniformBufferFixture)
	{
		SetupTest("Second");

		// @TODO
	}
	BOOST_AUTO_TEST_SUITE_END() // TestSuite_UniformBuffer
	BOOST_AUTO_TEST_SUITE_END() // TestSuite_RenUtils
	BOOST_AUTO_TEST_SUITE_END() // TestSuite_Impl
	BOOST_AUTO_TEST_SUITE_END() // TestSuite_QRen
} // Test::ISpr::QSpr::IMPL
} // Test::ISpr::QSpr
} // Test::ISpr
} // Test