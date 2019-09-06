#ifndef ASSEMBLY_HPP
#define ASSEMBLY_HPP

#include <string>

#include "Write.hpp"


class Assembly {
public:
    Assembly() { LabelNumber = 0; }
    
    void AddLibrary(std::string FunctionName);
    
    std::string GetLabel();
    
    void PostLabel(int Section, std::string Label);
    
    void BuildSkeleton();
    
    void FinishUp();

private:
    int LabelNumber;
    Writing Write;
};


// AddLibrary() Adds a function (Windows API or a Linux CLibrary call) to the
// library section of our assembly so we can use that function in our program.
void Assembly::AddLibrary(std::string FunctionName) {
    Write.Line(Write.ToLibrary, "extern " + FunctionName);
    return ;
}


// GetLabel() generates and returns a unique label for whenever we need a
// temporary label.
std::string Assembly::GetLabel() {
    std::string Result;
    ++LabelNumber;
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
    if (OS == Windows) {
        // Initializes ExitProcess by adding it to the library
        AddLibrary("ExitProcess");
        Write.Line(Write.ToResource, "1 ICON \"Asm\\Default.ico\"");
        Write.Line(Write.ToData, "section .data");
        Write.Line(Write.ToFireUp, "section .text");
        Write.Line(Write.ToFireUp, "%include \"Asm\\MACROS.INC\"");
        Write.Line(Write.ToFireUp, "global START");
        PostLabel(Write.ToFireUp, "START");
        PostLabel(Write.ToFinishUp, "Exit");
    }
    if (OS == Linux) {
        AddLibrary("exit");
        Write.Line(Write.ToData, "section .data");
        Write.Line(Write.ToFireUp, "section .text");
        Write.Line(Write.ToFireUp, "%include \"Asm\\MACROS.INC\"");
        Write.Line(Write.ToFireUp, "global _start");
        PostLabel(Write.ToFireUp, "_start");
        PostLabel(Write.ToFinishUp, "Exit");
    }
}


// FinishUp() does the opposite of BuildSkeleton. It ensures KoolB app can
// shut down and exit properly. It is essentially a clean up function.
void Assembly::FinishUp(){
    if (OS == Windows){
        Write.Line(Write.ToFinishUp, "stdcall ExitProcess,0");
    }
    if (OS == Linux){
        Write.Line(Write.ToFinishUp, "ccall exit,0");
    }
}

#endif // ASSEMBLY_HPP