#pragma once

#include <cstdint>

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

constexpr size_t CB_UINT16_SIZE = sizeof(uint16_t);
constexpr const char* CB_UINT16_HLSL_TYPE = "short";

constexpr size_t CB_UINT32_SIZE = sizeof(uint32_t);
constexpr const char* CB_UINT32_HLSL_TYPE = "uint";

constexpr size_t CB_FLOAT_SIZE = sizeof(float);
constexpr const char* CB_FLOAT_HLSL_TYPE = "float";

constexpr size_t CB_VEC_FOUR_SIZE = sizeof(float) * 4;
constexpr const char* CB_VEC_FOUR_HLSL_TYPE = "float4";

constexpr size_t CB_VEC_THREE_SIZE = sizeof(float) * 3;
constexpr const char* CB_VEC_THREE_HLSL_TYPE = "float3";

constexpr size_t CB_VEC_TWO_SIZE = sizeof(float) * 2;
constexpr const char* CB_VEC_TWO_HLSL_TYPE = "float2";


constexpr size_t CB_MATRIX_FOUR_FOUR_COLUMN_SIZE = CB_VEC_FOUR_SIZE;

// WARNING!!! matrix 3x3 is always stored as a set of 3 four-component vectors!
constexpr size_t CB_MATRIX_THREE_THREE_COLUMN_SIZE = CB_VEC_FOUR_SIZE;

constexpr size_t CB_MATRIX_FOUR_FOUR_SIZE = 4 * CB_MATRIX_FOUR_FOUR_COLUMN_SIZE;
constexpr const char* CB_MATRIX_FOUR_FOUR_HLSL_TYPE = "float4x4";

constexpr size_t CB_MATRIX_THREE_THREE_SIZE = 3 * CB_MATRIX_THREE_THREE_COLUMN_SIZE;
constexpr const char* CB_MATRIX_THREE_THREE_HLSL_TYPE = "float3x3";

} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv