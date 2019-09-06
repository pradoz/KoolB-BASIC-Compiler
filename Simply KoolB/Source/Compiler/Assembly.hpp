#include <string>


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
};


// AddLibrary() Adds a function (Windows API or a Linux CLibrary call) to the
// library section of our assembly so we can use that function in our program.
void Assembly::AddLibrary(std::string FunctionName){
    Write.Line(Write.ToLibrary, "extern " + FunctionName);
    return ;
}


// GetLabel() generates and returns a unique label for whenever we need a
// temporary label.



// PostLabel() posts a label to the desired section of the assembly language.



// BuildSkeleton() adds all the assembly, tells the linker where the program
// starts, adds a default icon to the program, etc.



// FinishUp() does the opposite of BuildSkeleton. It ensures KoolB app can
// shut down and exit properly. It is essentially a clean up function.