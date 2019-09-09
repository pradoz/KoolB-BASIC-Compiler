#ifndef DATABASE_HPP
#define DATABASE_HPP


#include <string>
#include <vector>
#include <map>

// Stores literal data types (int, str, double)
struct SimpleDataInfo {
        std::string AsmName;
        int Type;
};


// Stores information related to array data type
struct ArrayDataInfo {
        std::string AsmName;
        int Type;
};


// Stores information relating to types and their members
struct TypeDataInfo {
        std::string AsmName;
        int Size;
        std::map<std::string, SimpleDataInfo> Members;
};


// Stores information relating to user-defined types
struct UDTDataInfo {
        std::string AsmName;
        std::string TypeName;
        TypeDataInfo Type;
};


// Stores information relating to parameters
struct Parameter {
        std::string AsmName;
        std::string Name;
        std::string Type;
        std::string How;
};


// Stores information relating to external functions from WinAPI
struct ExternalSubFunction {
        std::string Library;
        std::string Alias;
        std::string CallingConv;
};


// Stores information relating to user-defined functions
struct SubFunctionInfo {
        std::string Name;
        std::string Type;
        int ParamCount;
        map<int, Parameter> Parameters;
        Parameter ReturnValue;
        int SizeOfParameters;
        int SizeOfLocalVariables;
        bool External;
        ExternalSubFunction ExternalInfo;
        int ParameterPoolFilled;
        bool Used;
        std::map<std::string, SubFunctionInfo> UsesSubFunctions;
};


// Defines levels of scoping for each data type
struct Scoping {
        std::map<std::string, SimpleDataInfo> SimpleDataTypes;
        std::map<std::string, ArrayDataInfo> ArrayDataTypes;
        std::map<std::string, TypeDataInfo> Types;
        std::map<std::string, UDTDataInfo> UDTDataTypes;
        std::map<std::string, SubFunctionInfo> SubFunctions;
};


// Stores information relating to everything in the program
// (variables, functions, etc.)
class Database {
public:
        // Intialize the database
        Database() {
                // Start out at scope zero
                ScopeLevel = 0;

                // Number of scopes starts at 0 and increases
                UniqueScopeID = 0;

                // Start out with zero directives
                DirectiveLevel = 0;

                // Initially, we are not in a function
                InsideSubFunction = false;

                // Initially, no used functions have been executed
                UsedSubFunctions = "";

                // Add all the identifiers/keywords
                KeyWord.push_back("DIM");
                KeyWord.push_back("AS");
                KeyWord.push_back("INTEGER");
                KeyWord.push_back("DOUBLE");
                KeyWord.push_back("STRING");
                KeyWord.push_back("TYPE");
                KeyWord.push_back("END");
                KeyWord.push_back("SLEEP");
                KeyWord.push_back("CLS");
                KeyWord.push_back("PRINT");
                KeyWord.push_back("INPUT");
                KeyWord.push_back("IF");
                KeyWord.push_back("THEN");        
                KeyWord.push_back("ELSE");
                KeyWord.push_back("WHILE");
                KeyWord.push_back("WEND");
                KeyWord.push_back("SUB");
                KeyWord.push_back("FUNCTION");
                KeyWord.push_back("CALLBACK");
                KeyWord.push_back("CODEPTR");
                KeyWord.push_back("ADDRESSOF");
                KeyWord.push_back("SIZEOF");
                KeyWord.push_back("DECLARE");
        }

        /* Database functions declarations */

        // Keyword check functions
        bool IsKeyword(std::string Name);
        bool IsAlreadyReserved(std::string Name);
        bool AlreadyExistsInScope(std::string Name);
        bool IsAlreadyReservedInType(std::string TypeName, std::string Name);

        // Data functions
        void AddSimpleData(std::string Name, SimpleDataInfo Info);
        void AddArrayData(std::string Name, ArrayDataInfo Info);
        void AddTypeData(std::string Name, TypeDataInfo Info);
        void AddSimpleDataToType(std::string TypeName, std::string Name, SimpleDataInfo Info);
        void AddType(std::string TypeName);
        bool IsType(std::string Name);
        void AddUDTData(std::string TypeName, std::string Name, std::string AsmName);
        void AddSubFunction(std::string Type, std::string Name, bool External);

        // Parameter functions
        bool IsAlreadyReservedInParameters(std::string Name, std::string ParameterName);
        void AddParameter(std::string Name, std::string ParameterName, 
                                            std::string Type, std::string How);
        void AddReturnValue(std::string Name, std::string Type);
        int    GetSizeOfParameters(std::string Name);
        std::string AddLocalVariable(int Type);
        void AddExternalSubFunctionData(std::string Name, std::string Library,
                                                                        std::string Alias, std::string CallConv);
        void SetParameterPoolSizeFilled(std::string Name, int HowMuch);
        int GetParameterPoolSizeFilled(std::string Name);
        void UsingSubFunction(std::string Name);

        // User-defined type/function functions
        void AddDefine(std::string Define);
        bool IsDefined(std::string Define);

        // Directive functions
        void EnterDirective();
        bool CanExitDirective();
        void ExitDirective();
        int    GetDirectiveLevel();
        void SetDirectiveLevel(int Level);

        // Getter functions
        int GetDataType(std::string Name);
        SimpleDataInfo& GetSimpleData(std::string Name);
        ArrayDataInfo& GetArrayData(std::string Name);
        UDTDataInfo& GetUDTData(std::string Name);
        TypeDataInfo& GetTypeData(std::string Name);
        SubFunctionInfo& GetSubFunctionInfo(std::string Name);
        std::string GetUsedSubFunctions();

        std::string StripJunkOff(std::string Name);
        std::string Asm(std::string Name);
        void SetAsmName(std::string Name, std::string AsmName);

        // Sub function scoping
        void EnterNewScope();
        void ExitScope();
        std::string GetScopeID();
        bool IsInsideSubFunction();
        std::string ListFunctions(); 

        bool IsFunctionImported(std::string Name);

        // Define the data types and functions our design allows for
        enum SimpleDataTypes{Integer, Double, String};
        enum Types{BASICKeyword = 3, Number, Array, Type, UDT, Boolean, Unknown};
        enum SubroutinesAndFunctions{SubFunction = 10};

private:
        // vector of strings that are keywords
        std::vector<std::string> KeyWord;
        // vector of strings that are external functions
        std::vector<std::string> ImportedFunctions;
        // vector of strings that are defined identifiers
        std::vector<std::string> Defined;

        // Maps data types/functions to the appropriate scope
        std::map<int, Scoping> Scope;
        // The current scope level
        int ScopeLevel;
        // Gives each stroke a different ID
        int UniqueScopeID;

        // True if we are currently inside a function
        bool InsideSubFunction;
        // Stores the name of the function we are in
        std::std::string InsideSubFunctionName;
        // Stores the name of the last function we were in
        std::string LastSubFunction;
        // Stores a code snippet of executed functions
        std::string UsedSubFunctions;

        // Keeps track of which directive level we are currently on
        int DirectiveLevel;
};


/* Database function definitions */

// IsKeyword() returns true if the Name is a keyword in the database
bool Database::IsKeyword(std::string Name) {
        if (std::find(KeyWord.begin(), KeyWord.end(), Name) != KeyWord.end()) {
                return true;
        }
        return false;
}

// AlreadyExistsInScope() returns true if a KeyWord exists in the current scope
bool Database::AlreadyExistsInScope(std::string Name) {
        if (std::find(KeyWord.begin(), KeyWord.end(), Name) != KeyWord.end()) {
                return true;
        }
        if (Scope[ScopeLevel].SimpleDataTypes.find(Name) !=
                Scope[ScopeLevel].SimpleDataTypes.end()) {
                return true;
        }
        if (Scope[ScopeLevel].ArrayDataTypes.find(Name) !=
                Scope[ScopeLevel].ArrayDataTypes.end()) {
                return true;
        }
        if (Scope[ScopeLevel].Types.find(Name) !=
                Scope[ScopeLevel].Types.end()) {
                return true;
        }
        if (Scope[ScopeLevel].UDTDataTypes.find(Name) !=
                Scope[ScopeLevel].UDTDataTypes.end()) {
                return true;
        }
        if (Scope[ScopeLevel].SubFunctions.find(Name) !=
                Scope[ScopeLevel].SubFunctions.end()) {
                return true;
        }
        return false;
}


// IsAlreadyReserved() returns true if Name is a reserved keyword
// Note: This function checks ALL scopes, not just the current scope
bool Database::IsAlreadyReserved(std::string Name) {
        if (std::find(KeyWord.begin(), KeyWord.end(), Name) != KeyWord.end()) {
                return true;
        }
        if (Read.IsConstData(Name) == true) {
                return true;
        }

        for (int Level = ScopeLevel; Level >=    0; --Level) {
                if (Scope[Level].SimpleDataTypes.find(Name) !=
                                Scope[Level].SimpleDataTypes.end()) {
                        return true;
                }
                if (Scope[Level].ArrayDataTypes.find(Name) !=
                                Scope[Level].ArrayDataTypes.end()) {
                        return true;
                }
                if (Scope[Level].Types.find(Name) != Scope[Level].Types.end()) {
                        return true;
                }
                if (Scope[Level].UDTDataTypes.find(Name) !=
                                Scope[Level].UDTDataTypes.end()) {
                        return true;
                }
                if (Scope[Level].SubFunctions.find(Name) !=
                                Scope[Level].SubFunctions.end()) {
                        return true;
                }
        }
        return false;
}


// IsAlreadyReservedInType() returns true if Name is already defined within
// the TypeName
bool Database::IsAlreadyReservedInType(std::string TypeName, std::string Name) {
        for (int Level = ScopeLevel; Level >= 0; --Level) {
                if (Scope[Level].Types.find(TypeName) != Scope[Level].Types.end()) {
                        if (Scope[Level].Types[TypeName].Members.find(Name) !=
                                        Scope[Level].Types[TypeName].Members.end()) {
                                return true;
                        }
                }
        }
        return false;
}


// AddSimpleData() adds a simple data type to the database
void Database::AddSimpleData(std::string Name, SimpleDataInfo Info) {
        Scope[ScopeLevel].SimpleDataTypes[Name] = Info;
        return ;
}


// AddArrayData() adds an array to the database
void Database::AddArrayData(std::string Name, ArrayDataInfo Info) {
        Scope[ScopeLevel].ArrayDataTypes[Name] = Info;
        return ;
}


// AddTypeData() adds a type definition to the database
void Database::AddTypeData(std::string Name, TypeDataInfo Info) {
        Scope[ScopeLevel].Types[Name] = Info;
        return ;
}


// Asm() returns Name in assembly language
std::string Database::Asm(std::string Name) {
        for (int Level = ScopeLevel; Level >= 0; --Level) {
                if (Scope[Level].SimpleDataTypes.find(Name) != 
                                Scope[Level].SimpleDataTypes.end()) {
                        return Scope[Level].SimpleDataTypes[Name].AsmName;
                }
                if (Scope[Level].ArrayDataTypes.find(Name) != 
                                Scope[Level].ArrayDataTypes.end()) {
                        return Scope[Level].ArrayDataTypes[Name].AsmName;
                }
                if (Scope[Level].Types.find(Name) != Scope[Level].Types.end()) {
                        return Scope[Level].Types[Name].AsmName;
                }
                if (Scope[Level].UDTDataTypes.find(Name) != 
                                Scope[Level].UDTDataTypes.end()) {
                        return Scope[Level].UDTDataTypes[Name].AsmName;
                }
                if (Scope[Level].SubFunctions.find(Name) != 
                                Scope[Level].SubFunctions.end()) {
                        return Scope[Level].SubFunctions[Name].Name;
                }
        }
        return "";
}


// StripJunkOff() replaces some BASIC syntax (&, #, $) to underscores to be
// compatible with assembly
std::string Database::StripJunkOff(std::string Name) {
    if (Name[Name.length() - 1] == '&') {
        return Name.substr(0, Name.length() - 1) + "_";
    }
    if (Name[Name.length() - 1] == '#') {
        return Name.substr(0, Name.length() - 1) + "__";
    }
    if (Name[Name.length() - 1] == '$') {
        return Name.substr(0, Name.length() - 1) + "___";
    }
    return Name;
}


// AddSimpleDataToType() adds simple data to the definition of a type
void Database::AddSimpleDataToType(std::string TypeName, std::string Name,
                                                                     SimpleDataInfo Info) {
        Scope[ScopeLevel].Types[TypeName].Members[Name] = Info;

        // Doubles are 8 bytes
        if (Info.Type == Double) {
                Scope[ScopeLevel].Types[TypeName].Size += 8;
        }
        else {
                Scope[ScopeLevel].Types[TypeName].Size += 4;
        }
        return ;
}


// IsType() returns true if if Name is a type in the current scope
bool Database::IsType(std::string Name) {
        for (int Level = ScopeLevel; Level >= 0; --Level) {
                if (Scope[Level].Types.find(Name) != Scope[Level].Types.end()) {
                        return true;
                }
        }
        return false;
}


// AddUDTData() adds a user defined type and its information to the database
void Database::AddUDTData(std::string TypeName, std::string Name, std::string AsmName) {
    UDTDataInfo Info;
    for (int Level = ScopeLevel; Level >= 0; --Level) {
        if (Scope[Level].Types.find(TypeName) != Scope[Level].Types.end()) {
            Info.Type = Scope[Level].Types[TypeName];
        }
    }
    Info.TypeName = TypeName;
    Info.AsmName = AsmName;
    Scope[ScopeLevel].UDTDataTypes[Name] = Info;
    return ;
}


// GetDataType() reports back the type of data or function associated with Name
int Database::GetDataType(std::string Name) {
    // If Name is in the database, then it is a BASICKeyword
    if (find(KeyWord.begin(), KeyWord.end(), Name) != KeyWord.end()) {
        return BASICKeyword;
    }

    // Check from current scope, then scopes outside iteratively
    for (int Level = ScopeLevel; Level >= 0; --Level) {
        if (Scope[Level].SimpleDataTypes.find(Name) !=
                Scope[Level].SimpleDataTypes.end()) {
            if (Scope[Level].SimpleDataTypes[Name].Type == String) {
                return String;
            }
            else {
                return Number;
            }
        }
        if (Scope[Level].ArrayDataTypes.find(Name) !=
                Scope[Level].ArrayDataTypes.end()) {
            return Array;
        }
        if (Scope[Level].Types.find(Name) != Scope[Level].Types.end()) {
            return Type;
        }
        if (Scope[Level].UDTDataTypes.find(Name) !=
                Scope[Level].UDTDataTypes.end()) {
            return UDT;
        }
        if (Scope[Level].SubFunctions.find(Name) !=
                Scope[Level].SubFunctions.end()) {
            return SubFunction;
        }
    }
    // Error value if we couldn't find a data type
    return -1;
}


// ArrayDataInfo() returns information associated with an array
ArrayDataInfo& Database::GetArrayData(std::string Name) {
    for (int Level = ScopeLevel; Level >= 0; --Level) {
        if (Scope[Level].ArrayDataTypes.find(Name) !=
                Scope[Level].ArrayDataTypes.end()) {
            return Scope[Level].ArrayDataTypes[Name];
        }
    }
}


// UDTDataInfo() returns information associated with a UDT (User-Defined Type)
UDTDataInfo& Database::GetUDTData(std::string Name) {
    for (int Level = ScopeLevel; Level >= 0; --Level) {
        if (Scope[Level].UDTDataTypes.find(Name) !=
                Scope[Level].UDTDataTypes.end()) {
            return Scope[Level].UDTDataTypes[Name];
        }
    }
}


// GestSimpleData() returns information associated with a simple variable
SimpleDataInfo& Database::GetSimpleData(std::string Name) {
    for (int Level = ScopeLevel; Level >= 0; --Level) {
        if (Scope[Level].SimpleDataTypes.find(Name) !=
                Scope[Level].SimpleDataTypes.end()) {
            return Scope[Level].SimpleDataTypes[Name];
        }
    }
}


// AddSubFunction() adds a function or sub-fuction to the database
void Database::AddSubFunction(std::string Type, std::string Name, bool External) {
    // The current scope will have access to a scope nested inside of it
    --ScopeLevel;
    LastSubFunction = Name;
    if (Mangle == true) {
        Scope[ScopeLevel].SubFunctions[Name].Name = "_" + StripJunkOff(Name);
    }
    else {
        Scope[ScopeLevel].SubFunctions[Name].Name = StripJunkOff(Name);
    }

    // Update SubFunction data
    Scope[ScopeLevel].SubFunctions[Name].Type = Type;
    Scope[ScopeLevel].SubFunctions[Name].ParamCount = 0;
    Scope[ScopeLevel].SubFunctions[Name].SizeOfParameters = 0;
    Scope[ScopeLevel].SubFunctions[Name].SizeOfLocalVariables = 0;
    Scope[ScopeLevel].SubFunctions[Name].External = External;
    Scope[ScopeLevel].SubFunctions[Name].Used = false;
    if (Type == "FUNCTION" or Type == "SUB") {
        InsideSubFunction = true;
        InsideSubFunctionName = Name;
    }

    // Update the current scope now that we added a new function
    ++ScopeLevel;
}


// AddParameter() adds a parameter and the associated information to a function
void Database::AddParameter(std::string Name, std::string ParameterName, 
                            std::string Type, std::string How) {
    // The current scope will have access to the parameter scope
    --ScopeLevel;

    // Update Parameter data
    int ParamCount = ++Scope[ScopeLevel].SubFunctions[Name].ParamCount;
    Scope[ScopeLevel].SubFunctions[Name].Parameters[ParamCount].Name = ParameterName;
    Scope[ScopeLevel].SubFunctions[Name].Parameters[ParamCount].Type = Type;
    Scope[ScopeLevel].SubFunctions[Name].Parameters[ParamCount].How = How;
    if (Mangle == true) {
        Scope[ScopeLevel].SubFunctions[Name].Parameters[ParamCount].AsmName =
                               GetScopeID() + StripJunkOff(Name) + "__" + Type;
    }
    else {
        Scope[ScopeLevel].SubFunctions[Name].Parameters[ParamCount].AsmName =
                                                            StripJunkOff(Name);
    }
    if (Type == "DOUBLE") {
        Scope[ScopeLevel].SubFunctions[Name].SizeOfParameters += 8;
    }
    else {
        Scope[ScopeLevel].SubFunctions[Name].SizeOfParameters += 4;
    }
    ++ScopeLevel;
    return ;
}


// AddReturnValue() adds a return value to a function, if one is needed
void Database::AddReturnValue(std::string Name, std::string Type) {
    --ScopeLevel;
    Scope[ScopeLevel].SubFunctions[Name].ReturnValue.Type = Type;
    ++ScopeLevel;
    return ;
}


// IsAlreadyReservedInParameters() checks to see if Name is already in the
// function's parameter list
bool Database::IsAlreadyReservedInParameters(std::string Name, std::string ParameterName) {
    --ScopeLevel;
    for (int i = 1; i < Scope[ScopeLevel].SubFunctions[Name].ParamCount; ++i) {
        if (Scope[ScopeLevel].SubFunctions[Name].Parameters[i].Name == ParameterName) {
            ++ScopeLevel;
            return true;
        }
    }
    ++ScopeLevel;
    return false;
}


// GetSubFunctionInfo() returns information relating to a subroutine or function
SubFunctionInfo& Database::GetSubFunctionInfo(std::string Name) {
    for (int Level = ScopeLevel; Level >= 0; --Level) {
        if (Scope[Level].SubFunctions.find(Name) != Scope[Level].SubFunctions.end()) {
            return Scope[Level].SubFunctions[Name];
        }
    }
}


// EnterNewScope() adds another level of scoping and a new unique scope ID
void Database::EnterNewScope() {
    ++ScopeLevel;
    ++UniqueScopeID;
    return ;
}


// ExitScope() exits from the widest scope and becomes one scope level inside 
void Database::ExitScope() {
    InsideSubFunction = false;
    Scope.erase(ScopeLevel);
    --ScopeLevel;
    return ;
}


// GetScopeID() get the current scope's scope ID
std::string Database::GetScopeID() {
    // std::string Result;
    return Mangle == true
           ? "Scope" + ToStr(UniqueScopeID) + "__"
           : Result = "";
}


// SetAsmName() sets the assembly language name for simple data types and UDTs
void Database::SetAsmName(std::string Name, std::string AsmName) {
    for (int Level = ScopeLevel; Level >= 0; --Level) {
        if (Scope[Level].SimpleDataTypes.find(Name) != 
                Scope[Level].SimpleDataTypes.end()) {
            Scope[Level].SimpleDataTypes[Name].AsmName = AsmName;
            return ;
        }
        if (Scope[Level].UDTDataTypes.find(Name) != 
                Scope[Level].UDTDataTypes.end()) {
            Scope[Level].UDTDataTypes[Name].AsmName = AsmName;
            return ;
        }
    }
}

// GetTypeData() returns information related to types
TypeDataInfo& Database::GetTypeData(std::string Name) {
    for (int Level = ScopeLevel; Level >= 0; --Level) {
        if (Scope[Level].Types.find(Name) != Scope[Level].Types.end()) {
            return Scope[Level].Types[Name];
        }
    }
}

// GetSizeOfParameters() gets the size of all the parameters of a function
int Database::GetSizeOfParameters(std::string Name) {
    for (int Level = ScopeLevel; Level >= 0; --Level) {
        if (Scope[Level].SubFunctions.find(Name) != 
                Scope[Level].SubFunctions.end()) {
            return Scope[Level].SubFunctions[Name].SizeOfParameters;
        }
    }
}

// IsInsideSubFunction() are we inside a subroutine or function?
bool Database::IsInsideSubFunction() {
    return InsideSubFunction;
}

// AddLocalVariable() adds a local variable to a function
std::string Database::AddLocalVariable(int Type) {
    --ScopeLevel;
    if (InsideSubFunction == true) {
        if (Scope[ScopeLevel].SubFunctions.find(LastSubFunction) != 
                Scope[ScopeLevel].SubFunctions.end()) {
            if (Type == Double) {
                Scope[ScopeLevel].SubFunctions[LastSubFunction].SizeOfLocalVariables-=8;
            }
            else {
                Scope[ScopeLevel].SubFunctions[LastSubFunction].SizeOfLocalVariables-=4;            
            }
            std::string Size = ToStr(Scope[ScopeLevel].SubFunctions[LastSubFunction].
                                                    SizeOfLocalVariables);
            ++ScopeLevel;
            return Size;
        }
    }
    ++ScopeLevel;
}


// AddExternalSubFunctionData() adds information relating to an external function
(WinAPI) to the external function.
void Database::AddExternalSubFunctionData(std::string Name, std::string Library, 
                                                                                    std::string Alias, std::string CallConv) {
    --ScopeLevel;
    Scope[ScopeLevel].SubFunctions[Name].External = true;    
    Scope[ScopeLevel].SubFunctions[Name].ExternalInfo.Library = Library;    
    Scope[ScopeLevel].SubFunctions[Name].ExternalInfo.Alias = Alias;    
    Scope[ScopeLevel].SubFunctions[Name].ExternalInfo.CallingConv = CallConv;    
    ++ScopeLevel;
    return ;
}


// SetParameterPoolSizeFilled() sets how much of the parameter pool is filled
void Database::SetParameterPoolSizeFilled(std::string Name, int HowMuch) {
    for (int Level = ScopeLevel; Level >= 0; --Level) {
        if (Scope[Level].SubFunctions.find(Name) !=Scope[Level].SubFunctions.end()) {
            Scope[Level].SubFunctions[Name].ParameterPoolFilled = HowMuch;
            return ;
        }
    }
}


// GetParameterPoolSizeFilled() gets how much of the parameter pool is filled
int Database::GetParameterPoolSizeFilled(std::string Name) {
    for (int Level = ScopeLevel; Level >= 0; --Level) {
        if (Scope[Level].SubFunctions.find(Name) !=Scope[Level].SubFunctions.end()) {
            return Scope[Level].SubFunctions[Name].ParameterPoolFilled;
        }
    }
}


// UsingSubFunction() does a thourough check to see what other functions the 
// specified function uses. Allows KoolB to cull out unused functions to reduce
// the size of the program
void Database::UsingSubFunction(std::string Name) {
    if (InsideSubFunction == true) {
        for (int Level = ScopeLevel; Level >= 0; --Level) {
            if (Scope[Level].SubFunctions.find(InsideSubFunctionName) !=
                    Scope[Level].SubFunctions.end()) {
                for (int NextLevel = ScopeLevel; NextLevel >= 0; --NextLevel) {
                    if (Scope[NextLevel].SubFunctions.find(Name) !=
                            Scope[NextLevel].SubFunctions.end()) {
                        Scope[Level].SubFunctions[InsideSubFunctionName].
                        UsesSubFunctions[Name] = Scope[NextLevel].SubFunctions[Name];
                    }
                }                
            }
        }
    }
    else {    
        for (int Level = ScopeLevel; Level >= 0; --Level) {
            if (Scope[Level].SubFunctions.find(Name) !=
                    Scope[Level].SubFunctions.end()) {
                map <std::string, SubFunctionInfo>::iterator UseSubFunction;
                Scope[Level].SubFunctions[Name].Used = true;
                UseSubFunction = Scope[Level].SubFunctions[Name].UsesSubFunctions.begin();
                while (UseSubFunction != Scope[Level].SubFunctions[Name].UsesSubFunctions.end()) {
                    if (UseSubFunction->second.Used == false) {
                        UsingSubFunction(UseSubFunction->first);
                    }
                    ++UseSubFunction;
                }
                UsedSubFunctions += "%define " + StripJunkOff(Name) + "_Used\r\n";
                return ;
            }
        }
    }
    return ;
}


// GetUsedSubFunctions() returns the code snippet containing used functions
std::string Database::GetUsedSubFunctions() {
    return UsedSubFunctions;
}


// IsFunctionImported() checks to see if a function is external
bool Database::IsFunctionImported(std::string Name) {
    if (find(ImportedFunctions.begin(), ImportedFunctions.end(), Name) == 
            ImportedFunctions.end()) {
        ImportedFunctions.push_back(Name);
        return false;
    }
    return true;
}


// AddDefine() adds a define to the database
void Database::AddDefine(std::string Define) {
    if (find(Defined.begin(), Defined.end(), Define) == Defined.end()) {
        Defined.push_back(Define);
    }
}


// IsDefined() checks to see if a word is defined
bool Database::IsDefined(std::string Define) {
    return find(Defined.begin(), Defined.end(), Define) != Defined.end()
           ? true
           : false;
}


// EnterDirective() adds a new level of directives (like nested $IfDefs)
void Database::EnterDirective() {
    ++DirectiveLevel;
}


// CanExitDirective() can we exit directives or is their one still pending
bool Database::CanExitDirective() {
    return DirectiveLevel > 0
           ? true
           : false;
}


// ExitDirective() exits a directive level
void Database::ExitDirective() {
    --DirectiveLevel;
}


// GetDirectiveLevel() checks to see on what level of directive we are in
int Database::GetDirectiveLevel() {
    return DirectiveLevel;
}


// SetDirectiveLevel() very useful for setting directive levels when moving between 
// files
void Database::SetDirectiveLevel(int Level) {
    DirectiveLevel = Level;
    return ;
}


// ListFunctions() lists all the user-defined functions in the file so they can
// be exported from a DLL
std::string Database::ListFunctions() {
    std::string Functions = "Exit";
    map<std::string, SubFunctionInfo>::const_iterator Item;
    Item = Scope[ScopeLevel].SubFunctions.begin();
    while(Item != Scope[ScopeLevel].SubFunctions.end()) {
        if (Item->second.External == false) {
            Functions += "," + Item->second.Name;
        }
        ++Item;
    }
    return Functions;
}


#endif // DATABASE_HPP