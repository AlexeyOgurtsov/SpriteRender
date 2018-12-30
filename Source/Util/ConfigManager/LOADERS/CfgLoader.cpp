#include "CfgLoader.h"
#include "CfgLoader_IMPL.h"
#include <fstream>
#include <boost/container/static_vector.hpp>

namespace Dv::ConfigManager
{
	CfgLoadResult EvaluateExpression(Value *pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams);

	ECfgLoadResultCode ProcessCommand_NoArguments(std::string_view InCommandName, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		// TODO
		return ECfgLoadResultCode::UnknownCommandName;
	}

	CfgLoadResult EvaluateArray(Value* pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		// TODO
		return result;
	}

	constexpr int MAX_FUNCTION_ARGUMENTS = 16 + 4; // 16 - minimal to allow matrix init
	constexpr int MAX_PROPERTY_SETTERS = 300; // // 16 - minimal to allow matrix init

	// Process object intantiation using the '(' scopes (funcional style).
	// The type name of the object must be already read and provided as an argument.
	// The '{' element must also be read already.
	// Object can always be moved from.
	CfgLoadResult EvaluateObjectConstruction(const IValueType* pInType, Value* pOutResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		boost::container::static_vector<Value, MAX_FUNCTION_ARGUMENTS> args;
		while (true)
		{
			result = StartLexem(pStream);
			if (result.IsFailed())
			{
				return result;
			}
			if (pStream->IsEOF())
			{
				// Here, all arguments have been read, but NO ')' character found,
				// so it's an error
				result.Code = ECfgLoadResultCode::ConstructorBracketNotClosed;
				break;
			}
			if (pStream->Peek() == ',')
			{
				// Here we parsed a comma, we must skip it and go to the next lexem
				pStream->Skip(); // Skip the comma
				result = StartLexem(pStream);
				continue; // Go to the next argument
			}
			if (pStream->Peek() == ')')
			{
				// Here we successfully parsed an object instantiation sequence
				pStream->Skip(); // Skipping ')'
				break;
			}

			// Check that number of arguments is not greater than allowed.
			if ((args.size()+1) >= MAX_FUNCTION_ARGUMENTS)
			{
				result.Code = ECfgLoadResultCode::MaxFunctionArgumentsExceeded;
				return result;
			}

			// Reading an argument
			args.emplace_back();
			auto pNextArg = &args.back();
			result = EvaluateExpression(pNextArg, pStream, pManager, InParams);

			if (result.IsFailed())
			{
				return result;
			}
		}

		// Here we must construct the objects from the arguments
		result.ObjAction = pInType->CreateValue(pOutResult, args.data(), args.size());
		if (result.ObjAction.IsFailed())
		{
			result.Code = ECfgLoadResultCode::ObjectInstantiateError;
			return result;
		}

		return result;
	}

	CfgLoadResult ReadPropertyName(std::string_view* pOutIdentifier, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;

		result = ParseIdentifier_SkipWhitespaces(pStream, pOutIdentifier);
		if (result.IsFailed())
		{
			return result;
		}

		return result;
	}

	bool CanAssignValueTo(const ValueTypeDecl& InLeftType, const Value& InValue)
	{
		return InValue.CanConvertTo(InLeftType);
	}

	bool CanAssignValueToProp(const ValueTypeDecl& InPropType, const Value& InValue)
	{
		return CanAssignValueTo(InPropType, InValue);
	}

	// Reads the next property setter;
	CfgLoadResult ReadAndValidatePropertySetter(PropertySetter* pResult, const IValueType* pInType, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		std::string_view propName;
		result = ReadPropertyName(&propName, pStream, pManager, InParams);
		pResult->first = propName;
		if (result.IsFailed())
		{
			return result;
		}

		// Check that property exists
		auto pObjectProperties = &(pInType->GetProperties());
		auto it_prop = pObjectProperties->find(std::string(propName));
		if (it_prop == pObjectProperties->end())
		{
			result.Code = ECfgLoadResultCode::PropertySetterInexistentProperty;
			return result;
		}

		// Parsing an equality sign
		result = StartLexem(pStream);
		if (result.IsFailed())
		{
			return result;
		}
		if (pStream->Peek() != '=')
		{
			result.Code = ECfgLoadResultCode::PropertySetterValueNotProvided;
			return result;
		}
		pStream->Skip(); // Skipping the equality sign ('=')

		// Parse value expression
		result = StartLexem(pStream);
		if (result.IsFailed())
		{
			return result;
		}

		result = EvaluateExpression(&pResult->second, pStream, pManager, InParams);
		if (result.IsFailed())
		{
			return result;
		}

		// Check that property type corresponds the initializing expression type
		auto pProps = pInType->GetProperties();
		
		if (false == CanAssignValueToProp(it_prop->second.TypeDecl, pResult->second))
		{
			result.Code = ECfgLoadResultCode::PropertySetterWrongPropertyType;
			return result;
		}

		return result;
	}

	CfgLoadResult EvaluateObjectPropertyListConstruction(const IValueType* pInType, Value* pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		PropertySetterMap propertySetters;
		while (true)
		{
			result = StartLexem(pStream);
			if (result.IsFailed())
			{
				return result;
			}
			if (pStream->IsEOF())
			{
				// Here, all arguments have been read, but NO '}' character found,
				// so it's an error
				result.Code = ECfgLoadResultCode::ClassBracketNotClosed;
				break;
			}
			if (pStream->Peek() == ',')
			{
				// Here we parsed a comma, we must skip it and go to the next lexem
				pStream->Skip(); // Skip the comma
				result = StartLexem(pStream);
				continue; // Go to the next argument
			}
			if (pStream->Peek() == '}')
			{
				// Here we successfully parsed an object instantiation sequence
				pStream->Skip(); // Skipping ')'
				break;
			}

			// Try to read the next property setter
			PropertySetter propSetter;
			result = ReadAndValidatePropertySetter(&propSetter, pInType, pStream, pManager, InParams);
			if (result.IsFailed())
			{
				return result;
			}

			// Check that the property assigner was NOT specified yet
			auto it_existing_setter = propertySetters.find(propSetter.first);
			if (it_existing_setter != propertySetters.cend())
			{
				result.Code = ECfgLoadResultCode::PropertySetterAmbiguity;
				return result;
			}


			// Check that number of property setters is not greater than allowed.
			if (propertySetters.size() >= MAX_PROPERTY_SETTERS)
			{
				result.Code = ECfgLoadResultCode::MaxPropertySettersExceeded;
				return result;
			}
			propertySetters.emplace(propSetter);
		}

		// Here we successfully read all property setters, so try to create a new value
		result.ObjAction = pInType->CreateValueByPropSetters(pResult, std::move(propertySetters));
		if (result.ObjAction.IsFailed())
		{
			result.Code = ECfgLoadResultCode::ObjectInstantiateError;
		}

		return result;
	}

	// Process object declaration of any type (function, type sequence).
	// The type name of the object must be already read and provided as an argument.
	// Object can always be moved from.
	CfgLoadResult EvaluateObject(const IValueType* pInType, Value* pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		result = StartLexem(pStream);
		if (pStream->IsEOF())
		{
			result.Code = ECfgLoadResultCode::ObjectInstantiationArgsNotProvided;
			return result;
		}
		if (result.IsFailed())
		{
			return result;
		}
		switch (pStream->Peek())
		{
		case '(':
			pStream->Skip();
			return EvaluateObjectConstruction(pInType, pResult, pStream, pManager, InParams);

		case '{':
			pStream->Skip();
			return EvaluateObjectPropertyListConstruction(pInType, pResult, pStream, pManager, InParams);

		default:
			break;
		}
		// Here an unknown object instantiation syntax was used
		result.Code = ECfgLoadResultCode::WrongObjectInstantiationSyntax;
		return result;
	}

	CfgLoadResult EvaluateVariable(Value* pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		std::string_view variableName;
		result.LexCode = ParseIdentifier(pStream, &variableName, &result.SourceLoc.Loc);
		if (result.LexCode != ELexemParseCode::Success)
		{
			result.Code = ECfgLoadResultCode::LexemError;
			return result;
		}
		auto pVarValue = GetVar_OrNull(variableName, pManager);
		if (pVarValue == nullptr)
		{
			result.Identifier = variableName;
			result.Code = ECfgLoadResultCode::UnknownVariableName;
			return result;
		}

		// Here we have a valid existing variable:
		*pResult = *pVarValue;

		return result;
	}

	CfgLoadResult EvaluateConstant(Value* pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;

		// Skip the '#'
		pStream->Skip();

		// WARNING!! Here we should NOT skip the whitespaces, as the enum or constant must follow the # immediately!!!
		if (pStream->IsEOF())
		{
			result.Code = ECfgLoadResultCode::ConstantNameExpected;
			return result;
		}

		// Here the the enum name, or constant name is expected
		std::string_view firstIdentifier;
		result.LexCode = ParseIdentifier(pStream, &firstIdentifier, &result.SourceLoc.Loc);
		if (result.LexCode != ELexemParseCode::Success)
		{
			result.Code = ECfgLoadResultCode::LexemError;
			return result;
		}

		// We successfully getted the first lexem
		// As enums are NOT supported yet, it's to be treated as a constant name.
		const ConstDesc* pConstDesc = TryGetConstantDesc(firstIdentifier, pManager);
		if (pConstDesc == nullptr)
		{
			result.Code = ECfgLoadResultCode::UnknownConstantName;
			return result;
		}

		// Here we have a constant name read correctly
		*pResult = pConstDesc->Val;

		return result;
	}

	CfgLoadResult EvaluateVariableOrObject(Value* pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;

		std::string_view firstLexem;
		result.LexCode = ParseIdentifier(pStream, &firstLexem, &result.SourceLoc.Loc);
		if (result.LexCode != ELexemParseCode::Success)
		{
			result.Code = ECfgLoadResultCode::LexemError;
			return result;
		}

		auto pValueType = GetNullOrValueType(firstLexem, pManager);
		if (pValueType != nullptr)
		{
			// Here we have a valid value type, so it's an object declaration
			return EvaluateObject(pValueType, pResult, pStream, pManager, InParams);
		}
		// Here we have a variable, because it's NOT a type
		return EvaluateVariable(pResult, pStream, pManager, InParams);
	}

	CfgLoadResult EvaluateFloat(Value* pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		float flt;
		result.LexCode = ParseFloat(pStream, &flt, &result.SourceLoc.Loc);
		if (result.LexCode != ELexemParseCode::Success)
		{
			result.Code = ECfgLoadResultCode::LexemError;
			return result;
		}

		// Here, we have a float variable, so return it
		*pResult = Value(flt);

		return result;
	}

	CfgLoadResult EvaluateInt(Value* pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		int the_int;
		result.LexCode = ParseInt(pStream, &the_int, &result.SourceLoc.Loc);
		if (result.LexCode != ELexemParseCode::Success)
		{
			result.Code = ECfgLoadResultCode::LexemError;
			return result;
		}


		// Here, we have an int variable, so return it
		*pResult = Value(the_int);

		return result;
	}

	// Evaluate number:
	// Either float or int.
	CfgLoadResult EvaluateNumber(Value* pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		result.LexCode = IsFloat(pStream->GetCurrPtr(), pStream->CountRestToRead());
		if (result.LexCode == ELexemParseCode::NonLexemChar)
		{
			// Here, if NonLexemChar is returned, it's NOT a float number
			return EvaluateInt(pResult, pStream, pManager, InParams);
		}
		if (result.LexCode != ELexemParseCode::Success)
		{
			// Here, an error was encountered
			result.Code = ECfgLoadResultCode::LexemError;
			return result;
		}
		// Here, we have an int (as it's not float)
		return EvaluateFloat(pResult, pStream, pManager, InParams);
	}

	CfgLoadResult EvaluateString(Value* pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		std::string_view the_string;
		result.LexCode = ParseQuotedString(pStream, &the_string, &result.SourceLoc.Loc);
		if (result.LexCode != ELexemParseCode::Success)
		{
			result.Code = ECfgLoadResultCode::LexemError;
			return result;
		}


		// Here, we have an int variable, so return it
		*pResult = Value(the_string);

		return result;
	}

	CfgLoadResult TryEvaluatePrimaryExpression(bool* pbOutEvaluated, Value* pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		*pbOutEvaluated = true;

		if (pStream->IsEOF())
		{
			result.Code = ECfgLoadResultCode::ValueToAssignNotProvided;
			return result;
		}

		if (pStream->Peek() == '[')
		{
			return EvaluateArray(pResult, pStream, pManager, InParams);
		}

		if (pStream->Peek() == '#')
		{
			return EvaluateConstant(pResult, pStream, pManager, InParams);
		}
		
		if (IsIdentifierStartChar(pStream->Peek()))
		{
			return EvaluateVariableOrObject(pResult, pStream, pManager, InParams);
		}

		if (pStream->Peek() == '.' || isdigit(pStream->Peek()) || (pStream->Peek() == '-'))
		{
			return EvaluateNumber(pResult, pStream, pManager, InParams);
		}

		if (pStream->Peek() == '\"')
		{
			return EvaluateString(pResult, pStream, pManager, InParams);
		}

		*pbOutEvaluated = false;
		return result;
	}

	CfgLoadResult EvaluateExpression(Value *pResult, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		CfgLoadResult result;
		bool bPrimaryExprEvaluated = false;
		result = TryEvaluatePrimaryExpression(&bPrimaryExprEvaluated, pResult, pStream, pManager, InParams);
		if (false == bPrimaryExprEvaluated)
		{
			result.Code = ECfgLoadResultCode::IllegalLexemInExpression;
			return result;
		}
		return result;
	}

	ECfgLoadResultCode ProcessAssignment(std::string_view InVariableName, Value&& InRightSideResult, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		pManager->GetLog() << "LoadCfgFile: Variable assigning..." << std::endl;
		pManager->GetLog() << std::left << std::setw(32) << "Variable name: " << InVariableName << std::endl;

		auto pConfigTable = pManager->GetConfigTable();
		std::string VarNameStr = std::string(InVariableName);
		auto it_variable = pConfigTable->find(VarNameStr);
		if (it_variable == pConfigTable->end())
		{
			pManager->GetLog() << "LoadCfgFile: Variable with the given name does NOT exist - creating new" << std::endl;
			auto [it_registerd_var,bRegistered] = pConfigTable->insert(std::make_pair(VarNameStr, std::move(InRightSideResult)));
			it_variable = it_registerd_var;
		}
		else
		{
			pManager->GetLog() << "LoadCfgFile: Variable successfully found in the config table" << std::endl;
			pManager->GetLog() << "Variable type: " << std::endl;
			pManager->GetLog() << it_variable->second.TypeDecl << std::endl;

			if (false == CanAssignValueTo(it_variable->second.TypeDecl, InRightSideResult))
			{
				pManager->GetLog() << "LoadCfgFile: unable to assign value of the variable by name: type mismatch" << std::endl;
				return ECfgLoadResultCode::TypeMismatch;
			}

			// Perform the assignment itself
			const auto& varTypeDecl = it_variable->second.TypeDecl;
			if (varTypeDecl.Category == EValueCategory::Object)
			{
				// Variable is an object: 
				varTypeDecl.pType->MoveValue(&it_variable->second, std::move(InRightSideResult));
			}
			else
			{
				// Variable is a value type
				it_variable->second = InRightSideResult;
			}
		}

		// Now the variable is successfully assigned, dumping its value:
		pManager->GetLog() << "New variable state: " << std::endl;
		pManager->GetLog() << it_variable->second << std::endl;

		pManager->GetLog() << "LoadCfgFile: Variable assigning DONE" << std::endl;
		return ECfgLoadResultCode::Success;
	}

	CfgLoadResult LoadCfgFile(std::string_view InFilename, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		std::string fileText;
		InputStream inputStream;
		bool bOpened = TryOpenInputStream(&inputStream, InFilename, &fileText);
		if (false == bOpened)
		{
			pManager->GetLog() << "LoadCfgFile: Unable to open config file: " << InFilename << std::endl;
			CfgLoadResult result { ECfgLoadResultCode::WrongFilename };
			return result;
		}
		auto result = LoadCfgFile(InFilename, &inputStream, pManager, InParams);
		return result;
	}

	CfgLoadResult LoadCfgFile(std::string_view InFilename, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams)
	{
		pManager->GetLog() << "LoadCfgFile: Loading config..." << std::endl;
		pManager->GetLog() << "Filename: " << InFilename << std::endl;
		CfgLoadResult result{}; // Successful result, by default-constructing
		
		while (true)
		{
			// Read the first lexem
			std::string_view firstLexem;

			result = StartLexem(pStream);
			if (result.IsFailed())
			{
				pManager->GetLog() << "LoadCfgFile: Error while parsing the first lexem of the instruction: " << std::endl;
				pManager->GetLog() << result << std::endl;
				break;
			}

			if (pStream->IsEOF())
			{
				// Here, the first lexem is NOT found, so we stop the loading
				pManager->GetLog() << "LoadCfgFile: The next instruction is not FOUND - stopping parsing " << std::endl;
				pManager->GetLog() << result << std::endl;
				break;
			}

			// Try to read the first lexem as an identifier
			result = ParseIdentifier_SkipWhitespaces(pStream, &firstLexem);
			if (result.IsFailed())
			{
				if (result.Code != ECfgLoadResultCode::LexemError)
				{
					result.Code = ECfgLoadResultCode::WrongInstruction;
				}
				pManager->GetLog() << "LoadCfgFile: The first lexem is NOT an identifier" << std::endl;
				pManager->GetLog() << result << std::endl;
				break;
			}

			// Try to the the second lexem:
			result = StartLexem(pStream);
			if (result.IsFailed())
			{
				// Here we have error of comment
				pManager->GetLog() << "LoadCfgFile: Error while trying to find the second lexem of the instruction" << std::endl;
				pManager->GetLog() << result << std::endl;
				break;
			}
			if (pStream->IsEOF())
			{
				result.Code = ECfgLoadResultCode::WrongInstruction;
				// Here, the second lexem is NOT found, so we have an unfinished instruction
				pManager->GetLog() << "LoadCfgFile: wrong lexem -  no second lexem is found" << std::endl;
				pManager->GetLog() << result << std::endl;
				break;
			}
			// Here we have a second lexem of an unknown kind

			if (pStream->Peek() == ';')
			{
				// Here we have an instruction of the following form: "identifier;"
				result.Code = ProcessCommand_NoArguments(firstLexem, pManager, InParams);
				if (result.IsFailed())
				{
					// Here the instruction execution failed due to some error
					pManager->GetLog() << "LoadCfgFile: Error trying to execute a command without arguments" << std::endl;
					pManager->GetLog() << result << std::endl;
					break;
				}
				else
				{
					// Go to the next command, if this command was executed successfully
					pStream->Skip(); // skipping ';'
					continue;
				}
			}

			if (pStream->Peek() == '=')
			{
				pStream->Skip();

				// Skip whitespaces
				result = StartLexem(pStream);
				if (result.IsFailed())
				{
					return result;
				}

				// If EOF is found, then NO value was provided for the assignment operator
				if (pStream->IsEOF())
				{
					result.Code = ECfgLoadResultCode::ValueToAssignNotProvided;
					return result;
				}

				// Evaluate the right-side expression of the assignment,
				// until the ';' character is found
				Value rightSideResult;
				result = EvaluateExpression(&rightSideResult, pStream, pManager, InParams);
				if (result.IsFailed())
				{
					// Here expression evaluation is failed
					pManager->GetLog() << "LoadCfgFile: Expression evaluation failed: " << std::endl;
					pManager->GetLog() << result << std::endl;
					break;
				}

				// Skip the instruction separator
				result = StartLexem(pStream);
				if (result.IsFailed())
				{
					pManager->GetLog() << "LoadCfgFile: error trying to find the ';' separator: " << std::endl;
					pManager->GetLog() << result << std::endl;
					break;
				}
				if (pStream->IsEOF())
				{
					pManager->GetLog() << "LoadCfgFile: EOF trying to find the ';' separator: " << std::endl;
					pManager->GetLog() << result << std::endl;
					result.Code = ECfgLoadResultCode::SemicolonNotFound;
					break;
				}
				if (pStream->Peek() != ';')
				{
					pManager->GetLog() << "LoadCfgFile: wrong  identifier after the right-hand expression of the assignment operator:" << std::endl;
					pManager->GetLog() << result << std::endl;
					result.Code = ECfgLoadResultCode::SemicolonNotFound;
					break;
				}
				pStream->Skip(); // skipping ';'

				// Here we have an assignment instruction
				result.Code = ProcessAssignment(firstLexem, std::move(rightSideResult), pManager, InParams);

				if (result.IsFailed())
				{
					pManager->GetLog() << "LoadCfgFile: Error while processing assignment: " << std::endl;
					pManager->GetLog() << result << std::endl;
					break;
				}
				else
				{
					// Successfully assigned, so we may go to the next instruction
					continue;
				}
			}

			
		}

		pManager->GetLog() << "LoadCfgFile: Loading config: DONE" << std::endl;
		return result;
	}
} // Dv::ConfigManager