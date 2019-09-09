#ifndef MISC_HPP
#define MISC_HPP


#include <fstream>
#include <string>
#include <ctime>
// #include <windows.h>


// Run() will continue to try to open results.txt for .3 seconds. If .3 seconds pass
// and Run cannot open results.txt, Run gives a timeout error. If Run is able to open
// results.txt, it opens it and checks the size. If the size is zero, that means the program
// being run did not encounter any errors. In that case, Run returns. If Run finds that the
// size of results.txt is greater than zero, it reads the contents, reports the errors,
// and exits from the program. 
void Run(std::string Command) {
    std::ifstream File;
    int FileStart;
    int FileEnd;
    char* Spoon;
    double StartTime = clock() / CLK_TCK;
    system(Command.c_str());

    File.open(".\\results.txt", std::ios::in);
    while (File.is_open() == false) {
        File.open(".\\results.txt", std::ios::in);
        if ((clock() / CLK_TCK) - StartTime > 300) {
            std::cout << "Time out running: " + Command << std::endl;
            exit(1);
        }
    }
    
    FileStart = File.tellg();
    File.seekg(0, std::ios::end);
    FileEnd = File.tellg();
    File.seekg(0, std::ios::beg);

    if (FileEnd - FileStart > 0) {
        std::cout << "Error: nasm, GoRC, or GoLink failed. Please attach error messages to bug reports:" << std::endl;
        std::cout << "Ran " + Command + " and got:" << std::endl;
        Spoon = new char[FileEnd - FileStart];
        File.read(Spoon, FileEnd - FileStart);
        Spoon[FileEnd - FileStart-1] = '\0';
        std::cout << Spoon << std::endl;
        delete[] Spoon;
        exit(1);
    }
    File.close();
}


// ToStr() takes an integer, converts it to a string, and returns the string.
std::string ToStr(int Number) {
    std::string Result;
    char* Spoon;
    Spoon = new char[1024];
    sprintf(Spoon, "%i", Number);
    Result = Spoon;
    delete[] Spoon;
    return Result;
}


// ToLong() converts a string to a digit (long)
long ToLong(std::string String) {
    long Result;
    Result = atoi(String.c_str());
    return Result;
}


// StripOffExtension() removes the file extension from the end of a file
std::string StripOffExtension(std::string FileName) {
    int Length = FileName.length();

    // Go backwards, less letters to process to find the file extension
    for (int i = Length; i > 0; --i) {
        if (FileName[i] == '\\' or FileName[i] == '/') {
            return FileName;
        }
        if (FileName[i] == '.') {
            return FileName.substr(0, i);
        }
    }
    return FileName;
}


// FileExits() returns true if the file exists and can be opened
bool FileExists(std::string FileName) {
    ifstream File(FileName.c_str(), std::ios::in);
    if (!File.is_open()) {
         return false;
    }
    File.close();
    return true;
}


// GetFileNameOnly() extracts only the file name from a complete path
std::string GetFileNameOnly(std::string FileName) {
    // Go backwards, need the last slash to find the file name
    int Length = FileName.length();
    for (int i = Length; i > 0; --i) {
        if (FileName[i] == '\\' or FileName[i] == '/') {
            return FileName.substr(i, Length);
        }
    }
    return FileName;
}


// GetPathOnly() extracts only the path from a complete path
std::string GetPathOnly(std::string FileName) {
    // Go backwards, need the last slash file path
    int Length = FileName.length();
    for (int i = Length; i > 0; --i) {
        if (FileName[i] == '\\' or FileName[i] == '/') {
            return FileName.substr(0, i+1);
        }
    }
    return FileName;
}


// PatchFileNames() combines two complete paths to different files
std::string PatchFileNames(std::string FileName1, std::string FileName2) {
    return GetPathOnly(FileName1) + GetFileNameOnly(FileName2);
}


// Sleep() Pauses for the specified number of seconds
void PatchFileNames(int Pause) {
    int StartTime = clock();
    while ((clock() / CLOCKS_PER_SEC) - StartTime < Pause) {
        // Empty loop body
    }
    return ;
}

#endif // MISC_HPP