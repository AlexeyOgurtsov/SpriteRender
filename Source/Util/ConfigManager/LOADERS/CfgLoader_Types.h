#pragma once

#include <string>
#include <iomanip>
#include "../CFG/ValueType_Types.h"
#include "../CORE/SourceLocation.h"
#include "../CORE/InputStreamLexTypes.h"

namespace Dv::ConfigManager
{
	struct CfgLoadParams
	{
	};

	enum class ECfgLoadResultCode
	{
		Success = 0,
		LexemError, // Error occured while parsing a lexem
		IllegalLexemInExpression, // Illegal lexem was encountered while parsing expression
		MaxFunctionArgumentsExceeded,
		MaxPropertySettersExceeded,
		ObjectInstantiationArgsNotProvided,
		WrongObjectInstantiationSyntax,
		ObjectInstantiateError, // Error occured while trying to instantiate an object 
		PropertySetterWrongPropertyType, // Type mismatch for the property setter
		PropertySetterInexistentProperty, // Inexisting property name is provided
		PropertySetterStartsWrong, // Error occured while trying to parse the next property getter
		PropertySetterValueNotProvided, // Only key is provided, but NOT value
		PropertySetterAmbiguity, // Property setter for property with the same name provided more than once
		ValueToAssignNotProvided, // Value to assign to the given variable is NOT provided
		TypeMismatch, // Expression evaluates to a type, that does not match the expected type
		UnknownTypeName, // Type with the given name is not registered in the type library
		UnknownCommandName,
		UnknownConstantName, 
		UnknownVariableName,
		ConstructorBracketNotClosed, // Constructor ('(') bracket not closed
		ArrayBracketNotClosed, //  Array ('[') bracket not closed
		ClassBracketNotClosed, // Class ('{') bracket not closed 
		CommentNotClosed, // Comment was NOT closed properly
		ConstantNameExpected, // After the # sign the constant name is expected
		SemicolonNotFound,
		WrongInstruction,
		WrongFilename // We was unable to include or run a script file, because the file does NOT exist
	};
	inline std::string GetLoadResultCodeString(ECfgLoadResultCode InCode)
	{
		switch (InCode)
		{
		case ECfgLoadResultCode::Success:
			return "<<Success>>";

		case ECfgLoadResultCode::LexemError:
			return "<<Error occured while parsing a lexem>>";

		case ECfgLoadResultCode::MaxFunctionArgumentsExceeded:
			return "<<Maximal number of function arguments exceeded>>";

		case ECfgLoadResultCode::MaxPropertySettersExceeded:
			return "<<Maximal number of property settersexceeded>>";

		case ECfgLoadResultCode::IllegalLexemInExpression:
			return "<<Illegal lexem was encountered while parsing expression>>";

		case ECfgLoadResultCode::ObjectInstantiationArgsNotProvided:
			return "<<Object instantiation arguments not provided>>";

		case ECfgLoadResultCode::WrongObjectInstantiationSyntax:
			return "<<Wrong object instantiation syntax>>";

		case ECfgLoadResultCode::ObjectInstantiateError:
			return "<<Error occured while trying to instantiate an object>>";

		case ECfgLoadResultCode::PropertySetterWrongPropertyType:
			return "<<Type mismatch for the property setter>>";

		case ECfgLoadResultCode::PropertySetterInexistentProperty:
			return "<<Inexisting property name is provided>>";

		case ECfgLoadResultCode::PropertySetterStartsWrong:
			return "<<Error occured while trying to parse the next property getter>>";

		case ECfgLoadResultCode::PropertySetterValueNotProvided:
			return "<<Only key is provided for property setter, but NOT value>>";

		case ECfgLoadResultCode::PropertySetterAmbiguity:
			return "<<Property setter for property with the same name provided more than once>>>";

		case ECfgLoadResultCode::ValueToAssignNotProvided:
			return "<<Value to assign to the given variable is NOT provided>>";

		case ECfgLoadResultCode::TypeMismatch:
			return "Expression evaluates to a type, that does not match the expected type";

		case ECfgLoadResultCode::UnknownTypeName:
			return "Type with the given name is not registered in the type library";

		case ECfgLoadResultCode::UnknownCommandName:
			return "<<Command with the given name is not registered>>";

		case ECfgLoadResultCode::UnknownConstantName:
			return "<<Constant with the given name is not registered>>";

		case ECfgLoadResultCode::UnknownVariableName:
			return "<<Variable with the given name is not registered>>";

		case ECfgLoadResultCode::ConstructorBracketNotClosed:
			return "<<Constructor ('(') bracket not closed>>";

		case ECfgLoadResultCode::ArrayBracketNotClosed:
			return "<<Array bracket is not closed>>";

		case ECfgLoadResultCode::ClassBracketNotClosed:
			return "<<Class ('{') bracket not closed>>";

		case ECfgLoadResultCode::CommentNotClosed:
			return "<<Comment was NOT closed properly>>";

		case ECfgLoadResultCode::SemicolonNotFound:
			return "<<Semicolon not found>>";

		case ECfgLoadResultCode::ConstantNameExpected:
			return "<<After the # sign the constant name is expected>>";

		case ECfgLoadResultCode::WrongInstruction:
			return "<<WrongInstruction>>";

		case ECfgLoadResultCode::WrongFilename:
			return "<<We was unable to include or run a script file, because the file does NOT exist>>";

		default:
			break;
		}
		return "UnknownErrorCode";
	}

	template<class Strm>
	Strm& operator<<(Strm& strm, ECfgLoadResultCode InCode)
	{
		strm << GetLoadResultCodeString(InCode);
		return strm;
	}

	struct CfgLoadResult
	{
		ECfgLoadResultCode             Code;
		SourceLocation                 SourceLoc;
		ELexemParseCode                LexCode;
		ObjectActionResult             ObjAction;
		std::string                    Identifier;

		bool                           IsSuccess() const { return Code == ECfgLoadResultCode::Success; }
		bool                           IsFailed() const { return false == IsSuccess(); }

		// Default cotr: Create a successfull result
		CfgLoadResult() : Code(ECfgLoadResultCode::Success) {}
		explicit CfgLoadResult(ECfgLoadResultCode InResultCode) : Code(InResultCode) {}
	};
	template<class Strm>
	Strm& operator<<(Strm& strm, const CfgLoadResult& InVal)
	{
		strm << "Code: " << InVal.Code << std::endl;
		strm << "Source location: " << std::endl;
		if (false == InVal.Identifier.empty())
		{
			strm << "Identifier: " << InVal.Identifier << std::endl;
		}
		strm << InVal.SourceLoc << std::endl;
		strm << "LexCode: " << InVal.LexCode << std::endl;
		strm << "ObjAction: " << InVal.ObjAction << std::endl;
		return strm;
	}
} // Dv::ConfigManager