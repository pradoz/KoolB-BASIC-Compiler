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

    /* Database Member functions */

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
    int  GetSizeOfParameters(std::string Name);
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
    int  GetDirectiveLevel();
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
































#endif // DATABASE_HPP