#include "ConfigManager.h"
#include "ConfigManager_AppInfo.h"
#include <iomanip>

namespace Dv::ConfigManager
{
	CfgManager::CfgManager(std::string_view InName) :
		_name(InName),
		_appInfo(CreateAppInfo())
	{
	}

	void CfgManager::BindLog(std::ofstream* pInLog)
	{
		GetLog() << "CfgManager: new log is to be binded" << std::endl;
		_pLog = pInLog;
		GetLog() << "CfgManager: this log for binded, previous log existed" << std::endl;
	}

	template<class T>
	void SetVarImpl(std::string_view InName, T&& InValue, CfgManager* pConfigManager)
	{
		std::string nameCopy{ InName };
		auto pConfigTable = pConfigManager->GetConfigTable();
		auto it_existing = pConfigTable->find(nameCopy);
		if (it_existing == pConfigTable->cend())
		{
			auto[it_inserted, bRegistered] = pConfigTable->emplace(nameCopy, std::move(InValue));
			it_existing = it_inserted;
		}
		else
		{
			it_existing->second = std::forward<T>(InValue);
		}
	}
	template<class T>
	bool TrySetVarImpl(std::string_view InName, T&& InValue, CfgManager* pConfigManager)
	{
		std::string nameCopy{ InName };
		auto pConfigTable = pConfigManager->GetConfigTable();
		auto it_existing = pConfigTable->find(nameCopy);
		if (it_existing == pConfigTable->cend())
		{
			auto[it_inserted, bRegistered] = pConfigTable->emplace(nameCopy, std::move(InValue));
			it_existing = it_inserted;
			return true;
		}
		else
		{
			const auto& DestType = it_existing->second.TypeDecl;
			// We must check that type can be assigned to
			if (false == InValue.CanConvertTo(it_existing->second.TypeDecl))
			{
				return false;
			}
			it_existing->second.Val = ConvertValTo(std::forward<T>(InValue), DestType);
			return true;
		}
	}

	void SetVar(std::string_view InName, Value&& InValue, CfgManager* pConfigManager)
	{
		return SetVarImpl(InName, std::move(InValue), pConfigManager);
	}

	void SetVar(std::string_view InName, const Value& InValue, CfgManager* pConfigManager)
	{
		return SetVarImpl(InName, InValue, pConfigManager);
	}

	bool TrySetVar(std::string_view InName, const Value& InValue, CfgManager* pConfigManager)
	{
		return TrySetVarImpl(InName, InValue, pConfigManager);
	}

	bool TrySetVar(std::string_view InName, Value&& InValue, CfgManager* pConfigManager)
	{
		return TrySetVarImpl(InName, std::move(InValue), pConfigManager);
	}


	bool TrySetBoolVar(std::string_view InName, bool bInValue, CfgManager* pConfigManager)
	{
		return TrySetVar(InName, Value(bInValue), pConfigManager);
	}

	bool TrySetIntVar(std::string_view InName, int InValue, CfgManager* pConfigManager)
	{
		return TrySetVar(InName, Value(InValue), pConfigManager);
	}

	bool TrySetFloatVar(std::string_view InName, float InValue, CfgManager* pConfigManager)
	{
		return TrySetVar(InName, Value(InValue), pConfigManager);
	}

	bool TrySetStringVar(std::string_view InName, std::string_view InValue, CfgManager* pConfigManager)
	{
		return TrySetVar(InName, Value(InValue), pConfigManager);
	}

	bool TrySetAnyVar(std::string_view InName, std::any&& InValue, IValueType* pInType, CfgManager* pConfigManager)
	{
		return TrySetVar(InName, Value(pInType, std::move(InValue)), pConfigManager);
	}

	Value* GetVarPtr_OrNull(std::string_view InName, CfgManager* pConfigManager)
	{
		auto pConfigTable = pConfigManager->GetConfigTable();
		auto it = pConfigTable->find(std::string(InName));
		if (it == pConfigTable->end())
		{
			return nullptr;
		}
		return &it->second;
	}

	const Value* GetVar_OrNull(std::string_view InName, const CfgManager* pConfigManager)
	{
		auto pConfigTable = pConfigManager->GetConfigTable();
		auto it = pConfigTable->find(std::string(InName));
		if (it == pConfigTable->end())
		{
			return nullptr;
		}
		return &it->second;
	}

	Value* GetVarPtr(std::string_view InName, CfgManager* pConfigManager)
	{
		Value* pValue = GetVarPtr_OrNull(InName, pConfigManager);
		BOOST_ASSERT_MSG(pValue, "ConfigManager: GetVarPtr: variable to get does NOT exist");
		return pValue;
	}

	const Value* GetVar(std::string_view InName, const CfgManager* pConfigManager)
	{
		const Value* pValue = GetVar_OrNull(InName, pConfigManager);
		BOOST_ASSERT_MSG(pValue, "ConfigManager: GetVar: variable to get does NOT exist");
		return pValue;
	}

	bool TryGetBoolVar(bool* pOutValue, std::string_view InName, const CfgManager* pConfigManager)
	{
		auto pVar = GetVar_OrNull(InName, pConfigManager);
		if (pVar == nullptr)
		{
			return false;
		}
		if (false == pVar->CanConvertTo(EValueCategory::Bool))
		{
			return false;
		}
		*pOutValue = pVar->GetBool();
		return true;
	}

	bool TryGetIntVar(int* pOutValue, std::string_view InName, const CfgManager* pConfigManager)
	{
		auto pVar = GetVar_OrNull(InName, pConfigManager);
		if (pVar == nullptr)
		{
			return false;
		}
		if (false == pVar->CanConvertTo(EValueCategory::Int))
		{
			return false;
		}
		*pOutValue = pVar->GetInt();
		return true;
	}

	bool TryGetFloatVar(float* pOutValue, std::string_view InName, const CfgManager* pConfigManager)
	{
		auto pVar = GetVar_OrNull(InName, pConfigManager);
		if (pVar == nullptr)
		{
			return false;
		}
		if (false == pVar->CanConvertTo(EValueCategory::Float))
		{
			return false;
		}
		*pOutValue = pVar->GetFloat();
		return true;
	}

	const std::string* TryGetStringVar(std::string_view InName, const CfgManager* pConfigManager)
	{
		auto pVar = GetVar_OrNull(InName, pConfigManager);
		if (pVar == nullptr)
		{
			return false;
		}
		if (false == pVar->CanConvertTo(EValueCategory::String))
		{
			return false;
		}
		return &pVar->GetString();
	}

	std::any* TryGetAnyVar(std::string_view InName, CfgManager* pConfigManager)
	{
		auto pVar = GetVarPtr_OrNull(InName, pConfigManager);
		if (pVar == nullptr)
		{
			return false;
		}
		if (pVar->GetValueCategory() != EValueCategory::Object)
		{
			return false;
		}
		return &std::get<std::any>(pVar->Val);
	}

	bool GetBoolVar(std::string_view InName, const CfgManager* pConfigManager)
	{
		return GetVar(InName, pConfigManager)->GetBool();
	}

	int GetIntVar(std::string_view InName, const CfgManager* pConfigManager)
	{
		return GetVar(InName, pConfigManager)->GetInt();
	}

	float GetFloatVar(std::string_view InName, const CfgManager* pConfigManager)
	{
		return GetVar(InName, pConfigManager)->GetFloat();
	}

	const std::string& GetStringVar(std::string_view InName, const CfgManager* pConfigManager)
	{
		return GetVar(InName, pConfigManager)->GetString();
	}

	void SetConstant(std::string_view InName, const Value& InValue, CfgManager* pCfgManager)
	{
		EGetSetTypedEntityCode res = TrySetConstant(InName, std::move(InValue), pCfgManager);
		BOOST_ASSERT_MSG(res != EGetSetTypedEntityCode::Success, "ConfigManager: SetConstant: setting constant must succeed");
	}

	void SetBoolConstant(std::string_view InName, bool bInValue, CfgManager* pCfgManager)
	{
		EGetSetTypedEntityCode res = TrySetBoolConstant(InName, bInValue, pCfgManager);
		BOOST_ASSERT_MSG(res == EGetSetTypedEntityCode::Success, "ConfigManager: SetBoolConstant: setting constant must succeed");
	}

	void SetFloatConstant(std::string_view InName, float InValue, CfgManager* pCfgManager)
	{
		EGetSetTypedEntityCode res = TrySetFloatConstant(InName, InValue, pCfgManager);
		BOOST_ASSERT_MSG(res == EGetSetTypedEntityCode::Success, "ConfigManager: SetFloatConstant: setting constant must succeed");
	}

	void SetIntConstant(std::string_view InName, int InValue, CfgManager* pCfgManager)
	{
		EGetSetTypedEntityCode res = TrySetIntConstant(InName, InValue, pCfgManager);
		BOOST_ASSERT_MSG(res == EGetSetTypedEntityCode::Success, "ConfigManager: SetIntConstant: setting constant must succeed");
	}

	void SetStringConstant(std::string_view InName, std::string_view InValue, CfgManager* pCfgManager)
	{
		EGetSetTypedEntityCode res = TrySetStringConstant(InName, InValue, pCfgManager);
		BOOST_ASSERT_MSG(res == EGetSetTypedEntityCode::Success, "ConfigManager: SetStringConstant: setting constant must succeed");
	}

	void SetAnyConstant(std::string_view InName, std::any&& InValue, IValueType* pInType, CfgManager* pCfgManager)
	{
		EGetSetTypedEntityCode res = TrySetAnyConstant(InName, std::move(InValue), pInType, pCfgManager);
		BOOST_ASSERT_MSG(res == EGetSetTypedEntityCode::Success, "ConfigManager: SetAnyConstant: setting constant must succeed");
	}

	EGetSetTypedEntityCode TrySetConstant(std::string_view InName, const Value& InValue, CfgManager* pCfgManager)
	{
		auto pConstTable = pCfgManager->GetConstantTable();
		ConstantTable::iterator it = pConstTable->find(std::string(InName));
		if (it != pConstTable->cend())
		{
			return EGetSetTypedEntityCode::AlreadyExists;
		}
		ConstDesc constDesc { InValue };
		pConstTable->emplace(std::string(InName), constDesc);
		return EGetSetTypedEntityCode::Success;
	}


	EGetSetTypedEntityCode TrySetBoolConstant(std::string_view InName, bool bInValue, CfgManager* pCfgManager)
	{
		return TrySetConstant(InName, Value{bInValue}, pCfgManager);
	}

	EGetSetTypedEntityCode TrySetIntConstant(std::string_view InName, int InValue, CfgManager* pCfgManager)
	{
		return TrySetConstant(InName, Value{ InValue }, pCfgManager);
	}

	EGetSetTypedEntityCode TrySetFloatConstant(std::string_view InName, float InValue, CfgManager* pCfgManager)
	{
		return TrySetConstant(InName, Value{ InValue }, pCfgManager);
	}

	EGetSetTypedEntityCode TrySetStringConstant(std::string_view InName, std::string_view InValue, CfgManager* pCfgManager)
	{
		return TrySetConstant(InName, Value{ InValue }, pCfgManager);
	}

	template<class T>
	EGetSetTypedEntityCode TrySetAnyConstantImpl(std::string_view InName, T&& InValue, IValueType* pInType, CfgManager* pCfgManager)
	{
		if (false == pInType->GetInfo().bCanBeAssigned)
		{
			return EGetSetTypedEntityCode::TypeIsNotAssignable;
		}
		return TrySetConstant(InName, Value{ pInType, std::forward<T>(InValue) }, pCfgManager);
	}

	EGetSetTypedEntityCode TrySetAnyConstant(std::string_view InName, std::any&& InValue, IValueType* pInType, CfgManager* pCfgManager)
	{
		return TrySetAnyConstantImpl(InName, std::move(InValue), pInType, pCfgManager);
	}

	const ConstDesc* GetConstantDesc(std::string_view InName, const CfgManager* pCfgManager)
	{
		const ConstDesc* pDesc = TryGetConstantDesc(InName, pCfgManager);
		BOOST_ASSERT_MSG(pDesc, "ConfigManager: GetConstantDesc must succeed");
		return pDesc;
	}

	const ConstDesc* TryGetConstantDesc(std::string_view InName, const CfgManager* pCfgManager)
	{
		auto pConstants = pCfgManager->GetConstantTable();
		auto it = pConstants->find(std::string(InName));
		if (it == pConstants->cend()) { return nullptr; }
		return &it->second;
	}

	EGetSetTypedEntityCode TryGetTypedConstantDesc(const ConstDesc** ppOutConst, const ValueTypeDecl& InTypeDecl, std::string_view InName, const CfgManager* pCfgManager)
	{
		*ppOutConst = TryGetConstantDesc(InName, pCfgManager);
		if (false == *ppOutConst)
		{
			return EGetSetTypedEntityCode::DoesNotExist;
		}
		if (false == (*ppOutConst)->Val.CanConvertTo(InTypeDecl))
		{
			return EGetSetTypedEntityCode::TypeMismatch;
		}
		return EGetSetTypedEntityCode::Success;
	}

	EGetSetTypedEntityCode TryGetTypedObjectConstantDesc(const ConstDesc** ppOutConst, IValueType* pInValueType, std::string_view InName, const CfgManager* pCfgManager)
	{
		return TryGetTypedConstantDesc(ppOutConst, ValueTypeDecl{ pInValueType }, InName, pCfgManager);
	}

	template<class T>
	bool TryGetTypedConstantImpl(T* pOutValue, EValueCategory InValueCategory, std::string_view InName, const CfgManager* pCfgManager)
	{
		const ConstDesc* pConst = nullptr;
		EGetSetTypedEntityCode getTypeCode = TryGetTypedConstantDesc(&pConst, ValueTypeDecl{ InValueCategory }, InName, pCfgManager);
		if (getTypeCode != EGetSetTypedEntityCode::Success)
		{
			return false;
		}
		*pOutValue = pConst->Val.ConvertTo_ValueOrPtr<T>();
		return true;
	}

	bool GetBoolConstant(std::string_view InName, CfgManager* pCfgManager)
	{
		bool bVal;
		bool bGetted = TryGetBoolConstant(&bVal, InName, pCfgManager);
		BOOST_ASSERT_MSG(bGetted, "ConfigManager: GetBoolConstant: must succeed");
		return bVal;
	}

	float GetFloatConstant(std::string_view InName, CfgManager* pCfgManager)
	{
		float val;
		bool bGetted = TryGetFloatConstant(&val, InName, pCfgManager);
		BOOST_ASSERT_MSG(bGetted, "ConfigManager: GetFloatConstant: must succeed");
		return val;
	}

	int GetIntConstant(std::string_view InName, CfgManager* pCfgManager)
	{	
		int val;
		bool bGetted = TryGetIntConstant(&val, InName, pCfgManager);
		BOOST_ASSERT_MSG(bGetted, "ConfigManager: GetIntConstant: must succeed");
		return val;
	}

	const std::string& GetStringConstant(std::string_view InName, CfgManager* pCfgManager)
	{
		const std::string* pVal = TryGetStringConstant(InName, pCfgManager);
		BOOST_ASSERT_MSG(pVal, "ConfigManager: GetStringConstant: must succeed");
		return *pVal;
	}

	const std::any* GetAnyConstant(std::string_view InName, IValueType* pInType, CfgManager* pCfgManager)
	{
		const std::any* pVal = TryGetAnyConstant(InName, pInType, pCfgManager);
		BOOST_ASSERT_MSG(pVal, "ConfigManager: GetAnyConstant: must succeed");
		return pVal;
	}

	bool TryGetBoolConstant(bool* pOutValue, std::string_view InName, const CfgManager* pCfgManager)
	{
		return TryGetTypedConstantImpl<bool>(pOutValue, EValueCategory::Bool, InName, pCfgManager);
	}

	bool TryGetIntConstant(int* pOutValue, std::string_view InName, const CfgManager* pCfgManager)
	{
		return TryGetTypedConstantImpl<int>(pOutValue, EValueCategory::Int, InName, pCfgManager);
	}

	bool TryGetFloatConstant(float* pOutValue, std::string_view InName, const CfgManager* pCfgManager)
	{
		return TryGetTypedConstantImpl<float>(pOutValue, EValueCategory::Float, InName, pCfgManager);
	}

	const std::string* TryGetStringConstant(std::string_view InName, const CfgManager* pCfgManager)
	{
		const std::string* pOutResult;
		bool bGetted = TryGetTypedConstantImpl<const std::string*>(&pOutResult, EValueCategory::String, InName, pCfgManager);
		if (false == bGetted)
		{
			return nullptr;
		}
		return pOutResult;
	}

	const std::any* TryGetAnyConstant(std::string_view InName, IValueType* pInValueType, const CfgManager* pCfgManager)
	{
		const ConstDesc* pConst = nullptr;
		EGetSetTypedEntityCode result = TryGetTypedObjectConstantDesc(&pConst, pInValueType, InName, pCfgManager);
		if (EGetSetTypedEntityCode::Success != result)
		{
			return nullptr;
		}
		return &std::get<std::any>(pConst->Val.Val);
	}

	Value GetVar_OrDefault(std::string_view InName, Value InValue, const CfgManager* pConfigManager)
	{
		const Value* pGettedValue = GetVar_OrNull(InName, pConfigManager);
		if (pGettedValue)
		{
			return *pGettedValue;
		}
		return InValue;
	}

	Value GetVar_OrDefault(std::string_view InName, Value&& InValue, const CfgManager* pConfigManager)
	{
		const Value* pGettedValue = GetVar_OrNull(InName, pConfigManager);
		if (pGettedValue)
		{
			return *pGettedValue;
		}
		return std::move(InValue);
	}

	void DumpConfigManager(const CfgManager* pCfgManager)
	{
		pCfgManager->GetLog() << "Application info: " << std::endl;
		pCfgManager->GetLog() << pCfgManager->GetAppInfo();
		DumpConstants(pCfgManager);
		DumpConfigVariables(pCfgManager);
		DumpValueTypes(pCfgManager);
	}

	void DumpConstants(const CfgManager* pCfgManager)
	{
		pCfgManager->GetLog() << "CfgManager: Dumping constants..." << std::endl;
		auto pConstants = pCfgManager->GetConstantTable();
		int i = 0;
		pCfgManager->GetLog() << "Total " << pConstants->size() << " constants registered:" << std::endl;
		for (const auto& nameDesc : *pConstants)
		{
			pCfgManager->GetLog() << std::left << std::setw(32) << "Constant index: " << i << std::endl;
			pCfgManager->GetLog() << std::left << std::setw(32) << "Constant name: " << nameDesc.first << std::endl;
			pCfgManager->GetLog() << std::left << std::setw(32) << "Constant description: " << std::endl;
			pCfgManager->GetLog() << nameDesc.second << std::endl;
		}
		pCfgManager->GetLog() << "CfgManager: Dumping constants DONE" << std::endl;
	}

	void DumpConfigVariables(const CfgManager* pCfgManager)
	{
		pCfgManager->GetLog() << "CfgManager: Dumping config variables..." << std::endl;
		auto pConfigTable = pCfgManager->GetConfigTable();
		int i = 0;
		pCfgManager->GetLog() << "Total " << pConfigTable->size() << " config variables registered:"  << std::endl;
		for (const auto& nameVal : *pConfigTable)
		{
			pCfgManager->GetLog() << std::left << std::setw(32) << "Variable index: " << i << std::endl;
			pCfgManager->GetLog() << std::left << std::setw(32) << "Variable name: " << nameVal.first << std::endl;
			pCfgManager->GetLog() << std::left << std::setw(32) << "Value: " << std::endl;
			pCfgManager->GetLog() << nameVal.second << std::endl;
			i++;
		}
		pCfgManager->GetLog() << "CfgManager: Dumping config variables DONE" << std::endl;
	}

	void DumpValueTypes(const CfgManager* pCfgManager)
	{
		pCfgManager->GetLog() << "CfgManager: Dumping value types..." << std::endl;
		auto pTypeLib = pCfgManager->GetTypeLib();
		int i = 0;
		pCfgManager->GetLog() << "Total " << pTypeLib->size() << " value types registered:" << std::endl;
		for (const auto& nameValueType : *pTypeLib)
		{
			pCfgManager->GetLog() << std::left << std::setw(32) << "Type index: " << i << std::endl;
			pCfgManager->GetLog() << std::left << std::setw(32) << "Type name: " << nameValueType.second->GetName() << std::endl;
			i++;
		}
		pCfgManager->GetLog() << "CfgManager: Dumping value types DONE" << std::endl;
	}

	IValueType* GetRegisteredValueType(CfgManager* pManager, std::string_view InName)
	{
		auto pValueType = pManager->GetType_OrNull(InName);
		BOOST_ASSERT_MSG(pValueType, "ConfigManager: GetRegisteredValueType requires that the value type is registered");
		return pValueType;
	}
} // Dv::ConfigManager