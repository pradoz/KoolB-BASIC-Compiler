#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include <string>


class Assembly {
public:
    Assembly() { LabelNumber = 0; TempVariable = 0; }
    
    void AddLibrary(std::string FunctionName);
    std::string GetLabel();
    void PostLabel(int Section, std::string Label);
    void BuildSkeleton();
    void FinishUp();

private:
    int LabelNumber;  // Track the number of temporary labels used
    int TempVariable; // Track the number of  variables stored in .data section
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

    // TODO:
    // Enable optimzation routine
    // if (Optimize == true){
    //     OptimizeSubFunctions();
    // }
    return ;
}

#endif // ASSEMBLY_HPP