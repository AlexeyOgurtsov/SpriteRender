#include "Com.h"
#include <d3d11.h>

namespace Test::IMPL
{
	void ComReleaser::operator() (IUnknown* pNullOrInterface)
	{
		if(pNullOrInterface)
		{
			pNullOrInterface->Release();
		}
	}
} // Test::IMPL