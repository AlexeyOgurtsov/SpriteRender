#include "../IFrameCheckContextUtils.h"
#include "../IFrameCheckContext.h"
#include "../Utils/TestUtils.h"
#include "../Utils/MathUtils.h"
#include "../IMPL/Utils/RenTextureUtils.h"

namespace Test
{
	bool RT_TexelMatches(IFrameCheckContext* pContext, UINT InRow, UINT InX, const void* pInRefTexel, DXGI_FORMAT InRefFormat)
	{		
		BOOST_ASSERT(pContext);
		return IMPL::TexelMatches(pInRefTexel, InRefFormat, pContext->GetDevCon(), pContext->GetRT(), InRow, InX);		
	}
} // Test