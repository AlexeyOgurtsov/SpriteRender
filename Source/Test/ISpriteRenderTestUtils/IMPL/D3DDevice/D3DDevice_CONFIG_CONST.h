#pragma once

#include <d3d11.h>

namespace Test::IMPL
{
/****************************************************************************
* Device
****************************************************************************/
constexpr D3D_FEATURE_LEVEL                DEFAULT_FEATURE_LEVEL                      = D3D_FEATURE_LEVEL_11_0;
constexpr unsigned int                     DEFAULT_DEVICE_FLAGS                       = D3D11_CREATE_DEVICE_DEBUG;

/****************************************************************************
* Device mode
****************************************************************************/
constexpr unsigned int                     DEFAULT_REFRESH_RATE                        = 60;
constexpr unsigned int                     DEFAULT_SWAP_CHAIN_WIDTH                    = 1024;
constexpr unsigned int                     DEFAULT_SWAP_CHAIN_HEIGHT                   = 768;
constexpr DXGI_MODE_SCANLINE_ORDER         DEFAULT_SCANLINE_ORDER                      = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
constexpr BOOL                             WINDOWED                                    = TRUE;

/****************************************************************************
* Swap chain
****************************************************************************/
constexpr DXGI_SWAP_EFFECT                 DEFAULT_SWAP_EFFECT                         = DXGI_SWAP_EFFECT_DISCARD;
} // Test::IMPL