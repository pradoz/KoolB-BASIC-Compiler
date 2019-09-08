// Include libraries
#include <iostream>
#include <string> 
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <map>
#include <algorithm>


// Set OS to Windows
#define Windows

#ifdef Windows
  #include <windows.h>
#endif


// Include modules
#include "Misc.hpp"
#include "KoolBRead.hpp"
  Reading Read;
#include "Database.hpp"
  Database Data;
#include "Write.hpp"
  Writing Write;
#include "Assembly.hpp"
  Assembly Asm;

// Configure program settings
enum OS{Windows, Linux};
enum AppType{GUI, Console, CGI};
int OS = Windows;
int AppType = Console;

// Core routines for compilation process
void Start();
void Compile(int argc, char* argv[]);
void Stop();

double StartTime 0.0;
double TempTime = 0.0;
int Pause = 0



int main(int argc, char* argv[]) {
    #ifdef Windows
    printf("\n   Windows BASIC compiler v1.0 \n\r\n");
    #endif
    Start();
    Compile(argc, argv);
    Stop();
    Sleep(Pause);
    return 0;
}


void Start() {
    StartTime = clock();
    TempTime = StartTime;
    return ;
}


// Gathers file to compile, opens it, and passes it to CompileIt() where it
// becomes assembly, then links the program.
void Compile(int argc, char* argv[]) {
    // Create two strings to hold the FileName and the TargetOS
    std::string FileName;
    std::string TargetOS;

    // If arg count is invalid, print the usage and exit
    if (argc != 2) {
        // Print error message and prompt user for filename
        printf("Usage: KoolB <filename>\nEnter Filename: ");

        // Windows API defines MAX_PATH=260
        char* InFile = new char[MAX_PATH];
        scanf("%s", FileName.c_str()); // read the file 
        FileName = InFile;
        delete[] InFile;
        printf("\r\n");

        // Set pause to 4 ms
        Pause = 4;
    }
    else { // argc == 2
        FileName = argv[1];
    }
    
    // Open the file
    Read.OpenBook(FileName);
    
    // Generate assembly language
    printf("Currently compiling \"%s\":\r\n", FileName.c_str());
    // CompileIt.Compile(Read, false);
    Asm.BuildSkeleton();
    
    // Clean up assembly language
    Asm.FinishUp();
    
    // Print the time taken to compile
    printf(" - Compile time -->  %f seconds\r\n",
           (double)(clock() - TempTime) / (double)CLK_TCK);
    TempTime = clock();

    // Assemble and link the program
    Write.BuildApp(FileName);
    return ;
}

// Prints out time spent compiling, assembling, and linking.
void Stop() {
  printf(" - Total time   -->  %f seconds\r\n",
         (double)(clock() - StartTime) / (double)CLOCKS_PER_SEC);
  return ;
}