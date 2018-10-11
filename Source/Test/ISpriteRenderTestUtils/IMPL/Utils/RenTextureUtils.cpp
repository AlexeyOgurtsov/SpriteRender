#include "RenTextureUtils.h"
#include "../../Utils/TestUtils.h"

namespace Test::IMPL
{
	void ZeroMainMip_ByMap(ID3D11DeviceContext* pDevCon, ID3D11Texture2D* pInTexture, D3D11_MAP InMap)
	{
		D3D11_TEXTURE2D_DESC Desc;
		pInTexture->GetDesc(&Desc);
		
		D3D11_MAPPED_SUBRESOURCE MapSub;
		HRESULT hr = pDevCon->Map(pInTexture, 0, InMap, 0, &MapSub);
		BOOST_ASSERT_MSG(SUCCEEDED(hr), "ZeroMainMip_ByMap: Map must succeed");
		ZeroMemory(MapSub.pData, MapSub.RowPitch * Desc.Height);

		pDevCon->Unmap(pInTexture, 0);
	}
} // Test::IMPL