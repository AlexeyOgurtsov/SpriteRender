#include "SpriteRender/IMPL/Utils/String/Text.h"
#include <iomanip>

namespace Dv
{
namespace Spr
{ 
namespace Utils
{
	template<class StrmType>
	int WriteTextTempl(StrmType& oStrm, std::string_view text, bool bNumberLines)
	{
		int i = 0; // index of source char
		int line_index = 1;
		while(true)
		{
			if (i >= static_cast<int>(text.size()) )
			{
				break;
			}
			auto new_i = text.find('\n', i);
			if(new_i == std::string_view::npos)
			{
				new_i = text.size();
			}
			const auto str_length = new_i - i;
			oStrm << std::left << std::setw(7) << line_index << ":" << text.substr(i, str_length) << std::endl;
			if (new_i == text.size())
			{
				break;
			}
			i = new_i + 1;
			line_index++;
		}
		return line_index;
	}

	int WriteText(std::ofstream& oStrm, std::string_view text, bool bNumberLines)
	{
		return WriteTextTempl(oStrm, text, bNumberLines);
	}
} // Dv::Spr::Utils
} // Dv::Spr
} // Dv