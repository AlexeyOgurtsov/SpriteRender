#pragma once

#include "CFG.h"

namespace Dv::ConfigManager
{
	class IValueType;

	enum class EObjectInstantiateResultCode
	{
		Success,
		WrongArgumentCount,
		TypeMismatch, // Type of property setter or argument does NOT match the expected type
		WrongValue,
		ConstructionSyntaxNotSupported,
		PropSettersSyntaxNotSupported
	};

	inline std::string GetObjectInstantiateResultCodeString(EObjectInstantiateResultCode InCode)
	{
		switch (InCode)
		{
		case EObjectInstantiateResultCode::Success:
			return "<<Success>>";

		case EObjectInstantiateResultCode::WrongArgumentCount:
			return "<<Wrong argument count>>";

		case EObjectInstantiateResultCode::TypeMismatch:
			return "<<Type of property setter or argument does NOT match the expected type>>";

		case EObjectInstantiateResultCode::WrongValue:
			return "<<Value for the given property or function argument is incorrect>>";

		case EObjectInstantiateResultCode::ConstructionSyntaxNotSupported:
			return "<<Construction-style instantiation syntax is not supported by the given object>>";
		
		case EObjectInstantiateResultCode::PropSettersSyntaxNotSupported:
			return "<<Property setter style instantion syntax is not supported by the given object>>";

		default:
			break;
		}
		return "<<Unknown code>>";
	}

	template<class Strm>
	Strm& operator<<(Strm& strm, EObjectInstantiateResultCode InCode)
	{
		strm << GetObjectInstantiateResultCodeString(InCode);
		return strm;
	}

	struct ObjectActionResult
	{
		EObjectInstantiateResultCode              Code;
		std::string                               FirstFailedPropertyName;
		int                                       FirstFailedArgIndex;

		// Default ctor: creates success result
		ObjectActionResult() : Code(EObjectInstantiateResultCode::Success), FirstFailedPropertyName(""), FirstFailedArgIndex(0) {}
		explicit ObjectActionResult(EObjectInstantiateResultCode InCode) : Code(InCode), FirstFailedPropertyName(""), FirstFailedArgIndex(0) {}

		bool          IsSuccess          () const         { return Code == EObjectInstantiateResultCode::Success; }
		bool          IsFailed           () const         { return false == IsSuccess(); }
	};

	template<class Strm>
	Strm& operator<<(Strm& strm, const ObjectActionResult& InResult)
	{
		strm << "Code: " << InResult.Code << std::endl;
		strm << "First failed property:" << InResult.FirstFailedPropertyName << std::endl;
		strm << "First failed argument index: " << InResult.FirstFailedArgIndex << std::endl;
		return strm;
	}


	struct ValuePropertyDesc
	{
		std::string                   Name;
		ValueTypeDecl                 TypeDecl;

		ValuePropertyDesc(std::string_view InName, const ValueTypeDecl& InTypeDecl) : Name(InName), TypeDecl(InTypeDecl) {}
	};
	using ValueProperties = std::unordered_map<std::string, ValuePropertyDesc>;
} // Dv::ConfigManager