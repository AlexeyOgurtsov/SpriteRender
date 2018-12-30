#include "CFG.h"
#include "IValueType.h"

namespace Dv::ConfigManager
{
	template<> EValueCategory GetValueCategory<bool>()
	{
		return EValueCategory::Bool;
	}

	template<> EValueCategory GetValueCategory<int>()
	{
		return EValueCategory::Int;
	}

	template<> EValueCategory GetValueCategory<float>()
	{
		return EValueCategory::Float;
	}

	template<> EValueCategory GetValueCategory<std::string_view>()
	{
		return EValueCategory::String;
	}

	template<> EValueCategory GetValueCategory<std::string>()
	{
		return EValueCategory::String;
	}

	template<> EValueCategory GetValueCategory<const std::string*>()
	{
		return EValueCategory::String;
	}

	Value::Value(const IValueType* pInType, std::any&& pInObject) : TypeDecl(pInType), Val(std::move(pInObject))
	{
	}

	Value::Value(const IValueType* pInType, const std::any& pInCopyableObject) : TypeDecl(pInType), Val(pInCopyableObject)
	{
		BOOST_ASSERT_MSG(pInType->GetInfo().bCanBeAssigned, "ConfigManager: Value constructor: object must be copyable");
	}

	Value::Value(bool bInValue) : Val(bInValue), TypeDecl{ EValueCategory::Bool } 
	{
	}

	Value::Value(int InValue) : Val(InValue), TypeDecl{ EValueCategory::Int } 
	{
	}

	Value::Value(float InValue) : Val(InValue), TypeDecl{ EValueCategory::Float }
	{
	}

	Value::Value(const std::string& InValue) : Val(InValue), TypeDecl{ EValueCategory::String } 
	{
	}

	Value::Value(std::string&& InValue) : Val(std::move(InValue)), TypeDecl{ EValueCategory::String } 
	{
	}

	Value::Value(std::string_view InValue) : Val(std::string(InValue)), TypeDecl{ EValueCategory::String } 
	{
	}

	EValueCategory Value::GetValueCategory() const
	{ 
		return static_cast<EValueCategory>(Val.index()); 
	}

	template<>
	bool Value::ConvertTo_ValueOrPtr<bool>() const
	{
		BOOST_ASSERT(GetValueCategory() == EValueCategory::Bool);
		return GetFloat();
	}


	template<>
	float Value::ConvertTo_ValueOrPtr<float>() const
	{
		BOOST_ASSERT(GetValueCategory() == EValueCategory::Float);
		return GetFloat();
	}

	template<>
	int Value::ConvertTo_ValueOrPtr<int>() const
	{
		BOOST_ASSERT(GetValueCategory() == EValueCategory::Int);
		return GetInt();
	}

	template<>
	const std::any* Value::ConvertTo_ValueOrPtr<const std::any*>() const
	{
		BOOST_ASSERT(GetValueCategory() == EValueCategory::Object);
		return std::any_cast<const std::any*>(Val);
	}

	template<>
	const std::string* Value::ConvertTo_ValueOrPtr<const std::string*>() const
	{
		BOOST_ASSERT(GetValueCategory() == EValueCategory::String);
		return &GetString();
	}


	bool Value::CanConvertTo(const ValueTypeDecl& InTo) const
	{
		return TypeDecl.CanConvertTo(InTo);
	}

	bool Value::CanConvertTo(EValueCategory InCategory) const
	{
		return TypeDecl.CanConvertTo(InCategory); 
	}

	bool Value::CanConvertToFloat() const
	{ 
		return TypeDecl.CanConvertToFloat();
	}

	bool Value::GetBool() const
	{
		if (GetValueCategory() == EValueCategory::Int)
		{
			return GetInt() != 0;
		}
		return std::get<bool>(Val);
	}

	int Value::GetInt() const
	{
		if (GetValueCategory() == EValueCategory::Bool)
		{
			return GetBool() ? 1 : 0;
		}
		return std::get<int>(Val);
	}

	float Value::GetFloat() const
	{
		if (GetValueCategory() == EValueCategory::Int) { return static_cast<float>(GetInt()); }
		return std::get<float>(Val);
	}

	const std::string& Value::GetString() const
	{
		return std::get<std::string>(Val);
	}

	bool TypeIs(const Value* pInValue, IValueType* pInValueType)
	{
		return pInValue->TypeDecl.CanConvertTo(pInValueType);
	}

	bool TypeIs(const Value* pInArguments, int InArgIndex, IValueType* pInValueType)
	{
		return TypeIs(pInArguments + InArgIndex, pInValueType);
	}

	bool TypeIs(const Value* pInValue, EValueCategory InCategory)
	{
		return pInValue->TypeDecl.CanConvertTo(InCategory);
	}

	bool TypeIs(const Value* pInArguments, int InArgIndex, EValueCategory InCategory)
	{
		return TypeIs(pInArguments + InArgIndex, InCategory);
	}

	bool ForAll_TypeIs(int* pOutFailedArgIndex, const Value* pInArguments, int InCount, IValueType* pInValueType)
	{
		for (int i = 0; i < InCount; i++)
		{
			if (false == TypeIs(pInArguments, i, pInValueType))
			{
				*pOutFailedArgIndex = i;
				return false;
			}
		}
		return true;
	}

	bool ForAll_TypeIs(int* pOutFailedArgIndex, const Value* pInArguments, int InCount, EValueCategory InCategory)
	{
		for (int i = 0; i < InCount; i++)
		{
			if (false == TypeIs(pInArguments, i, InCategory))
			{
				*pOutFailedArgIndex = i;
				return false;
			}
		}
		return true;
	}
} // Dv::ConfigManager