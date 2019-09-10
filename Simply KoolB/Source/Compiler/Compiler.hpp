#ifndef FILENAME_HPP
#define FILENAME_HPP

#include <string>

class Compiler {
public:
    void Compile(Reading SourceFile, bool IsIncFile);
    void Statement();
    void DIM();
    void NotDIM();

    // Variables
    void DIMCreate(std::string Name);

    // Types
    void TYPE();
    void TYPECreate(std::string TypeName, std::string Name);

    // Arrays
    void GetArraySize();
    void DIMCreateArray(std::string Name);
    void Assignment();

    // Expressions
    int Expression(int Type, std::string ExtraInfo = "");

    // Binary Operators
    void AddSubtract();
    void MultiplyDivideMod();
    void Exp(); // Exponents


private:
    // Read contains the BASIC source code to compile
    Reading Read;
};

// Compile() takes a a reading object and starts the compilation process.
// Using the $Include command, it can recursively compile multiple files 
void Compiler::Compile(Reading SourceFile, bool IsIncFile) { 
    // Store the source code into Read
    Read = SourceFile;
    Read.GetNextWord();

    // Skip new lines
    while (Read.WordType() == Read.EndOfLine) {
        Read.GetNextWord();
    }

    // If its not an $Include file, start the program
    if (!IsIncFile) {
        PrepareProgram();
    }

    // Reads each line and pass control off (main compilation body)
    while (Read.WordType() != Read.None) {
        // If we have an Identifier, then we have a Statement
        if (Read.WordType() == Read.Identifier) {
                // Check if the user tried to declare an invalid variable
                if (!Statement()) {
                    NotDIM();
                    Assignment();
                }
        }
        // If we have a symbol as the first word of the line, then we have
        // a directive.
        else if (Read.WordType() == Read.Symbol) {
            if (Read.Word() == "$") {
                Read.GetNextWord();
                Directives(Read.Word());
            }
        }
        else if (Read.WordType() == Read.EndOfLine) {
            // Do nothing at end of line
        }
        else {
            // Display error message regarding invalid user input
            Error.BadStatement(Read);
        }
        // Check for a newline or the end of the file after each command
        if (Read.WordType() != Read.EndOfLine and Read.WordType() != Read.None) {
            Error.EndOfLine(Read);
        }
        Read.GetNextWord();
    }
    // Check to see if we missed a something ($End If, etc.)
    if (Data.CanExitDirective()) {
        Error.NoEndToIfDef(Read);
    }
    return ;
}




// Statement() checks all commands when passed an identifier.
// If none of the commands match, then we may have an undeclared variable..
bool Compiler::Statement() {
    // The user is creating a variable
    if (Read.Word() == "DIM") {
        DIM();
        return true;
    }
    // The user is defining a type
    if (Read.Word() == "TYPE") {
        TYPE();
        return true;
    }
    // The user is defining a function
    if (Read.Word() == "SUB" or Read.Word() == "FUNCTION") {
        SubFunction();
        return true;
    }
    // The user is declaring an external function
    if (Read.Word() == "DECLARE") {
        DeclareSubFunction();
        return true;
    }
    // The user is using a built-in console command
    if (DealWithConsoleKeywords()) {
        return true;
    }
    // The user is using a control statement
    if (DealWithControlStatements()) {
        return true;
    }

    // If the word is already reserved, and not a keyword, then we are 
    // defining a variable or calling a function
    if (Data.IsAlreadyReserved(Read.Word())) {
        if (Data.IsKeyword(Read.Word())) {
            return false;
        }
        if (Data.GetDataType(Read.Word()) == Data.SubFunction) {
            CallSubFunction(Read.Word());
            Read.GetNextWord();
            return true;
        }
        Assignment();
        return true;
    }
    return false;
}


// DIM - explicitly creating a variable

/* Examples:
1. Creating variable(s) A, B, and C.:
    DIM A As Integer, B As Double, C As String

2. Creating an array(s) A, B, and C:
    DIM A(1) As Integer, B(5+1) As Double, C(Num*Size) As String

3. Creating a User-Defined Type:
    DIM A As MyNewDataType
*/
void Compiler::DIM() {
    std::string Name;
    // Initially assume the declaration is not an array
    bool IsArray = false;

    // Get the first DIM statement, loop back if there are more than one.
    do {
        Read.GetNextWord();
        Name = Read.Word();
        // Check to see if the variable name is legal
        if (Read.WordType() != Read.Identifier) {
            if (Read.WordType() == Read.None or Read.WordType() == Read.EndOfLine) {
                Error.ExpectedNameAfterDIM(Read);
            }
            Error.BadName(Read);
        }

        // Check to see if the variable already exists in the scope
        if (Data.AlreadyExistsInScope(Name)) {
            Error.AlreadyReserved(Read);
        }

        Read.GetNextWord();

        // Check for array declaration
        if (Read.Word() == "(") {
            IsArray = true;
            // Get the array size so we allocate the correct amount of memory.
            GetArraySize();
            Read.GetNextWord();
        }

        // Check for invalid syntax
        if (Read.Word() != "AS") {
            Error.ExpectedAs(Read);
        }

        Read.GetNextWord();

        // Create the variable or array
        if (!IsArray) {
            DIMCreate(Name);
        }
        else {
            DIMCreateArray(Name);
        }
        Read.GetNextWord();
    // Multiple variable declarations should be separated by a comma
    } while (Read.Word() == ",");
    return ;
}


// NotDIM() if we are using an undeclared variable like so: PI# = 3.14, then
// create the variable and continue with the assignment.
void Compiler::NotDIM() {
    std::string Name = Read.Word();
    char LastChar;
    
    // If the variable has an invalid declaration, report an error.
    if (Read.WordType() != Read.Identifier) {
        Error.BadName(Read);
    }

    // If the variable already exists in the scope, report an error and return.
    if (Data.AlreadyExistsInScope(Name)) {
        Error.AlreadyReserved(Read);
        return ;
    }

    // Get the last character
    LastChar = Name[Name.length() - 1];

    // Validate last character as: Integer& or Double# or String$
    if (LastChar == '&' or LastChar == '#' or LastChar == '$') {
        // If the last character is a &, then create an integer
        if (LastChar == '&') {
            if (Mangle) {
                Asm.CreateInteger(Name, Data.StripJunkOff(Name) + "_Integer");
            }
            else {
                Asm.CreateInteger(Name, Data.StripJunkOff(Name));            
            }
        }

        // If the last character is a #, then create a double
        if (LastChar == '#') {
            if (Mangle) {
                Asm.CreateDouble(Name, Data.StripJunkOff(Name) + "_Double");
            }
            else {
                Asm.CreateDouble(Name, Data.StripJunkOff(Name));
            }
        }

        // If the last character is a $, then create a string
        if (LastChar == '$') {
            if (Mangle) {
                Asm.CreateString(Name, Data.StripJunkOff(Name) + "_String");
            }
            else {
                Asm.CreateString(Name, Data.StripJunkOff(Name));            
            }
        }
    }
    else {
        // If the variable has no existing type, report an error.
        Error.NoType(Read);
    }
    return ;
}


// DIMCreate() creates an Integer, Double, String, or User-Defined Type.
/* Example:
  DIM A As String
*/
void Compiler::DIMCreate(std::string Name) {

    // Create a new integer
    if (Read.Word() == "INTEGER") {
        if (Mangle) {
            Asm.CreateInteger(Name, Data.StripJunkOff(Name) + "_Integer");
        }
        else {
            Asm.CreateInteger(Name, Data.StripJunkOff(Name));
        }
        return ;
    }

    // Create a new double
    if (Read.Word() == "DOUBLE") {
        if (Mangle) {
            Asm.CreateDouble(Name, Data.StripJunkOff(Name) + "_Double");
        }
        else {
            Asm.CreateDouble(Name, Data.StripJunkOff(Name));
        }
        return ;
    }

    // Create a new string
    if (Read.Word() == "STRING") {
        if (Mangle) {
            Asm.CreateString(Name, Data.StripJunkOff(Name) + "_String");
        }
        else {
            Asm.CreateString(Name, Data.StripJunkOff(Name));
        }
        return ;
    }

    // Create a new User-Defined Type
    if (Data.IsType(Read.Word())) {
        if (Mangle) {
            Asm.CreateUDT(Read.Word(), Name, Data.StripJunkOff(Name) + "_UDT");
        }
        else {
            Asm.CreateUDT(Read.Word(), Name, Data.StripJunkOff(Name));
        }
        return ;
    }

    // Could not determine what type of variable to create, report an error.
    Error.BadType(Read);
    return ;
}


// TYPE() parses the definition of a type and adds it to the .data section
void Compiler::TYPE() {
    std::string TypeName;
    std::string Name;
    Read.GetNextWord();

    // Report an error if the identifier is illegal.
    if (Read.WordType() != Read.Identifier) {
        if (Read.WordType() == Read.None or
            Read.WordType() == Read.EndOfLine) {
            Error.ExpectedNameAfterTYPE(Read);
        }
        Error.BadName(Read);
    }
    TypeName = Read.Word();

    // Report an error if it is already in the scope
    if (Data.AlreadyExistsInScope(TypeName)) {
        Error.AlreadyReserved(Read);
    }

    // Start creating the type
    Asm.StartCreatingType(TypeName);
    Read.GetNextWord();
    if (Read.WordType() == Read.None) {
        Error.UnfinishedType(TypeName, Read);
    }
    if (Read.WordType() != Read.EndOfLine) {
        Error.EndOfLine(Read);
    }

    // Add the members of the type to the definition
    do {
        // Get the name of the next member
        Read.GetNextWord();
        Name = Read.Word();

        // Validate the name of the next member
        if (Read.WordType() != Read.Identifier) {
            if (Read.WordType() == Read.None) {
                Error.UnfinishedType(TypeName, Read);
            }
            if (Read.WordType() == Read.EndOfLine) {
                continue;
            }
            Error.BadName(Read);
        }

        // If the name is being used, report an error.
        if (Data.IsAlreadyReservedInType(TypeName, Name)) {
            Error.AlreadyReservedInType(Read);
        }
        Read.GetNextWord();

        /* Break loop condition */
        // We are done if we find "END TYPE"
        if (Name == "END" and Read.Word() == "TYPE") { break; }

        // Check syntax: <Name> As <Type>
        if (Read.Word() != "AS") {
            Error.ExpectedAs(Read);
        }
        Read.GetNextWord();

        // Call TYPECreate() to create the new member.
        TYPECreate(TypeName, Name);

        // If we reach EOF before the definition is complete, report an error.
        if (Read.WordType() == Read.None) {
            Error.UnfinishedType(TypeName, Read);
        }

        // If there is still other stuff on the line after the declaration
        // has ended, report an error.
        if (Read.WordType() != Read.EndOfLine) {
            Error.EndOfLine(Read);
        }
    } while (true); // Loop until we encounter an END TYPE or an error

    // Finish creating the type
    Asm.FinishCreatingType(TypeName);
    Read.GetNextWord();
    return ;
}


// TYPECreate() verifies that we have a legal variable declaration. If it is
// valid, then adds it to the type
void Compiler::TYPECreate(std::string TypeName, std::string Name) {
    if (Read.Word() == "INTEGER" or
        Read.Word() ==  "DOUBLE" or
        Read.Word() ==  "STRING") {

        // Create integer type
        if (Read.Word() == "INTEGER") {
            if (Mangle) {
                Asm.CreateTypeInteger(TypeName, Name, Data.StripJunkOff(Name) +
                    "_Integer");
            }
            else {
                Asm.CreateTypeInteger(TypeName, Name, Data.StripJunkOff(Name));
            }
        }

        // Create double type
        if (Read.Word() == "DOUBLE") {
            if (Mangle) {
                Asm.CreateTypeDouble(TypeName, Name, Data.StripJunkOff(Name) +
                    "_Double");
            }
            else {
                Asm.CreateTypeDouble(TypeName, Name, Data.StripJunkOff(Name));
            }
        }

        // Create string type
        if (Read.Word() == "STRING") {
            if (Mangle) {
                Asm.CreateTypeString(TypeName, Name, Data.StripJunkOff(Name) +
                    "_String");
            }
            else {
                Asm.CreateTypeString(TypeName, Name, Data.StripJunkOff(Name));
            }
        }
        Read.GetNextWord();
    }
    else {
        // We couldn't determine the type, report an error.
        Error.BadType(Read);
    }
    return ;
}


// GetArraySize() gets the size of the array the user declared.
void Compiler::GetArraySize() {
    Expression(Data.Number);

    // We need an ending parenthesis for valid syntax
    if (Read.Word() != ")") {
        Error.ExpectedEndingParenthesis(Read);
    }
    return ;
}


// DIMCreateArray - creates an array of integer, doubles, or std::strings
/* Example:
  DIM A(100*Num) As Integer
Note: This does not support nested arrays or arrays of User-Defined Types                              
*/
void Compiler::DIMCreateArray(std::string Name) {

    // Create an array of integers
    if (Read.Word() == "INTEGER") {
        if (Mangle) {
            Asm.CreateIntegerArray(Name, Data.StripJunkOff(Name) + "_Integer");
        }
        else {
            Asm.CreateIntegerArray(Name, Data.StripJunkOff(Name));
        }
        return ;
    }

    // Create an array of doubles
    if (Read.Word() == "DOUBLE") {
        if (Mangle) {
            Asm.CreateDoubleArray(Name, Data.StripJunkOff(Name) + "_Double");
        }
        else {
            Asm.CreateDoubleArray(Name, Data.StripJunkOff(Name));
        }
        return ;
    }

    // TODO:
    // IS THIS A BUG???
    // Create an array of strings
    if (Read.Word() == "STRING") {
        if (Mangle) {
            Asm.CreateStringArray(Name, Data.StripJunkOff(Name) + "_String");
        }
        else {
            Asm.CreateStringArray(Name, Data.StripJunkOff(Name) + "_String");
        }
        return ;
    }
    // We couldn't determine the type, report an error.
    Error.BadType(Read);
    return ;
}



// Assignment() assigns variable and deals with it.
// This could get messy (see Examples)
/*
Examples:
PI# = 3.14
ABC = "Hello" + MyStr$ + "to you"
Google = 100^4 / 400 * (MyInt&-1 + (A - C^2)) + 1
*/
void Compiler::Assignment() {
    std::string Name = Read.Word();

    // Const data is read-only, we cannot modify it
    if (Read.IsConstData(Name)) {
        Error.CannotModifyConstData(Read);
    }
    Read.GetNextWord();

    // If there is a dot after Name, then it's a member of a User-Defined Type.
    if (Read.Word() == ".") {

        // Get which Member to assign
        std::string Member = AssignUDTMember(Name);
        Read.GetNextWord();

        // Check for an equals sign, which is the assigment operator.
        if (Read.Word() != "=") {
            Error.ExpectedAssignment(Read);
        }

        // Check if the expression this is a string or number.
        if (Data.GetUDTData(Name).Type.Members[Member].Type == Data.String) {
            Expression(Data.String);
        }
        else {
            Expression(Data.Number);
        }

        // The expression is parsed and the result is on the stack, assign it.
        Asm.AssignUDTMember(Name, Data.GetUDTData(Name).TypeName, Member,
            Data.GetUDTData(Name).Type.Members[Member].Type);
        return ;
    }

    // If we have parenthesis, then it is an array assignment to an item.
    if (Read.Word() == "(") {
        // Get the array item to assign
        AssignArrayItem(Name);
        Read.GetNextWord();

        // Check for an equals sign, which is the assigment operator.
        if (Read.Word() != "=") {
            Error.ExpectedAssignment(Read);
        }

        // Determine whether to parse a number or string.
        if (Data.GetArrayData(Name).Type == Data.String) {
            Expression(Data.String);
        }
        else {
            Expression(Data.Number);
        }

        // Assign the item to the array.
        Asm.AssignArrayItem(Name, Data.GetArrayData(Name).Type);
        return ;
    }

    // If it was not an array or UDT, we need to check the literal data types.
    // Check for an equals sign, which is the assigment operator.
    if (Read.Word() != "=") {
        Error.ExpectedAssignment(Read);
    }

    // If the variable is a number, then assign it a number expression.
    if (Data.GetDataType(Name) == Data.Number) {
        Expression(Data.Number);
        Asm.AssignIt(Name);
        return ;
    }
    // If the variable is a string, then assign it a string expression.
    if (Data.GetDataType(Name) == Data.String) {
        Expression(Data.String);
        Asm.AssignIt(Name);
        return ;
    }

    // If the variable is an array, then assign it an array expression.
    // Note, this will result in an error: Array1 = Array2 + Array3 
    if (Data.GetDataType(Name) == Data.Array) {
        Expression(Data.Array, Name);
        Asm.AssignIt(Name);
        Read.GetNextWord();
        if (Read.WordType() != Read.EndOfLine) {
            Error.CanOnlyAssign(Read);
        }
        return ;
    }

    // If the variable is a User-Defined Type, then assign it an User-Defined
    // Type expression.
    if (Data.GetDataType(Name) == Data.UDT) {
        Expression(Data.UDT, Name);
        Asm.AssignIt(Name);
        Read.GetNextWord();
        if (Read.WordType() != Read.EndOfLine) {
            Error.CanOnlyAssign(Read);
        }
        return ;
    }
    return ;
}


// Expression() parses expressions then passes information to a corresponding
// function.
// Example:
//   A# = 3.14 * (4 + MyNum#^2)
//        ^^^^^^^^^^^^^^^^^^^^^   Numeric expression
//   B$ = "Hello " + Name
//        ^^^^^^^^^^^^^^^         String expression
//   Array1 = Array2
//            ^^^^^^              Copy arrays/User-Defined Types
//   Print A# + 3 * MyNum#
//         ^^^^^^^^^^^^^^^        Undefined expression
// Note: numeric expressions are calculated as doubles, no matter what. No integer
// arithmetic is performed, even on integers.
int Compiler::Expression(int Type, std::string ExtraInfo) {
    // Numeric expression
    if (Type == Data.Number) {
        // Get the first number
        Core(Data.Number);

        // Start at the lowest operator precedence
        AddSubtract();
        return Type;
    }
    // String expression
    if (Type == Data.String) {
        // Get the first string
        Core(Data.String);

        // Concatenate (add) strings
        StringAdd();
        return Type;
    }

    // Copy arrays
    if (Type == Data.Array) {
        AssignArray(ExtraInfo);
        return Type;
    }
    // Copy UDT's
    if (Type == Data.UDT) {
        AssignUDT(ExtraInfo);
        return Type;
    }

    // If we have an unknown expression, we need to search for clues.
    if (Type == Data.Unknown) {
        Type = Core(Data.Unknown);
        // Attempt to parse a numeric expression
        if (Type == Data.Number) {
            AddSubtract();
            return Data.Number;
        }
        // Attempt to parse a string expression
        if (Type == Data.String) {
            StringAdd();
            return Data.String;
        }
    }
    // Unable to parse expression 
    Error.UnknownExpressionType(Read);
    return -1;
}


// AddSubtract() handles addition and subraction of numeric types.
// Examples:
//   A# = 2.14 + 1
//   A# = 5.14 - 2
void Compiler::AddSubtract() {
    // Store the operator
    std::string Operator;
    Read.GetNextWord();

    // Handle higher precedence, if needed
    MultiplyDivideMod();

    // After completing higher precedence operations, check for
    // addition/subraction.
    while (Read.Word() == "+" or Read.Word() == "-") {
        Operator = Read.Word(); // Save the current word
        Core(Data.Number); // Get the second expression
        Read.GetNextWord(); // Get the next word
        MultiplyDivideMod(); // Handle higher precedence, if needed

        // Perform the operation
        if (Operator == "+") {
            Asm.CalculateAddition();
        }
        if (Operator == "-") {
            Asm.CalculateSubtract();
        }
    }
    return ;
}


// MultiplyDivideMod - handles multiplication, division (floating point), and
// modulus operations on numeric types.
// Examples:
//   A# = 2 * 2
//   A# = 4 / 2
//   A# = 6 MOD 4 --> 2
void Compiler::MultiplyDivideMod() {
    // Store operator, because we might have to do higher operations first
    std::string Operator;

    // Handle higher precedence, if needed (exponents)
    Exp();

    // After completing higher precedence operations, check for, check for
    // *, /, and MOD operators.
    while (Read.Word() == "*" or Read.Word() == "/" or Read.Word() == "MOD") {
        Operator = Read.Word(); // Save the current word
        Core(Data.Number); // Get the second expression
        Read.GetNextWord(); // Get the second word
        Exp(); // Handle higher precedence, if needed (exponents)

        // Perform the operation
        if (Operator == "*") {
            Asm.CalculateMultiply();
        }
        if (Operator == "/") {
            Asm.CalculateDivide();
        }
        if (Operator == "MOD") {
            Asm.CalculateMOD();
        }
    }
    return ;
}


// Exp() deals with exponents - it is the hightest arithmetic operation
// Example:
// A# = 2 ^ 3
void Compiler::Exp() {
    // Currently the highest precedence operator, so execute it if we find it.
    while (Read.Word() == "^") {
        // Get the second expression
        Core(Data.Number);

        // Evaluate the exponent expression
        Asm.CalculateExp();

        // Get the next word
        Read.GetNextWord();
    }
    return ;
}


// Core() retrieves a single component of an expression (string, array or array
// item, UDT, UDT member, or unknown type.
// Examples:
//   A# = 2 + 2
//        ^---^---Gets numbers
//   B$ = "ABC" + "DEF"
//        ^^^^^---^^^^^---Gets std::strings
//   A# = (3 + 4)/Number#
//        ^^---^^-^^^^^^^---Gets numbers and parenthesis
int Compiler::Core(int Type, bool GetNextWord) {
    // Initially, assume the value to be nonnegative
    bool isNegative = false;

    // Allow for boolean expressions
    if (GetNextWord) {
        Read.GetNextWord();
    }

    // Check to see if we have a negative number
    if (Read.Word() == "-" and Read.WordType() != Read.String) {
        isNegative = true;
        Read.GetNextWord();
    }

    // Parenthesis expression
    if (Read.Word() == "(") {
        Type = Expression(Type);
        // Make sure we can handle things like: A# = 2 * -(1+1)
        if (Type == Data.Number and isNegative) {
            Asm.Negate();
        }

        // If there is no ending parenthesis, then this statement is invalid.
        if (Read.Word() != ")") {
            Error.UnmatchedParenthesis(Read);
        }
        return Type;
    }

    // If we have no way of knowing the expression's type, (ex: Print "Hello")
    if (Type == Data.Unknown) {
        // Determine the type based on the beginning of the expression
        if (Read.WordType() == Read.String) {
            Type = Data.String;
        }
        if (Read.WordType() == Read.Number) {
            Type = Data.Number;
        }

        // If we have an identifier (like Print A$), then we need to check the
        // variable's type before proceeding.
        if (Read.WordType() == Read.Identifier) {
            // Case where the type is a string
            if (Data.GetDataType(Read.Word()) == Data.String) {
                Type = Data.String;
            }

            // Case where the type is a number
            if (Data.GetDataType(Read.Word()) == Data.Number) {
                Type = Data.Number;
                if (isNegative) {
                    Asm.Negate();
                }
            }

            // Case where the type is a array
            if (Data.GetDataType(Read.Word()) == Data.Array) {
                Type = Data.GetArrayData(Read.Word()).Type;
                LoadArray(Read.Word(), Data.Unknown);
                if (Type == Data.Double or Type == Data.Integer) {
                    Type = Data.Number;
                }
                if (isNegative) {
                    Asm.Negate();
                }
                return Type;
            }

            // Case where the type is a user-defined type
            if (Data.GetDataType(Read.Word()) == Data.UDT) {
                Type = LoadUDT(Read.Word(), Data.Unknown);
                if (Type == Data.Double or Type == Data.Integer) {
                    Type = Data.Number;
                }
                if (isNegative) {
                    Asm.Negate();
                }
                return Type;
            }

            // If the expression is a function, then call it and check its
            // return value to determine its type.
            if (Data.GetDataType(Read.Word()) == Data.SubFunction) {
                std::string ReturnValueType = Data.GetSubFunctionInfo(Read.Word()).ReturnValue.Type;
                if (ReturnValueType == "INTEGER" or ReturnValueType == "DOUBLE") {
                    Type = Data.Number;
                }
                if (ReturnValueType == "STRING") {
                    Type = Data.String;
                }
                CallSubFunction(Read.Word(), true);
                return Type;
            }
        }
        // Check built-in data types
        if (Read.Word() == "CODEPTR" or Read.Word() == "CALLBACK" or
            Read.Word() == "SIZEOF" or Read.Word() == "ADDRESSOF") {
            Type = Data.Number;
        }
    }

    // Case for string expressions
    if (Type == Data.String) {
        // If the string is constant, then return it.
        if (Read.WordType() == Read.String) {
            Asm.LoadString(Read.Word());
            return Data.String;
        }

        // If we have a valid identifier, then return the string.
        if (Read.WordType() == Read.Identifier) {
            if (!Data.IsAlreadyReserved(Read.Word())) {
                Error.UndeclaredVariable(Read);
            }
            if (Data.GetDataType(Read.Word()) == Data.String) {
                Asm.LoadString(Read.Word());
                return Data.String;
            }
            if (Data.GetDataType(Read.Word()) == Data.UDT) {
                LoadUDT(Read.Word(), Data.String);
                return Data.String;
            }
            if (Data.GetDataType(Read.Word()) == Data.Array) {
                LoadArray(Read.Word(), Data.String);
                return Data.String;
            }
        }

        // If we have a function, then call it.
        if (Data.GetDataType(Read.Word()) == Data.SubFunction) {
            std::string ReturnValueType =
                Data.GetSubFunctionInfo(Read.Word()).ReturnValue.Type;
            if (ReturnValueType != "STRING") {
                Error.ExpectedStringData(Read);
            }
            Type = Data.String;
            CallSubFunction(Read.Word(), true);
            return Type;
        }
        // We expected a string, but didn't find one.
        Error.ExpectedStringData(Read);
    }

    // Case for number expressions
    if (Type == Data.Number) {
        // If we have a plain number, then return it.
        if (Read.WordType() == Read.Number) {
            Asm.LoadNumber(Read.Word());
            if (isNegative) {
                Asm.Negate();
            }
            return Data.Number;
        }
        // If we have a valid identifier, assign the number to the variable.
        if (Read.WordType() == Read.Identifier) {
            if (!Data.IsAlreadyReserved(Read.Word())) {
                Error.UndeclaredVariable(Read);
            }
            if (Data.GetDataType(Read.Word()) == Data.Number) {
                Asm.LoadNumber(Read.Word());
                if (isNegative) {
                    Asm.Negate();
                }
                return Data.Number;
            }
            if (Data.GetDataType(Read.Word()) == Data.UDT) {
                LoadUDT(Read.Word(), Data.Number);
                if (isNegative) {
                    Asm.Negate();
                }
                return Data.Number;
            }
            if (Data.GetDataType(Read.Word()) == Data.Array) {
                LoadArray(Read.Word(), Data.Number);
                if (isNegative) {
                    Asm.Negate();
                }
                return Data.Number;
            }
            // SizeOf() built-in function - gets the amount of memory to store
            // a variable.
            if (Read.Word() == "SIZEOF") {
                int DataType = 0;
                Read.GetNextWord();
                // SizeOf() is a function, check for parenthesis
                if (Read.Word() != "(") {
                    Error.ExpectedParameters(Read);
                }
                Read.GetNextWord();

                // Unable to get the size of if it is not a variable
                if (Data.IsAlreadyReserved(Read.Word()) != true or
                    Data.IsKeyword(Read.Word())) {
                    Error.ExpectedVariable(Read);
                }
                DataType = Data.GetDataType(Read.Word());

                // Check to see if we have a valid data type
                if (DataType == Data.Number or DataType == Data.String or
                    DataType == Data.UDT or DataType == Data.Type) {
                    if (DataType == Data.Number) {
                        if (Data.GetSimpleData(Read.Word()).Type == Data.Integer) {
                            // Integers are 4 bytes
                            Asm.PushNumber(4);
                        }
                        else {
                            // Doubles are 8 bytes
                            Asm.PushNumber(8);
                        }
                    }
                    if (DataType == Data.String) {
                        // Strings are four bytes. Call Len to get the length 
                        Asm.PushNumber(4);
                    }

                    // UDT's can vary in size
                    if (DataType == Data.UDT) {
                        int Size = Data.GetUDTData(Read.Word()).Type.Size;
                        Asm.PushNumber(Size);
                    }
                    // TYPE's can vary in size
                    if (DataType == Data.Type) {
                        int Size = Data.GetTypeData(Read.Word()).Size;
                        Asm.PushNumber(Size);
                    }
                }
                else {
                    // Bad type was passed, report an error.
                    Error.BadType(Read);
                }
                Read.GetNextWord();

                // Check for closing parenthesis
                if (Read.Word() != ")") {
                    Error.ExpectedEndingParenthesis(Read);
                }
                return Data.Number;
            }

            // AddressOf() returns the address where the variable is stored
            // in memory.
            if (Read.Word() == "ADDRESSOF") {
                int DataType = 0;
                Read.GetNextWord();
                // It is a function, check for parenthesis
                if (Read.Word() != "(") {
                    Error.ExpectedParameters(Read);
                }
                Read.GetNextWord();

                // Check the address of a variable
                if (Data.IsAlreadyReserved(Read.Word()) != true or Data.IsKeyword(Read.Word())) {
                    Error.ExpectedVariable(Read);
                }
                DataType = Data.GetDataType(Read.Word());

                // Check to ensure it is a valid type
                if (DataType == Data.Number or DataType == Data.String or DataType == Data.UDT) {
                    Asm.PushAddress(Read.Word(), DataType);
                }
                else if (DataType == Data.SubFunction) {
                    // Unable to obtain the address of a non-function variable
                    if (Data.GetDataType(Read.Word()) != Data.SubFunction) {
                        Error.ExpectedSubFunction(Read);
                    }
                    // If optimization is enabled, mark this function as used.
                    if (Optimize) {
                        Data.UsingSubFunction(Read.Word());
                    }
                    Asm.Callback(Read.Word());
                }
                else {
                    Error.BadType(Read);
                }
                Read.GetNextWord();

                // Check for closing parenthesis
                if (Read.Word() != ")") {
                    Error.ExpectedEndingParenthesis(Read);
                }
                return Data.Number;
            }

            // Built-in functions to return the address of a function
            if (Read.Word() == "CALLBACK" or Read.Word() == "CODEPTR") {
                Read.GetNextWord();

                // CallBack()/CodePtr() is a function, check for parenthesis
                if (Read.Word() != "(") {
                    Error.ExpectedParameters(Read);
                }
                Read.GetNextWord();

                // Unable to obtain the address of a non-function variable
                if (Data.GetDataType(Read.Word()) != Data.SubFunction) {
                    Error.ExpectedSubFunction(Read);
                }

                // If optimization is enabled, mark this function as used.
                if (Optimize) {
                    Data.UsingSubFunction(Read.Word());
                }
                Asm.Callback(Read.Word());
                Read.GetNextWord();

                // Check for closing parenthesis
                if (Read.Word() != ")") {
                    Error.ExpectedEndingParenthesis(Read);
                }
                return Data.Number;
            }
        }

        // If we have a function, call the function and check its return value.
        if (Data.GetDataType(Read.Word()) == Data.SubFunction) {
            // Check the return value to see if it is a number
            std::string ReturnValueType = Data.GetSubFunctionInfo(Read.Word()).ReturnValue.Type;
            if (ReturnValueType != "INTEGER" and ReturnValueType != "DOUBLE") {
                Error.ExpectedNumberData(Read);
            }
            Type = Data.Number;
            CallSubFunction(Read.Word(), true);
            return Type;
        }
        // Error, this isn't a numeric expression
        Error.ExpectedNumberData(Read);
    }
    // Something bad happened, most likely an invalid expression
    Error.UnknownExpression(Read);
    return -1;
}











#endif // FILENAME_HPP