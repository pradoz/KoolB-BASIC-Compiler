#ifndef FILENAME_HPP
#define FILENAME_HPP

#include <string>

class Compiler {
public:
    void Compile(Reading SourceFile, bool IsIncFile);
    void Statement();
    void DIM();
    void NotDIM();


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
    if (Data.CanExitDirective() == true) {
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
            if (Mangle == true) {
                Asm.CreateInteger(Name, Data.StripJunkOff(Name) + "_Integer");
            }
            else {
                Asm.CreateInteger(Name, Data.StripJunkOff(Name));            
            }
        }
        // If the last character is a #, then create a double
        if (LastChar == '#') {
            if (Mangle == true) {
                Asm.CreateDouble(Name, Data.StripJunkOff(Name) + "_Double");
            }
            else {
                Asm.CreateDouble(Name, Data.StripJunkOff(Name));
            }
        }
        // If the last character is a $, then create a string
        if (LastChar == '$') {
            if (Mangle == true) {
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
















#endif // FILENAME_HPP