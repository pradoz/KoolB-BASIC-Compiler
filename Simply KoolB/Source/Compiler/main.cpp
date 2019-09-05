#include <iostream>
#include <stdlib.h>
#include <string>
#include <windows.h>



// Global variables
std::string FileName;

// General functions
void RequestFile();


int main(int argc, char *argv[]) {
    // Print welcome
    std::cout << "Simply KoolB Compiler" << std::endl << std::endl;
    // Check for an existing filename
    if (argc != 2) {
        RequestFile();
    }
    else {
        FileName = argv[1];
    }

    std::cout << "User wants to compile the file:" << FileName
              << std::endl << std::endl;

    system("PAUSE");
    return 0;
}

void RequestFile() {
    char Temp[1024];

    // Prompt user for the name of the file to compile
    std::cout << "Please enter the file to compile: ";
    std::cin.getline(Temp, 1024);
    FileName = Temp;
    std::cout << std::endl << std::endl;
}
