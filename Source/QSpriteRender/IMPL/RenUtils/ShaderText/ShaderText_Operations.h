#pragma once

#include <vector>
#include <string>

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

std::string GetShaderOp_Vec3_to_Vec4(const std::string& InVec3Expr);

void AppendStmt_Assign_MulVec3ByMatrix44
(
	std::vector<std::string>* pOutText, 
	const std::string& InVarName,
	const std::string& InVecExpr,
	const std::string& InMatrixExpr
);

} // D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv