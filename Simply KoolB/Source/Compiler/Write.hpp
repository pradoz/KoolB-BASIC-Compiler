#ifndef WRITE_HPP
#define WRITE_HPP



// #include <fstream>
// #include <string>
// #include <windows.h>
// #include "Misc.hpp"

class Writing {
public:
    Writing() {
        Resources = ";Resource file gnerated by the compiler\n";
        Library = ";Library functions to import to the program\n";
        FireUpApp = "\n\n;Initialize everything to prepare the program to run\n";
        MainApp = "\n\n;Main body of the program where the program runs\n";
        FinishUpApp = "\n\n;Prepare the program to exit, then terminate the program\n";
        AppData = "\n\n;Data section of the program\n";
    }

    // Formats a string of assembly language code into a line and adds it to
    // the specified section.
    void Line(int Section, std::string Line);

    // Writes all the sections of the programs to files based on the FileName.
    void File(std::string FileName);

    // Runs the assembler, resource compiler, and linker produces an executable
    void BuildApp(std::string FileName);

    // Handles exiting and entering subs/functions
    void EnterSubFunction();
    void ExitSubFunction();

    enum Sections {
        ToData,
        ToFireUp,
        ToMain,
        ToFinishUp,ToResource,
        ToLibrary,
        ToFunction
    };

private:
    /*
    - AppData - Contains variables and data that the program will need.

    - Functions - Contains the assembly language related to functions.

    - FireUpApp - Contains initialization routines for the program like object
    constructors and internal code that gets the program ready to run user code.

    - MainApp - Contains the user's code.

    - FinishAppUp - Contains routines to clean up after the user and prepare
    the program to exit. Frees all memory associated with objects, variables,
    and the program in general.

    - Resources - Contains instructions for the resource compiler to tell it
    what files and resources to include and how to include them.

    - Library - Contains all the external functions that the program will
    need. (Windows API or Linux C-Library calls)

    - InSubFunction - True if we are currently inside of a function

    - SubFunctionFireUpApp - Initializes a function

    - SubFunctionMainApp - Main body of the function

    - SubFunctionFinishUpApp - Clean up memory/parameters after a function call
    */

    std::string AppData;
    std::string Functions;
    std::string FireUpApp;
    std::string MainApp;
    std::string FinishUpApp;
    std::string Resources;
    std::string Library;
    bool InSubFunction;
    std::string SubFunctionFireUpApp;
    std::string SubFunctionMainApp;
    std::string SubFunctionFinishUpApp;
};


// Line() takes Asm, which is a line of assembly language, and adds it to the
// desired Section
void Writing::Line(int Section, std::string Asm) {
    std::string Line = Asm + "\n";
    switch (Section) {
        // Write to section: Data
        case ToData:
            AppData += Line;
            break;

        // Write to section: FireUp
        case ToFireUp:
            if (InSubFunction) {
                SubFunctionFireUpApp += Line;
                break;
            }
            FireUpApp += Line;
            break;

        // Write to section: Main
        case ToMain:
            if (InSubFunction) {
                SubFunctionMainApp += Line;
                break;
            }
            MainApp += Line;
            break;

        // Write to section: FinishUp
        case ToFinishUp:
            if (InSubFunction) {
                SubFunctionFinishUpApp += Line;
                break;
            }
            FinishUpApp += Line;
            break;

        // Write to section: Resource
        case ToResource:
            Resources += Line;
            break;

        // Write to section: Library
        case ToLibrary:
            Library += Line;
            break;

        // Write to section: Function
        case ToFunction:
            Functions += Line;
            break;
        default:
            std::cout << "ERR: Cannot write to an invalid section." << std::endl;
            break;
    }
    return ;
}


void Writing::File(std::string FileName) {
    std::string AsmFileName = FileName + ".asm";
    std::string ResourceFileName = FileName + ".rc";
    std::ofstream AsmFile;
    std::ofstream ResourceFile;

    // Open the assembly and resource files
    AsmFile.open(AsmFileName.c_str(), std::ios::out);
    ResourceFile.open(ResourceFileName.c_str(), std::ios::out);

    // Write to the assembly file
    AsmFile << Library << std::endl;
    AsmFile << FireUpApp << std::endl;
    AsmFile << MainApp << std::endl;
    AsmFile << FinishUpApp << std::endl;
    AsmFile << Functions << std::endl;
    AsmFile << AppData << std::endl;

    // Write to the resource file
    ResourceFile << Resources << std::endl;

    // Close the assembly and resource files
    AsmFile.close();
    ResourceFile.close();
    return ;
}


// BuildApp() writes the file to the disk, assembles the file, and links it
void Writing::BuildApp(std::string FileName) {
    std::cout << "XXXX Entered BuildApp()" << std::endl;
    // Strip the file extension so we don't append multiple file extensions
    FileName = StripOffExtension(FileName);

    // Write the file to disk
    File(FileName);

    std::cout << "XXXX Wrote the file to disk" << std::endl;
    // For Windows OS, use NASM and GoLink
    #ifdef Windows
    std::cout << "XXXX Entered ifdef Windows, trying to run NASM..." << std::endl;
        // Run NASM
        Run("Asm\\NASM -f win32 \"" + FileName + ".asm\"");
        // Run("Asm\\NASM -E results.txt -f win32 -o \"" + FileName + ".obj\" \"" + FileName + ".asm\"");
        printf(" - Assemble time -> %f seconds\r\n", 
                   (double)(clock() - TempTime) / (double)CLK_TCK);

        TempTime = clock();

        // Run GoRC
        Run("Asm\\GoRC /r /ni /nw \"" + FileName + ".rc\" > results.txt");

        // Run GoLink
        // Case where we need to link a console app
        if (AppType == Console) {
            Run("Asm\\GoLink /ni /nw /CONSOLE \"" + FileName + ".obj\" \"" + FileName
                + ".res\" kernel32.dll user32.dll gdi32.dll msvcrt.dll > results.txt");
        }
        // Case where we need to link a GUI app
        if (AppType == GUI) {
            Run("Asm\\GoLink /ni /nw \"" + FileName + ".obj\" \"" + FileName +
                ".res\" kernel32.dll user32.dll gdi32.dll msvcrt.dll > results.txt");
        }
        // Case where we need to link a DLL that exports all the user-defined
        // functions in the database
        if (AppType == DLL) {
            Run("Asm\\GoLink /ni /nw /DLL \"" + FileName + ".obj\" /EXPORTS " +
            Data.ListFunctions() + " \"" + FileName + ".res\" kernel32.dll "
            "user32.dll gdi32.dll msvcrt.dll > results.txt");        
        }

        printf(" - Linking time -> %f seconds\r\n",
                   (double)(clock() - TempTime) / (double)CLOCKS_PER_SEC);

        TempTime = clock();
    #endif

        // TODO: Compression with PEPaCK
        // Compress the program or DLL with PEPaCK
        // if (Compress) {
        //     if (AppType == GUI or AppType == Console) {
        //         Run("Asm\\pepack \"" + FileName + ".exe\" > results.txt");
        //     }
        //     if (AppType == DLL) {
        //         Run("Asm\\pepack \"" + FileName + ".dll\" > results.txt");
        //     }
        //     printf(" - Compress time -> %f seconds\r\n",
        //                (double)(clock() - TempTime) / (double)CLK_TCK);
        // }
    // If we are using Link, use NASM (Netwide Assembler) and GNU ld
    // #ifdef Linux
    //     Run("nasm -E results.txt -f elf \"" + FileName + ".asm\"");
    //     printf(" - Assemble time ->    %f seconds\r\n", (double)(clock() - TempTime) / (double)CLK_TCK);
    //     TempTime = clock();
    //     // GUI and Console are linked identically
    //     if (AppType == GUI || AppType == Console) {
    //         Run("ld -s -dynamic-linker /lib/ld-linux.* -o \"" + FileName + "\" \"" + 
    //                 FileName + ".o\" /lib/libc.* /lib/libm.* /lib/libdl.* > results.txt");
    //     }
    //     // Link a shared library for Linux
    //     if (AppType == DLL) {
    //         Run("ld -s -dynamic-linker /lib/ld-linux.* -shared -o \"" + FileName + 
    //                 "\" \"" + FileName + ".o\" /lib/libc.* /lib/libm.* /lib/libdl.* "
    //                 "> results.txt");
    //     }
    //     printf(" - Linking time    ->    %d seconds\r\n", (double)(clock() - TempTime) / (double)CLK_TCK);
    //     TempTime = clock();
    //     // To compress, strip the symbols and then compress it with UPX
    //     if (Compress == true) {
    //         if (AppType == GUI || AppType == Console) {
    //             Run("strip \"" + FileName + "\" > results.txt");
    //             Run("upx \"" + FileName + "\" > results.txt");
    //         }
    //         if (AppType == DLL) {
    //             Run("upx \"" + FileName + ".so\" > results.txt");
    //         }
    //         printf(" - Compress time ->    %f seconds\r\n", (double)(clock() - TempTime) / 
    //                                                                                                 (double)CLK_TCK);
    //     }
    // #endif
    return ;
}


// EnterSubFunction() resets function values to prepare a function declaration
void Writing::EnterSubFunction() {
    // Set InSubFunction to true since we are entering a sub or function
    InSubFunction = true;

    // Reset the strings for the sub or function to be written
    SubFunctionFireUpApp = "";
    SubFunctionMainApp = "";
    SubFunctionFinishUpApp = "";
}


// ExitSubFunction() combines function information and writes it to the app
void Writing::ExitSubFunction() {
    // Set InSubFunction to false since we are exiting a sub or function
    InSubFunction = false;

    // Finish writing the sub or function in assembly
    Line(ToFunction, SubFunctionFireUpApp);
    Line(ToFunction, SubFunctionMainApp);
    Line(ToFunction, SubFunctionFinishUpApp);
}




#endif // WRITE_HPP
