#ifndef ERRORS_HPP
#define ERRORS_HPP


class Errors {
public:
    // Core functions to format the reported error
    void Prepare(Reading SourceFile);
    void PrintError(std::string Cause);
    void PrintLocation();
    void PrintCause(std::string Cause);
    void PrintCode();
    void PrintErrorPosition();

    // Errors for individual functions
    void EndOfLine(Reading SourceFile);

    void BadStatement(Reading SourceFile);
    void BadName(Reading SourceFile);
    void ExpectedNameAfterDIM(Reading SourceFile);
    void BadType(Reading SourceFile);
    void NoType(Reading SourceFile);
    void UnfinishedType(std::string TypeName, Reading SourceFile);

    void AlreadyReserved(Reading SourceFile);
    void OnlyNumbersForArrays(Reading SourceFile);
    void OnlyIntegersForArrays(Reading SourceFile);
    void ExpectedAs(Reading SourceFile);
    void ExpectedName(Reading SourceFile);
    void ExpectedNameAfterTYPE(Reading SourceFile);

    void ExpectedParenthesis(Reading SourceFile);
    void ExpectedEndingParenthesis(Reading SourceFile);
    void UnmatchedParenthesis(Reading SourceFile);
    void ExpectedSeparatorForInput(Reading SourceFile);
    void ExpectedThen(Reading SourceFile);
    void ExpectedPeriod(Reading SourceFile);

    void ExpectedStringData(Reading SourceFile);
    void ExpectedNumberData(Reading SourceFile);
    void ExpectedArrayData(Reading SourceFile);
    void ExpectedAssignment(Reading SourceFile);
    void ExpectedArray(Reading SourceFile);
    void ExpectedUDT(Reading SourceFile);

    void AlreadyReservedInType(Reading SourceFile);
    void CanOnlyAssign(Reading SourceFile);
    void TypesDoNotMatch(Reading SourceFile);
    void UnknownExpression(Reading SourceFile);
    void UndeclaredVariable(Reading SourceFile);
    void InvalidMember(Reading SourceFile);
    void UnableToAccessMember(Reading SourceFile);
    void UnknownExpressionType(Reading SourceFile);
    void ExpectedEndingArrayParenthesis(Reading SourceFile);
    void UnableToAccessItem(Reading SourceFile);

    void NoEndIf(Reading SourceFile);
    void NoDo(Reading SourceFile);

    void ExpectedParameters(Reading SourceFile);
    void AlreadyReservedInParameters(Reading SourceFile);

    void NoEndToParameters(Reading SourceFile);
    void NoEndSubFunction(Reading SourceFile);
    void MustEndIfStatementFirst(Reading SourceFile);
    void MustEndWhileLoopFirst(Reading SourceFile);
    void EndInWrongPlace(Reading SourceFile);

    void MismatchedSubFunction(Reading SourceFile);
    void SubFunctionMustBeGlobal(Reading SourceFile);
    void ExpectedNextParameter(Reading Sourcefile);

    void InvalidLibrary(Reading SourceFile);
    void InvalidAlias(Reading SourceFile);
    void InvalidCallingConvention(Reading SourceFile);

    void ExpectedExternalSubFunction(Reading SourceFile);
    void ExpectedSubFunction(Reading SourceFile);

    void ExpectedFileNameAfterInclude(Reading SourceFile);
    void ExpectedEndAsm(Reading SourceFile);
    void ExpectedEndOfLineAfterAsm(Reading SourceFile);
    void ExpectedVariable(Reading SourceFile);    

    void NoEndToAsm(Reading SourceFile);

    void InvalidDirective(Reading SourceFile);
    void InvalidAppType(Reading SourceFile);

    void CannotChangeAppType(Reading SourceFile);
    void CannotOptimize(Reading SourceFile);
    void ExpectedOnOrOff(Reading SourceFile);

    void DefineMustBeIdentifier(Reading SourceFile);
    void NoEndToIfDef(Reading SourceFile);

    void CannotExitDirective(Reading SourceFile);

    void CannotModifyConstData(Reading SourceFile);
    void ConstDataMustBeNumberOrString(Reading SourceFile);
    void CanOnlyNegateNumbers(Reading SourceFile);    


private:
    // The source code to the program that has an error
    std::string Book;

    // The filename of the program that has an error
    std::string BookName;

    // Our current position in the file
    long BookMark;

    // The current line
    long CurrentLine;

    // The word or token in error
    std::string CurrentWord;

    // The type of toke in error
    int TypeOfWord;

    // The whole line of code that is in error
    std::string CodeLine;

    // The position where the word in error begings
    long ErrorLength;
};


// Prepare() queries information from the Reading object, so we can format the 
void Errors::Prepare(Reading SourceFile) {
    CurrentWord = SourceFile.Word();
    TypeOfWord = SourceFile.WordType();
    Book = SourceFile.GetBook();
    BookName = SourceFile.GetBookName();
    BookMark = SourceFile.GetBookMark();
    CurrentLine = SourceFile.GetCurrentLine();
    return ;
}


// PrintError() is the core function for the error class
void Errors::PrintError(std::string Cause) {
    PrintLocation();
    PrintCause(Cause);
    PrintCode();
    PrintErrorPosition();
    return ;
}


// PrintLocation() prints the file and line where an error occured
void Errors::PrintLocation() {
    // If we are at the end of a line, then we need to move the CurrentLine
    // one line up because it was previously incremented
    if (CurrentWord == "\r\n") {
        --CurrentLine;
    }
    printf("\nError in: \"%s\" | Line %i:\r\n", BookName.c_str(), CurrentLine);
    return ;
}


// PrintCause() prints out the actual error generated by the compiler
void Errors::PrintCause(std::string Cause) {
    long Position;

    // Replace "" (no token) with "End-Of-File"
    Position = Cause.find("\"\"", 0);
    if (Position != std::string::npos) {
        Cause.replace(Position, 2, "End-Of-File");
    }
    // Replace '\n' with "End-Of-Line"
    Position = Cause.find("\"\n\"", 0);
    if (Position != std::string::npos) {
        Cause.replace(Position, 3, "End-Of-Line");
    }

    printf("    Cause: %s\r\n", Cause.c_str());
    return ;
}


// PrintCode() extracts and prints the line that the error occurs on
void Errors::PrintCode() {
    long Position = BookMark - 1;
    long StartPos;
    long EndPos;

    // If the current position is on a newline, back up one to avoid blank lines
    if (CurrentWord == "\r\n") {
        Position -= 1;
    }

    // Get a reference to the position of the  last newline
    StartPos = Book.rfind("\r", Position);

    // If there is no last newline, then we are on the first line of the file
    if (StartPos == std::string::npos) {
        StartPos = 0;
    }
    else {
        // Skip over the newline
        StartPos += 1;
    }

    // Find the next newline position
    EndPos = Book.find("\r", Position);

    // If no newline is found, then we are at the end of the file
    if (EndPos == std::string::npos) {
        EndPos = Book.length();
    }

    // Extract the line inbetween the two lines
    CodeLine = Book.substr(StartPos, EndPos - StartPos);

    // Calculate the length of the error message in characters
    ErrorLength = BookMark - StartPos - CurrentWord.length();

    // If CurrentWord is a newline, skip it
    if (CurrentWord == "\r\n") {
        ++ErrorLength;
    }

    printf("    Line of code in error:\r\n");
    printf("        %s\r\n", CodeLine.c_str());
    return ;
}


// PrintErrorPosition prints a string of "^^^^" under the Word the compiler
// discovered an error at
void Errors::PrintErrorPosition() {
    int Position = 0;
    printf("        ");

    // Print out whitespace until we get to the beginning of the word in error
    while (Position < ErrorLength) {

        // Print out a whitespace tab
        if (CodeLine[Position] == '\t') {
            printf("\t");
        }
        else { // Print out a single whitespace
            printf(" ");
        }
        ++Position;
    }

    // Check for a "^" character
    Position = 1;

    // "Underline" the Word where the error occured with carets
    while (Position < (int)CurrentWord.length()) {
        printf("^");
        ++Position;
    }

    printf("^--Error!\r\n");
    return ;
}


void Errors::EndOfLine(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("End statement (End-Of-Line) not found."
               " Error at: \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::BadStatement(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Beginning statement not found."
               " Error at: \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::AlreadyReserved(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Already reserved. \"" + CurrentWord + "\" was declared twice.");
    exit(1);
    return ;
}


void Errors::BadType(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Unable to determine the data type of: \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::NoType(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("No type declared for: \"" + CurrentWord + "\", add a valid data type like $ to the end.");
    exit(1);
    return ;
}


void Errors::BadName(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Invalid name found at: \"" + CurrentWord + "\""
               "Only alphanumeric characters are allowed for Identifiers.");
    exit(1);
    return ;
}


void Errors::ExpectedNameAfterDIM(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected an Identifier after \"DIM\". Found: \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::ExpectedAs(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected \"AS\" after Identifier. Found: \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::OnlyNumbersForArrays(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Arrays may only contain an integer size of elements.");
    exit(1);
    return ;
}


void Errors::OnlyIntegersForArrays(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("\"" + CurrentWord + "\" is a floating point number. Try again with an Integer.");
    exit(1);
    return ;
}


void Errors::ExpectedEndingParenthesis(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected closing parenthesis (\")\"). Found: \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::ExpectedNameAfterTYPE(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected Identifier for a User-Defined Type. Found: \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::UnfinishedType(std::string TypeName, Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected END TYPE after UDT Declaration: \"" + TypeName + "\"");
    exit(1);
    return ;
}


void Errors::ExpectedName(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Invalid Identifier: \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::AlreadyReservedInType(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Variable is already declared in type:  \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::ExpectedAssignment(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected an assignment operator following: \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::ExpectedArray(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Invalid array assignment.");
    exit(1);
    return ;
}


void Errors::ExpectedUDT(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected a User-Defined Type in assignment.");
    exit(1);
    return ;
}


void Errors::CanOnlyAssign(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("In this expression, you only assign, not add, subtract, etc. For example, with types, arrays, and classes, you can only assign.");
    exit(1);
    return ;
}


void Errors::ExpectedStringData(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected a string expression.");
    exit(1);
    return ;
}


void Errors::ExpectedNumberData(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected a number expression");
    exit(1);
    return ;
}


void Errors::ExpectedArrayData(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected an array expression.");
    exit(1);
    return ;
}


void Errors::TypesDoNotMatch(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("The array or User-Defined Type is an invalid type.");
    exit(1);
    return ;
}


void Errors::UnmatchedParenthesis(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Invalid parenthesis. Unable to evaluate the expression.");
    exit(1);
    return ;
}


void Errors::UnknownExpression(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Invalid expression.");
    exit(1);
    return ;
}


void Errors::UndeclaredVariable(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("" + CurrentWord + " was referenced before initialization.");
    exit(1);
    return ;
}


void Errors::ExpectedPeriod(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected a dot \".\" identifier after the User-Defined Type.");
    exit(1);
    return ;
}


void Errors::InvalidMember(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Invalid member: " + CurrentWord + ". Not found in the specified type.");
    exit(1);
    return ;
}


void Errors::UnableToAccessMember(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Unable to access memeber variables of User-Defined Types directly.");
    exit(1);
    return ;
}


void Errors::UnknownExpressionType(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Unable to determine expression type.");
    exit(1);
    return ;
}


void Errors::ExpectedEndingArrayParenthesis(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("No closing parenthesis found after array reference.");
    exit(1);
    return ;
}


void Errors::UnableToAccessItem(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Indexed accessing is not supported for this data type.");
    exit(1);
    return ;
}


void Errors::ExpectedParenthesis(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("No parenthesises found to access the array item(s).");
    exit(1);
    return ;
}


void Errors::ExpectedSeparatorForInput(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("No separator found after INPUT between string variable.");
    exit(1);
    return ;
}


void Errors::ExpectedThen(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected THEN after I. Found: \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::NoEndIf(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected EndIf after IF...THEN. \"END IF\" commandwas not found.");
    exit(1);
    return ;
}


void Errors::NoDo(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("DO condition not found after WHILE loop.");
    exit(1);
    return ;
}


void Errors::ExpectedParameters(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected a list of parameters, not \"" + CurrentWord + "\"If your function/sub has no parameters, use () .");
    exit(1);
    return ;
}


void Errors::AlreadyReservedInParameters(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("\"" + CurrentWord + "\" is already a reserved parameter.");
    exit(1);
    return ;
}


void Errors::NoEndToParameters(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("No closing parenthesis found after function parameters.");
    exit(1);
    return ;
}


void Errors::NoEndSubFunction(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("End Sub or End Function statement not found after a function.");
    exit(1);
    return ;
}


void Errors::MustEndIfStatementFirst(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("You must end any and all If statements before ending this sub or function.");
    exit(1);
    return ;
}


void Errors::MustEndWhileLoopFirst(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("While loop did not terminate before exiting the subfunction or function.");
    exit(1);
    return ;
}


void Errors::EndInWrongPlace(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("The END keyword can only appear in Subs,  Functions, or If statements.");
    exit(1);
    return ;
}


void Errors::MismatchedSubFunction(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("It appears that your sub has an End Function or your function has an End Sub - please correct this.");
    exit(1);
    return ;
}


void Errors::SubFunctionMustBeGlobal(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Cannot create a subfunction or function inside a subfunction, "
               "function, If statement, or While loop.");
    exit(1);
    return ;
}


void Errors::ExpectedNextParameter(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected another parameter after a comma in the parameter list.");
    exit(1);
    return ;
}


void Errors::InvalidLibrary(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Library names must be enclosed in quotation marks.");
    exit(1);
    return ;
}


void Errors::InvalidAlias(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Alias names must be enclosed in quotation marks.");
    exit(1);
    return ;
}


void Errors::InvalidCallingConvention(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Invalid calling convention - the calling convention may be either 'C' or 'STD'.");
    exit(1);
    return ;
}


void Errors::ExpectedExternalSubFunction(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Exptected an external sub or function. You cannot DECLARE user-defined subs or functions.");
    exit(1);
    return ;
}


void Errors::ExpectedSubFunction(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected Sub or Function after DECLARE or parameter(s) for CALLBACK/CODEPTR.");
    exit(1);
    return ;
}


void Errors::ExpectedFileNameAfterInclude(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected a string containing the filename of the BASIC file to include.");
    exit(1);
    return ;
}


void Errors::NoEndToAsm(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("No $End Asm statement was found before the end of the file.");
    exit(1);
    return ;
}


void Errors::ExpectedEndOfLineAfterAsm(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected an End-Of-Line after the $Asm statement.");
    exit(1);
    return ;
}


void Errors::InvalidDirective(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("This directive is invalid - valid directives include:$Asm, $AppType, $Include.");
    exit(1);
    return ;
}


void Errors::InvalidAppType(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("The AppType you choose is invalid - valid types are: Console, GUI, DLL.");
    exit(1);
    return ;
}


void Errors::CannotChangeAppType(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("You cannot change the AppType this far into the program, you must use $AppType before any other statement.");
    exit(1);
    return ;
}


void Errors::CannotOptimize(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("You cannot optimize an app this far into the program, you must use $Optimze before other statements.");
    exit(1);
    return ;
}


void Errors::ExpectedOnOrOff(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("The $Optimize and $Compress directives expect either ON or OFF, not \"" + CurrentWord + "\"");
    exit(1);
    return ;
}


void Errors::DefineMustBeIdentifier(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Defined words must be identifiers, not numbers or symbols.");
    exit(1);
    return ;
}


void Errors::NoEndToIfDef(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("There is a missing $End If to one or more of your $IfDef or $IfNDef statements.");
    exit(1);
    return ;
}


void Errors::CannotExitDirective(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("$End If not found after $IfDef or $IfNDef statements.");
    exit(1);
    return ;
}


void Errors::CannotModifyConstData(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Unable to modify or assign const data.");
    exit(1);
    return ;
}


void Errors::ExpectedEndAsm(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("End Asm not found after $ in an $Asm session.");
    exit(1);
    return ;
}


void Errors::ConstDataMustBeNumberOrString(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("$Const data must be a numeric or string literal.");
    exit(1);
    return ;
}


void Errors::CanOnlyNegateNumbers(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Negate operation is undefined for the string data type.");
    exit(1);
    return ;
}


void Errors::ExpectedVariable(Reading SourceFile) {
    Prepare(SourceFile);
    PrintError("Expected the parameter to be a variable.");
    exit(1);
    return ;
}

#endif // ERRORS_HPP