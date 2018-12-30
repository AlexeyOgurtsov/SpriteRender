#pragma once

#include "CFG.h"
#include "ValueType_Types.h"

namespace Dv::ConfigManager
{
	// Maps name to value
	using PropertySetter = std::pair<std::string,Value>;
	using PropertySetterMap = std::unordered_map<std::string,Value>;

	// Never returns nullptr:
	Value* GetByKey_OrNull(const PropertySetterMap& InSetters, std::string_view InName);
	Value* GetByKey_OrDefault(const PropertySetterMap& InSetters, std::string_view InName, Value&& InValue);
	Value* GetByKey_OrDefault(const PropertySetterMap& InSetters, std::string_view InName, const Value& InValue);

	struct ValueTypeInfo
	{
		bool bCanBeAssigned                 = true;
		bool bCanBeDefaultConstructed       = true;
	};

	/*****************************************************************************
	* Manager for values of the given type
	*****************************************************************************/
	class IValueType
	{
	public:
		virtual ~IValueType() = default;

		// name
		virtual const std::string& GetName() const = 0;

		virtual const ValueTypeInfo& GetInfo() const = 0;

		virtual const ValueProperties& GetProperties() const = 0;

		// Conversion to string
		virtual std::string ToString(const Value* pValue) const = 0;

		// Create default value
		virtual Value CreateDefault() const = 0;

		// Crate value: warning the value may be moved!!!
		virtual ObjectActionResult CreateValue(Value* pOutValue, const Value* pInArguments, int InCount) const = 0;
		// Create value by sequence: warning the value may be moved!!!
		virtual ObjectActionResult CreateValueByPropSetters(Value* pOutValue, PropertySetterMap&& InSetters) const = 0;

		virtual void MoveValue(Value* pOutValue, Value&& InSource) const = 0;
		virtual void CopyValue(Value* pOutValue, const Value& InSource) const = 0;
	};

	using ValueTypeHandle           = std::unique_ptr<IValueType>;
	using ValueTypeLibrary          = std::unordered_map<std::string, ValueTypeHandle>;

	template<class T>
	struct ValueTypeTraits;
} // Dv::ConfigManager