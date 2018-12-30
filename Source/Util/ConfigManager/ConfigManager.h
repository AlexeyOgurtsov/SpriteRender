#pragma once

#include "CFG/IValueType.h"
#include <fstream>
#include "Util/CORE/APP/APP_INFO_EXTENDED.h"

namespace Dv::ConfigManager
{
	class CfgManager
	{
	public:
		CfgManager(std::string_view InName);

		void Initialize(std::ofstream* pInLog);

		const Core::AppInfoExtended&   GetAppInfo          () const { return _appInfo; }

		const std::string&             GetName             () const { return _name; }

		IValueType*                    GetType             (std::string_view InName) const;
		IValueType*                    GetType_OrNull      (std::string_view InName) const;

		const ValueTypeLibrary*        GetTypeLib          () const         { return &_typeLib; }
		ValueTypeLibrary*              GetTypeLib          ()               { return &_typeLib; }

		const ConstantTable*           GetConstantTable    () const         { return &_constTable; }
		ConstantTable*                 GetConstantTable    ()               { return &_constTable; }

		const ConfigTable*             GetConfigTable      () const         { return &_configTable; }
		ConfigTable*                   GetConfigTable      ()               { return &_configTable; }

		std::ofstream&                 GetLog              () const         { return *_pLog; }
		void                           BindLog             (std::ofstream* pInLog);

	private:
		Core::AppInfoExtended                      _appInfo;
		std::string                                _name;
		ValueTypeLibrary                           _typeLib;
		ConstantTable                              _constTable;
		ConfigTable                                _configTable;
		std::ofstream*                             _pLog = nullptr;
	};

	void DumpConfigManager(const CfgManager* pCfgManager);
	void DumpConstants(const CfgManager* pCfgManager);
	void DumpConfigVariables(const CfgManager* pCfgManager);
	void DumpValueTypes(const CfgManager* pCfgManager);

	enum class EGetSetTypedEntityCode
	{
		Success = 0,
		DoesNotExist,
		AlreadyExists,
		TypeMismatch,
		TypeIsNotAssignable // We are to be using an assignment (not move) for the given object type, but it's not defined for the given type
	};

	void                        SetConstant                         (std::string_view InName, const Value& InValue, CfgManager* pCfgManager);
	void                        SetBoolConstant                     (std::string_view InName, bool bInValue, CfgManager* pCfgManager);
	void                        SetFloatConstant                    (std::string_view InName, float InValue, CfgManager* pCfgManager);
	void                        SetIntConstant                      (std::string_view InName, int InValue, CfgManager* pCfgManager);
	void                        SetStringConstant                   (std::string_view InName, std::string_view InValue, CfgManager* pCfgManager);
	void                        SetAnyConstant                      (std::string_view InName, std::any&& InValue, IValueType* pInType, CfgManager* pCfgManager);

	template<class T>
	void                        SetObjectConstant                   (std::string_view InName, T&& InValue, CfgManager* pConfigManager);

	EGetSetTypedEntityCode      TrySetConstant                      (std::string_view InName, const Value& InValue, CfgManager* pCfgManager);
	EGetSetTypedEntityCode      TrySetBoolConstant                  (std::string_view InName, bool bInValue, CfgManager* pCfgManager);
	EGetSetTypedEntityCode      TrySetIntConstant                   (std::string_view InName, int InValue, CfgManager* pCfgManager);
	EGetSetTypedEntityCode      TrySetFloatConstant                 (std::string_view InName, float InValue, CfgManager* pCfgManager);
	EGetSetTypedEntityCode      TrySetStringConstant                (std::string_view InName, std::string_view InValue, CfgManager* pCfgManager);
	EGetSetTypedEntityCode      TrySetAnyConstant                   (std::string_view InName, std::any&& InValue, IValueType* pInType, CfgManager* pCfgManager);

	template<class T>
	EGetSetTypedEntityCode      TrySetObjectConstant                (std::string_view InName, T&& InValue, CfgManager* pConfigManager);

	const ConstDesc*            GetConstantDesc                     (std::string_view InName, const CfgManager* pCfgManager);
	const ConstDesc*            TryGetConstantDesc                  (std::string_view InName, const CfgManager* pCfgManager);
	EGetSetTypedEntityCode      TryGetTypedConstantDesc             (const ConstDesc** ppOutConst, const ValueTypeDecl& InTypeDecl, std::string_view InName, const CfgManager* pCfgManager);
	EGetSetTypedEntityCode      TryGetTypedObjectConstantDesc       (const ConstDesc** ppOutConst, IValueType* pInValueType, std::string_view InName, const CfgManager* pCfgManager);

	bool                        TryGetBoolConstant        (bool* pOutValue, std::string_view InName, const CfgManager* pCfgManager);
	bool                        TryGetIntConstant         (int* pOutValue, std::string_view InName, const CfgManager* pCfgManager);
	bool                        TryGetFloatConstant       (float* pOutValue, std::string_view InName, const CfgManager* pCfgManager);
	const std::string*          TryGetStringConstant      (std::string_view InName, const CfgManager* pCfgManager);
	const std::any*             TryGetAnyConstant         (std::string_view InName, IValueType* pInValueType, const CfgManager* pCfgManager);

	bool                        GetBoolConstant          (std::string_view InName, CfgManager* pCfgManager);
	float                       GetFloatConstant         (std::string_view InName, CfgManager* pCfgManager);
	int                         GetIntConstant           (std::string_view InName, CfgManager* pCfgManager);
	const std::string&          GetStringConstant        (std::string_view InName, CfgManager* pCfgManager);
	const std::any*             GetAnyConstant           (std::string_view InName, IValueType* pInType, CfgManager* pCfgManager);

	template<class T>
	const T*                    GetObjectConstant         (std::string_view InName, const CfgManager* pCfgManager);

	template<class T>
	const T*                    TryGetObjectConstant      (std::string_view InName, const CfgManager* pCfgManager);


	// Sets the given variable to the given value
	void            SetVar                              (std::string_view InName, Value&& InValue, CfgManager* pConfigManager);
	// Sets the given variable to the given value
	// (The copy of the variable is used)
	void            SetVar                              (std::string_view InName, const Value& InValue, CfgManager* pConfigManager);

	// Checks the type, return false if the type does not match
	bool            TrySetVar                           (std::string_view InName, const Value& InValue, CfgManager* pConfigManager);
	bool            TrySetVar                           (std::string_view InName, Value&& InValue, CfgManager* pConfigManager);

	bool            TrySetBoolVar                       (std::string_view InName, bool bInValue, CfgManager* pConfigManager);
	bool            TrySetIntVar                        (std::string_view InName, int InValue, CfgManager* pConfigManager);
	bool            TrySetFloatVar                      (std::string_view InName, float InValue, CfgManager* pConfigManager);
	bool            TrySetStringVar                     (std::string_view InName, std::string_view InValue, CfgManager* pConfigManager);
	bool            TrySetAnyVar                        (std::string_view InName, std::any&& InValue, IValueType* pInType, CfgManager* pConfigManager);

	template<class T>
	bool            TrySetObjectVar                     (std::string_view InName, T&& InValue, CfgManager* pConfigManager);

	// GetVar:
	// Returns variable with the given name
	// (checks with assert, that the variable with the given name is registered)
	const Value*    GetVar                              (std::string_view InName, const CfgManager* pConfigManager);
	Value*          GetVarPtr                           (std::string_view InName, CfgManager* pConfigManager);
	
	bool                        GetBoolVar                          (std::string_view InName, const CfgManager* pConfigManager);
	int                         GetIntVar                           (std::string_view InName, const CfgManager* pConfigManager);
	float                       GetFloatVar                         (std::string_view InName, const CfgManager* pConfigManager);
	const std::string&          GetStringVar                        (std::string_view InName, const CfgManager* pConfigManager);

	bool                        TryGetBoolVar(bool* pOutValue, std::string_view InName, const CfgManager* pConfigManager);
	bool                        TryGetIntVar(int* pOutValue, std::string_view InName, const CfgManager* pConfigManager);
	bool                        TryGetFloatVar(float* pOutValue, std::string_view InName, const CfgManager* pConfigManager);
	const std::string*          TryGetStringVar(std::string_view InName, const CfgManager* pConfigManager);

	template<class T>
	T*                          TryGetObjectVar(std::string_view InName, CfgManager* pConfigManager);

	template<class T>
	T*                          GetObjectVar(std::string_view InName, CfgManager* pConfigManager);

	std::any*                   TryGetAnyVar(std::string_view InName, CfgManager* pConfigManager);


	// GetVar_OrNull: returns nullptr, if not found
	const Value*    GetVar_OrNull                       (std::string_view InName, const CfgManager* pConfigManager);
	Value*          GetVarPtr_OrNull                    (std::string_view InName, CfgManager* pConfigManager);

	// GetVar_OrDefault: always returns value
	// (registers a new var with the given name and value, if NOT found)
	// Variable is copied.
	Value           GetVar_OrDefault                    (std::string_view InName, Value InValue, const CfgManager* pConfigManager);
	Value           GetVar_OrDefault                    (std::string_view InName, Value&& InValue, const CfgManager* pConfigManager);

	template<class T>
	T*  GetObjectVar(std::string_view InName, CfgManager* pConfigManager)
	{
		Value* pVar = GetVarPtr(InName, pConfigManager);
		std::any& valAny = std::get<std::any>(pVar->Val);
		return std::any_cast<T>(&valAny);
	}

	inline void CfgManager::Initialize(std::ofstream* pInLog)
	{
		_pLog = pInLog;
		GetLog() << "CfgManager: Initialized" << std::endl;
	}

	inline IValueType* CfgManager::GetType(std::string_view InName) const
	{
		auto pType = GetType_OrNull(InName);
		BOOST_ASSERT_MSG(pType, "CfgManager::GetType: Type is not registered");
		return pType;
	}

	inline IValueType* CfgManager::GetType_OrNull(std::string_view InName) const
	{
		auto it_type = _typeLib.find(std::string(InName));
		if (it_type == _typeLib.cend())
		{
			return nullptr;
		}
		else
		{
			return it_type->second.get();
		}
	}


	template<class T>
	const T* GetObjectConstant(std::string_view InName, const CfgManager* pCfgManager)
	{
		const T* pObj = TryGetObjectConstant<T>(InName, pCfgManager);
		BOOST_ASSERT_MSG(pObj, "GetObjectConstant: the get operation must succeed");
		return pObj;
	}

	template<class T>
	const T* TryGetObjectConstant(std::string_view InName, const CfgManager* pCfgManager)
	{
		auto pValueType = ValueTypeTraits<T>::GetValueType(pConfigManager);
		auto pObj = TryGetAnyConstant(InName, pValueType, pCfgManager);
		return std::any_cast<const T*>(pObj);
	}

	template<class T>
	T* TryGetObjectVar(std::string_view InName, CfgManager* pConfigManager)
	{
		std::any* pAny = TryGetAnyVar(InName, pConfigManager);
		if (pAny == nullptr)
		{
			return nullptr;
		}
		return &std::any_cast<T&>(*pAny);
	}

	template<class T>
	bool TrySetObjectVar(std::string_view InName, T&& InValue, CfgManager* pConfigManager)
	{
		auto pValueType = ValueTypeTraits<T>::GetValueType(pConfigManager);
		return TrySetAnyVar(InName, std::make_any<T>(std::forward<T>(InValue)), pValueType, pConfigManager);
	}

	template<class T>
	void SetObjectConstant(std::string_view InName, T&& InValue, CfgManager* pConfigManager)
	{
		EGetSetTypedEntityCode res = TrySetObjectConstant(InName, std::forward<T>(InValue), pConfigManager);
		BOOST_ASSERT_MSG(res != EGetSetTypedEntityCode::Success, "ConfigManager: SetObjectConstant: setting constant must succeed");
	}

	template<class T>
	EGetSetTypedEntityCode TrySetObjectConstant(std::string_view InName, T&& InValue, CfgManager* pConfigManager)
	{
		auto pValueType = ValueTypeTraits<T>::GetValueType(pConfigManager);
		return TrySetAnyConstant(InName, std::make_any<T>(std::forward<T>(InValue)), pValueType, pConfigManager);
	}
} // Dv::ConfigManager