#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

// #include <string>


class Assembly {
public:
    Assembly() { LabelNumber = 0; TempVariable = 0; }

    void AddLibrary(std::string FunctionName);
    std::string GetLabel();
    void PostLabel(int Section, std::string Label);
    void BuildSkeleton();
    void FinishUp();

    void InitMemory();
    void GetPreSetVariables();
    void PrepareErrorMessages();

    void CreateInteger(std::string Name, std::string AsmName);
    void CreateDouble(std::string Name, std::string AsmName);
    void CreateString(std::string Name, std::string AsmName);

    void CreateIntegerArray(std::string Name, std::string AsmName);
    void CreateDoubleArray(std::string Name, std::string AsmName);
    void CreateStringArray(std::string Name, std::string AsmName);

    void StartCreatingType(std::string TypeName);
    void CreateTypeInteger(std::string Typename, std::string Name, std::string AsmName);
    void CreateTypeDouble(std::string Typename, std::string Name, std::string AsmName);
    void CreateTypeString(std::string Typename, std::string Name, std::string AsmName);
    void CreateUDT(std::string UDT, std::string Name, std::string AsmName);
    void FinishCreatingType(std::string TypeName);

    void LoadString(std::string String);
    void LoadNumber(std::string Number);
    void LoadUDTMember(std::string UDT, std::string TypeName, std::string Member, int Type);
    void LoadArrayItem(std::string Array, int Type);
    void LoadDefaults(int Type);

    void AllocMemory(int Section, std::string Size);
    void FreeMemory(int Section, std::string Name);
    void AllocStringArray(std::string AsmName);
    void FreeStringArray(std::string AsmName);
    void CopyArray(std::string Array);
    void CopyStringArray(std::string Array);
    void AddStrings();
    void GetStringLength(std::string String);
    void CopyUDT(std::string UDT);
    void CopyMemoryOfSize(std::string To, std::string From, std::string Size);
    void CopyString(std::string To, std::string From);
    void AppendString(std::string To, std::string From);
    void AssignIt(std::string Name);
    void AssignUDTMember(std::string UDT, std::string TypeName, std::string Member, int Type);
    void AssignArrayItem(std::string Array, int Type);

    void CalculateStart();
    void CalculateExp();
    void CalculateMultiply();
    void CalculateDivide();
    void CalculateMOD();
    void CalculateAddition();
    void CalculateSubtract();
    void CalculateEnd();

    void CompareNumbers();
    void LoadNumberRelation(int Relation);
    void CompareStrings();
    void LoadStringRelation(int Relation);

    void PushNumber(int Number);
    void PushAddress(std::string Name, int Type);

    void MOV(int Section, std::string WhereTo, std::string What);
    void PUSH(std::string What);
    void POP(std::string What);

    void RoundToInteger();
    void ConvertToString();
    void ConvertToNumber();
    void Negate();

    void InitConsole();
    void ConsoleSleep();
    void FormatTime();
    void ConsoleCls();
    void ConsolePrint(int Type);
    void ConsolePrintNewLine();
    void ConsoleInput();
    void EndProgram();

    void Or();
    void And();
    void Not();

    std::string StartIf();
    std::string StartNewIfSection(std::string Endlabel, std::string DoneIf);
    std::string ElseIf(std::string EndLabel, std::string DoneIf);
    std::string Else(std::string EndLabel, std::string DoneIf);
    void EndIf(std::string EndLabel, std::string DoneIf);

    std::string PrepareWhile();
    std::string StartWhile();
    void StopWhile(std::string StartWhileLabel, std::string EndWhileLabel);

    void CreateSubFunction(std::string Name, bool External);
    void EndCreateSubFunction(std::string Name, bool External);
    void InvokeSubFunction(std::string Name);

    void ReturnValue(std::string Name, std::string Type);
    void CleanUpReturnValue(std::string Name, std::string Type);
    void PushReturnValue(std::string Name, std::string Type);

    void LoadExternalSubFunction(std::string Name);
    void CallExternalSubFunction(std::string Name);

    void AllocateParameterPool(int Size);
    void AddToParameterPool(int Type, int Where);
    void PushParameterPool(int Type, int Where);
    void FreeParameterPool();
    void Callback(std::string Name);

    void AdjustStack(int Size);
    void OptimizeSubFunctions();

    enum Relations{Equal, NotEqual, Greater, Less, GreaterOrEqual, LessOrEqual};

private:
    // Track the number of temporary labels used
    int LabelNumber;

    // Track the number of variables stored in the .data section
    int TempVariable;
};


// AddLibrary() Adds a function (Windows API or a Linux CLibrary call) to the
// library section of our assembly so we can use that function in our program.
void Assembly::AddLibrary(std::string FunctionName) {
    // If the function is not already in the database, then import it.
    if (!Data.IsFunctionImported(FunctionName)) {
        Write.Line(Write.ToLibrary, "extern " + FunctionName);
    }
    return ;
}


// GetLabel() generates and returns the next available temporary label.
std::string Assembly::GetLabel() {
    std::string Result;

    // Get a different label
    ++LabelNumber;

    // Set the new label
    Result = "Label" + ToStr(LabelNumber);
    return Result;
}


// PostLabel() posts a label to the desired section of the assembly language.
void Assembly::PostLabel(int Section, std::string Label) {
    Write.Line(Section, Label + ":");
    return ;
}


// BuildSkeleton() adds all the assembly, tells the linker where the program
// starts, adds a default icon to the program, etc.
void Assembly::BuildSkeleton() {
    // If the OS is Windows, then use what developed earlier.
  #ifdef Windows
    // Initializes ExitProcess by adding it to the library
    AddLibrary("ExitProcess");

    // Load default icon
    Write.Line(Write.ToResource, "KoolB ICON \"./Asm/Default.ico\"");

    // Add .data and .text section
    Write.Line(Write.ToData,     "section .data");
    Write.Line(Write.ToFireUp,   "section .text");

    // Include assembly macros: ccall, stdcall
    Write.Line(Write.ToFireUp,   "%include \"./Asm/MACROS.INC\"");

    // Add START section, where program exection starts
    Write.Line(Write.ToFireUp,   "global START");
    PostLabel(Write.ToFireUp,    "START");

    // Reserve the command line
    Write.Line(Write.ToFireUp,   "PUSH EBP");
    Write.Line(Write.ToFireUp,   "MOV EBP,ESP");
    Write.Line(Write.ToFireUp,   "PUSH EBX");
    Write.Line(Write.ToFireUp,   "PUSH ESI");
    Write.Line(Write.ToFireUp,   "PUSH EDI");

    // Exit the program
    PostLabel(Write.ToFinishUp,  "Exit");
  #endif
}


// FinishUp() does the opposite of BuildSkeleton. It ensures KoolB app can
// shut down and exit properly. It is essentially a clean up function.
void Assembly::FinishUp() {
  #ifdef Windows
    // If Windows OS is not generating a DLL, then we need to destroy the heap.
    AddLibrary("HeapDestroy");
    if (AppType == Console or AppType == GUI) {
        Write.Line(Write.ToFinishUp, "stdcall HeapDestroy,dword[HandleToHeap]");
    }

    // Undo command line reservation from BuildSkeleton()
    Write.Line(Write.ToFinishUp, "POP EDI");
    Write.Line(Write.ToFinishUp, "POP ESI");
    Write.Line(Write.ToFinishUp, "POP EBX");
    Write.Line(Write.ToFinishUp, "MOV ESP,EBP");
    Write.Line(Write.ToFinishUp, "POP EBP");

    // Return the status
    if (AppType == Console or AppType == GUI) {
        Write.Line(Write.ToFinishUp, "MOV EAX,dword[ExitStatus]");
        Write.Line(Write.ToFinishUp, "stdcall ExitProcess,dword[ExitStatus]");
        Write.Line(Write.ToFinishUp, "RET");
    }

    // If this is a DLL, something went wrong. Exit immediately.
    if (AppType == DLL) {
        Write.Line(Write.ToFinishUp, "MOV EAX,1");

        // Return 12 to POP the 3 parameters off the stack
        Write.Line(Write.ToFinishUp, "RET 12");
    }
  #endif

    // Enable optimzation routine
    if (Optimize) { OptimizeSubFunctions(); }
    return ;
}


// InitMemory() creates a memory pool where the compiler can access variables
void Assembly::InitMemory() {
    // Our ParameterPool is a pool of memory for passing parameters to functions
    Write.Line(Write.ToData, "ParameterPool dd 0");

    // Functions we need to import if we are on Windows OS
    #ifdef Windows
        std::string LabelGotMemory = GetLabel();
        // Import from the Windows API to our library
        AddLibrary("HeapCreate");  // Creates a heap of memory
        AddLibrary("HeapAlloc");   // Allocates memory out of that heap
        AddLibrary("HeapFree");    // Frees memory from the heap
        AddLibrary("MessageBoxA"); // Alert MessageBox

        if (AppType == GUI or AppType == Console) {
            // Store the handle to the heap
            Write.Line(Write.ToData,   "HandleToHeap dd 0");

            // Allocate 16KB to the heap and allow it to resize
            Write.Line(Write.ToFireUp, "stdcall HeapCreate,0,16384,0");

            // Store the return value (EAX) in HandleToHeap
            Write.Line(Write.ToFireUp, "MOV dword[HandleToHeap],EAX");

            // Check if HandleToHeap is 0
            Write.Line(Write.ToFireUp, "CMP dword[HandleToHeap],0");

            // If the return value is not 0, then handle is valid
            Write.Line(Write.ToFireUp, "JNE " + LabelGotMemory);

            // If the return value was 0, then jump to NoMemory
            Write.Line(Write.ToFireUp, "JMP NoMemory");

            // Write the label to assembly
            PostLabel(Write.ToFireUp, LabelGotMemory);
        }
    #endif
    // Linux uses the standard system memory, so there would be no ifdef/endif
    // statement for this case.

    // Create TempQWord1 and TempQWord2, the floating-point numbers we use to
    // perform arithmetic operations
    Write.Line(Write.ToData, "TempQWord1 dq 0.0");
    Write.Line(Write.ToData, "TempQWord2 dq 0.0");

    // Set True flag to negative one
    Write.Line(Write.ToData, "True dd -1");
    return ;
}


// GetPreSetVariables() gets and sets variables necessary (for Windows OS) to
// initialize the program
void Assembly::GetPreSetVariables() {
    // Import Windows API functions to the compiler library
    #ifdef Windows
        SimpleDataInfo Info;
        AddLibrary("GetModuleHandleA"); // Gets the module handle
        AddLibrary("GetCommandLineA");  // Gets command line

        // Create data to store the instance and command lines
        Write.Line(Write.ToData,       "Internal_HInstance dd 0");
        Write.Line(Write.ToData,       "Internal_CommandLine dd 0");

        // Get the program handle
        Write.Line(Write.ToFireUp,     "stdcall GetModuleHandleA,0");
        Write.Line(Write.ToFireUp,     "MOV dword[Internal_HInstance],EAX");

        // Add the variable to the database
        Info.AsmName = "Internal_HInstance";
        Info.Type = Data.Integer; // The instance is an integer by default
        Data.AddSimpleData("HINSTANCE", Info);

        // If this is a console program, get the command line
        if (AppType == Console) {
            Write.Line(Write.ToFireUp, "CALL GetCommandLineA");
            Write.Line(Write.ToFireUp, "MOV dword[Internal_CommandLine],EAX");    

            // Store the command line command in the database
            Info.AsmName = "Internal_CommandLine";
            Info.Type = Data.String; // Command input is a string by default
            Data.AddSimpleData("COMMANDLINE$", Info);
        }
    #endif
    return ;
}


// PrepareErrorMessages() creates standard error messages
void Assembly::PrepareErrorMessages() {

    Write.Line(Write.ToData, "NoMemMessage db \"Could not allocate memory.\",0");
    Write.Line(Write.ToData, "NoLibFound db \"Could not find library: \",0");
    Write.Line(Write.ToData, "NoFunctionFound db \"Cound not find function: "
                                                     "\",0");
    Write.Line(Write.ToData, "Error db \"An error occured\",0");

    // Import Windows API error functions to the compiler library
    #ifdef Windows
        AddLibrary("MessageBoxA");
        AddLibrary("lstrcat");
        AddLibrary("lstrlen");
        AddLibrary("lstrcpy");
        PostLabel(Write.ToFunction,  "NoMemory");
        Write.Line(Write.ToFunction, "MOV dword[ExitStatus],1");
        Write.Line(Write.ToFunction, "stdcall MessageBoxA,0,NoMemMessage,Error,0");
        Write.Line(Write.ToFunction, "JMP Exit");
        Write.Line(Write.ToFunction, "");
        PostLabel(Write.ToFunction,  "NoLibrary");

        Write.Line(Write.ToFunction, "MOV dword[ExitStatus],1");
        Write.Line(Write.ToFunction, "POP EBX");
        Write.Line(Write.ToFunction, "stdcall lstrlen,EBX");
        Write.Line(Write.ToFunction, "ADD EAX,30");

        if (AppType == Console or AppType == GUI) {
            Write.Line(Write.ToFunction, "stdcall HeapAlloc,dword[HandleToHeap],8,EAX");
        } 
        if (AppType == DLL) {
            AddLibrary("GetProcessHeap");
            Write.Line(Write.ToFunction, "CALL GetProcessHeap");
            Write.Line(Write.ToFunction, "stdcall HeapAlloc,EAX,8,EAX");
        }

        Write.Line(Write.ToFunction, "MOV EDI,EAX");
        Write.Line(Write.ToFunction, "stdcall lstrcpy,EDI,NoLibFound");
        Write.Line(Write.ToFunction, "stdcall lstrcat,EDI,EBX");
        Write.Line(Write.ToFunction, "stdcall MessageBoxA,0,EDI,Error,0");

        if (AppType == Console or AppType == GUI) {
            Write.Line(Write.ToFunction, "stdcall HeapAlloc,dword[HandleToHeap],8,EDI");
        } 
        if (AppType == DLL) {
            Write.Line(Write.ToFunction, "CALL GetProcessHeap");
            Write.Line(Write.ToFunction, "stdcall HeapAlloc,EAX,8,EDI");
        }

        Write.Line(Write.ToFunction, "JMP Exit");
        Write.Line(Write.ToFunction, "");
        PostLabel(Write.ToFunction,  "NoFunction");
        Write.Line(Write.ToFunction, "MOV dword[ExitStatus],1");
        Write.Line(Write.ToFunction, "POP EBX");
        Write.Line(Write.ToFunction, "stdcall lstrlen,EBX");
        Write.Line(Write.ToFunction, "ADD EAX,30");

        if (AppType == Console or AppType == GUI) {
            Write.Line(Write.ToFunction, "stdcall HeapAlloc,dword[HandleToHeap],8,EAX");
        } 
        if (AppType == DLL) {
            Write.Line(Write.ToFunction, "CALL GetProcessHeap");
            Write.Line(Write.ToFunction, "stdcall HeapAlloc,EAX,8,EAX");
        }

        Write.Line(Write.ToFunction, "MOV EDI,EAX");
        Write.Line(Write.ToFunction, "stdcall lstrcpy,EDI,NoFunctionFound");
        Write.Line(Write.ToFunction, "stdcall lstrcat,EDI,EBX");


        if (AppType == DLL) {
            Write.Line(Write.ToFunction, "CALL GetProcessHeap");
            Write.Line(Write.ToFunction, "stdcall HeapAlloc,EAX,8,EDI");
        }
        Write.Line(Write.ToFunction, "JMP Exit");
    #endif
    // #ifdef Linux
    //     PostLabel(Write.ToFunction,  "NoMemory");
    //     Write.Line(Write.ToFunction, "MOV dword[ExitStatus],1");
    //     Write.Line(Write.ToFunction, "ccall puts,Error");
    //     Write.Line(Write.ToFunction, "ccall puts,NoMemMessage");
    //     Write.Line(Write.ToFunction, "JMP Exit");
    // #endif
    return ;
}


// CreateInteger() creates a 4 byte integer variable
void Assembly::CreateInteger(std::string Name, std::string AsmName) {
    // If we are inside a function, then create a local
    if (Data.IsInsideSubFunction()) {
        // Subtract the space we use on the stack
        Write.Line(Write.ToFireUp, "SUB ESP,4");

        // Write.Line(Write.ToFireUp, "MOV dword[ESP+4],0");
        // The RESULT variable is in ESP - 4.
        // if (Name == "RESULT") {
        //     // Write.Line(Write.ToFireUp, "MOV dword[EBP-4],0");
        // }

        // Determine scope in relation to EBP
        AsmName = "EBP" + Data.AddLocalVariable(Data.Integer);
    }
    // If this is a normal variable, then use the unique scope ID
    else {
        AsmName = Data.GetScopeID() + AsmName;
    }

    // Store the integer in database
    SimpleDataInfo Info;
    Info.AsmName = AsmName;
    Info.Type= Data.Integer;
    Data.AddSimpleData(Name, Info);

    // If we are not inside of a function. Add it to the .data section
    if (!Data.IsInsideSubFunction()) {
        Write.Line(Write.ToData, AsmName + " dd 0");
    }
    return ;
}


// CreateDouble() creates a 8 byte floating-point number
void Assembly::CreateDouble(std::string Name, std::string AsmName) {
    if (Data.IsInsideSubFunction()) {
        // Subtract the space we use on the stack
        Write.Line(Write.ToFireUp, "SUB ESP,8");
        Write.Line(Write.ToFireUp, "MOV dword[ESP+8],0");
        Write.Line(Write.ToFireUp, "MOV dword[ESP+4],0");

        // RESULT is in ESP - 8
        if (Name == "RESULT") {
            Write.Line(Write.ToFireUp, "MOV dword[EBP-4],0");            
            Write.Line(Write.ToFireUp, "MOV dword[EBP-8],0"); 
        }

        // Determine scope in relation to EBP
        AsmName = "EBP" + Data.AddLocalVariable(Data.Double);
    }
    else { // Use the unique scope ID if it is a normal variable
        AsmName = Data.GetScopeID() + AsmName;
    }

    // Store the Double in the database
    SimpleDataInfo Info;
    Info.AsmName = AsmName;
    Info.Type = Data.Double;
    Data.AddSimpleData(Name, Info);

    // If we are not inside of a function. Add it to the .data section
    if (!Data.IsInsideSubFunction()) {
        Write.Line(Write.ToData, AsmName + " dq 0.0");
    }
    return ;
}


// CreateString() creates a std::string variable
void Assembly::CreateString(std::string Name, std::string AsmName) {
    // If we are inside a function, create a local variable on the stack
    if (Data.IsInsideSubFunction()) {
        Write.Line(Write.ToFireUp, "SUB ESP,4");
        AsmName = "EBP" + Data.AddLocalVariable(Data.String);
    }
    else { // Use the unique scope ID
        AsmName = Data.GetScopeID() + AsmName;
    }

    // Store the string in the compiler database
    SimpleDataInfo Info;
    Info.AsmName = AsmName;
    Info.Type        = Data.String;
    Data.AddSimpleData(Name, Info);

    // If we are not inside a function, add the string to the .data section
    if (!Data.IsInsideSubFunction()) {
        Write.Line(Write.ToData,         AsmName + " dd 0");
    }

    // Allocate 1 byte of memory, and store an empty string in it
    // Note: This is how we can locate the end of a string
    AllocMemory(Write.ToFireUp,    "1");
    Write.Line(Write.ToFireUp,     "MOV dword[" + AsmName + "],EAX");
    Write.Line(Write.ToFireUp,     "MOV byte[EAX],0");

    // If we are in an function, free memory before returning
    if (Name != "RESULT" and Data.IsInsideSubFunction()) {
        FreeMemory(Write.ToFinishUp, "dword[" + Data.Asm(Name) + "]");
    }
    return ;
}


// CreateIntegerArray() creates an array of integers
void Assembly::CreateIntegerArray(std::string Name, std::string AsmName) {
    // Store the integer array in the database
    AsmName = Data.GetScopeID() + AsmName;
    ArrayDataInfo Info;
    Info.AsmName = AsmName;
    Info.Type = Data.Integer;
    Data.AddArrayData(Name, Info);

    // Create variables for the array and the array size
    Write.Line(Write.ToData,     AsmName + " dd 0");
    Write.Line(Write.ToData,     AsmName + "_Size dd 0");

    // Only integer sizes are valid for array sizes. Round any floating point
    // numbers found in the array size
    RoundToInteger();

    // Get the array size from the stack
    Write.Line(Write.ToMain,     "POP EBX");

    // Increment the size from 0 to 1
    Write.Line(Write.ToMain,     "INC EBX");

    // Store the array size (a pointer to the array's head)
    Write.Line(Write.ToMain,     "MOV dword[" + AsmName + "_Size],EBX");

    // Multiply the size by 4 to allocate enough space to store integers
    Write.Line(Write.ToMain,     "MOV EAX,4");
    Write.Line(Write.ToMain,     "MUL EBX");

    // Allocate memory to store all of the integers in the array
    AllocMemory(Write.ToMain,    "EAX");

    // The array handle will by default access its location in memory
    Write.Line(Write.ToMain,     "MOV dword[" + AsmName + "],EAX");

    // Free memory before exiting the program
    FreeMemory(Write.ToFinishUp, "dword[" + Data.Asm(Name) + "]");
    return ;
}


// CreateDoubleArray() creates an array of doubles
void Assembly::CreateDoubleArray(std::string Name, std::string AsmName) {
    // Add the double array to the database
    AsmName = Data.GetScopeID() + AsmName;
    ArrayDataInfo Info;
    Info.AsmName = AsmName;
    Info.Type = Data.Double;
    Data.AddArrayData(Name, Info);

    // Create variables for the array and the array size
    Write.Line(Write.ToData,         AsmName + " dd 0");
    Write.Line(Write.ToData,         AsmName + "_Size dd 0");
    RoundToInteger();

    // Get the array size from the stack
    Write.Line(Write.ToMain,     "POP EBX");

    // Increment the size from 0 to 1
    Write.Line(Write.ToMain,     "INC EBX");

    // Store the array size (a pointer to the array's head)
    Write.Line(Write.ToMain,     "MOV dword[" + AsmName + "_Size],EBX");

    // Multiple the size by 8 so there is enough space to store Doubles
    Write.Line(Write.ToMain,     "MOV EAX,8");
    Write.Line(Write.ToMain,     "MUL EBX");

    // Allocate memory to store all the doubles in the array
    AllocMemory(Write.ToMain,    "EAX");

    // The array handle will access its location in memory
    Write.Line(Write.ToMain,     "MOV dword[" + AsmName + "],EAX");

    // Free memory before exiting the program
    FreeMemory(Write.ToFinishUp, "dword[" + AsmName + "]");
    return ;
}


// CreateStringArray() creates an array of strings
void Assembly::CreateStringArray(std::string Name, std::string AsmName) {
    // Add the string array to the database
    AsmName = Data.GetScopeID() + AsmName;
    ArrayDataInfo Info;
    Info.AsmName = AsmName;
    Info.Type = Data.String;
    Data.AddArrayData(Name, Info);

    // Add the array name and array size
    Write.Line(Write.ToData,         AsmName + " dd 0");
    Write.Line(Write.ToData,         AsmName + "_Size dd 0");
    RoundToInteger();

    // Get the array size from the stack
    Write.Line(Write.ToMain,     "POP EBX");

    // Increment the stack from 0 to 1
    Write.Line(Write.ToMain,     "INC EBX");

    // Store a pointer to the array head
    Write.Line(Write.ToMain,     "MOV dword[" + AsmName + "_Size],EBX");

    // Multiply the size by 4 so there is enough space to store a string
    Write.Line(Write.ToMain,     "MOV EAX,4");
    Write.Line(Write.ToMain,     "MUL EBX");

    // Allocate memory to store all of the strings in the array
    AllocMemory(Write.ToMain,    "EAX");

    // The array name will access its location in memory, by deault
    Write.Line(Write.ToMain,     "MOV dword[" + AsmName + "],EAX");

    // Initialize the array with empty strings
    AllocStringArray(AsmName);

    // Free memory before exiting the program
    FreeStringArray(AsmName);
    return ;
}


// StartCreatingType()
void Assembly:: StartCreatingType(std::string TypeName) {
    TypeDataInfo Info;

    // Get the information name
    if (Mangle) {
        Info.AsmName = Data.GetScopeID() +Data.StripJunkOff(TypeName) + "_TYPE";
    }
    else {
        Info.AsmName = Data.GetScopeID() +Data.StripJunkOff(TypeName);
    }

    // Get the information size
    Info.Size = 0;

    // Store the type information in the database
    Data.AddTypeData(TypeName, Info);

    // Create a header for a STRUC
    Write.Line(Write.ToData, "STRUC " + Info.AsmName);
    return ;
}


// CreateTypeInteger() creates space for an unintialized integer
void Assembly::CreateTypeInteger(std::string TypeName, std::string Name, std::string AsmName) {
    // Add the integer information to the database
    AsmName = Data.GetScopeID() + AsmName;
    SimpleDataInfo Info;
    Info.AsmName = AsmName;
    Info.Type = Data.Integer;
    Data.AddSimpleDataToType(TypeName, Name, Info);

    // Declare unintialized storage space for the integer
    Write.Line(Write.ToData, "." + AsmName + " resd 1");
    return ;
}


// CreateTypeDouble() creates space for an unintialized double
void Assembly::CreateTypeDouble(std::string TypeName, std::string Name, std::string AsmName) {
    // Add the double information to the database
    AsmName = Data.GetScopeID() + AsmName;
    SimpleDataInfo Info;
    Info.AsmName = AsmName;
    Info.Type = Data.Double;
    Data.AddSimpleDataToType(TypeName, Name, Info);

    // Declare unintialized storage space for the integer
    Write.Line(Write.ToData, "." + AsmName + " resq 1");
    return ;
}


// CreateTypeString() creates space for an unintialized string
void Assembly::CreateTypeString(std::string TypeName, std::string Name, std::string AsmName) {
    // Add the string information to the database
    AsmName = Data.GetScopeID() + AsmName;
    SimpleDataInfo Info;
    Info.AsmName = AsmName;
    Info.Type = Data.String;
    Data.AddSimpleDataToType(TypeName, Name, Info);

    // Declare unintialized storage space for the integer
    Write.Line(Write.ToData, "." + AsmName + " resd 1");
    return ;
}


// CreateUDT() creates space for an instance of a User-Defined Type
void Assembly::CreateUDT(std::string UDT, std::string Name, std::string AsmName) {
    // Store the UDT and its member variable information in the database
    if (Mangle) {
        AsmName = Data.GetScopeID() + Data.StripJunkOff(Name) + "_UDT";
    }
    else {
        AsmName = Data.GetScopeID() + Data.StripJunkOff(Name);
    }
    Data.AddUDTData(UDT, Name, AsmName);

    // Initiliaze the STRUC in the .data section
    Write.Line(Write.ToData, AsmName +    " ISTRUC " + Data.Asm(UDT));
    Write.Line(Write.ToData, "IEND");
    return ;
}


// FinishCreatingType() marks the end of a TYPE declaration with ENDSTRUC
void Assembly::FinishCreatingType(std::string TypeName) {
    Write.Line(Write.ToData, "ENDSTRUC");
    return ;
}


// LoadString() loads a string expression into memory
void Assembly::LoadString(std::string String) {
    // Case where the string is a constant
    if (Data.GetDataType(String) != Data.String) {
        // Keep track of the temporary variable we need to hold the value
        ++TempVariable;

        // Declare the string in the .data section
        Write.Line(Write.ToData,  "String_" + ToStr(TempVariable) + " db \"" + String + "\",0");

        // Move the string into the EBX register
        Write.Line(Write.ToMain,  "MOV EBX,String_" + ToStr(TempVariable));

        // Get the length of the string
        GetStringLength("EBX");
        Write.Line(Write.ToMain,  "MOV EDI,EAX");

        // Allocate memory for the string
        AllocMemory(Write.ToMain, "EDI");
        Write.Line(Write.ToMain,  "MOV EBX,EAX");

        // Copy the string into memory
        CopyString("EBX", "String_" + ToStr(TempVariable));

        // Push the string on the stack
        Write.Line(Write.ToMain, "PUSH EBX");
        return ;
    }
    else {
        // Get the length of the string
        GetStringLength("dword[" + Data.Asm(String) + "]");
        Write.Line(Write.ToMain,    "MOV EBX,EAX");

        // Allocate memory for the string
        AllocMemory(Write.ToMain, "EBX");
        Write.Line(Write.ToMain,    "MOV EDI,EAX");

        // Copy the string into memory
        CopyString("EDI", "dword[" + Data.Asm(String) + "]");

        // Push the string on the stack
        Write.Line(Write.ToMain, "PUSH EAX");
        return ;
    }
}


// LoadNumber() loads either a number expression onto the stack
void Assembly::LoadNumber(std::string Number) {
    // Case where the number is a constant
    if (Data.GetDataType(Number) != Data.Number) {
        // Keep track of the temporary variable we need to hold the value
        ++TempVariable;

        // Check for a decimal point in the number
        if (Number.find(".", 0) != std::string::npos) {
            Write.Line(Write.ToData, "Number_"+ToStr(TempVariable)+ " dq 0" + Number);
        }
        else { // Add .0 to the end of the number
            Write.Line(Write.ToData, "Number_" + ToStr(TempVariable) + " dq " + Number + ".0");
        }
        // Push the double to the stack
        Write.Line(Write.ToMain,"PUSH dword[Number_" + ToStr(TempVariable) + "+4]");
        Write.Line(Write.ToMain,"PUSH dword[Number_" + ToStr(TempVariable) + "]");
    }
    else { // Load the variable to the stack
        // If we are dealing with an integer, then convert it to a double
        if (Data.GetSimpleData(Number).Type == Data.Integer) {
            // Initialize FPU after checking for pending unmasked floating-point exceptions
            Write.Line(Write.ToMain, "FINIT");

            // Load the integer
            Write.Line(Write.ToMain, "FILD dword[" + Data.Asm(Number) + "]");

            // Store the integer as a double
            Write.Line(Write.ToMain, "FST qword[TempQWord1]");

            // Push the double to the stack
            Write.Line(Write.ToMain, "PUSH dword[TempQWord1+4]");
            Write.Line(Write.ToMain, "PUSH dword[TempQWord1]");
        }
        else { // Push the double to the stack
            Write.Line(Write.ToMain, "PUSH dword[" + Data.Asm(Number) + "+4]");
            Write.Line(Write.ToMain, "PUSH dword[" + Data.Asm(Number) + "]");
        }
    }
    return ;
}


// LoadUDTMember() loads a UDT's data member to the stack
void Assembly::LoadUDTMember(std::string UDT, std::string TypeName,std::string Member, int Type) {
    // Get the assembly name of the member
    std::string MemberAsmName = Data.GetUDTData(UDT).Type.Members[Member].AsmName;

    // Check the data type of the member
    if (Data.GetUDTData(UDT).Type.Members[Member].Type == Data.String) {
        // Get the address of the string
        Write.Line(Write.ToMain,    "MOV ESI,dword[" + Data.Asm(UDT) + "+" + 
                             Data.Asm(TypeName) + "." + MemberAsmName + "]");
        // Get the length of the string
        GetStringLength("ESI");
        Write.Line(Write.ToMain,    "MOV EBX,EAX");

        // Allocate enough memory to store the string
        AllocMemory(Write.ToMain,   "EBX");
        Write.Line(Write.ToMain,    "MOV EDI,EAX");

        // Copy the UDT member string to the new string
        CopyString("EDI", "ESI");

        // Push the new string to the stack
        Write.Line(Write.ToMain,    "PUSH EAX");
    }
    else if (Data.GetUDTData(UDT).Type.Members[Member].Type == Data.Double) {
        // Push the double to the stack
        Write.Line(Write.ToMain,    "PUSH dword[" + Data.Asm(UDT) + "+" +
                   Data.Asm(TypeName) + "." + MemberAsmName + "+4]");
        Write.Line(Write.ToMain,    "PUSH dword[" + Data.Asm(UDT) + "+" +
                   Data.Asm(TypeName) + "." + MemberAsmName + "]");
    }
    else if (Data.GetUDTData(UDT).Type.Members[Member].Type == Data.Integer) {
        // Initialize the FPU
        Write.Line(Write.ToMain,    "FINIT");

        // Load the integer
        Write.Line(Write.ToMain,    "FILD dword[" + Data.Asm(UDT) + "+" +
                   Data.Asm(TypeName) + "." + MemberAsmName + "]");

        // Store the integer as a double
        Write.Line(Write.ToMain,    "FST qword[TempQWord1]");

        // Push the double to the stack
        Write.Line(Write.ToMain,    "PUSH dword[TempQWord1+4]");
        Write.Line(Write.ToMain,    "PUSH dword[TempQWord1]");
    }

    return ;
}


// LoadArrayItem() loads an item outside of the array
void Assembly::LoadArrayItem(std::string Array, int Type) {
    // These labels handle out of bounds checks and other basic array errors
    std::string OutOfBoundsLabel = GetLabel();
    std::string OKLabel = GetLabel();

    // Get the integer index of the item
    RoundToInteger();
    Write.Line(Write.ToMain, "POP EBX");

    // Check to make sure we got an actual value
    Write.Line(Write.ToMain,       "CMP EBX,dword[" + Data.Asm(Array) + "_Size]");
    Write.Line(Write.ToMain,       "JGE " + OutOfBoundsLabel);
    Write.Line(Write.ToMain,       "CMP EBX,0");
    Write.Line(Write.ToMain,       "JLE " + OutOfBoundsLabel);

    // Get the head (location in memory) of the array
    Write.Line(Write.ToMain,        "MOV EDI,dword[" + Data.Asm(Array) + "]");
    if (Type == Data.String) {
        // Get the address of the string in the array
        Write.Line(Write.ToMain,    "MOV ESI,dword[EDI+EBX*4]");

        // Get the length of the string
        GetStringLength("ESI");
        Write.Line(Write.ToMain,    "MOV EBX,EAX");

        // Allocate enough memory to store the string
        AllocMemory(Write.ToMain,   "EBX");
        Write.Line(Write.ToMain,    "MOV EBX,EAX");

        // Copy the string into memory
        CopyString("EBX", "ESI");

        // Push the string to the stack
        Write.Line(Write.ToMain,    "PUSH EBX");
    }
    else if (Type == Data.Double) {
        // Initiliaze the FPU (Floating-Point Processor)
        Write.Line(Write.ToMain,    "FINIT");

        // Load the double from the array
        Write.Line(Write.ToMain,    "FLD qword[EDI+EBX*8]");

        // Store the floating-point number
        Write.Line(Write.ToMain,    "FST qword[TempQWord1]");

        // Push the floating-point number to the stack
        Write.Line(Write.ToMain,    "PUSH dword[TempQWord1+4]");
        Write.Line(Write.ToMain,    "PUSH dword[TempQWord1]");
    }
    else if (Type == Data.Integer) {
        // Initiliaze the FPU (Floating-Point Processor)
        Write.Line(Write.ToMain,    "FINIT");

        // Load the integer
        Write.Line(Write.ToMain,    "FILD dword[EDI+EBX*4]");

        // Store the integer as a floating-point number
        Write.Line(Write.ToMain,    "FST qword[TempQWord1]");

        // Push the floating-point number to the stack
        Write.Line(Write.ToMain,    "PUSH dword[TempQWord1+4]");
        Write.Line(Write.ToMain,    "PUSH dword[TempQWord1]");
    }

    // Jump to the OK label, then check if we are out of bounds or have an error
    Write.Line(Write.ToMain,        "JMP " + OKLabel);
    PostLabel(Write.ToMain,         OutOfBoundsLabel);

    // If we discovered an error, then load default values
    LoadDefaults(Type);
    PostLabel(Write.ToMain,         OKLabel);
    return ;
}


// LoadDefaults() loads the default values if we are unable to locate the real
// values (for example, accessing an array out of bounds)
void Assembly::LoadDefaults(int Type) {
    if (Type == Data.String) {
        // Load an empty string as the default string
        AllocMemory(Write.ToMain, "1");
        Write.Line(Write.ToMain,  "MOV byte[EAX],0");
        Write.Line(Write.ToMain,  "PUSH EAX");
    }
    if (Type == Data.Double or Type == Data.Integer) {
        // Load zero as the default number
        Write.Line(Write.ToMain,  "FINIT");
        Write.Line(Write.ToMain,  "FLDZ");
        Write.Line(Write.ToMain,  "FST qword[TempQWord1]");
        Write.Line(Write.ToMain,  "PUSH dword[TempQWord1+4]");
        Write.Line(Write.ToMain,  "PUSH dword[TempQWord1]");
    }
    return ;
}


// AllocMemory() gets some memory to store something in
void Assembly::AllocMemory(int Section, std::string Size) {
    // Call HeapAlloc from the Windows API to get memory from the heap
    #ifdef Windows
        std::string MemoryOKLabel = GetLabel();
        AddLibrary("HeapAlloc");

        // If this is a GUI or Console program, call memory from the heap
        if (AppType == GUI or AppType == Console) {
            Write.Line(Section, "stdcall HeapAlloc,dword[HandleToHeap],8," + Size);
        }

        // If this is a DLL, call the heap and use it to allocate memory
        if (AppType == DLL) {
            AddLibrary("GetProcessHeap");
            Write.Line(Section, "CALL GetProcessHeap");
            Write.Line(Section, "stdcall HeapAlloc,EAX,8," + Size);
        }

        // Jump to error if we did not get the memory
        Write.Line(Section, "CMP EAX,0");
        Write.Line(Section, "JNE " + MemoryOKLabel);
        Write.Line(Section, "JMP NoMemory");
        PostLabel(Section,    MemoryOKLabel);
    #endif

    // For Linux: Get the memory from the system using malloc
    // #ifdef Linux
    //     std::string MemoryOKLabel = GetLabel();
    //     AddLibrary("malloc");     // Gets memory from the system
    //     Write.Line(Section, "ccall malloc," + Size);
    //     Write.Line(Section, "CMP EAX,0");
    //     Write.Line(Section, "JNE " + MemoryOKLabel);
    //     Write.Line(Section, "JMP NoMemory");
    //     PostLabel(Section,    MemoryOKLabel);
    // #endif
    return ;
}


// FreeMemory() frees the memory when it is no longer needed
void Assembly::FreeMemory(int Section, std::string Name) {
    // HeapFree from the Windows API releases the memory back to the heap
    #ifdef Windows
        AddLibrary("HeapFree");
        // GUI or Console apps use the heap we created earlier
        if (AppType == GUI or AppType == Console) {
            Write.Line(Section, "stdcall HeapFree,dword[HandleToHeap],0," + Name);
        }
        // DLLs use the calling program's heap
        if (AppType == DLL) {
            AddLibrary("GetProcessHeap");
            Write.Line(Section, "CALL GetProcessHeap");
            Write.Line(Section, "stdcall HeapFree,EAX,0," + Name);
        }
    #endif
    // Under Linux, we release the memory directly back to the system
    // #ifdef Linux
    //     AddLibrary("free");     // Release memory back to the system
    //     Write.Line(Section, "ccall free," + Name);
    // #endif
    return ;
}


// AllocStringArray() helps initialize a string array with empty strings
void Assembly::AllocStringArray(std::string AsmName) {
    // Get start and end points of the array
    std::string StartLabel = GetLabel();
    std::string EndLabel = GetLabel();

    // Start with the first array item
    Write.Line(Write.ToMain,     "MOV EBX,1");
    PostLabel(Write.ToMain,      StartLabel);

    // Create an empty string
    AllocMemory(Write.ToMain,    "1");

    // Get the address to store the string in
    Write.Line(Write.ToMain,     "MOV ECX,dword[" + AsmName + "]");

    // Store the address of the string in the array
    Write.Line(Write.ToMain,     "MOV dword[ECX+EBX*4],EAX");

    // Add a null-terimator to the end of the string
    Write.Line(Write.ToMain,     "MOV byte[EAX],0");

    // Advance to the next array item
    Write.Line(Write.ToMain,     "INC EBX");

    // Do a bounds check before looping back into the array
    Write.Line(Write.ToMain,     "CMP EBX,dword[" + AsmName + "_Size]");
    Write.Line(Write.ToMain,     "JLE " + StartLabel);
    PostLabel(Write.ToMain,      EndLabel);
    return ;
}


// FreeStringArray() frees all the strings in an array
void Assembly::FreeStringArray(std::string AsmName) {
    // Get start and end points of the array
    std::string StartLabel = GetLabel();
    std::string EndLabel = GetLabel();

    // Start with the first string (item) in the array
    Write.Line(Write.ToFinishUp,     "MOV EBX,1");
    PostLabel(Write.ToFinishUp,      StartLabel);

    // Get the address of the array
    Write.Line(Write.ToFinishUp,     "MOV ECX,dword[" + AsmName + "]");

    // Get the address of the array item by incrementing the address of the
    // the address of the array up 4 bytes
    Write.Line(Write.ToFinishUp,     "MOV EAX,dword[ECX+EBX*4]");

    // Free the string at the current array index
    FreeMemory(Write.ToFinishUp,     "EAX");

    // Go to the next index of the array
    Write.Line(Write.ToFinishUp,     "INC EBX");

    // Bounds check. Compare the current index to the size
    Write.Line(Write.ToFinishUp,     "CMP EBX,dword[" + AsmName + "_Size]");

    // If we are in bounds, and have not encountered a null-terminator, then
    // we can continue
    Write.Line(Write.ToFinishUp,     "JLE " + StartLabel);
    PostLabel(Write.ToFinishUp,      EndLabel);

    // Free the array from memory
    FreeMemory(Write.ToFinishUp,     "dword[" + AsmName + "]");
    return ;
}


// CopyArray() copies an array and all of its contents into another array
void Assembly::CopyArray(std::string Array) {
    // Get the address off the array size and store it in the ESI register
    Write.Line(Write.ToMain,         "MOV ESI,dword[" + Data.Asm(Array) + "_Size]");

    // If we have an array of doubles, the size of an item is 8 bytes
    // Note:
    // ESI is a Source index register, used for string and memory array copying
    if (Data.GetArrayData(Array).Type == Data.Double) {
        Write.Line(Write.ToMain,     "MOV EAX,8");
    }

    // Arrays of integers and doubles are 4 bytes per item
    if (Data.GetArrayData(Array).Type == Data.Integer or Data.GetArrayData(Array).Type == Data.String) {
        Write.Line(Write.ToMain,     "MOV EAX,4");        
    }

    // Calculate total array size (item_size * array_size)
    Write.Line(Write.ToMain,         "MUL ESI");
    Write.Line(Write.ToMain,         "MOV EBX,EAX");

    // Allocate memory for the new array
    AllocMemory(Write.ToMain,        "EBX");

    // For an array of strings, copy each string into the new array
    if (Data.GetArrayData(Array).Type == Data.String) {
        CopyStringArray(Array);
    }

    // For numeric types, copy memory into the new the array
    else {
        Write.Line(Write.ToMain,     "MOV EDI,EAX");
        CopyMemoryOfSize("EDI", "dword[" + Data.Asm(Array) + "]", "EBX");
    }

    // Push the new array and array size onto stack
    Write.Line(Write.ToMain,         "PUSH EDI");
    Write.Line(Write.ToMain,         "PUSH dword[" + Data.Asm(Array) + "_Size]");
    return ;
}


// CopyStringArray() helps copy an array of strings 
void Assembly::CopyStringArray(std::string Array) {
    // Get start and end points of the array
    std::string StartLabel = GetLabel();
    std::string EndLabel     = GetLabel();

    // Set EBX to zero so we can use it as a counter
    Write.Line(Write.ToMain,    "MOV EBX,0");

    // The EDI register contains our duplicate array to be filled
    Write.Line(Write.ToMain,    "MOV EDI,EAX");

    // Start the loop
    PostLabel(Write.ToMain,        StartLabel);

    // Get the address of the original array
    Write.Line(Write.ToMain,     "MOV ECX,dword[" + Data.Asm(Array) + "]");

    // Get the address of the string in the original array
    Write.Line(Write.ToMain,     "MOV EAX,dword[ECX+EBX*4]");

    // Get the current string length
    GetStringLength("EAX");

    // Allocate memory for the current string
    AllocMemory(Write.ToMain,    "ESI");

    // Move the address of the newly allocated memory into the new array
    Write.Line(Write.ToMain,     "MOV dword[EDI+EBX*4],EAX");

    // Get the address of the original array, again
    Write.Line(Write.ToMain,     "MOV ECX,dword[" + Data.Asm(Array) + "]");

    // Get the address of the item in the original array, again
    Write.Line(Write.ToMain,     "MOV ECX,dword[ECX+EBX*4]");

    // Copy the string from the original array to the newly allocated memory
    CopyMemoryOfSize("EAX", "ECX", "ESI");

    // Go onto the next array item
    Write.Line(Write.ToMain,     "INC EBX");

    // Out of bounds check. Terminates the loop before going out of bounds
    Write.Line(Write.ToMain,     "CMP EBX,dword[" + Data.Asm(Array) + "_Size]");
    Write.Line(Write.ToMain,     "JLE " + StartLabel);
    PostLabel(Write.ToMain,        EndLabel);
    return ;
}


// AddStrings() concatenates two strings into a single string
void Assembly::AddStrings() {
    // Get the address of the first string
    Write.Line(Write.ToMain,    "POP ESI");

    // Get the address of the first string's length
    GetStringLength("ESI");
    Write.Line(Write.ToMain,    "MOV EDI,EAX");

    // Get the address of the second string
    Write.Line(Write.ToMain,    "POP EBX");

    // Get the address of the second string's length
    GetStringLength("EBX");

    // Concatenate the strings
    Write.Line(Write.ToMain,    "ADD EAX,EDI");

    // Since we have two null-terminators, we can safely remove one
    Write.Line(Write.ToMain,    "DEC EAX");

    // Allocate memory for the new string to be stored
    AllocMemory(Write.ToMain,   "EAX");

    // Copy the first string to the memory allocated for the new string
    CopyString("EAX", "EBX");

    // Append the second string to the memory allocated for the new string
    AppendString("EAX", "ESI");

    // Push the new string to the stack
    Write.Line(Write.ToMain,    "PUSH EAX");

    // Free the two strings we concatenated
    FreeMemory(Write.ToMain,    "EBX");
    FreeMemory(Write.ToMain,    "ESI");
    return ;
}


// GetStringLength() gets the length of a string
void Assembly::GetStringLength(std::string String) {
    // Import lstrlenA from Windows API
    #ifdef Windows
        AddLibrary("lstrlenA");
        Write.Line(Write.ToMain, "stdcall lstrlenA," + String);

        // Increment EAX to leave space for a null-terminator
        Write.Line(Write.ToMain, "INC EAX");
    #endif
    // #ifdef Linux
    //     AddLibrary("strlen");         // Returns the length of a string
    //     Write.Line(Write.ToMain, "ccall strlen," + String);
    //     // Add room for the null terminator
    //     Write.Line(Write.ToMain, "INC EAX");
    // #endif
    return ;
}


// CopyUDT() copies a UDT (BUG: string values are not copied, but reflected)
void Assembly::CopyUDT(std::string UDT) {
    // Get the address of the UDT and the UDT size that we want to copy
    Write.Line(Write.ToMain,  "MOV EDI," + Data.Asm(Data.GetUDTData(UDT).TypeName) + "_size");

    // Allocate enough memory to store the new UDT
    AllocMemory(Write.ToMain, "EDI");

    // Copy the original UDT into the new UDT
    Write.Line(Write.ToMain,  "MOV EBX,EAX");

    // Copy the original UDT size into the new UDT size
    CopyMemoryOfSize("EBX", "" + Data.Asm(UDT), "EDI");

    // Push the new UDT to the stack
    Write.Line(Write.ToMain,  "PUSH EBX");
    return ;
}


// CopyMemoryOfSize() copies one memory block to another memory block
void Assembly::CopyMemoryOfSize(std::string To, std::string From, std::string Size) {
    #ifdef Windows
        // Windows API function to copy memory
        AddLibrary("RtlMoveMemory");
        Write.Line(Write.ToMain, "stdcall RtlMoveMemory," + To + "," + From + "," + Size);
        return ;
    #endif
    // #ifdef Linux
    //     AddLibrary("memcpy"); // Copies memory
    //     Write.Line(Write.ToMain, "ccall memcpy," + To + "," + From + "," + Size);
    //     return ;
    // #endif
    return ;
}


// CopyString() imports a library function to copy a string
void Assembly::CopyString(std::string To, std::string From) {
    // Windows API function to copy a string
    #ifdef Windows
        AddLibrary("lstrcpyA");
        Write.Line(Write.ToMain, "stdcall lstrcpyA,"+ To + "," + From);
        return ;
    #endif
    // #ifdef Linux
    //     AddLibrary("strcpy");
    //     Write.Line(Write.ToMain, "ccall strcpy,"+ To + "," + From);
    //     return ;
    // #endif
}


// AppendString() imports a library function to concatenate two strings
void Assembly::AppendString(std::string To, std::string From) {
    // Windows API function to concatenate two strings
    #ifdef Windows
        AddLibrary("lstrcatA");
        Write.Line(Write.ToMain, "stdcall lstrcatA,"+ To + "," + From);
        return ;
    #endif
    // #ifdef Linux
    //     AddLibrary("strcat");
    //     Write.Line(Write.ToMain, "ccall strcat,"+ To + "," + From);
    //     return ;
    // #endif
}


// AssignIt() assigns a variable the result of an expression
void Assembly::AssignIt(std::string Name) {
    // Case where the compiler is assigning a array
    if (Data.GetDataType(Name) == Data.Array) {
        // Pop the array and array size from the stack
        Write.Line(Write.ToMain, "POP EBX");
        Write.Line(Write.ToMain, "POP ESI");

        // Store the array and array size
        Write.Line(Write.ToMain, "MOV dword[" + Data.Asm(Name) + "],ESI");
        Write.Line(Write.ToMain, "MOV dword[" + Data.Asm(Name) + "_Size],EBX");
    }

    // Case where the compiler is assigning a User-Defined Type
    if (Data.GetDataType(Name) == Data.UDT) {
        // Pop the UDT from the stack
        Write.Line(Write.ToMain, "POP EBX");

        // Copy the UDT, its size, and its members over
        CopyMemoryOfSize("dword[" + Data.Asm(Name) + "]", "EBX", 
                          Data.Asm(Data.GetUDTData(Name).TypeName) + "_size");

        // Free the UDT from memory
        FreeMemory(Write.ToMain, "EBX");
    }

    // Case where the compiler is assigning a string
    if (Data.GetDataType(Name) == Data.String) {
        // Pop the string from the stack
        Write.Line(Write.ToMain, "POP EBX");

        // Free the original string from memory
        FreeMemory(Write.ToMain, "dword[" + Data.Asm(Name) + "]");

        // Store the new string
        Write.Line(Write.ToMain, "MOV dword[" + Data.Asm(Name) + "],EBX");
        return ;
    }

    // Case where the compiler is assigning a numeric type
    if (Data.GetDataType(Name) == Data.Number) {
        // If the numeric type is double
        if (Data.GetSimpleData(Name).Type == Data.Double) {
            // Pop the double off the stack
            Write.Line(Write.ToMain, "POP dword[TempQWord1]");
            Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

            // Initialize the FPU
            Write.Line(Write.ToMain, "FINIT");

            // Load the double
            Write.Line(Write.ToMain, "FLD qword[TempQWord1]");

            // Then store it in the variable
            Write.Line(Write.ToMain, "FST qword[" + Data.Asm(Name) + "]");
        }

        // If the numeric type is integer
        if (Data.GetSimpleData(Name).Type == Data.Integer) {
            // Round the number to an integer
            RoundToInteger();

            // Pop the integer off the stack
            Write.Line(Write.ToMain, "POP dword[TempQWord1]");

            // Initialize the FPU
            Write.Line(Write.ToMain, "FINIT");   

            // Load the integer as a double
            Write.Line(Write.ToMain, "FILD dword[TempQWord1]");

            // Store the double
            Write.Line(Write.ToMain, "FIST dword[" + Data.Asm(Name) + "]");
        }
    }
    return ;
}


// AssignUDTMember() assigns a member variable to a User-Defined Type
void Assembly::AssignUDTMember(std::string UDT, std::string TypeName,
                               std::string Member, int Type) {
    // Get the assembly language name of the member variable
    std::string MemberAsmName = Data.GetUDTData(UDT).Type.Members[Member].AsmName;

    // Case where the member variable is a string
    if (Type == Data.String) {
        // Free the original string from memory
        FreeMemory(Write.ToMain, "dword["+Data.Asm(UDT)+"+"+Data.Asm(TypeName)+"."+MemberAsmName+"]");

        // Get the new string
        Write.Line(Write.ToMain, "POP ESI");

        // Store the string in the UDT
        Write.Line(Write.ToMain, "MOV dword["+Data.Asm(UDT)+"+"+Data.Asm(TypeName)+"."+MemberAsmName+"],ESI");
    }

    // Case where the member variable is a double
    if (Type == Data.Double) {
        // Store the double in the UDT
        Write.Line(Write.ToMain, "POP dword["+Data.Asm(UDT)+"+"+Data.Asm(TypeName)+"."+MemberAsmName+"]");
        Write.Line(Write.ToMain, "POP dword["+Data.Asm(UDT)+"+"+Data.Asm(TypeName)+"."+MemberAsmName+"+4]");
    }

    // Case where the member variable is an integer
    if (Type == Data.Integer) {
        // Round the number on the stack to an integer
        RoundToInteger();

        // Pop the integer off the stack
        Write.Line(Write.ToMain, "POP dword[TempQWord1]");

        // Initialize the FPU
        Write.Line(Write.ToMain, "FINIT");

        // Load the integer as a double
        Write.Line(Write.ToMain, "FILD dword[TempQWord1]");

        // And store the double
        Write.Line(Write.ToMain, "FIST dword["+Data.Asm(UDT)+"+"+Data.Asm(TypeName)+"."+MemberAsmName+"]");
    }
    return ;
}


// AssignArrayItem() assigns an item to an array
void Assembly::AssignArrayItem(std::string Array, int Type) {
    // Label for Out Of Bounds Error
    std::string OutOfBoundsLabel = GetLabel();

    // Case where we pop a string off of the stack
    if (Type == Data.String) {
        Write.Line(Write.ToMain, "POP ESI");
    }

    // Case where we pop a numeric type off of the stack
    if (Type == Data.Double or Type == Data.Integer) {
        Write.Line(Write.ToMain, "POP dword[TempQWord1]");
        Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");
    }

    // Round the array index to an integer
    RoundToInteger();

    // Get the array index that we are assigning a variable to
    Write.Line(Write.ToMain,     "POP EBX");

    // If the index is out of bounds, report an error
    Write.Line(Write.ToMain,     "CMP EBX,dword[" + Data.Asm(Array) + "_Size]");
    Write.Line(Write.ToMain,     "JGE " + OutOfBoundsLabel);
    Write.Line(Write.ToMain,     "CMP EBX,0");
    Write.Line(Write.ToMain,     "JLE " + OutOfBoundsLabel);

    // Case where the array item being assigned is a string
    if (Type == Data.String) {
        // Get the address of the array
        Write.Line(Write.ToMain, "MOV EDI,dword[" + Data.Asm(Array) + "]");

        // Free the old string from memory
        FreeMemory(Write.ToMain, "dword[EDI+EBX*4]");

        // Store the string at the array indexy
        Write.Line(Write.ToMain, "MOV dword[EDI+EBX*4],ESI");
    }

    // Case where the array item being assigned is a double
    if (Type == Data.Double) {
        // Get the address of the array
        Write.Line(Write.ToMain, "MOV EDI,dword[" + Data.Asm(Array) + "]");

        // Initialize the FPU
        Write.Line(Write.ToMain, "FINIT");

        // Load the double
        Write.Line(Write.ToMain, "FLD qword[TempQWord1]");

        // Store the double at the array index
        Write.Line(Write.ToMain, "FST qword[EDI+EBX*8]");
    }

    // Case where the array item being assigned is an integer
    if (Type == Data.Integer) {
        // Push the number to the stack
        Write.Line(Write.ToMain, "PUSH dword[TempQWord1+4]");
        Write.Line(Write.ToMain, "PUSH dword[TempQWord1]");

        // Round the number on the stack to an integer
        RoundToInteger();

        // Pop the integer off the stack
        Write.Line(Write.ToMain, "POP dword[TempQWord1]");

        // Initialize the FPU
        Write.Line(Write.ToMain, "FINIT");

        // Load the integer as a double (floating-point)
        Write.Line(Write.ToMain, "FILD dword[TempQWord1]");

        // Store the double at the array index
        Write.Line(Write.ToMain, "FIST dword[EDI+EBX*4]");
    }

    // Report any errors that occurred
    PostLabel(Write.ToMain, OutOfBoundsLabel);
    return ;
}


// CalculateStart() is called at the beginning of a binary operation on doubles
void Assembly::CalculateStart() {
    // Pop the first double from the stack and store it
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

    // Pop the second double from the stack and store it
    Write.Line(Write.ToMain, "POP dword[TempQWord2]");
    Write.Line(Write.ToMain, "POP dword[TempQWord2+4]");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load the two doubles
    Write.Line(Write.ToMain, "FLD qword[TempQWord1]");
    Write.Line(Write.ToMain, "FLD qword[TempQWord2]");
    return ;
}


// CalculateExp() raises on double to the power of another double
void Assembly::CalculateExp() {
    CalculateStart();

    // Import external library function for computing powers
    AddLibrary("pow");
    Write.Line(Write.ToMain, "ccall pow,dword[TempQWord2],dword[TempQWord2+4],"
                                      "dword[TempQWord1],dword[TempQWord1+4]");
    CalculateEnd();
    return ;
}


// CalculateMultiply() computes the product of two doubles in the FPU
void Assembly::CalculateMultiply() {
    CalculateStart();

    // Multiply the two floats together
    Write.Line(Write.ToMain, "FMUL ST0,ST1");
    CalculateEnd();
    return ;
}


// CalculateDivide() computers the quotient of two doubles in the FPU
void Assembly::CalculateDivide() {
    CalculateStart();

    // Divide the two floats
    Write.Line(Write.ToMain, "FDIV ST0,ST1");
    CalculateEnd();
    return ;
}


// CalculateMOD() applies the fractional modulus operator to two doubles.
void Assembly::CalculateMOD() {
    CalculateStart();

    // Import external library function
    AddLibrary("fmod");
    Write.Line(Write.ToMain, "ccall fmod,dword[TempQWord2],dword[TempQWord2+4],"
                                                     "dword[TempQWord1],dword[TempQWord1+4]");
    CalculateEnd();
    return ;
}


// CalculateAddition() computes the sum of two doubles in the FPU
void Assembly::CalculateAddition() {
    CalculateStart();

    // Add the two floating-point numbers
    Write.Line(Write.ToMain, "FADD ST0,ST1");
    CalculateEnd();
    return ;
}


// CalculateSubtract() computes the difference between two doubles in the FPU
void Assembly::CalculateSubtract() {
    CalculateStart();

    // Subtract ST1 from ST0
    Write.Line(Write.ToMain, "FSUB ST0,ST1");
    CalculateEnd();
    return ;
}


// CalculateEnd() pushes the result of a calculation to the stack
void Assembly::CalculateEnd() {
    Write.Line(Write.ToMain, "MOV EAX,dword[TempQWord1]");
    Write.Line(Write.ToMain, "FST qword[TempQWord1]");
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1+4]");
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1]");
    return ;
}


// CompareNumbers() Compare two numbers
void Assembly::CompareNumbers() {
    CalculateStart();

    // Compare the two numbers
    Write.Line(Write.ToMain, "FCOM ST1");

    // Store the status word in the AX register
    Write.Line(Write.ToMain, "FSTSW AX");

    // Wait for operation to complete
    Write.Line(Write.ToMain, "WAIT");

    // Store AX as a flag for use with the normal JMP operators
    Write.Line(Write.ToMain, "SAHF");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load true as the value value, that is the default value
    Write.Line(Write.ToMain, "FILD dword[True]");

    return ;
}


// LoadNumberRelation() adds a Jump if command in assembly based after
// evaluating of the number relation
void Assembly::LoadNumberRelation(int Relation) {
    std::string True = GetLabel();

    // JE - Jump if Equal
    if (Relation == Equal) {
        Write.Line(Write.ToMain, "JE " + True);
    }

    // JNE - Jump if Not Equal
    if (Relation == NotEqual) {
        Write.Line(Write.ToMain, "JNE " + True);   
    }

    // JA - Jump if Above
    if (Relation == Greater) {
        Write.Line(Write.ToMain, "JA " + True);
    }

    // JB - Jump if Below
    if (Relation == Less) {
        Write.Line(Write.ToMain, "JB " + True);
    }

    // JAE - Jump if Above or Equal
    if (Relation == GreaterOrEqual) {
        Write.Line(Write.ToMain, "JAE " + True);
    }

    // JBE - Jump if Below or Equal
    if (Relation == LessOrEqual) {
        Write.Line(Write.ToMain, "JBE " + True);
    }

    // Load zero if we did not jump out of the current sequence
    Write.Line(Write.ToMain,     "FLDZ");

    // Write the result of the comparison in assembly language
    PostLabel(Write.ToMain,      True);

    // Push the result of the comparison on to the stack
    CalculateEnd();
    return ;
}


// CompareStrings() compares two strings 
void Assembly::CompareStrings() {
    // Get both strings we need to compare from the stack
    Write.Line(Write.ToMain, "POP EBX");
    Write.Line(Write.ToMain, "POP EDI");

    // Import Windows API function lstrcpmA
    #ifdef Windows
        AddLibrary("lstrcmpA");
        Write.Line(Write.ToMain, "stdcall lstrcmpA,EDI,EBX");
    #endif
    // If we are using Linux, we will call strcmp
    // #ifdef Linux
    //     AddLibrary("strcmp");
    //     Write.Line(Write.ToMain, "ccall strcmp,EDI,EBX");
    // #endif

    // Store the return value
    Write.Line(Write.ToMain, "MOV ESI,EAX");

    // Free the strings we compared
    FreeMemory(Write.ToMain, "EBX");
    FreeMemory(Write.ToMain, "EDI");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load true as the default value
    Write.Line(Write.ToMain, "FILD dword[True]");

    // Compare the the resulting value of the comparison to 0
    Write.Line(Write.ToMain, "CMP ESI,0");

    return ;
}


// LoadStringRelation() adds a Jump if statement command after evaluating the
// string relation
void Assembly::LoadStringRelation(int Relation) {
    std::string True = GetLabel();

    // JE - Jump if Equal
    if (Relation == Equal) {
        Write.Line(Write.ToMain, "JE " + True);
    }

    // JNE - Jump if NotEqual
    if (Relation == NotEqual) {
        Write.Line(Write.ToMain, "JNE " + True);   
    }

    // JG - Jump if Greater
    if (Relation == Greater) {
        Write.Line(Write.ToMain, "JG " + True);
    }

    // JL - Jump if Less
    if (Relation == Less) {
        Write.Line(Write.ToMain, "JL " + True);
    }

    // JGE - Jump if GreaterOrEqual
    if (Relation == GreaterOrEqual) {
        Write.Line(Write.ToMain, "JGE " + True);
    }

    // JLE - Jump if LessOrEqual
    if (Relation == LessOrEqual) {
        Write.Line(Write.ToMain, "JLE " + True);
    }

    // Load zero if we did not jump out of the current sequence
    Write.Line(Write.ToMain,     "FLDZ");

    // Write the result of the comparison in assembly language
    PostLabel(Write.ToMain,      True);

    // Push the result of the comparison on to the stack
    CalculateEnd();
    return ;
}


// PushNumber() pushes a constant numeric expression to the stack
void Assembly::PushNumber(int Number) {
    // Convert the constant numeric expression to a string and store it
    Write.Line(Write.ToMain, "MOV dword[TempQWord1]," + ToStr(Number));

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load the integer as a double
    Write.Line(Write.ToMain, "FILD dword[TempQWord1]");

    // Store the double
    Write.Line(Write.ToMain, "FSTP qword[TempQWord1]");

    // Push the double to the stack
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1+4]");
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1]");
    return ;
}


// PushAddress() pushes the address of a variable to the stack
void Assembly::PushAddress(std::string Name, int Type) {
    // Push the address of the numeric expression to the stack
    if (Type == Data.Number) {
        // If we are inside a sub or function, then we need its address, which
        // is located on the stack
        if (Data.IsInsideSubFunction()) {
            // Get the assembly language name of the number
            std::string AsmName = Data.Asm(Name);

            // Find the index of the hyphen
            int Pos = Data.Asm(Name).find("-", 0);
            if (Pos != std::string::npos) {
                // Get the number from the assembly language
                std::string Temp = AsmName.substr(Pos+1, Name.length());

                // Move the address of EBP
                Write.Line(Write.ToMain, "MOV EAX,EBP");

                // Subtract this from the EBP address
                Write.Line(Write.ToMain, "SUB EAX," + Temp);
                PUSH("EAX");
            }
            else {
            // If there is a parameter, get the index of the "+" character
            int Pos = Data.Asm(Name).find("+", 0);
                if (Pos != std::string::npos) {
                    std::string Temp = AsmName.substr(Pos +1, Name.length());
                    Write.Line(Write.ToMain, "MOV EAX,EBP");
                    Write.Line(Write.ToMain, "ADD EAX," + Temp);
                    PUSH("EAX");
                }
                else {
                    // If an error happened or input was invalid, push a zero
                    PushNumber(0);
                }
            }
        }
        else { // If its not a sub or function, then push the name to the stack
            PUSH(Data.Asm(Name));
        }
    }
    // If the type is a string, push the address to the stack
    if (Type == Data.String) {
        PUSH("dword[" + Data.Asm(Name) + "]");
    }
    // If the type is a udt, push the address to the stack
    if (Type == Data.UDT) {
        PUSH(Data.Asm(Name));
    }
    // If the type is a subfunction, push the address to the stack
    if (Type == Data.SubFunction) {
        PUSH(Data.Asm(Name));   
    }

    // Pop the address of the temp variable
    POP("dword[TempQWord1]");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load the integer as a double
    Write.Line(Write.ToMain, "FILD dword[TempQWord1]");

    // Store the double
    Write.Line(Write.ToMain, "FST qword[TempQWord1]");

    // Push the double to the stack
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1+4]");
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1]");
    return ;
}


// MOV() copies data from one location to another
void Assembly::MOV(int Section, std::string WhereTo, std::string What) {
    Write.Line(Section, "MOV " + WhereTo + "," + What);
    return ;
}


// PUSH() pushes data to the top of the stack
void Assembly::PUSH(std::string What) {
    Write.Line(Write.ToMain, "PUSH " + What);
    return ;
}


// POP() pops data from the top of the stack
void Assembly::POP(std::string What) {
    Write.Line(Write.ToMain, "POP " + What);
    return ;
}


// RoundToInteger() applies the floor function to a double/floating-point value
void Assembly::RoundToInteger() {
    // Add the external library function for floor
    AddLibrary("floor");

    // Pop the double off the stack
    Write.Line(Write.ToMain, "POP dword[TempQWord2]");
    Write.Line(Write.ToMain, "POP dword[TempQWord2+4]");

    // Apply the floor function to the double
    Write.Line(Write.ToMain, "ccall floor,dword[TempQWord2],dword[TempQWord2+4]");

    // Store the double as an integer
    Write.Line(Write.ToMain, "FIST dword[TempQWord2]");

    // Push the integer to the stack
    Write.Line(Write.ToMain, "PUSH dword[TempQWord2]");
    return ;
}


// ConvertToString() convert a double to a string
// Note: The number 3 will be converted '3.', so we must remove the trailing
// decimal in the case where the number should not have a decimal (ex: 3.14)
void Assembly::ConvertToString() {
    // Prepare labels
    std::string StartLabel = GetLabel();
    std::string FoundLabel = GetLabel();
    std::string EndLabel = GetLabel();

    // Pop the double off the stack
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

    // Allocate 100 bytes of memory to store 100 characters
    AllocMemory(Write.ToMain, "100");
    Write.Line(Write.ToMain, "MOV EBX,EAX");

    // Import _gcvt from the Windows API
    #ifdef Windows
        AddLibrary("_gcvt");
        Write.Line(Write.ToMain, "ccall _gcvt,dword[TempQWord1],"
                                                         "dword[TempQWord1+4],50,EBX");
    #endif
    // Under Linux, call gcvt
    // #ifdef Linux
    //     AddLibrary("gcvt");
    //     Write.Line(Write.ToMain, "ccall gcvt,dword[TempQWord1],"
    //                                                      "dword[TempQWord1+4],50,EBX");
    // #endif

    // Get the length of the string
    GetStringLength("EBX");
    Write.Line(Write.ToMain, "MOV EDI,EAX");

    // Add a null-terminator to mark the end of the string
    Write.Line(Write.ToMain, "MOV ECX,0");

    // Remove trailing periods and write the start label
    PostLabel(Write.ToMain,    StartLabel);

    // Loop until we find the null-terminator or beginning of the string
    Write.Line(Write.ToMain, "CMP byte[EBX+ECX],0");
    Write.Line(Write.ToMain, "JE " + FoundLabel);
    Write.Line(Write.ToMain, "INC ECX");
    Write.Line(Write.ToMain, "CMP ECX,EDI");
    Write.Line(Write.ToMain, "JL " + StartLabel);
    Write.Line(Write.ToMain, "JMP " + EndLabel);
    PostLabel(Write.ToMain,  FoundLabel);

    // Now that we have found the null-terminator, check for a trailing decimal
    Write.Line(Write.ToMain, "DEC ECX");
    Write.Line(Write.ToMain, "CMP byte[EBX+ECX],\'.\'");
    Write.Line(Write.ToMain, "JNE " + EndLabel);

    // If we do have <number><period><null-terminator>, replace the period with 
    // a second null-terminator
    Write.Line(Write.ToMain, "MOV byte[EBX+ECX],0");
    PostLabel(Write.ToMain,    EndLabel);

    // And return the string
    Write.Line(Write.ToMain, "PUSH EBX");
    return ;
}


// ConvertToNumber() converts a string to a number
void Assembly::ConvertToNumber() {
    // Import external library function
    AddLibrary("atof");

    // Pop the string from the stack
    Write.Line(Write.ToMain, "POP EBX");

    // Convert the string to the number
    Write.Line(Write.ToMain, "ccall atof,EBX");

    // Free the string from memory
    FreeMemory(Write.ToMain, "EBX");

    // Store the floating-point number
    Write.Line(Write.ToMain, "FST qword[TempQWord1]");

    // Push the floating-point (double) to the stack
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1+4]");
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1]");
    return ;
}


// Negate() assigns a number the oposite parity.
void Assembly::Negate() {
    // Pop the double off the stack
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load the double
    Write.Line(Write.ToMain, "FLD qword[TempQWord1]");

    // Change the parity
    Write.Line(Write.ToMain, "FCHS");

    // Store the double
    Write.Line(Write.ToMain, "FST qword[TempQWord1]");

    // Push the double to the stack
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1+4]");
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1]");
    return ;
}


// InitConsole() starts the the console
void Assembly::InitConsole() {
    // Need some variables
    Write.Line(Write.ToData,         "NoConsoleMessage db \'Error - Cannot access the "
                                     "console handles for Input/Output.\',0");
    Write.Line(Write.ToData,         "ConsoleTemp dd 0");

    // Add a newline character after each call to PRINT
    Write.Line(Write.ToData,         "ConsoleNewLine db 10,0");

    // Initialize console
    #ifdef Windows
        // Add a few commans to clear, read from, and write to the screen
        Write.Line(Write.ToData,     "ConsoleClear db \'CLS\',0");
        Write.Line(Write.ToData,     "HandleToInput dd 0");
        Write.Line(Write.ToData,     "HandleToOutput dd 0");

        // Initialize data for the console to run
        if (AppType == Console) {
            // Add labels that we can jump to
            std::string InputOKLabel= GetLabel();
            std::string OutputOKLabel = GetLabel();

            // Import Windows API function that retrieves a handle to the
            // specified standard device
            AddLibrary("GetStdHandle");

            // Store the read handle
            Write.Line(Write.ToFireUp,   "stdcall GetStdHandle,-10");
            Write.Line(Write.ToFireUp,   "MOV dword[HandleToInput],EAX");

            // Store the write handle
            Write.Line(Write.ToFireUp,   "stdcall GetStdHandle,-11");
            Write.Line(Write.ToFireUp,   "MOV dword[HandleToOutput],EAX");

            // Verify the read handle
            Write.Line(Write.ToFireUp,   "CMP dword[HandleToInput],-1");
            Write.Line(Write.ToFireUp,   "JNE " + InputOKLabel);
            Write.Line(Write.ToFireUp,   "JMP NoConsole");
            PostLabel(Write.ToFireUp,     InputOKLabel);

            // Verify the write handle
            Write.Line(Write.ToFireUp,   "CMP dword[HandleToOutput],-1");
            Write.Line(Write.ToFireUp,   "JNE " + OutputOKLabel);
            Write.Line(Write.ToFireUp,   "JMP NoConsole");       
            PostLabel(Write.ToFireUp,     OutputOKLabel);

            // Report an error if we could not verify the read/write handles
            PostLabel(Write.ToFunction,  "NoConsole");
            Write.Line(Write.ToFunction, "MOV dword[ExitStatus],1");
            Write.Line(Write.ToFunction, "stdcall MessageBoxA,0,NoConsoleMessage,Error,0");
            Write.Line(Write.ToFunction, "JMP Exit");
        }
    #endif
    // Linux uses "clear" to clear the console
    // #ifdef Linux
    //     // The clear screen command is not cls, but clear under Linux
    //     Write.Line(Write.ToData,     "ConsoleClear db \'clear\',0");
    // #endif    
}


// ConsoleSleep() Pauses the program for a set amount of seconds
void Assembly::ConsoleSleep() {
    #ifdef Windows
        // Convert seconds to milliseconds
        FormatTime();
        AddLibrary("Sleep");       // Pause execution for X milliseconds

        // Get the milliseconds to sleep
        Write.Line(Write.ToMain, "POP EBX");

        // Now pause execution
        Write.Line(Write.ToMain, "stdcall Sleep,EBX");
    #endif
    // #ifdef Linux
    //     // With Linux, we can only sleep for seconds
    //     AddLibrary("sleep");    // Pauses execution for X seconds
    //     // So we will just round the number to an integer
    //     RoundToInteger();
    //     // Get the number
    //     Write.Line(Write.ToMain, "POP EBX");
    //     // Now pause execution
    //     Write.Line(Write.ToMain, "ccall sleep,EBX");
    // #endif
    return ;
}


// FormatTime() converts seconds into milliseconds
void Assembly::FormatTime() {
    #ifdef Windows
        // Pop the double off the stack
        Write.Line(Write.ToMain, "POP dword[TempQWord1]");
        Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

        // Store 1000 so we can do math and convert to milliseconds
        Write.Line(Write.ToMain, "MOV dword[TempQWord2],1000");

        // Initialize the FPU
        Write.Line(Write.ToMain, "FINIT");

        // Load the double
        Write.Line(Write.ToMain, "FLD qword[TempQWord1]");

        // Load the integer 1000
        Write.Line(Write.ToMain, "FILD dword[TempQWord2]");

        // Convert seconds to milliseconds
        Write.Line(Write.ToMain, "FMUL ST0,ST1");

        // Round ST0 to the nearest integer
        Write.Line(Write.ToMain, "FRNDINT");

        // Store the integer
        Write.Line(Write.ToMain, "FIST dword[TempQWord1]");

        // Push the integer to the stack
        Write.Line(Write.ToMain, "PUSH dword[TempQWord1]");
    #endif
    return ;
}


// ConsoleCls() clears the console screen and resets the cursor position
void Assembly::ConsoleCls() {
    // Import external library function for command line commands
    AddLibrary("system");
    #ifdef Windows
        // Windows calls CLS
        // TODO: test possible CLS BUG
        // i think it should be:
        // Write.Line(Write.ToMain, "stdcall system,ConsoleClear")
        Write.Line(Write.ToMain, "stdcall system,ConsoleClear");
    #endif
    // #ifdef Linux
    //     Write.Line(Write.ToMain, "ccall system,ConsoleClear");
    // #endif
    return ;
}


// ConsolePrint() prints a number or string to the console
void Assembly::ConsolePrint(int Type) {
    // If we are printing a string, let's be about it
    if (Type == Data.String) {
        // Note: Here, we use WriteFile because WriteConsole cannot be
        // redirected to a file
        #ifdef Windows
            // Windows API function the compiler uses to write to the console
            AddLibrary("WriteFile");

            // Get the string from the stack
            Write.Line(Write.ToMain, "POP EBX");

            // Get the length of the string
            GetStringLength("EBX");

            // We don't want to print out the null-terminator, so decrement length
            Write.Line(Write.ToMain, "DEC EAX");

            // Now call WriteFile on the console output handle
            Write.Line(Write.ToMain, "stdcall WriteFile,dword[HandleToOutput],EBX,"
                                                             "EAX,ConsoleTemp,0");
            // Now free the string
            FreeMemory(Write.ToMain, "EBX");
        #endif
        // Under Linux, a simple, yet classic printf will do
        // #ifdef Linux
        //     AddLibrary("printf");
        //     // Get the string
        //     Write.Line(Write.ToMain, "POP EBX");
        //     // Print the string
        //     Write.Line(Write.ToMain, "ccall printf,EBX");
        //     // Free the string
        //     FreeMemory(Write.ToMain, "EBX");
        // #endif
    }
    if (Type == Data.Number) {
        #ifdef Windows
            AddLibrary("WriteFile");     // Write a string to a file (or console)
            // Convert the number to a string
            ConvertToString();
            // Get the string
            Write.Line(Write.ToMain, "POP EBX");
            // Get the length of the string
            GetStringLength("EBX");
            // Decrement the length of the string so we don't print the null-terminator
            Write.Line(Write.ToMain, "DEC EAX");
            // Write to the console output handle
            Write.Line(Write.ToMain, "stdcall WriteFile,dword[HandleToOutput],EBX,"
                                                             "EAX,ConsoleTemp,0");
            // Free the string
            FreeMemory(Write.ToMain, "EBX");
        #endif
        // Things are always so much more simple under Linux!
        // #ifdef Linux
        //     AddLibrary("printf");
        //     // Convert the double to a string
        //     ConvertToString();
        //     // Get the string
        //     Write.Line(Write.ToMain, "POP EBX");
        //     // Print the string
        //     Write.Line(Write.ToMain, "ccall printf,EBX");
        //     // Free the string
        //     FreeMemory(Write.ToMain, "EBX");
        // #endif
    }
    return ;
}


// ConsolePrintNewLine() Prints a new line, so each PRINT is on a single line
void Assembly::ConsolePrintNewLine() {
    #ifdef Windows
        // Write a new line character to the console
        Write.Line(Write.ToMain, "stdcall WriteFile,dword[HandleToOutput],"
                                                    "ConsoleNewLine,1,ConsoleTemp,0");
    #endif
    // #ifdef Linux
    //     // Write \n to the console
    //     Write.Line(Write.ToMain, "ccall printf,ConsoleNewLine");
    // #endif
    return ;
}


// ConsoleInput() Reads a string from the console
void Assembly::ConsoleInput() {
    #ifdef Windows
        // Import Windows API function to handle console input
        AddLibrary("ReadFile");

        // Allocate 16KB to store string
        AllocMemory(Write.ToMain,   "16384");
        Write.Line(Write.ToMain,    "MOV EBX,EAX");

        // Read from the console
        Write.Line(Write.ToMain,    "stdcall ReadFile,dword[HandleToInput],EBX,16383,"
                                                            "ConsoleTemp,0");

        // Get the string length
        GetStringLength("EBX");

        // Decrement the length 3 times
        Write.Line(Write.ToMain,    "DEC EAX");
        Write.Line(Write.ToMain,    "DEC EAX");
        Write.Line(Write.ToMain,    "DEC EAX");
        Write.Line(Write.ToMain,    "MOV EDI,EAX");

        // Allocate memory for the new string 
        AllocMemory(Write.ToMain, "EDI");
        Write.Line(Write.ToMain,    "MOV ESI,EAX");

        // Copy the string
        CopyMemoryOfSize("ESI", "EBX", "EDI");

        // Append a null-terminator to the end of the string
        Write.Line(Write.ToMain,    "MOV byte[ESI+EDI],0");

        // And add the string to the stack
        Write.Line(Write.ToMain,    "PUSH ESI");
    #endif
    // #ifdef Linux
    //     AddLibrary("gets");
    //     // Since gets can cause an access violation, set size really large (64KB)
    //     AllocMemory(Write.ToMain, "65536");
    //     Write.Line(Write.ToMain,    "MOV EBX,EAX");
    //     // Get the information from the console
    //     Write.Line(Write.ToMain,    "ccall gets,EBX");
    //     // Get the length of the string the user entered
    //     GetStringLength("EBX");
    //     Write.Line(Write.ToMain,    "MOV EDI,EAX");
    //     // Allocate memory for the new string (so we don't occupy 64KB when only 
    //     // several bytes might be needed
    //     AllocMemory(Write.ToMain, "EDI");
    //     Write.Line(Write.ToMain,    "MOV ESI,EAX");
    //     // Copy the string
    //     CopyMemoryOfSize("ESI", "EBX", "EDI");
    //     // Null-terminate the string
    //     Write.Line(Write.ToMain,    "MOV byte[ESI+EDI],0");
    //     // And add the string to the stack
    //     Write.Line(Write.ToMain,    "PUSH ESI");
    // #endif
    return ;
}


// EndProgram() jumps to the exit routine
void Assembly::EndProgram() {
    Write.Line(Write.ToMain, "JMP Exit");
    return ;
}


// Or() is an implementation of the logical OR operator
void Assembly::Or() {
    // Declare labels for true, false, and end
    std::string TrueLabel = GetLabel();
    std::string FalseLabel = GetLabel();
    std::string CompareNextLabel = GetLabel();
    std::string EndLabel = GetLabel();

    // Pop the two operands off the stack
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load the first operand
    Write.Line(Write.ToMain, "FLD qword[TempQWord1]");

    // Load zero
    Write.Line(Write.ToMain, "FLDZ");

    // Compare the first operand to zero
    Write.Line(Write.ToMain, "FCOM ST1");

    // Get the status word
    Write.Line(Write.ToMain, "FSTSW AX");
    Write.Line(Write.ToMain, "WAIT");

    // Store AH in flags
    Write.Line(Write.ToMain, "SAHF");

    // If the first condition is false, then we need to test the second
    Write.Line(Write.ToMain, "JE " + CompareNextLabel);

    // If the first condition was true, then there is no need to test the
    // second condition. Instead, clean up the stack
    // Pop the conditions off the stack
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

    // Add a Jump if True label
    Write.Line(Write.ToMain, "JMP " + TrueLabel);
    PostLabel(Write.ToMain,   CompareNextLabel);

    // Same comparison process as above
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");
    Write.Line(Write.ToMain, "FINIT");
    Write.Line(Write.ToMain, "FLD qword[TempQWord1]");
    Write.Line(Write.ToMain, "FLDZ");
    Write.Line(Write.ToMain, "FCOM ST1");
    Write.Line(Write.ToMain, "FSTSW AX");
    Write.Line(Write.ToMain, "WAIT");
    Write.Line(Write.ToMain, "SAHF");

    // If both comparison evaluated to false, then jump to the false label
    Write.Line(Write.ToMain, "JE " + FalseLabel);

    // If the result was true, then jump to te true label
    PostLabel(Write.ToMain,    TrueLabel);
    Write.Line(Write.ToMain, "FINIT");
    Write.Line(Write.ToMain, "FILD dword[True]");
    Write.Line(Write.ToMain, "JMP " + EndLabel);
    PostLabel(Write.ToMain,   FalseLabel);
    Write.Line(Write.ToMain, "FINIT");
    Write.Line(Write.ToMain, "FLDZ");
    PostLabel(Write.ToMain,   EndLabel);
    CalculateEnd();
    return ;
}


// And() is an implementation of the logical AND operator
void Assembly::And() {
    // Declare labels for true, false, and end
    std::string TrueLabel = GetLabel();
    std::string FalseLabel = GetLabel();
    std::string CompareNextLabel = GetLabel();
    std::string EndLabel = GetLabel();

    // Pop the first condition off the stack
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load the first condition and zero
    Write.Line(Write.ToMain, "FLD qword[TempQWord1]");
    Write.Line(Write.ToMain, "FLDZ");

    // Evaluate the first condition
    Write.Line(Write.ToMain, "FCOM ST1");
    Write.Line(Write.ToMain, "FSTSW AX");
    Write.Line(Write.ToMain, "WAIT");
    Write.Line(Write.ToMain, "SAHF");
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

    // If the first condition was false, then there is no need to continue
    Write.Line(Write.ToMain, "JNE " + CompareNextLabel);

    // If the first condition was true, then we need to check any additional
    // comparisons as well
    Write.Line(Write.ToMain, "JMP " + FalseLabel);
    PostLabel(Write.ToMain,   CompareNextLabel);

    // Same process as above for the next condition
    Write.Line(Write.ToMain, "FINIT");
    Write.Line(Write.ToMain, "FLD qword[TempQWord1]");
    Write.Line(Write.ToMain, "FLDZ");
    Write.Line(Write.ToMain, "FCOM ST1");
    Write.Line(Write.ToMain, "FSTSW AX");
    Write.Line(Write.ToMain, "WAIT");
    Write.Line(Write.ToMain, "SAHF");

    // If the final comparison results in true, load the true label
    Write.Line(Write.ToMain, "JNE " + TrueLabel);

    // If any of the conditions resulted in false, load the false label
    Write.Line(Write.ToMain, "JMP " + FalseLabel);
    PostLabel(Write.ToMain,   TrueLabel);
    Write.Line(Write.ToMain, "FINIT");
    Write.Line(Write.ToMain, "FILD dword[True]");
    Write.Line(Write.ToMain, "JMP " + EndLabel);
    PostLabel(Write.ToMain,   FalseLabel);
    Write.Line(Write.ToMain, "FLDZ");

    // Post the end label and end the calculation
    PostLabel(Write.ToMain,   EndLabel);
    CalculateEnd();
    return ;
}


// Not() is an implementation of the logical NOT operator
void Assembly::Not() {
    // Declare labels for true and false
    std::string FalseLabel = GetLabel();
    std::string TrueLabel = GetLabel();

    // Pop the first condiiton off the stack
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load the first condition
    Write.Line(Write.ToMain, "FLD qword[TempQWord1]");

    // Load zero (which represents false) 
    Write.Line(Write.ToMain, "FLDZ");

    // Evalutate the comparison
    Write.Line(Write.ToMain, "FCOM ST1");

    // Store the status word
    Write.Line(Write.ToMain, "FSTSW AX");
    Write.Line(Write.ToMain, "WAIT");

    // Store AH in flag
    Write.Line(Write.ToMain, "SAHF");

    // If the comparison is true, then negate it to be false
    // We can keep the zero value to be a "flag" for false
    Write.Line(Write.ToMain, "JNE " + FalseLabel);

    // If the comparison was false, then negate it to be true
    Write.Line(Write.ToMain, "FINIT");
    Write.Line(Write.ToMain, "FILD dword[True]");
    PostLabel(Write.ToMain,   FalseLabel);

    // End the calcuation
    CalculateEnd();
    return ;
}


// StartIf() stores the End label after a comparison, which is used for the IF
// statement
std::string Assembly::StartIf() {
    // End and Temp labels
    std::string EndLabel = GetLabel();
    std::string TempLabel = GetLabel();

    // Pop the result of the comparison off the stack
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load the result of the comparison
    Write.Line(Write.ToMain, "FLD qword[TempQWord1]");

    // Load zero as a flag for false
    Write.Line(Write.ToMain, "FLDZ");

    // Evaluate the comparison
    Write.Line(Write.ToMain, "FCOM ST1");

    // Store the status word flags in the AX register
    Write.Line(Write.ToMain, "FSTSW AX");

    // Wait until the operation is complete
    Write.Line(Write.ToMain, "WAIT");

    // Store the flags in AH, so that they are in a usable state
    Write.Line(Write.ToMain, "SAHF");

    // If the result is not false, then the comparison was true and we should
    // continue to the code inside the If block
    Write.Line(Write.ToMain, "JNE " + TempLabel);

    // If the comparison was false, jump to the end of the If block
    Write.Line(Write.ToMain, "JMP " + EndLabel);
    PostLabel(Write.ToMain,   TempLabel);

    // Return EndLabel so we can skip over the If statement without executing
    // any code inside the If block
    return EndLabel;
}


// StartNewIfSection() starts an If block
std::string Assembly::StartNewIfSection(std::string LastEndLabel, std::string DoneIf) {
    Write.Line(Write.ToMain, "JMP " + DoneIf);
    PostLabel(Write.ToMain,   LastEndLabel);
    return "";
}


// ElseIf() starts an Else If block
std::string Assembly::ElseIf(std::string LastEndLabel, std::string DoneIf) {
    // Labels to mark places the compiler should "Jump" too
    std::string EndLabel = GetLabel();
    std::string TempLabel = GetLabel();

    // Pop the the result of the comparison off the stack
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load the result
    Write.Line(Write.ToMain, "FLD qword[TempQWord1]");

    // Load a zero as a flag for false
    Write.Line(Write.ToMain, "FLDZ");

    // Evaluate the comparison
    Write.Line(Write.ToMain, "FCOM ST1");

    // Store the flags in the AX register
    Write.Line(Write.ToMain, "FSTSW AX");

    // Wait until the operation is complete
    Write.Line(Write.ToMain, "WAIT");

    // Store the flags in the AH register
    Write.Line(Write.ToMain, "SAHF");

    // If the result of the comparison is true, then execute the code inside
    // the Else If block
    Write.Line(Write.ToMain, "JNE " + TempLabel);
    Write.Line(Write.ToMain, "JMP " + EndLabel);
    PostLabel(Write.ToMain,    TempLabel);

    // Return EndLabel so we can skip over the If statement without executing
    // any code inside the Else If block
    return EndLabel;
}


// Else() starts the Else block and executes the code inside if reached
std::string Assembly::Else(std::string EndLabel, std::string DoneIf) {
    // Get a label to mark the end of the else block
    std::string NextEndLabel = GetLabel();

    // If we executed code inside an If/Else If block, then the compiler
    // should not execute any code inside the Else block
    Write.Line(Write.ToMain, "JMP " + DoneIf);

    // If we did not execute code inside an If/Else If block, then the compiler
    // should execute code inside the Else block
    PostLabel(Write.ToMain,    EndLabel);
    return NextEndLabel;
}


// EndIf() tells the compiler to jump when it exits an If/Else If/Else block 
void Assembly::EndIf(std::string EndLabel, std::string DoneIf) {
    // Add a jump command to the end of the If/Else If/Else block
    Write.Line(Write.ToMain, "JMP " + EndLabel);

    // Post the label in the If/Else If statements so they can exit without
    // executing code
    PostLabel(Write.ToMain, EndLabel);
    PostLabel(Write.ToMain, DoneIf);
    return ;
}


// PrepareWhile() marks the location where a loop begins
std::string Assembly::PrepareWhile() {
    // Get a label to mark the start of the loop
    std::string StartLabel = GetLabel();

    // Post the label
    PostLabel(Write.ToMain, StartLabel);
    return StartLabel;
}


// StartWhile() executes code if the loop condition is true
std::string Assembly::StartWhile() {
    std::string EndWhileLabel = GetLabel();
    std::string TempLabel = GetLabel();

    // Pop the comparison off the stack
    Write.Line(Write.ToMain, "POP dword[TempQWord1]");
    Write.Line(Write.ToMain, "POP dword[TempQWord1+4]");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load the result of the comparison
    Write.Line(Write.ToMain, "FLD qword[TempQWord1]");

    // Load 0 as a flag for false
    Write.Line(Write.ToMain, "FLDZ");

    // Evaluate the comparison (check if it is not zero)
    Write.Line(Write.ToMain, "FCOM ST1");

    // Store flags in the AX register
    Write.Line(Write.ToMain, "FSTSW AX");

    // Wait for the operation to complete
    Write.Line(Write.ToMain, "WAIT");

    // Store the flags in th AH register so the compiler can access them
    Write.Line(Write.ToMain, "SAHF");

    // If the loop condition is not true, exit the loop and jump to EndLabel
    // If the loop condition was true, execute the code inside the loop body
    Write.Line(Write.ToMain, "JNE " + TempLabel);
    Write.Line(Write.ToMain, "JMP " + EndWhileLabel);
    PostLabel(Write.ToMain,    TempLabel);

    // Return the position of loop's end so the compiler knows where to jump if
    // the loop condition evaluates to false
    return EndWhileLabel;
}


// StopWhile() tells the compiler to jump where the loop ends and to continue
// executing the rest of the program
void Assembly::StopWhile(std::string StartWhileLabel, std::string EndWhileLabel) {
    Write.Line(Write.ToMain, "JMP " + StartWhileLabel);
    PostLabel(Write.ToMain, EndWhileLabel);
    return ;
}


// CreateSubFunction() creates the beginning of a sub or function
void Assembly::CreateSubFunction(std::string Name, bool External) {
    // Cannot define an external function, we can only import them
    if (!External) {
        Write.Line(Write.ToFunction, "");
        // If the optimization flag is on, then add an ifdef statement to
        // include or exclude from assembling.
        if (Optimize) {
            Write.Line(Write.ToFunction, "%ifdef " + Data.StripJunkOff(Name)+"_Used");
        }
        Write.Line(Write.ToFunction, "");

        // This is a global function
        Write.Line(Write.ToFunction, "global " + Data.Asm(Name));

        // The function begins at the label
        Write.Line(Write.ToFunction, Data.Asm(Name) + ":");

        // Reserve registers and the stack until the function is created
        Write.Line(Write.ToFunction, "PUSH EBP");
        Write.Line(Write.ToFunction, "MOV EBP,ESP");
        Write.Line(Write.ToFunction, "PUSH EBX");
        Write.Line(Write.ToFunction, "PUSH EDI");
        Write.Line(Write.ToFunction, "PUSH ESI");
    }
    return ;
}


// EndCreateSubFunction() finishes creating a sub or function
void Assembly::EndCreateSubFunction(std::string Name, bool External) {
    // Case where the function is user-defined
    if (!External) {
        // Restore the registers and the stack
        Write.Line(Write.ToFunction, "POP ESI");
        Write.Line(Write.ToFunction, "POP EDI");
        Write.Line(Write.ToFunction, "POP EBX");
        Write.Line(Write.ToFunction, "MOV ESP,EBP");
        Write.Line(Write.ToFunction, "POP EBP");

        // RET <Size of Parameters> clears the stack after returning
        #ifdef Windows
            Write.Line(Write.ToFunction, "RET " +
                       ToStr(Data.GetSubFunctionInfo(Name).SizeOfParameters));
        #endif
        // Linux: RET will do
        // #ifdef Linux
        //     Write.Line(Write.ToFunction, "RET");
        // #endif
        // If we are optimizing, let's go ahead and end the block of the function
        if (Optimize) {
            Write.Line(Write.ToFunction, "");
            Write.Line(Write.ToFunction, "%endif");
        }
    }

    // Case where the function is external (imported from a library)
    if (External) {
        // If optimize is enabled, then inline an ifdef assembly directive
        if (Optimize) {
            Write.Line(Write.ToData, "%ifdef " + Data.StripJunkOff(Name) + "_Used");
        }

        // Store the address to the function
        Write.Line(Write.ToData, Data.Asm(Name) + " dd 0");

        // Store the handle of the library that the function 
        Write.Line(Write.ToData, Data.Asm(Name) + "_LibHandle dd 0");

        // Store the alias of the external function
        Write.Line(Write.ToData, Data.Asm(Name) + "_Alias db \"" + 
                                         Data.GetSubFunctionInfo(Name).ExternalInfo.Alias + "\",0");
        // Store the name of the library the function belongs to
        Write.Line(Write.ToData, Data.Asm(Name) + "_Lib db \"" + 
                                     Data.GetSubFunctionInfo(Name).ExternalInfo.Library + "\",0");

        // Define which calling convention to use convention to use
        if (Data.GetSubFunctionInfo(Name).ExternalInfo.CallingConv == "STD") {
            Write.Line(Write.ToData, Data.Asm(Name) + "_Call db 0");
        }
        else {
            Write.Line(Write.ToData, Data.Asm(Name) + "_Call db 1");        
        }


        // Import Windows API function to free the library from memory
        #ifdef Windows
            AddLibrary("FreeLibrary");
            if (Optimize) {
                Write.Line(Write.ToFinishUp, "%ifdef "+Data.StripJunkOff(Name)+"_Used");
            }
            // For a DLL, the FreeLibrary has already been called because of the 
            // positioning - FreeLibrary must go in a certain place for DLLs
            if (AppType == GUI or AppType == Console) {
                Write.Line(Write.ToFinishUp, "stdcall FreeLibrary,dword[" + 
                                                                         Data.Asm(Name) + "_LibHandle]");
            }
            if (Optimize) {
                Write.Line(Write.ToFinishUp, "%endif");
            }
        #endif
        // #ifdef Linux
        //     AddLibrary("dlclose"); // Tells the system we don't need the library
        //     Write.Line(Write.ToFinishUp, "ccall dlclose," + Data.Asm(Name) + 
        //                                                              "_LibHandle");
        // #endif

        // If optimization is enabled, add an endif to the end of the function
        if (Optimize) {
            Write.Line(Write.ToData, "%endif");
        }
    }
    return ;
}


// InvokeSubFunction() invokes a function call
void Assembly::InvokeSubFunction(std::string Name) {
    // If this is a user-defined function, then tell the compiler to call it
    if (!Data.GetSubFunctionInfo(Name).External) {
        Write.Line(Write.ToMain, "CALL " + Data.Asm(Name));
    }
    // If this is an external function, then we may need to import it before
    // telling the compiler to call it
    if (Data.GetSubFunctionInfo(Name).External) {
        CallExternalSubFunction(Name);
    }
    return ;
}


// ReturnValue() returns the value from a function call
void Assembly::ReturnValue(std::string Name, std::string Type) {
    // If we are returning a double, then load it to the FPU
    if (Type == "DOUBLE") {
        Write.Line(Write.ToFunction, "FINIT");
        Write.Line(Write.ToFunction, "FLD qword[EBP-8]");
    }
    // If we are returning a string or integer, move to EAX register
    if (Type == "INTEGER" or Type == "STRING") {
        Write.Line(Write.ToFunction, "MOV EAX,dword[EBP-4]");
    }
    return ;
}


// CleanUpReturnValue() avoids memory leaks after returning a double or string
void Assembly::CleanUpReturnValue(std::string Name, std::string Type) {
    // Initialize the FPU
    if (Type == "DOUBLE") {
        Write.Line(Write.ToMain, "FINIT");
    }
    // Free the string from memory
    if (Type == "STRING") {
        FreeMemory(Write.ToMain, "EAX");
    }
    return ;
}


// PushReturnValue() However, if we are going to use the return value, push it
// onto the stack
void Assembly::PushReturnValue(std::string Name, std::string Type) {
    // If we have an integer, just load it, and drop through to the second
    // if statement to actually push it onto the stack
    if (Type == "INTEGER") {
        Write.Line(Write.ToMain, "MOV dword[TempQWord1],EAX");
        Write.Line(Write.ToMain, "FINIT");
        Write.Line(Write.ToMain, "FILD dword[TempQWord1]");
    }
    // Once we have the double in the FPU, store it and push both parts
    if (Type == "DOUBLE" or Type == "INTEGER") {
        Write.Line(Write.ToMain, "FST qword[TempQWord1]");
        Write.Line(Write.ToMain, "PUSH dword[TempQWord1+4]");
        Write.Line(Write.ToMain, "PUSH dword[TempQWord1]");
    }
    // If all we have is a std::string, go ahead and push it
    if (Type == "STRING") {
        Write.Line(Write.ToMain, "PUSH EAX");
    }
    return ;
}


// LoadExternalSubFunction() loads a function from an external library
void Assembly::LoadExternalSubFunction(std::string Name) {
    #ifdef Windows
        // Import Windows API function to load to the library
        AddLibrary("LoadLibraryA");
        // Import Windows API function to get the address of the function
        AddLibrary("GetProcAddress");

        // Labels to verify the function was imported and declared
        std::string GoodLibraryLabel = GetLabel();
        std::string GoodSubFunctionLabel = GetLabel();

        // Store the library handle so the compiler can locate the function
        Write.Line(Write.ToMain, "stdcall LoadLibraryA," + Data.Asm(Name) + "_Lib");
        Write.Line(Write.ToMain, "MOV dword[" + Data.Asm(Name) + "_LibHandle],EAX");

        // Check to see if we succesfully obtained the library handle
        Write.Line(Write.ToMain, "CMP EAX,0");
        Write.Line(Write.ToMain, "JNE " + GoodLibraryLabel);
        Write.Line(Write.ToMain, "PUSH " + Data.Asm(Name) + "_Lib");
        Write.Line(Write.ToMain, "JMP NoLibrary");
        PostLabel(Write.ToMain,   GoodLibraryLabel);

        // Get the address of the function inside the library 
        Write.Line(Write.ToMain, "stdcall GetProcAddress,dword[" + Data.Asm(Name) + 
                                                         "_LibHandle]," + Data.Asm(Name) + "_Alias");
        // Store the function
        Write.Line(Write.ToMain, "MOV dword[" + Data.Asm(Name) + "],EAX");

        // Check to see if we succesfully obtained the function address
        Write.Line(Write.ToMain, "CMP EAX,0");
        Write.Line(Write.ToMain, "JNE " + GoodSubFunctionLabel);
        Write.Line(Write.ToMain, "PUSH " + Data.Asm(Name) + "_Alias");
        Write.Line(Write.ToMain, "JMP NoFunction");
        PostLabel(Write.ToMain,   GoodSubFunctionLabel);

        // For a DLL, we need to free the library
        if (AppType == DLL) {
            Write.Line(Write.ToFinishUp, "stdcall FreeLibrary,dword[" + 
                                                                     Data.Asm(Name) + "_LibHandle]");
        }
    #endif
    // Linux uses different functions
    // #ifdef Linux
    //     AddLibrary("dlerror"); // Reports the error in loading a library
    //     AddLibrary("dlopen"); // Gets the handle to a library
    //     AddLibrary("dlsym"); // Gets the address of a function in the library
    //     std::string GoodLibraryLabel = GetLabel();
    //     std::string GoodSubFunctionLabel = GetLabel();
    //     // Get the handle to the library
    //     Write.Line(Write.ToMain, "ccall dlopen," + Data.Asm(Name) + "_Lib,1");
    //     Write.Line(Write.ToMain, "MOV dword[" + Data.Asm(Name) + "_LibHandle],EAX");
    //     // Make sure the handle is valid
    //     Write.Line(Write.ToMain, "CMP EAX,0");
    //     Write.Line(Write.ToMain, "JNE " + GoodLibraryLabel);
    //     Write.Line(Write.ToMain, "JMP Exit");
    //     PostLabel(Write.ToMain,    GoodLibraryLabel);
    //     // Get the address of the function
    //     Write.Line(Write.ToMain, "ccall dlysm,dword[" + Data.Asm(Name) + 
    //                                                      "_LibHandle]," + Data.Asm(Name) + "_Alias");
    //     Write.Line(Write.ToMain, "MOV dword[" + Data.Asm(Name) + "],EAX");
    //     // Ad make sure that we have a valid address, too
    //     Write.Line(Write.ToMain, "CMP EAX,0");
    //     Write.Line(Write.ToMain, "JNE " + GoodSubFunctionLabel);
    //     Write.Line(Write.ToMain, "JMP Exit");
    //     PostLabel(Write.ToMain,    GoodSubFunctionLabel);
    // #endif
    return ;
}


// CallExternalSubFunction() calls an external function
void Assembly::CallExternalSubFunction(std::string Name) {
    // Label to call the function
    std::string CallLabel = GetLabel();

    // Check to see if we have a valid function at the address to call
    Write.Line(Write.ToMain, "CMP dword[" + Data.Asm(Name) + "],0");
    Write.Line(Write.ToMain, "JNE " + CallLabel);

    // If not, then load we need to load the external function's library before
    // calling it
    LoadExternalSubFunction(Name);
    PostLabel(Write.ToMain,   CallLabel);

    // Call the function at its address
    Write.Line(Write.ToMain, "CALL dword[" + Data.Asm(Name) + "]");
    return ;
}


// AllocateParameterPool() stores a function's parameters temporarily before
// pushing them to the stack
void Assembly::AllocateParameterPool(int Size) {
    // Push the current pool of memory recursively until we have all the
    // parameters
    PUSH("dword[ParameterPool]");

    // Allocate memory for the next pool
    AllocMemory(Write.ToMain, ToStr(Size));

    // Create memory in the pool to store the parameter
    Write.Line(Write.ToMain, "MOV dword[ParameterPool],EAX");
    return ;
}


// AddToParameterPool() adds a variable to the pool of parameters
void Assembly::AddToParameterPool(int Type, int Where) {
    // Get the parameter pool
    Write.Line(Write.ToMain, "MOV EAX,dword[ParameterPool]");

    // For integers, strings, and types, just pop directly to the pool
    if (Type == Data.Integer or Type == Data.String or Type == Data.Type) {
        Write.Line(Write.ToMain, "POP dword[EAX+" + ToStr(Where) + "]");
    }

    // Doubles require a double pop
    if (Type == Data.Double) {
        Write.Line(Write.ToMain, "POP dword[EAX+" + ToStr(Where) + "]");
        Write.Line(Write.ToMain, "POP dword[EAX+" + ToStr(Where) + "+4]");
    }
    return ;
}


// PushParameterPool() pushes a parameter from the pool to the stack
void Assembly::PushParameterPool(int Type, int Where) {
    Write.Line(Write.ToMain, "MOV EAX,dword[ParameterPool]");

    // If we have an integer or std::string, just do a simple push
    if (Type == Data.Integer    || Type == Data.String) {
        Write.Line(Write.ToMain, "PUSH dword[EAX+" + ToStr(Where) + "]");
    }

    // Push both parts of a double
    if (Type == Data.Double) {
        Write.Line(Write.ToMain, "PUSH dword[EAX+" + ToStr(Where) + "+4]");
        Write.Line(Write.ToMain, "PUSH dword[EAX+" + ToStr(Where) + "]");
    }    

    // Get the address of the type, and then push the actual type
    if (Type == Data.Type) {
        Write.Line(Write.ToMain, "MOV EAX,dword[EAX+" + ToStr(Where) + "]");
        Write.Line(Write.ToMain, "PUSH dword[EAX]");
    }
    return ;
}


// FreeparameterPool() Free the pool of parameters
void Assembly::FreeParameterPool() {
    // Reserve the return value by pushing it to the stack
    PUSH("EAX");

    // Get the parameter pool from memory
    Write.Line(Write.ToMain, "MOV EAX,dword[ParameterPool]");

    // Free the parameter pool from memory
    FreeMemory(Write.ToMain, "EAX");

    // Pop the return value
    POP("EAX");

    // Get the previous parameter pool we pushed before creating this pool
    Write.Line(Write.ToMain, "POP dword[ParameterPool]");
}


// Callback() gets the address of a function
void Assembly::Callback(std::string Name) {
    // Case where an external function is called
    if (Data.GetSubFunctionInfo(Name).External) {
        std::string PushLabel = GetLabel();

        // Check to see if we have a valid address to push
        Write.Line(Write.ToMain, "CMP dword[" + Data.Asm(Name) + "],0");
        Write.Line(Write.ToMain, "JNE " + PushLabel);

        // If not, load the external function before calling it
        LoadExternalSubFunction(Name);
        PostLabel(Write.ToMain,   PushLabel);

        // Push the address of the function
        PUSH("dword[" + Data.Asm(Name) + "]");
    }
    else { // Otherwise, simply push the data to the stack
        PUSH(Data.Asm(Name));
    }
    // Pop the address
    POP("dword[TempQWord1]");

    // Initialize the FPU
    Write.Line(Write.ToMain, "FINIT");

    // Load the address of the function
    Write.Line(Write.ToMain, "FILD dword[TempQWord1]");

    // Store it as a double
    Write.Line(Write.ToMain, "FST qword[TempQWord1]");

    // Push the function to the stack
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1+4]");
    Write.Line(Write.ToMain, "PUSH dword[TempQWord1]");
    return ;
}


// AdjustStack() removes parameters from the stack after a function that uses
// the C calling convention is called
void Assembly::AdjustStack(int Size) {
    Write.Line(Write.ToMain, "ADD ESP," + ToStr(Size));
    return ;
}


// OptimizeSubFunctions() returns a formatted list of all the functions calls
// in a program and writes them to the assembly language file
// Note: Functions that are not called will not be written in assembly
void Assembly::OptimizeSubFunctions() {
    Write.Line(Write.ToLibrary, Data.GetUsedSubFunctions());
    return ;
}


#endif // ASSEMBLY_HPP