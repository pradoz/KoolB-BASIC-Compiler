#include <iostream>
#include <windows.h>


enum OS{Windows, Linux};
enum AppType{GUI, Console, CGI};
int OS = Windows;
int AppType = Console;


// Include Read and Write modules
#include "KoolBRead.hpp"
#include "Write.hpp"

#include "Assembly.hpp"
#include "Misc.hpp"

Reading read;
Writing write;
Assembly Asm;


void Start();
void Compile(int argc, char* argv[]);
void Stop();

DWORD StartTime;




int main(int argc, char *argv[]) {
    std::cout << "Compiling the program..." << std::endl;
    Start();
    Compile(argc, argv);
    Stop();
    return 0;
}


void Start() {
    StartTime = GetTickCount();
    return ;
}


void Compile(int argc, char* argv[]) {
    // Create two strings to hold the FileName and the TargetOS
    std::string FileName;
    std::string TargetOS;

    // If arg count is invalid, print the usage and exit
    if (argc < 2 || argc > 4) {
        std::cout << "Usage: <compiler-executable> [OS] <filename>" << std::endl << std::endl;
        std::cout << "\tOptions:" << std::endl;
        std::cout << "\t-Windows\tCompile for Windows" << std::endl;
        std::cout << "\t-Linux\t\tCompile for Linux" << std::endl;
        exit(1);
    }

    if (argc == 3) {
        TargetOS = argv[1]; // assign TargetOS the second parameter
        FileName = argv[2]; // assign FileName the third parameter
        if (TargetOS == "-Windows" || TargetOS == "-Linux") {
            if (TargetOS == "-Linux") {
                OS = Linux;
            }
        }
        else {
            std::cout << "Usage: <compiler-executable> [OS] <filename>" << std::endl << std::endl;
            std::cout << "\tOptions:" << std::endl;
            std::cout << "\t-Windows\tCompile for Windows" << std::endl;
            std::cout << "\t-Linux\t\tCompile for Linux" << std::endl;
            exit(1);
        }
    }
    else {
    FileName = argv[1];
    }
    
    read.OpenBook(FileName);
    Asm.BuildSkeleton();
    Asm.FinishUp();
    write.BuildApp(FileName);
    return  ;
}


void Stop() {
    std::cout << "Compile Time:\t " << (GetTickCount() - StartTime) / 1000.0  << std::endl;
    return ;
}