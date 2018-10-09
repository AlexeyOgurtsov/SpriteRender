#pragma once

#include <d3d11.h>
#include "ISprite/SpriteMaterialTypedefs.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
	/**
	* Base class for all render states.
	*/
	class SpriteMaterialInstanceRenderState
	{
	public:
		SpriteMaterialInstanceRenderState(SpriteMaterialId InMaterialId) :
			_materialId(InMaterialId) {}

		/**
		* Material that this render state belongs to.
		*/
		SpriteMaterialId GetMaterialId() const { return _materialId; }

		/**
		* The main function that sets the render state.
		*/
		virtual void SetRenderState(ID3D11DeviceContext* pDevCon) const = 0;

	private:
		SpriteMaterialId _materialId;
	};
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv