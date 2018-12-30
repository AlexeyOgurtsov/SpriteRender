#pragma once

/*************************************************************************************
* Contains only types, describing config itself.
**************************************************************************************/

#include "../CORE/ConfigManagerSystem.h"

namespace Dv::ConfigManager
{
	class IValueType;

	// Value of the config property value.
	// Copyable;
	// std::any is pointer to some copyable value, typically POD (for example, vectors)
	// void * is pointer to some object
	using ValueVariant = std::variant<bool, int, float, std::string, std::any>;

	enum class EValueCategory
	{
		Bool,
		Int,
		Float,
		String,
		Object
	};

	template<class T> EValueCategory GetValueCategory();
	template<> EValueCategory GetValueCategory<bool>();
	template<> EValueCategory GetValueCategory<int>();
	template<> EValueCategory GetValueCategory<float>();
	template<> EValueCategory GetValueCategory<std::string>();
	template<> EValueCategory GetValueCategory<std::string_view>();
	template<> EValueCategory GetValueCategory<const std::string*>();

	template<class T> EValueCategory GetValueCategory()
	{
		static_assert(false, "This version of the GetValueCategory() Should NOT be called - explicit specialization function must always be called instead");
	}

	inline std::string GetValueCategoryString(EValueCategory InCode)
	{
		switch (InCode)
		{
		case EValueCategory::Bool:
			return "Bool";

		case EValueCategory::Int:
			return "Int";

		case EValueCategory::Float:
			return "Float";

		case EValueCategory::String:
			return "String";

		case EValueCategory::Object:
			return "Object";

		default:
			break;
		}
		return "<<Unknown value category>>";
	}
	template<class Strm> 
	Strm& operator<<(Strm& strm, EValueCategory InCategory)
	{
		strm << GetValueCategoryString(InCategory);
		return strm;
	}

	struct ValueTypeDecl
	{
		const IValueType*          pType = nullptr;
		EValueCategory             Category;

		bool IsBool() const { return CanConvertTo(EValueCategory::Bool); }
		bool IsInt() const { return CanConvertTo(EValueCategory::Int); }
		bool IsFloat() const { return CanConvertTo(EValueCategory::Float); }
		bool IsString() const { return CanConvertTo(EValueCategory::String); }
		bool Is(const IValueType* pInType) const { return CanConvertTo(pInType); }

		bool CanConvertTo(const IValueType* pInValueType) const
		{
			return pType == pInValueType;
		}

		bool CanConvertTo(const ValueTypeDecl& InTo) const
		{
			if (Category == InTo.Category)
			{
				if (Category == EValueCategory::Object)
				{
					return pType == InTo.pType;
				}
				return true;
			}

			return CanConvertTo(InTo.Category);
		}

		bool CanConvertTo(EValueCategory RefCategory) const
		{
			if (pType != nullptr) { return false; }
			switch (RefCategory)
			{
			case EValueCategory::Float:
				return CanConvertToFloat();

			case EValueCategory::Int:
				return CanConvertToInt();

			case EValueCategory::Bool:
				return CanConvertToBool();

			default:
				return Category == RefCategory;
			}
		}
		bool CanConvertToFloat() const 
		{
			return (Category == EValueCategory::Float) || (Category == EValueCategory::Int);
		}

		bool CanConvertToInt() const
		{
			return (Category == EValueCategory::Int) || (Category == EValueCategory::Bool);
		}

		bool CanConvertToBool() const
		{
			return (Category == EValueCategory::Bool) || (Category == EValueCategory::Int);
		}

		ValueTypeDecl() = default;
		ValueTypeDecl(const IValueType* pInType) : pType(pInType), Category(EValueCategory::Object) {}
		ValueTypeDecl(EValueCategory InCategory) : Category(InCategory) {}
	};

	template<class T>
	ValueVariant ConvertValTo(T&& InValue, const ValueTypeDecl& InDestTypeDecl);

	template<class T>
	ValueVariant ConvertValTo(T&& InValue, const ValueTypeDecl& InDestTypeDecl)
	{
		switch (InDestTypeDecl.Category)
		{
		case EValueCategory::Bool:
			return InValue.GetBool();

		case EValueCategory::Int:
			return InValue.GetInt();

		case EValueCategory::Float:
			return InValue.GetFloat();

		case EValueCategory::String:
			return InValue.GetString();

		default:
			break;
		}
		return std::move(InValue);
	}

	template<class Strm>
	Strm& operator<<(Strm& strm, const ValueTypeDecl& InValue)
	{
		strm << "Type category: " << InValue.Category << std::endl;
		if (InValue.Category == EValueCategory::Object)
		{
			strm << "Object type: " << (InValue.pType ? InValue.pType->GetName() : "{nullptr}") << std::endl;
		}
		return strm;
	}

	struct Value
	{
		ValueTypeDecl            TypeDecl;
		ValueVariant             Val;
		
		EValueCategory           GetValueCategory       () const;


		// ConverTo:
		// T is return type, that is to be used for the given type (for example "const std::string*" for strings)
		template<class T>
		T                        ConvertTo_ValueOrPtr    () const;

		bool                     CanConvertTo            (const ValueTypeDecl& InTo) const;

		bool                     CanConvertTo            (EValueCategory InCategory) const;
		bool                     CanConvertToFloat       () const;

		template<class T>
		const T* GetObj() const
		{
			const auto& anyVal = std::get<std::any>(Val);
			return &std::any_cast<const T&>(anyVal);
		}

		template<class T>
		T* GetObj()
		{
			auto& anyVal = std::get<std::any>(Val);
			return &std::any_cast<T&>(anyVal);
		}

		bool                            GetBool             () const;
		int                             GetInt              () const;
		float                           GetFloat            () const;
		const std::string&              GetString           () const;

		Value() = default;
		Value(const IValueType* pInType, std::any&& pInObject);
		Value(const IValueType* pInType, const std::any& pInCopyableObject);
		explicit Value(bool bInValue);
		explicit Value(int InValue);
		explicit Value(float InValue);
		explicit Value(const std::string& InValue);
		explicit Value(std::string&& InValue);
		explicit Value(std::string_view InValue);
	};

	bool TypeIs(const Value* pInValue, EValueCategory InCategory);
	bool TypeIs(const Value* pInValue, IValueType* pInValueType);
	bool TypeIs(const Value* pInArguments, int InArgIndex, EValueCategory InCategory);
	bool TypeIs(const Value* pInArguments, int InArgIndex, IValueType* pInValueType);
	bool ForAll_TypeIs(int* pOutFailedArgIndex, const Value* pInArguments, int InCount, EValueCategory InCategory);
	bool ForAll_TypeIs(int* pOutFailedArgIndex, const Value* pInArguments, int InCount, IValueType* pInValueType);

	template<class Strm>
	Strm& operator<<(Strm& strm, const Value& InValue)
	{	
		strm << InValue.TypeDecl << std::endl;
		strm << "Value: "; // WARNING!!! Not std::endl should be printed here - it's handled conditionally
		if (InValue.GetValueCategory() == EValueCategory::Object)
		{
			strm << std::endl;
			strm << InValue.TypeDecl.pType->ToString(&InValue) << std::endl;
		}
		else
		{
			switch (InValue.GetValueCategory())
			{
			case EValueCategory::Bool:
				strm << std::get<bool>(InValue.Val) << std::endl;
				break;

			case EValueCategory::Float:
				strm << std::get<float>(InValue.Val) << std::endl;
				break;

			case EValueCategory::Int:
				strm << std::get<int>(InValue.Val) << std::endl;
				break;

			case EValueCategory::String:
				strm << std::get<std::string>(InValue.Val) << std::endl;
				break;

			default:
				BOOST_ASSERT_MSG(false, "Unknown value category");
				break;
			}
		}
		return strm;
	}

	struct ConstDesc
	{
		Value          Val;

		ConstDesc(const Value& InValue) :
			Val(InValue) {}
		ConstDesc(Value&& InValue) :
			Val(std::move(InValue)) {}
	};
	template<class Strm>
	Strm& operator<<(Strm& strm, const ConstDesc& InValue)
	{
		return strm << InValue.Val;
	}

	using ConstantTable = std::unordered_map<std::string, ConstDesc>;
	using ConfigTable = std::unordered_map<std::string, Value>;
} // Dv::ConfigManager