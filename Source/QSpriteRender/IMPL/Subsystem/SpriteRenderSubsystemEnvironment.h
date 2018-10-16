#pragma once

#include "QSpriteRender/IMPL/Environment/AmbientContext.h"
//#include "QSpriteRender/IMPL/Geometry/SpriteGeometryBuffer.h"
#include "QSpriteRender/IMPL/RenUtils/RenResources.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{

struct SConfig;
struct SSpriteRenderSubsystemInitializer;

namespace IMPL
{

/**
* Stores the environment and performs its initialization.
*/
class Environment
{
public:
	Environment(const SSpriteRenderSubsystemInitializer& InInitializer);

	/**
	* Get the config
	*/
	const SConfig& GetConfig() const { return _renResources.GetConfig(); }

	/**
	* Returns ambient context for the main thread.
	*/
	const AmbientContext* GetAmbientContext() const { return &_ambientContext; }

	/**
	* Returns ambient context for the main thread.
	*/
	AmbientContext* GetAmbientContext() { return &_ambientContext; }

	/**
	* Vertex buffer that contains all sprites.
	*/
	//const SpriteGeometryBuffer* GetSpriteGeometryBuffer() const { return &_spriteGeometryBuffer; }

	/**
	* Vertex buffer that contains all sprites.
	*/
	//SpriteGeometryBuffer* GetSpriteGeometryBuffer() { return &_spriteGeometryBuffer; }

	/**
	* Render-related resources.
	*/
	const D3D::RenResources* GetRenResources() const { return &_renResources; }

	/**
	* Render-related resources.
	*/
	D3D::RenResources* GetRenResources() { return &_renResources; }

private:
	AmbientContext _ambientContext;
	//SpriteGeometryBuffer _spriteGeometryBuffer;
	D3D::RenResources _renResources;
};

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // DV