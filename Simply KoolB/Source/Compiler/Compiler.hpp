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

    // TODO: (smooth) Operators


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



#endif // FILENAME_HPP