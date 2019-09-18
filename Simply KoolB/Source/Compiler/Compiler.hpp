#ifndef COMPILER_HPP
#define COMPILER_HPP

// #include <string>

class Compiler {
public:
    void Compile(Reading SourceFile, bool IsIncFile);
    bool Statement();
    void DIM();
    void NotDIM();

    /* Initializations */
    // Variables
    void DIMCreate(std::string Name);

    // Types
    void TYPE();
    void TYPECreate(std::string TypeName, std::string Name);

    // Array
    void GetArraySize();
    void DIMCreateArray(std::string Name);
    void Assignment();

    // Expressions
    int Expression(int Type, std::string ExtraInfo = "");

    /* Binary Operators */
    void AddSubtract();
    void MultiplyDivideMod();
    void Exp(); // Exponents

    /* Core processing function */
    int Core(int Type, bool GetNextWord = true);

    /* Loading and Assigments */
    void StringAdd();
    // Array
    void AssignArray(std::string Name);
    void AssignArrayItem(std::string Name);
    void LoadArray(std::string Array, int Type);
    // UDT
    void AssignUDT(std::string Name);
    std::string AssignUDTMember(std::string Name);
    int LoadUDT(std::string UDT, int Type);

    /* Logical operators and comparison/relation statements  */
    void Condition();

    // Logical Operators
    void Or();
    void And();
    void Not();

    // Relations
    int Relation(bool GetNextWord = true);
    bool IsRelation(std::string Word);

    // Comparison
    void CompareNumbers(std::string Relation);
    void CompareStrings(std::string Relation);

    // Conditional
    void If();

    /* Loop */
    // While loop
    void While(std::string StartWhileLabel);

    bool DealWithControlStatements(std::string LoopCommand = "");
    bool DealWithConsoleKeywords();
    void AssignInput();

    void SubFunction(bool External = false);
    void InsideSubFunction(std::string Name);
    void DeclareSubFunction();
    void CallSubFunction(std::string Name, bool ReturnValue = false);
    void SubFunctionLibAliasCall(std::string Name);

    void GetParameters(std::string Name); // done
    void PassParameters(std::string Name); // done
    void ByRefExpression(int Type);
    
    void PrepareParameters(std::string Name);
    void UnprepareParameters(std::string Name);

    void ChooseAppType();

    void OptimizeApp();

    bool Directives(std::string Directive);
    bool FirstDirectives(std::string Directive);
    void IncludeFile();

    void DropDownToAsm();
    void CompressApp();

    void IfDef();
    void IfNDef();

    void Const();

    void MangleNames();
    void PrepareProgram();

private:
    // Read contains the BASIC source code to compile
    Reading Read;
};

// Compile() takes a a reading object and starts the compilation process.
// Using the $Include command, it can recursively compile multiple files 
void Compiler::Compile(Reading SourceFile, bool IsIncFile) { 
    std::cout << "XXXX Entered Compiler::Compile()" << std::endl;
    // Store the source code into Read
    Read = SourceFile;
    Read.GetNextWord();

    std::cout << "XXXX Obtained SourceFile and GetNextWord() " << std::endl;
    // Skip new lines
    while (Read.WordType() == Read.EndOfLine) {
        Read.GetNextWord();
    }

    std::cout << "XXXX Skipped over whitespace " << std::endl;

    // If its not an $Include file, start the program
    if (!IsIncFile) {
        std::cout << "XXXX It is not an include file, call PrepareProgram() " << std::endl;
        PrepareProgram();
        std::cout << "XXXX Finished call to PrepareProgram() " << std::endl;
    }

    std::cout << "XXXX Reading each line " << std::endl;
    // Reads each line and pass control off (main compilation body)
    while (Read.WordType() != Read.None) {
        // If we have an Identifier, then we have a Statement
        if (Read.WordType() == Read.Identifier) {
            std::cout << "XXXX Found an Identifier" << std::endl;
                // Check if the user tried to declare an invalid variable
                if (!Statement()) {
                    std::cout << "XXXX Found Not a Statement" << std::endl;
                    NotDIM();
                    Assignment();
                }
                std::cout << "XXXX End check for Not a Statement" << std::endl;
        }
        // If we have a symbol as the first word of the line, then we have
        // a directive.
        else if (Read.WordType() == Read.Symbol) {
            std::cout << "XXXX Found a Symbol" << std::endl;
            if (Read.Word() == "$") {
                std::cout << "XXXX The symbol is a $" << std::endl;
                Read.GetNextWord();
                Directives(Read.Word());
            }
            std::cout << "XXXX Ended conditional for $" << std::endl;
        }
        else if (Read.WordType() == Read.EndOfLine) {
            std::cout << "XXXX Entered Do nothing at EOL $" << std::endl;
            // Do nothing at end of line
        }
        else {
            std::cout << "XXXX Bad Statement error" << std::endl;
            // Display error message regarding invalid user input
            Error.BadStatement(Read);
        }
    
        // Check for a newline or the end of the file after each command
        if (Read.WordType() != Read.EndOfLine and Read.WordType() != Read.None) {
            std::cout << "XXXX Not end of line or none" << std::endl;
            Error.EndOfLine(Read);
        }
        Read.GetNextWord();
    }
    // Check to see if we missed a something ($End If, etc.)
    if (Data.CanExitDirective()) {
        Error.NoEndToIfDef(Read);
    }
    std::cout << "XXXX Made it to the return statement in Compiler::Compile() " << std::endl;
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


// StringAdd() concatenates two strings together with a + or & operator
// Examples:
//   A$ = "Hello" + " how are " & "you!"
void Compiler::StringAdd() {
    Read.GetNextWord();
    while (Read.Word() == "+" or Read.Word() == "&") {
        // Get the second part of the expression
        Core(Data.String);
        Asm.AddStrings();
        Read.GetNextWord();
    }
    return ;
}


// AssignArray() copies one array to another
// Example:
//   Array1 = Array2
void Compiler::AssignArray(std::string Name) {
    Read.GetNextWord();
    // Can't assign a UDT to an array or anything!
    if (Data.GetDataType(Read.Word()) != Data.Array) {
        Error.ExpectedArray(Read);
    }
    // Make sure the arrays are of the same type
    if (Data.GetArrayData(Read.Word()).Type != Data.GetArrayData(Name).Type) {
        Error.TypesDoNotMatch(Read);
    }
    // Copy the array now
    Asm.CopyArray(Read.Word());
    return ;
}


// AssignArrayItem() loads the array item that we are assigning to the stack 
// Example:
//   Array(1+2*Num) = 2
void Compiler::AssignArrayItem(std::string Name) {
    // This operation is only valid for array data types
    if (Data.GetDataType(Name) != Data.Array) {
        Error.UnableToAccessItem(Read);
    }

    // Get the array item as an expression
    Expression(Data.Number);

    // Check for closing parenthesis
    if (Read.Word() != ")") {
        Error.ExpectedEndingParenthesis(Read);
    }
    return ;
}


// LoadArray() pushes an array item to the stack for computation
// Example:
//   A# = Numbers(1*Num)
//               ^^^^^^^---Loads the array item
void Compiler::LoadArray(std::string Array, int Type) {
    Read.GetNextWord();
    // Need array item, which is inside parenthesis
    if (Read.Word() != "(") {
        Error.ExpectedParenthesis(Read);
    }

    // Get the array item as an expression
    Expression(Data.Number);

    // Check for closing parenthesis
    if (Read.Word() != ")") {
        Error.ExpectedEndingParenthesis(Read);
    }

    // Load the array item into assembly
    Asm.LoadArrayItem(Array, Data.GetArrayData(Array).Type);
    return ;
}


// AssignUDT() copies one UDT to another
// Example:
//   UDT1 = UDT2
void Compiler::AssignUDT(std::string Name) {
    Read.GetNextWord();
    // This operation is only valid for UDT
    if (Data.IsType(Name)) {
        if (Data.GetUDTData(Read.Word()).TypeName != Name) {
            Error.TypesDoNotMatch(Read);
        }

        /// Copy the assigned UDT
        Asm.CopyUDT(Read.Word());
        Read.GetNextWord(); // Get the next word
        return ;
    }

    // Check to see if the final assignment resulted in a UDT
    if (Data.GetDataType(Read.Word()) != Data.UDT) {
        Error.ExpectedUDT(Read);
    }
    if (Data.GetUDTData(Read.Word()).TypeName != Data.GetUDTData(Name).TypeName) {
        Error.TypesDoNotMatch(Read);
    }

    // Copy the new UDT
    Asm.CopyUDT(Read.Word());
    return ;
}


// AssignUDTMember() assigns an expression to an UDT member
// Example:
//   MyUDT.Member = 2
std::string Compiler::AssignUDTMember(std::string Name) {
    std::string Member;
    // We need a UDT parent in order to assign it members
    if (Data.GetDataType(Name) != Data.UDT) {
        Error.UnableToAccessMember(Read);
    }

    // Get the next word and assign it to become the UDT's member
    Read.GetNextWord();
    Member = Read.Word();

    // If the member was not initialized in the UDT, then report an error
    if (!Data.IsAlreadyReservedInType(Data.GetUDTData(Name).TypeName, Member)) {
        Error.InvalidMember(Read);
    }
    return Member;
}


// LoadUDT() initialize a UDT member onto the stack
// Example:
//   A# = MyUDT.Number
int Compiler::LoadUDT(std::string UDT, int Type) {
    std::string Member;
    Read.GetNextWord();

    // Check for a dot identifier after the identifier
    if (Read.Word() != ".") {
        Error.ExpectedPeriod(Read);
    }

    Read.GetNextWord();
    Member = Read.Word();

    // Check if the member is already reserved in the UDT
    if (!Data.IsAlreadyReservedInType(Data.GetUDTData(UDT).TypeName, Member)) {
        Error.InvalidMember(Read);
    }

    // Set the type of the current type of the member
    if (Type == Data.Unknown) {
        Type = Data.GetUDTData(UDT).Type.Members[Member].Type;
    }

    // Load the member into assembly
    Asm.LoadUDTMember(UDT, Data.GetUDTData(UDT).TypeName, Member, Type);
    return Type;
}


// Condition() a very simple boolean expression parser. 
// Precendence (Highest to lowest): Not -> And -> OR
// Note: relations (=, <, >, <>, =>, and =<) have lower precedence than the
// logical operators.
// Example:
//   If 1 = 1 Or Not 3 <> 2 Then
void Compiler::Condition() {
    // This will check AND and NOT first. This simulates precedence
    Or();
}

// Or() returns 0 if both of the relationals are false, and -1 otherwise
// Example:
//   If 1=1 OR 2=2 Then
void Compiler::Or() {
    // Check for higher precedence operators before evaluating Or()
    And();

    // Evaluate the OR statement
    while (Read.Word() == "OR") {
        // Check for AND statment before evaluating another OR statment
        And();
        Asm.Or();
    }
    return ;
}

// And() returns 0 if either of the relational expressions is false, and -1
// otherwise
// Example:
//   If 1=1 AND 2<>2 Then
void Compiler::And() {
    // Check for higher precedence operators before evaluating And()
    Not();

    // Then loop while we have AND expressions
    while (Read.Word() == "AND") {
        // Check for NOT statment before evaluating another AND statment
        Not();
        Asm.And();
    }
    return ;
}

// Not() Returns 0 if the relation is true, and -1 otherwise
// Example:
//   If NOT 1=1 Then
void Compiler::Not() {
    // Check if the next word is NOT
    Read.GetNextWord();
    if (Read.Word() == "NOT") {
        // Call Relation() and then negate the results of that expression
        Relation();
        Asm.Not();
    }
    else {
        // We have the first part of an expression. We need to pass false to
        // Relation(), so that Core() will receive false as well, otherwise
        // our program will not have the first part of the next expression.
        // TODO: Implement something less hacky
        Relation(false);
    }
    return ;
}


// Relation() return 0 if the expression is false, and -1 if true
// Example:
//   If 1=1 Then
//      ^^^---Deal with an individual relational expression
int Compiler::Relation(bool GetNextWord) {
    std::string Relation;

    // Call the core routine to calculate the left hand side of the expression
    int Type = Core(Data.Unknown, GetNextWord);

    // Case where the relational expression is between two numeric types
    if (Type == Data.Number) {
        // Extract the operator
        AddSubtract();

        // If we find a relational operator, then get the other expression
        if (IsRelation(Read.Word())) {
            // Store which relational operator we have for future reference
            Relation = Read.Word();

            // Get the right hand side of the relational expression
            Core(Data.Number);
            AddSubtract();

            // Evaluate the relation
            CompareNumbers(Relation);
        }
        return Data.Number;
    }

    // Case where the relational expression is between two string types
    if (Type == Data.String) {
        // Extract the operator
        StringAdd();

        // OK, If we find a relational operator, then get the other expression
        if (IsRelation(Read.Word())) {
            // Store the relational operator for future reference
            Relation = Read.Word();

            // Get the right hand side of the relational expression
            Core(Data.String);
            StringAdd();

            // Evaluate the relation
            CompareStrings(Relation);
            return Data.Number;
        }

        // Convert it to an integer for comparison
        Asm.ConvertToNumber();
        return Data.String;
    }
}


// IsRelation() returns true if the current word is a relational operator, and
// false otherwise.
bool Compiler::IsRelation(std::string Word) {
    if (Word == "=" or Word == "<>" or Word == "<" or Word == ">" or
        Word == ">=" or Word == "<=") {
        return true;
    }
    return false;
}


// CompareNumbers() compares the results of two numeric expressions
void Compiler::CompareNumbers(std::string Relation) {
    // Evaluate what is necessary to make the comparison
    Asm.CompareNumbers();

    // Load the appropriate relational operator and its result into assembly
    if (Relation == "=") {
        Asm.LoadNumberRelation(Asm.Equal);
    }
    if (Relation == "<>") {
        Asm.LoadNumberRelation(Asm.NotEqual);
    }
    if (Relation == ">") {
        Asm.LoadNumberRelation(Asm.Greater);
    }
    if (Relation == "<") {
        Asm.LoadNumberRelation(Asm.Less);
    }
    if (Relation == ">=") {
        Asm.LoadNumberRelation(Asm.GreaterOrEqual);
    }
    if (Relation == "<=") {
        Asm.LoadNumberRelation(Asm.LessOrEqual);
    }
    return ;
}


// CompareStrings() compares two strings on the stack 
void Compiler::CompareStrings(std::string Relation) {
    // Load CompareStrings() into assembly
    Asm.CompareStrings();

    // Load true or false after evaluation the relational operator
    if (Relation == "=") {
        Asm.LoadStringRelation(Asm.Equal);
    }
    if (Relation == "<>") {
        Asm.LoadStringRelation(Asm.NotEqual);
    }
    if (Relation == ">") {
        Asm.LoadStringRelation(Asm.Greater);
    }
    if (Relation == "<") {
        Asm.LoadStringRelation(Asm.Less);
    }
    if (Relation == ">=") {
        Asm.LoadStringRelation(Asm.GreaterOrEqual);
    }
    if (Relation == "<=") {
        Asm.LoadStringRelation(Asm.LessOrEqual);
    }
    return ;
}


// If() evaluates the expression inside of an if statement
// <CodeBlock>
// End If
void Compiler::If() {
    // Store the initial condition and how to verify it
    std::string NextEnd = Asm.StartIf();
    std::string DoneIf = Asm.GetLabel();

    // Keep looping until we find a break condition
    while (1) {
        // Functions are invalid data types to evaluate inside an if statement
        if (Read.Word() == "SUB" or Read.Word() == "FUNCTION") {
            Error.SubFunctionMustBeGlobal(Read);
        }
        // If there is no valid statement, we need to explore
        if (!Statement()) {
            if (Read.Word() == "END") {
                Read.GetNextWord();

                // Invalid data types to evaluate inside an If statement
                if (Read.Word() == "SUB" or Read.Word() == "FUNCTION") {
                    Error.MustEndIfStatementFirst(Read);
                }

                // END IF is a break condition for the If statement
                if (Read.Word() == "IF") {
                    Read.GetNextWord();
                    break;
                }

                // No matter what type of end we have, nothing else must be on the line
                if (Read.WordType() != Read.EndOfLine and Read.WordType() != Read.None) {
                    Error.EndOfLine(Read);
                }

                // A single "End" was found, exit the program
                Asm.EndProgram();
                Read.GetNextWord();
                continue;
            }

            // If we have an ElseIf, we can handle that
            if (Read.Word() == "ELSEIF") {
                // Start a new section of the If code
                Asm.StartNewIfSection(NextEnd, DoneIf);

                // Get the condition
                Condition();

                // Make sure we have THEN followec by a new line
                if (Read.Word() != "THEN") {
                    Error.ExpectedThen(Read);
                }
                Read.GetNextWord();
                if (Read.WordType() != Read.EndOfLine) {
                    Error.EndOfLine(Read);
                }

                // Now, let's continue with the new part of the IF statement
                NextEnd = Asm.ElseIf(NextEnd, DoneIf);
                Read.GetNextWord();
                continue;
            }
            // We can handle ELSE too!
            if (Read.Word() == "ELSE") {
                // Make sure we have a new line
                Read.GetNextWord();
                if (Read.WordType() != Read.EndOfLine) {
                    Error.EndOfLine(Read);
                }
                // Prep for else statement, and then continue
                NextEnd = Asm.Else(NextEnd, DoneIf);
                Read.GetNextWord();
                continue;
            }
            // We found an undeclared variable
            if (Read.WordType() != Read.EndOfLine and Read.WordType() != Read.None) {
                NotDIM();
                Assignment();
                continue;
            }
        }
        // Check for an end of line
        if (Read.WordType() != Read.EndOfLine) {
            // If we encounter an end of the file before an End If, report an
            // error
            if (Read.WordType() == Read.None) {
                Error.NoEndIf(Read);
            }
            Error.EndOfLine(Read);
        }
        Read.GetNextWord();
    }

    // End the If statement
    Asm.EndIf(NextEnd, DoneIf);
    return ;
}


// While() handles the while loop execution
// Example:
//   <Block>
//   WEnd       <--- Ends the while statment
void Compiler::While(std::string StartWhile) {
    // Store EndWhile to pass to StopWhile after we finish looping
    std::string EndWhile = Asm.StartWhile();

    // Loop until we find a break condition (WEnd or error)
    while (1) {
        // Invalid types inside while statements
        if (Read.Word() == "SUB" or Read.Word() == "FUNCTION") {
            Error.SubFunctionMustBeGlobal(Read);
        }

        // Check for valid commands
        if (DealWithControlStatements() or
            Statement() or
            Read.WordType() == Read.EndOfLine or
            Read.WordType() == Read.None) {
            
            // Check for a new line after the command
            if (Read.WordType() != Read.EndOfLine) {
                // If the file ends before the word, we have an error
                if (Read.WordType() == Read.None) {
                    Error.NoEndIf(Read);
                }
                Error.EndOfLine(Read);
            }
            Read.GetNextWord();
            continue;
        }
        // Break condition: Found a WEND
        if (Read.Word() == "WEND") {
            Read.GetNextWord();
            break;
        }
        // Found an END, which is the end of a function, so it is invalid
        if (Read.Word() == "END") {
            Read.GetNextWord();
            if (Read.Word() == "SUB" or Read.Word() == "FUNCTION") {
                Error.MustEndWhileLoopFirst(Read);
            }
            Error.EndInWrongPlace(Read);
        }
        // Everything failed, use an undeclared variable
        NotDIM();
        Assignment();
    }
    // End the while statement
    Asm.StopWhile(StartWhile, EndWhile);
    return ;
}


// DealWithControlStatements() returns true if the While or If statement was
// completed succesfully.
// Examples:
//   If 1=1 Then
//   ^^^^^^^^^^^---Makes sure the beginning of an if statement is valid
//   While 1=1
//   ^^^^^^^^^---Makes sure that the beginning of a while loop is valid
bool Compiler::DealWithControlStatements(std::string LoopCommand) {
    // Case where we have an If statement
    if (Read.Word() == "IF") {
        // Get the condition we are checking
        Condition();

        // Check for keyword: "THEN"
        if (Read.Word() != "THEN") {
            Error.ExpectedThen(Read);
        }
        Read.GetNextWord();

        // If there is no end of line following "THEN", report an error
        if (Read.WordType() != Read.EndOfLine) {
            Error.EndOfLine(Read);
        }
        Read.GetNextWord();

        // Go inside the IF statement
        If();
        // Return true, meaning we dealt with a control statement
        return true;
    }
    // Case where we have an While statement
    if (Read.Word() == "WHILE") {
        // Set the stage for the while and get the fixed place to loop back to
        std::string StartWhile = Asm.PrepareWhile();
        // Then get the conditio (the conditon will be checked every time we loop)
        Condition();
        // Make sure we have a new line
        if (Read.WordType() != Read.EndOfLine) {
            Error.EndOfLine(Read);
        }
        Read.GetNextWord();
        // Go to the inside of the start statement
        While(StartWhile);
        // Yep, we handled a control statement
        return true;
    }
    // Nope, didn't find any control statements
    return false;
}


// DealWithConsoleKeywords() has built-in console keywords:
// Sleep - pause program for a number of seconds
// Cls   - clear the screen
// Print - print stuff to the screen
// Input - get stuff input from the screen
// Note: Unlike most functions, some keywords have no need for any parenthesis
// after their identifier.
bool Compiler::DealWithConsoleKeywords() {
    // Sleep passes a Number expression in seconds to an assembly object
    if (Read.Word() == "SLEEP") {
        Expression(Data.Number);
        Asm.ConsoleSleep();
        return true;
    }

    // CLS is a single line statement, needs no parameters
    if (Read.Word() == "CLS") {
        Asm.ConsoleCls();
        Read.GetNextWord();
        return true;
    }

    // Print case
    if (Read.Word() == "PRINT") {
        // Print out expressions separated by a ';' or ',' if there is multiple
        do {
            // Need to resolve the data type before printing it
            Asm.ConsolePrint(Expression(Data.Unknown));
        } while (Read.Word() == ";" or Read.Word() == ",");

        // Start a new line after printing out all valid expressions
        Asm.ConsolePrintNewLine();
        return true;
    }

    // Input case
    if (Read.Word() == "INPUT") {
        Read.GetNextWord();
        // If the word after INPUT is a string, we have a prompt, as in:
        // INPUT "Enter A Number: "; A#
        // Where it prompts the user for a number and saves it in A$
        if (Read.WordType() == Read.String) {
            Asm.LoadString(Read.Word());
            Asm.ConsolePrint(Data.String);
            Read.GetNextWord();
            // If we have a prompt for INPUT, we need at least one semi-colon
            // or comma to separate the prompt string from the variable we need
            // to store the input in
            if (Read.Word() != ";" and Read.Word() != ",") {
                Error.ExpectedSeparatorForInput(Read);
            }
            Read.GetNextWord();
        }
        // Get the console input
        Asm.ConsoleInput();
        // Assign the console input
        AssignInput();
        return true;
    }
    return false;
}


// AssignInput() stores the variable from the console
// Example:
// INPUT "Enter your number: "; A#
void Compiler::AssignInput() {
    std::string Name = Read.Word();
    // If the variable is not reserved, create an undeclared variable
    if (!Data.IsAlreadyReserved(Name)) {
        NotDIM();
    }
    Read.GetNextWord();

    // Check to see if we are assigning to a member of an UDT
    if (Read.Word() == ".") {
        std::string Member = AssignUDTMember(Name);

        // Convert the console input from string to the appropriate type of the
        // UDT member being assigned, if necessary.
        if (Data.GetUDTData(Name).Type.Members[Member].Type == Data.Double or
            Data.GetUDTData(Name).Type.Members[Member].Type == Data.Integer) {
            Asm.ConvertToNumber();
        }

        // Assign the member variable's name and info
        Asm.AssignUDTMember(Name, Data.GetUDTData(Name).TypeName,
                            Member, Data.GetUDTData(Name).Type.Members[Member].Type);
        Read.GetNextWord();
        return ;
    }

    // Check to see if we are assigning an array item
    if (Read.Word() == "(") {
        // Get the array item
        AssignArrayItem(Name);
        // The third item on the stack is the array item (EBX). To use
        // Asm.AssignArrayItem, the array item has to be second on the stack.
        // --> We need to re-order the stack so EBX is the second item.
        Asm.POP("EBX");
        Asm.POP("EDI");
        Asm.POP("ESI");
        Asm.PUSH("EDI");
        Asm.PUSH("EBX");
        Asm.PUSH("ESI");

        // Convert the string input, if necessary.
        if (Data.GetArrayData(Name).Type == Data.Integer or
            Data.GetArrayData(Name).Type == Data.Double) {
            Asm.ConvertToNumber();
        }

        // Assign the array item to the array
        Asm.AssignArrayItem(Name, Data.GetArrayData(Name).Type);
        Read.GetNextWord();
        return ;
    }

    // Check to see if we are assigning a numeric type (Int, Double)
    if (Data.GetDataType(Name) == Data.Number) {
        Asm.ConvertToNumber();
    }

    // Assign the number
    Asm.AssignIt(Name);
    return ;
}


// SubFunction() handles the declaration of all functions and subroutines of
// functions.
// Examples:
//   Sub A () As Integer : End SUB
//   Function Aaa (A As Integer, B As Double, C As String) As String : End Sub
//   Declare Function Sleep Lib "Kernel32.dll" Alias "Sleep" (A As Integer) As _
//       Integer
void Compiler::SubFunction(bool External) {
    std::string Type;
    std::string Name;
    std::string ReturnValueType;

    // Check if we have a function or subroutine of a function
    Type = Read.Word();
    Read.GetNextWord();

    // Get the name of the function
    Name = Read.Word();

    // Validate the name
    if (Read.WordType() != Read.Identifier) {
        Error.BadName(Read);
    }

    // Check if the name already exists in the scope
    if (Data.AlreadyExistsInScope(Name)) {
        Error.AlreadyReserved(Read);
    }

    // Update the scope of Read, Write, and Database objects
    Write.EnterSubFunction();
    Data.EnterNewScope();
    Read.GetNextWord();

    // Add the new sub/function to the database and create it in assembly
    Data.AddSubFunction(Type, Name, External);
    Asm.CreateSubFunction(Name, External);

    // For external functions, we need to get the library, its alias, and which
    // calling convention to use.
    if (External) {
        SubFunctionLibAliasCall(Name);
    }

    // Verify there is a parenthesis to nest parameters
    if (Read.Word() != "(") {
        Error.ExpectedParameters(Read);
    }

    // Loop through all available the parameters
    do {
        Read.GetNextWord();
        // Break condition, if we a closing parenthesis and have no parameters 
        if (Read.Word() == ")") {
            break;
        }

        // Get the current parameter
        GetParameters(Name);
        Read.GetNextWord();
    } while (Read.Word() == ","); // Keep looping while we have a comma

    // Verify we have a closing parenthesis after all the parameters
    if (Read.Word() != ")") {
        Error.NoEndToParameters(Read);
    }

    // Check the next word to see if it is "FUNCTION"
    // If it is, then we need the return value.
    Read.GetNextWord();
    if (Type == "FUNCTION") {
        // Expected keyword "AS" after the function parameters inform the
        // compiler the function type
        if (Read.Word() != "AS") {
            Error.ExpectedAs(Read);
        }

        // Check to see if the return type is valid
        Read.GetNextWord();
        if (Read.Word() != "INTEGER" and
            Read.Word() != "DOUBLE"  and
            Read.Word() != "STRING"  and
            Data.IsType(Read.Word())) {
            Error.BadType(Read); // Report an error
        }

        // Store return value's type, then store it in the database
        ReturnValueType = Read.Word();
        Data.AddReturnValue(Name, Read.Word());

        // If this is an internal function, then the compiler needs to create a
        // RESULT variable to store its return value in based on its type.
        if (!External) {
            std::string curr = Read.Word();
            if (curr == "INTEGER" or curr == "DOUBLE"  or curr == "STRING") {

                // Case where we have a integer
                if (curr == "INTEGER") {
                    Asm.CreateInteger("RESULT", "EBP-4");
                }

                // Case where we have a double
                if (curr == "DOUBLE") {
                    Asm.CreateDouble("RESULT", "EBP-8");
                }

                // Case where we have a string
                if (curr == "STRING") {
                    Asm.CreateString("RESULT", "EBP-4");
                }
            }

            // If the data type is a UDT, then the compiler needs add relevant
            // information to the database
            if (Data.IsType(curr)) {
                Data.AddUDTData(Type, "RESULT", Data.GetScopeID() + 
                                       Data.StripJunkOff("RESULT"));
            }
        }
        Read.GetNextWord();
    }

    // Prepare all of the functions parameters
    PrepareParameters(Name);

    // If we dont have an end of line, then the compiler should report an error
    if (Read.WordType() != Read.EndOfLine) {
        if (External and Read.WordType() == Read.None) {}
            else {
            Error.EndOfLine(Read);
        }
    }

    // If this is an internal function, then read inside its scope
    if (!External) {
        InsideSubFunction(Name);
    }

    // Tell the database and writing objects that we're done with the function
    Data.ExitScope();
    Write.ExitSubFunction();

    // If this was an internal function (not a sub-function or external), then
    // add its return its value
    if (Type == "FUNCTION" and !External) {
        Asm.ReturnValue(Name, ReturnValueType);
    }

    // Finish creating the subfunction
    Asm.EndCreateSubFunction(Name, External);
    return ;
}


// InsideSubFunction() parses the block of code inside a user-defined function
void Compiler::InsideSubFunction(std::string Name) {

    // Get the first word and keep looping until we encounter End Function,
    // End Sub, or undefined behavor, in which case the compiler will report
    // an error.
    Read.GetNextWord();
    while (1) {
        // The compiler will not allow function declarations inside functions
        if (Read.Word() == "SUB" or Read.Word() == "FUNCTION") {
            Error.SubFunctionMustBeGlobal(Read);
        }

        // Call statement to deal with normal commands, and if it doesn't find
        // anything, let's take over
        if (!Statement()) {
            // Case where Word is a directive
            if (Read.Word() == "$") {
                Read.GetNextWord();
                Directives(Read.Word());
            }

            // Case where Word is an "END" statement
            if (Read.Word() == "END") {
                Read.GetNextWord();

                // If we are ending a sub-function, then we break out of being
                // inside InsideSubFunction()
                if (Data.GetSubFunctionInfo(Name).Type == Read.Word()) {
                    Read.GetNextWord();
                    break;
                }

                // There is a type mismatch if we find "SUB" or "FUNCTION" at
                // this point.
                if (Read.Word() == "SUB" or Read.Word() == "FUNCTION") {
                    Error.MismatchedSubFunction(Read);
                }

                // Check for a new line character
                if (Read.WordType() != Read.EndOfLine) {
                    Error.EndOfLine(Read);
                }

                // If there is an "END" statement by itself on a line, then
                // terminate the program
                Asm.EndProgram();
                Read.GetNextWord();
                continue;
            }

            // Case where Word is a result, break since we are done
            if (Read.Word() == "RESULT") {
                Read.GetNextWord();
                break;
            }

            // Make sure the function has an end by checking for a new line
            // afterwards
            if (Read.WordType() == Read.None) {
                Error.NoEndSubFunction(Read);
            }
            if (Read.WordType() == Read.EndOfLine) {
                Read.GetNextWord();
                continue;
            }

            // Assign NotDIM (an undeclared variable)
            NotDIM();
            Assignment();
        }

        // Check for an end of line
        if (Read.WordType() != Read.EndOfLine) {

            // Report and error if we encounter the end of the file before an
            // "EndIf" statement
            if (Read.WordType() == Read.None) {
                Error.NoEndIf(Read);
            }
            Error.EndOfLine(Read);
        }
        // Get the next word
        Read.GetNextWord();
    }
    return ;
}


// DeclareSubFunction() validates if a function is an external function
// Example:
//   DECLARE SUB A()
void Compiler::DeclareSubFunction() {
    Read.GetNextWord();
    // Report an error if we got here and are not declaring a subfunction
    if (Read.Word() != "SUB" and Read.Word() != "FUNCTION") {
        Error.ExpectedSubFunction(Read);
    }

    // Mark the subfunction as declared
    SubFunction(true);
    return ;
}


// CallSubFunction() makes sure we have a function to call and then calls it
// Example:
//   A# = MyFunc(Integer1, Double2, String3)
void Compiler::CallSubFunction(std::string Name, bool ReturnValue) {
    // If optimization has been enables, include the subfunction in the
    // compiler database
    if (Optimize) {
        Data.UsingSubFunction(Name);
    }

    // Get the opening parenthesis to start reading subfunction parameters
    Read.GetNextWord();
    if (Read.Word() != "(") {
        Error.ExpectedParameters(Read);
    }

    // Case for a subroutine/subfunction
    if (Data.GetSubFunctionInfo(Name).Type == "SUB") {

        // If the function has no parameters, then simply call the function
        if (Data.GetSubFunctionInfo(Name).ParamCount == 0) {
            Asm.InvokeSubFunction(Name);
            Read.GetNextWord();
        }

        // Otherwise, read in the function parameters before invoking it
        else {
            PassParameters(Name);
            Asm.InvokeSubFunction(Name);
            UnprepareParameters(Name);
        }
    }

    // Case for a function
    if (Data.GetSubFunctionInfo(Name).Type == "FUNCTION") {

        // If the function has no parameters, then simply call the function
        if (Data.GetSubFunctionInfo(Name).ParamCount == 0) {
            Asm.InvokeSubFunction(Name);
            Read.GetNextWord();
        }

        // Otherwise, read in the function parameters before invoking it
        else {
            PassParameters(Name);
            Asm.InvokeSubFunction(Name);
            UnprepareParameters(Name);
        }

        // If the function has no return value, then we can clean it up
        if (!ReturnValue) {
            Asm.CleanUpReturnValue(Name, Data.GetSubFunctionInfo(Name).ReturnValue.Type);
        }

        // Otherwise we need to push the function's return value onto the stack
        else {
            Asm.PushReturnValue(Name, Data.GetSubFunctionInfo(Name).ReturnValue.Type);
        }
    }

    // Verify we ahave a closing parenthesis regardless if the function or
    // subroutine/subfunction had parameters. Report an error if we didn't find
    // a closing parenthesis.
    if (Read.Word() != ")") {
        Error.ExpectedParameters(Read);
    }
    return ;
}


// SubFunctionLibAliasCall() gets information necessary to make function calls
// Example:
//   Declare Function Sleep Lib "Kernel32.dll" Alias "Sleep" Call Std (I As _
//        external function information)          
void Compiler::SubFunctionLibAliasCall(std::string Name) {
    // The three things we need to know about the library where the function is
    // located, the actual name of the function, and what calling convention to use
    std::string Library;
    std::string Alias = Name;
    std::string CallConv;
    // On Windows, the default calling convention is standard 
    #ifdef Windows
    CallConv = "STD";
    #endif
    // On Linux, the default calling convention is C
    // #ifdef Linux
    // CallConv = "C";
    // #endif

    // Get the library where the function resides
    if (Read.Word() == "LIB") {
        Read.GetNextWord();

        // Check that the library is given in string type
        if (Read.WordType() != Read.String) {
            Error.InvalidLibrary(Read);
        }
        Library = Read.Word();
        Read.GetNextWord();
    }

    // Get the alias the compiler will use to call the function
    if (Read.Word() == "ALIAS") {
        Read.GetNextWord();

        // Type check for Alias
        if (Read.WordType() != Read.String) {
            Error.InvalidAlias(Read);
        }

        Alias = Read.Word();
        Read.GetNextWord();
    }

    // Handle the function call
    if (Read.Word() == "CALL") {
        Read.GetNextWord();

        // Right now, the calling convention must either be C or Std
        if (Read.Word() != "STD") {
            Error.InvalidCallingConvention(Read);
        }

        // Read in the calling convention
        CallConv = Read.Word();
        Read.GetNextWord();
    }

    // If we  have no library assigned, then report an error
    if (Library == "") {
        Error.ExpectedExternalSubFunction(Read);
    }

    // Add the external function information to the database
    Data.AddExternalSubFunctionData(Name, Library, Alias, CallConv);
    return ;
}



// GetParameters() stores all the parameters for each function in the database
// Example:
//   Function A (A As Integer, B As Double, C as String) As Integer
void Compiler::GetParameters(std::string Name) {
    std::string Type;
    std::string ParameterName;
    std::string How;

    // Check if the compiler is passing by value or reference
    if (Read.Word() == "BYVAL" or Read.Word() == "BYREF") {
        How = Read.Word();
        Read.GetNextWord();
    }
    // By default, the compiler will pass the parameter by value
    else {
        How = "BYVAL";
    }

    // Verify the name of the parameter
    ParameterName = Read.Word();
    if (Read.WordType() != Read.Identifier) {
        Error.BadName(Read);
    }

    // Check to see if the parameter is already reserved in the database
    if (Data.IsAlreadyReservedInParameters(Name, ParameterName)) {
        Error.AlreadyReservedInParameters(Read);
    }

    // Check to see if the next word is the keyword "AS"
    Read.GetNextWord();
    if (Read.Word() != "AS") {
        Error.ExpectedAs(Read);
    }

    // Report an error if the parameter is an invalid type
    Read.GetNextWord();
    Type = Read.Word();
    if (Type != "INTEGER" and Type != "DOUBLE" and Type != "STRING" and !Data.IsType(Type)) {
        Error.BadType(Read);
    }

    // Get the assembly data for the integer parameter
    if (Type == "INTEGER") {
        SimpleDataInfo Info;
        if (Mangle) {
            Info.AsmName = Data.GetScopeID() + Data.StripJunkOff(ParameterName);
        }
        else {
            Info.AsmName = Data.StripJunkOff(ParameterName);
        }
        Info.Type = Data.Integer;
        Data.AddSimpleData(ParameterName, Info);
    }

    // Get the assembly data for the double parameter
    if (Type == "DOUBLE") {
        SimpleDataInfo Info;
        if (Mangle) {
            Info.AsmName = Data.GetScopeID() + Data.StripJunkOff(ParameterName);
        }
        else {
            Info.AsmName = Data.StripJunkOff(ParameterName);
        }
        Info.Type = Data.Double;
        Data.AddSimpleData(ParameterName, Info);
    }

    // Get the assembly data for the string parameter
    if (Type == "STRING") {
        SimpleDataInfo Info;
        if (Mangle) {
            Info.AsmName = Data.GetScopeID() + Data.StripJunkOff(ParameterName);
        }
        else {
            Info.AsmName = Data.StripJunkOff(ParameterName);
        }
        Info.Type = Data.String;
        Data.AddSimpleData(ParameterName, Info);
    }

    // Get the assembly data for the UDT parameter
    if (Data.IsType(Type)) {
        if (Mangle) {
            Data.AddUDTData(Type, ParameterName, Data.GetScopeID() + Data.StripJunkOff(ParameterName));
        }
        else {
            Data.AddUDTData(Type, ParameterName, Data.StripJunkOff(ParameterName));
        }
    }

    // Add the parameter to the database
    Data.AddParameter(Name, ParameterName, Type, How);
    return ;
}


// PassParameters() passes the parameters to the function. It's important to
// note that functions expect parameters to be ordered, and they are coming off
// the call stack in reverse. The compiler will also have to deal with passing
// parameters by reference, so they return modified inside the function.
void Compiler::PassParameters(std::string Name) {
    // Get the number of parametershaven'tthe function has
    int ParamCount = Data.GetSubFunctionInfo(Name).ParamCount;

    // Allocate memory to store the parameters temporarily
    Asm.AllocateParameterPool(Data.GetSubFunctionInfo(Name).SizeOfParameters);

    // Set the initial pool size to 0
    Data.SetParameterPoolSizeFilled(Name, 0);

    // Store every parameter in the pool
    for (int i = 1; i <= ParamCount; ++i) {
        // Get information for the current parameter
        std::string How = Data.GetSubFunctionInfo(Name).Parameters[i].How;
        std::string Type = Data.GetSubFunctionInfo(Name).Parameters[i].Type;
        std::string ParamName = Data.GetSubFunctionInfo(Name).Parameters[i].Name;
        std::string AsmName = Data.GetSubFunctionInfo(Name).Parameters[i].AsmName;
        int Size;

        // Case where the type is a integer
        if (Type == "INTEGER") {

            // Get the current size of the parameter pool
            Size = Data.GetParameterPoolSizeFilled(Name);

            // Evaluate the expression
            if (How == "BYVAL") {
                Expression(Data.Number);
            }
            else {
                ByRefExpression(Data.Number);
            }

            // Round the result to an integer
            Asm.RoundToInteger();

            // Add the parameter to the pool
            Asm.AddToParameterPool(Data.Integer, Size);

            // Mark that the pool is filled with a 4 byte integer
            Data.SetParameterPoolSizeFilled(Name, Size + 4);
        }

        // Case where the type is a string
        if (Type == "STRING") {
            // Get the current size of the parameter pool
            Size = Data.GetParameterPoolSizeFilled(Name);

            // Evaluate the expression
            if (How == "BYVAL") {
                Expression(Data.String);
            }
            else {
                ByRefExpression(Data.String);
            }
            // Add the string to the pool of parameters
            Asm.AddToParameterPool(Data.String, Size);

            // Make room for the next parameter 
            Data.SetParameterPoolSizeFilled(Name, Size + 4);
        }

        // Case where the type is a double
        if (Type == "DOUBLE") {
            // Get the current size of the parameter pool
            Size = Data.GetParameterPoolSizeFilled(Name);

            // Evaluate the expression
            if (How == "BYVAL") {
                Expression(Data.Number);
                // If we are passing by value, a Double costs 8 bytes
                Asm.AddToParameterPool(Data.Double, Size);
                Data.SetParameterPoolSizeFilled(Name, Size + 8);
            }
            else {
                Expression(Data.Number);

                // If we are passing by reference, then we only need 4 bytes to
                // store a pointer to the Double.
                Asm.AddToParameterPool(Data.Integer, Size);
                Data.SetParameterPoolSizeFilled(Name, Size + 4);
            }
        }

        // Case where the type is a UDT
        if (Data.IsType(Type)) {
            // Get the current size of the parameter pool
            Size = Data.GetParameterPoolSizeFilled(Name);

            // Evaluate the expression
            Expression(Data.UDT, Type);

            // Add the parameter to the pool
            Asm.AddToParameterPool(Data.Type, Size);

            // Mark that the pool is filled with an additional 4 bytes
            Data.SetParameterPoolSizeFilled(Name, Size + 4);
        }

        // Break from the loop if we loaded all parameters
        if (i == ParamCount) {
            break;
        }

        // Find the next parameter, if one exists
        if (Read.Word() != ",") {
            Error.ExpectedNextParameter(Read);
        }
    }

    // Pop the parameters from the pool and push them to the stack
    // Note: We start at the end because they are loaded into the pool in
    //       reverse order initially.
    for (int j = ParamCount; j >= 1; --j) {
        std::string Type = Data.GetSubFunctionInfo(Name).Parameters[j].Type;
        std::string How = Data.GetSubFunctionInfo(Name).Parameters[j].How;
        if (Type == "INTEGER") {
            Data.SetParameterPoolSizeFilled(Name, Data.GetParameterPoolSizeFilled(Name) - 4);
            Asm.PushParameterPool(Data.Integer, Data.GetParameterPoolSizeFilled(Name));
        }
        if (Type == "STRING") {
            Data.SetParameterPoolSizeFilled(Name, Data.GetParameterPoolSizeFilled(Name) - 4);
            Asm.PushParameterPool(Data.String, Data.GetParameterPoolSizeFilled(Name));
        }
        if (Type == "DOUBLE") {
            Data.SetParameterPoolSizeFilled(Name, Data.GetParameterPoolSizeFilled(Name) - 8);
            Asm.PushParameterPool(Data.Double, Data.GetParameterPoolSizeFilled(Name));
        }
        if (Data.IsType(Type)) {
            Data.SetParameterPoolSizeFilled(Name, Data.GetParameterPoolSizeFilled(Name) - 4);
            if (How == "BYVAL") {
                Asm.PushParameterPool(Data.Type, Data.GetParameterPoolSizeFilled(Name));
            }
            else {
                Asm.PushParameterPool(Data.Integer, Data.GetParameterPoolSizeFilled(Name));
            }
        }
    }
    return ;
}


// PrepareParameters() converts parameter names into a name the stack can
// referenece as local variable
void Compiler::PrepareParameters(std::string Name) {
    // The parameters start with an 8 bytes offset onto the stack
    int offset = 8;

    // Get the total number of parameters
    int ParamCount = Data.GetSubFunctionInfo(Name).ParamCount;

    // Find each parameter's location on the stack
    for (int i = 1; i <= ParamCount; i++) {
        // Get current parameter's name and type
        std::string Type = Data.GetSubFunctionInfo(Name).Parameters[i].Type;
        std::string ParamName = Data.GetSubFunctionInfo(Name).Parameters[i].Name;
        // If the variable is a double, then it is offset by 8 bytes
        if (Type == "DOUBLE") {
            if (Type == "DOUBLE") {
                Data.SetAsmName(ParamName, "EBP+" + ToStr(offset));
                offset += 8;
            }
        }
        // If the variable is a string or integer, then it is offset by 4 bytes
        else {
            Data.SetAsmName(ParamName, "EBP+" + ToStr(offset));
            offset += 4;
        }
    }
    return ;
}


// UnprepareParameters() after the function or subroutine has been called, and
// returned its value if it returns one, free the memory that the parameters
// previously occupied. We need to push them all off of the stack to the pool,
// then we can free everything inside of the pool once we verify all parameters
// made it safely to the pool.
void Compiler::UnprepareParameters(std::string Name) {
    // Get the total number of parameters
    int ParamCount = Data.GetSubFunctionInfo(Name).ParamCount;

    // If we with the C calling convention, adjust the stack
    // if (Data.GetSubFunctionInfo(Name).ExternalInfo.CallingConv == "C") {
    //     Asm.AdjustStack(Data.GetSubFunctionInfo(Name).SizeOfParameters);
    // }

    // Set the Parameter pool size to 0
    Data.SetParameterPoolSizeFilled(Name, 0);

    for (int i = 1; i <= ParamCount; ++i) {
        // Get information associated with parameter
        std::string Type = Data.GetSubFunctionInfo(Name).Parameters[i].Type;
        std::string How = Data.GetSubFunctionInfo(Name).Parameters[i].How;
        std::string ParamName = Data.GetSubFunctionInfo(Name).Parameters[i].Name;
        bool External = Data.GetSubFunctionInfo(Name).External;
        std::string CallConv = Data.GetSubFunctionInfo(Name).ExternalInfo.CallingConv;

        // Check to see if we have a valid parameter
        if (Data.IsType(Type) or Type == "STRING" or Type == "DOUBLE") {
            // Strings and data types need to be freed to avoid memory leaks
            // Note: ONLY if they are passed by value.
            if ((Type == "STRING" or Data.IsType(Type)) and How != "BYREF") {
                // Preserve the return value of the parameter (EAX)
                Asm.PUSH("EAX");

                // Push the parameter from pool to the stack
                Asm.PushParameterPool(Data.String, Data.GetParameterPoolSizeFilled(Name));

                // Pop the parameter from the stack
                Asm.POP("ECX");

                // Free the memory
                Asm.FreeMemory(Write.ToMain, "ECX");

                // Restore the return value
                Asm.POP("EAX");

                // Adjust the pool size according to the current parameter
                Data.SetParameterPoolSizeFilled(Name, Data.GetParameterPoolSizeFilled(Name) + 4);
            }
            // We are not freeing any doubles or parameters passed by reference
            if (Type == "DOUBLE") {
                Data.SetParameterPoolSizeFilled(Name,
                    Data.GetParameterPoolSizeFilled(Name) + 4);
                if (How != "BYREF") {
                    Data.SetParameterPoolSizeFilled(Name,
                        Data.GetParameterPoolSizeFilled(Name) + 4);
                }
            }
        }
        // All other parameters just need to be accounted for
        else {
            Data.SetParameterPoolSizeFilled(Name,
                Data.GetParameterPoolSizeFilled(Name) + 4);
        }
    }

    // Free the parameters from the parameter pool
    Asm.FreeParameterPool();
    return ;
}


// ByRefExpression() Gets a single expression and gets the address of it
void Compiler::ByRefExpression(int Type) {
    Read.GetNextWord();

    // Check if string and identifier are valid
    if (Read.WordType() != Read.Identifier && Read.WordType() != Read.String) {
        Error.BadName(Read);
    }

    // Get the address of the string and push it to the stack
    if (Data.GetDataType(Read.Word()) == Data.String) {
        if (Type != Data.String) {
            Error.ExpectedStringData(Read);
        }
        Asm.PUSH("dword[" + Data.Asm(Read.Word()) + "]");
        Read.GetNextWord();
        return ;
    }

    // Get the address of the number and push it to the stack
    if (Data.GetDataType(Read.Word()) == Data.Integer or Data.GetDataType(Read.Word()) == Data.Double) {
        if (Type != Data.Number) {
            Error.ExpectedNumberData(Read);
        }
        Asm.PUSH(Data.Asm(Read.Word()));
        Read.GetNextWord();
        return ;
    }

    // Get the address of the UDT and push it to the stack
    if (Data.GetDataType(Read.Word()) == Data.UDT) {
        if (Type != Data.UDT) {
            Error.ExpectedUDT(Read);
        }
        Asm.PUSH(Data.Asm(Read.Word()));
        Read.GetNextWord();
        return ;
    }

    // Unexpected behavior occurred, report an error
    Error.BadName(Read);
    return ;
}


// ChooseAppType() sets the app type the compiler will generate
// Options: Console, GUI, or DLL (SO on linux <-- TODO)
// Examples:
//   $AppType Console
//   $AppType GUI
//   $AppType DLL
void Compiler::ChooseAppType() {
    Read.GetNextWord();

    // Set the requested AppType
    if (Read.Word() == "CONSOLE") {
        AppType = Console;
        return ;
    }
    if (Read.Word() == "GUI") {
        AppType = GUI;
        return ;
    }
    if (Read.Word() == "DLL") {
        AppType = DLL;
        // We will not be able to optimize a DLL in this case since they are
        // used at runtime instead of compile time.
        Optimize = false;
        return ;
    }

    // Report an error if we were passed an invalid AppType
    Error.InvalidAppType(Read);
    return ;
}


// Optimize() controls the optimization flag at compile time.
// Examples:
//   $Optimize ON
//   $Optimize OFF
void Compiler::OptimizeApp() {
    Read.GetNextWord();

    // Report an error if optimize was set to something other than ON/OFF
    if (Read.Word() != "ON" and Read.Word() != "OFF") {
        Error.ExpectedOnOrOff(Read);
    }

    // Turn optimization either on or off
    if (Read.Word() == "ON") {
        Optimize = true;
    }
    if (Read.Word() == "OFF") {
        Optimize = false;
    }
    return ;
}


// Directives() defines pre-processor directives on the BASIC source code
// These are not executable statements.
bool Compiler::Directives(std::string Directive) {
    // APPTYPE must be declared initially, it is not a directive.
    if (Directive == "APPTYPE") {
        Error.CannotChangeAppType(Read);
        return false;
    }
    // OPTIMIZE must be declared initially, it is not a directive.
    if (Directive == "OPTIMIZE") {
        Error.CannotOptimize(Read);
        return false;
    }

    // Check for the COMPRESS directive
    if (Directive == "COMPRESS") {
        CompressApp();
        Read.GetNextWord();
        return false;
    }

    // Check for the INCLUDE directive
    if (Directive == "INCLUDE") {
        IncludeFile();
        Read.GetNextWord();
        return false;
    }

    // Check for the ASM directive
    if (Directive == "ASM") {
        DropDownToAsm();
        return false;
    }

    // Check for the CONST directive
    if (Directive == "CONST") {
        Const();
        return false;
    }

    // Check for the DEFINE directive
    if (Directive == "DEFINE") {
        Read.GetNextWord();
        // Validate the identifier we want to DEFINE
        if (Read.WordType() != Read.Identifier) {
            Error.DefineMustBeIdentifier(Read);
        }
        // Add the DEFINE'd word to the database
        Data.AddDefine(Read.Word());
        Read.GetNextWord();
        return false;
    }

    // Check for the IFDEF directive
    if (Directive == "IFDEF") {
        IfDef();
        return false;
    }

    // Check for the IFNDEF directive
    if (Directive == "IFNDEF") {
        IfNDef();
        return false;
    }

    // Check for the ENDIF directive
    if (Directive == "ENDIF") {
        // If we have already ended all the $IfDef and $IfNDef, bail out
        if (Data.CanExitDirective() == false) {
            Error.CannotExitDirective(Read);
        }
        // Exit the directive
        Data.ExitDirective();
        Read.GetNextWord();
        return false;
    }

    // Check for the END directive
    if (Directive == "END") {
        Read.GetNextWord();
        // "EndIf" will end an $IfDef or $IfNDef
        if (Read.Word() == "IF") {
            // If we have already ended all the $IfDef and $IfNDef, then we
            // need to report an error
            if (Data.CanExitDirective() == false) {
                Error.CannotExitDirective(Read);
            }

            // Exit the directive
            Data.ExitDirective();
            Read.GetNextWord();
            return false;
        }
    }

    // Check for the MANGLE directive
    if (Directive == "MANGLE") {
        MangleNames();
        Read.GetNextWord();
        return false;
    }

    // Compiler found an invalid directive, report an error
    Error.InvalidDirective(Read);
    return false;
}


// FirstDirectives() handles the directives that must come before we can
// compile. If none are provided, then use defaults
bool Compiler::FirstDirectives(std::string Directive) {
    // Generate an app type (Console/GUI/DLL)
    if (Directive == "APPTYPE") {
        ChooseAppType();
        Read.GetNextWord();
        return true;
    }

    // Check for optimization flag
    if (Directive == "OPTIMIZE") {
        OptimizeApp();
        Read.GetNextWord();
        return true;
    }

    // Report an error if the user passed an invalid directive
    Error.InvalidDirective(Read);
    return false;
}


// IncludeFile() includes another BASIC source code file into the program
// Examples:
//   $Include "Libraries/String.inc" --> Includes from the compiler's folder
//   $Include "MyLibrary.inc"        --> Includes from the program's folder
void Compiler::IncludeFile() {
    // Store the old directive level
    int OldDirectiveLevel = Data.GetDirectiveLevel();

    // Set the new file's directive level to zero
    Data.SetDirectiveLevel(0);

    // Create new reading and compiler objects
    Reading NewFile;
    Compiler NewCompiler;
    Read.GetNextWord();

    // Get the file name, report an error if no file was found
    if (Read.WordType() != Read.String) {
        Error.ExpectedFileNameAfterInclude(Read);
    }

    // If the file doesn't exist, then we check if it is in the same directory
    // as the source code file
    if (!FileExists(Read.Word())) {
        // Add the file name of the file to include
        std::string NewFileName = PatchFileNames(Read.GetBookName(), Read.Word());

        // If that file exists, then try to compile it
        if (FileExists(NewFileName)) {
            // Open the file with the new reading object
            NewFile.OpenBook(NewFileName);
            // Compile it with parameter IncFile set to "true"
            NewCompiler.Compile(NewFile, true);

            // Set the directive level back to the previous level before this
            // $Include directive was encountered
            Data.SetDirectiveLevel(OldDirectiveLevel);
            return ;
        }
    }

    // Open the file with the new reading object
    NewFile.OpenBook(Read.Word());
    // Compile the included file and set the IncFile parameter to true
    NewCompiler.Compile(NewFile, true);
    // Set the directive level back to the previous level
    Data.SetDirectiveLevel(OldDirectiveLevel);
    return ;
}


// DropDownToAsm() writes inline assembly code
// Example:
//   $Asm
//     EXTERN ExitProcess
//     PUSH 0
//     CALL ExitProcess
//   $End Asm
void Compiler::DropDownToAsm() {
    std::string Line;
    Read.GetNextWord();

    // If we don't have an end of the line after $Asm, report an error
    if (Read.WordType() != Read.EndOfLine) {
        Error.ExpectedEndOfLineAfterAsm(Read);
    }

    // Don't set the source code to uppercase - preserve the case
    Read.SetUppercase(false);

    // Loop until we encounter "$End Asm" (or error)
    while (1) {
        // Reset the line
        Line = "";
        Read.GetNextWord();

        // If we found a new line first, then loop around again
        if (Read.WordType() == Read.EndOfLine) {
            continue;
        }

        // If we find an end of file, report an error
        if (Read.WordType() == Read.None) {
            Error.NoEndToAsm(Read);
        }

        // Store the current word
        Line = Read.Word();
        // Check to see if we have an "$End Asm"
        if (Read.Word() == "$") {
            Read.SetUppercase(true);
            Read.GetNextWord();
            if (Read.Word() == "END") {
                Read.GetNextWord();
                if (Read.Word() == "ASM") {
                    Read.GetNextWord();
                    return ;
                }
            }
            // If we don't find an "$End Asm", report an error
            Error.ExpectedEndAsm(Read);
        }

        // If we don't have a "$" as the first word of the line, then store the
        // rest of the assembly language in Line
        Line += Read.GetWholeLine();

        // Write the inlined assembly langugage to the writing object
        Write.Line(Write.ToMain, Line);
    }
    return ;
}


// CompressApp() controls the compression of the application (ON/OFF) 
// Note: $Compress On at the end of a program would still take effect
// Example:
//   $Compress ON
//   $Compress OFF
void Compiler::CompressApp() {
    Read.GetNextWord();
    // Turn the compression on or off
    if (Read.Word() == "ON") {
        Compress = true;
        return ;
    }
    if (Read.Word() == "OFF") {
        Compress = false;
        return ;
    }
    Error.ExpectedOnOrOff(Read);
    return ;
}


// IfDef() compiles the code if the identifier has been defined earlier.
// Otherwise, do not compile the code.
// Example:
//   $IfDef Windows
//     ' Code
//   $End If
void Compiler::IfDef() {
    // Enter the directive
    Read.GetNextWord();
    Data.EnterDirective();

    // If this identifier is already defined, do not redefine it. Instead,
    // return and continue to compile the code
    if (Data.IsDefined(Read.Word()) == true) {
        Read.GetNextWord();
        return ;
    }

    // If the identifier is not defined, read the code until we encounter an
    // "$EndIf" or an error
    while (1) {
        // Eat the whole line
        while (Read.WordType() != Read.EndOfLine) {
            Read.GetNextWord();
            // If we have come to the end of the file, error!
            if (Read.WordType() == Read.None) {
                Error.NoEndToIfDef(Read);
            }
        }
        Read.GetNextWord();
        // Check for nested $IfDefs or $IfNDefs
        if (Read.Word() == "$") {
            Read.GetNextWord();
            // Recursively call "IfDef"
            if (Read.Word() == "IFDEF") {
                IfDef();
            }
            // Recursively call "IfNDef"
            if (Read.Word() == "IFNDEF") {
                IfNDef();
            }
            // If we have "END IF", exit the directive
            if (Read.Word() == "END") {
                Read.GetNextWord();
                if (Read.Word() == "IF") {
                    Data.ExitDirective();
                    break;
                }
            }

            // If we have "ENDIF", exit the directive
            if (Read.Word() == "ENDIF") {
                Data.ExitDirective();
                break;
            }
        }
    }
    Read.GetNextWord();
    return ;
}


// IfNDef() compiles the code if the following identifier is not defined
// Example:
//   $IfNDef Windows
//     ' Code
//   $End If
void Compiler::IfNDef() {
    // If the identifier isn't defined, then enter a new level and return to
    // compiling the rest of the code
    Read.GetNextWord();
    if (!Data.IsDefined(Read.Word())) {
        Data.EnterDirective();
        Read.GetNextWord();
        return ;
    }

    // Loop until we encounter an "$End If" or an error
    while (1) {
        // Eat all lines, no sense in processing or compiling
        while (Read.WordType() != Read.EndOfLine) {
            Read.GetNextWord();
            // If we reach an end of the file before "$End If", report an error
            if (Read.WordType() == Read.None) {
                Error.NoEndToIfDef(Read);
            }
        }

        // Check if the first word on the next line is a $
        Read.GetNextWord();
        if (Read.Word() == "$") {
            Read.GetNextWord();
            // If we have an "$IfDef", handle it before nested statements
            if (Read.Word() == "IFDEF") {
                IfDef();
            }
            // If we have an "$IfNDef", handle it before nested statements
            if (Read.Word() == "IFNDEF") {
                IfNDef();
            }
            // If we have an "$End If", exit the directive and return
            if (Read.Word() == "END") {
                Read.GetNextWord();
                if (Read.Word() == "IF") {
                    Data.ExitDirective();
                    break;
                }
            }

            // If we have an "ENDIF", exit the directive and return
            if (Read.Word() == "ENDIF") {
                Data.ExitDirective();
                break;
            }
        }
    }
    Read.GetNextWord();
    return ;
}


// Const() is called when we need to apply constness to an identifier so that
// it returns a constant value
// Examples:
//   $Const WM_DESTROY = 2
//   $Const US = "United States"
void Compiler::Const() {
    std::string Name;
    std::string ConstData;
    Read.GetNextWord();
    Name = Read.Word();
    // If the constant identifier is not in the database, report an error
    if (Read.WordType() != Read.Identifier) {
        if (Read.WordType() == Read.None or Read.WordType() == Read.EndOfLine) {
            Error.ExpectedNameAfterDIM(Read);
        }
        Error.BadName(Read);
    }
    if (Data.AlreadyExistsInScope(Name)) {
        Error.AlreadyReserved(Read);
    }
    Read.GetNextWord();

    // Check for the assignment operator
    if (Read.Word() != "=") {
        Error.ExpectedAssignment(Read);
    }

    // Get the constant value
    Read.GetNextWord();
    ConstData = Read.Word();
    // Check for a negative number
    if (Read.Word() == "-") {
        Read.GetNextWord();
        // This compiler can only negate numbers
        if (Read.WordType() != Read.Number) {
            Error.CanOnlyNegateNumbers(Read);
        }
        // Add the number to the constant value
        ConstData += Read.Word();
    }

    // If we have numberic or string constant data, add it to the database
    if (Read.WordType() == Read.Number or Read.WordType() == Read.String) {
        Read.AddConstData(Name, Read.Word(), Read.WordType());
        Read.GetNextWord();
        return ;
    }

    // If the type was not valid for constness, report an error
    Error.ConstDataMustBeNumberOrString(Read);
    return ;
}


// MangleNames() minifies the code
void Compiler::MangleNames() {
    Read.GetNextWord();
    // Turn Mangle on or off
    if (Read.Word() == "ON") {
        Mangle = true;
        return ;
    }
    if (Read.Word() == "OFF") {
        Mangle = false;
        return ;
    }
    Error.ExpectedOnOrOff(Read);
    return ;
}


// PrepareProgram() determines the type of program to run, whether or not to
// optimize, and builds the core of the program
void Compiler::PrepareProgram() {
    // Define either Windows or Linux so the user can tell what type of OS 
    // the source code is being compiled on
    #ifdef Windows
        Data.AddDefine("WINDOWS");
    #endif
    // #ifdef Linux
    // Data.AddDefine("LINUX");
    // #endif

    // Get the AppType and Optimize directives
    bool BuiltProgram = false;
    while (Read.Word() == "$" and !BuiltProgram) {
        Read.GetNextWord();
        // If we have $AppType or $Optimize, get more directives
        if (Read.Word() == "APPTYPE" or Read.Word() == "OPTIMIZE") {
            FirstDirectives(Read.Word());
        }
        // Build the app with default settings
        else {
            // Write the assembly skeleton for the app
            Asm.BuildSkeleton();

            // Prepare error messages
            Asm.PrepareErrorMessages();

            // Initialize the memory management routines
            Asm.InitMemory();

            // Deal with the current directive
            Directives(Read.Word());
            BuiltProgram = true;
        }
        // Check for a new line after each command
        if (Read.WordType() != Read.EndOfLine) {
            Error.EndOfLine(Read);
        }
        Read.GetNextWord();
        // No sense in analyzing new lines, just skip past them
        while (Read.WordType() != Read.EndOfLine) {
            Read.GetNextWord();
        }
    }

    // If we exit the $ loop without having a directive, we still need to build
    // the app
    if (!BuiltProgram) {
        Asm.BuildSkeleton();
        Asm.PrepareErrorMessages();
        Asm.InitMemory();
    }

    // If this is a console or GUI app, prepare pre-set variables:
    // CommandLine$ and HInstance
    if (AppType == Console or AppType == GUI) {
        Asm.GetPreSetVariables();
        // Also, is this is a console app, intialize the console
        if (AppType == Console) {
            Asm.InitConsole();
        }
    }

    // Add a define for the user so they know what type of app we are building
    if (AppType == Console) {
        Data.AddDefine("COSNOLE");
    }
    if (AppType == GUI) {
        Data.AddDefine("GUI");
    }
    if (AppType == DLL) {
        Data.AddDefine("DLL");
    }
    return ;
}



#endif // COMPILER_HPP