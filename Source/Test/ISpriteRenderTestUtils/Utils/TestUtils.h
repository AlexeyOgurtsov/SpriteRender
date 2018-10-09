#pragma once

#include <fstream>
#include <iomanip>
#include <boost/assert.hpp>

/**
* Utils available anywhere (both in the IMPL and to the client).
*/

/**
* T_LOG: Outputs message into the output stream, but ONLY if output stream is opened.
*
* Usage: T_LOG(GetMainLog(), "Hello, " << Name);
*/
#ifndef T_LOG_TO
#define T_LOG_TO(InLogStrm, Msg) { if((InLogStrm).is_open()) { (InLogStrm) << Msg << std::endl; } }
#endif // T_LOG_TO

#ifndef T_LOG
#define T_LOG(Msg) { T_LOG_TO(GetLog(), Msg); }
#endif // T_LOG

namespace Test
{
} // Test