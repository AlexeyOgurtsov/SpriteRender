#include "ShaderText_Operations.h"
#include <boost/assert.hpp>

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

std::string GetShaderOp_Vec3_to_Vec4(const std::string& InVec3Expr)
{
	return std::string("float4(") + InVec3Expr + std::string(",1.0F)");
}

void AppendStmt_Assign_MulVec3ByMatrix44
(
	std::vector<std::string>* pOutText, 
	const std::string& InVarName,
	const std::string& InVecExpr,
	const std::string& InMatrixExpr
)
{
	BOOST_ASSERT(pOutText);
	std::string OpStr;

	OpStr.append("\t");
	OpStr.append(InVarName);
	OpStr.append("=");
	OpStr.append("mul");
	OpStr.append("(");
	OpStr.append(GetShaderOp_Vec3_to_Vec4(InVecExpr));
	OpStr.append(",");
	OpStr.append(InMatrixExpr);
	OpStr.append(")");
	OpStr.append(";");

	pOutText->push_back(OpStr);
}

} // D3D
	
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv