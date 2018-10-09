#pragma once

#include <fstream>
#include <string>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{
 
struct SAmbientContextInitializer
{
	std::ofstream* pLog;

	SAmbientContextInitializer(std::ofstream* pInLog) :
		pLog(pInLog) {}
};

class AmbientContext
{
public:
	AmbientContext(const SAmbientContextInitializer& InInitializer);

	std::ofstream& GetLog() const { return *_pLog; }

private:
	std::ofstream* _pLog;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // DV::Spr
} // Dv