#include <iostream>
#include <windows.h>

#include "KoolBRead.hpp"


void Start();
void Compile(int argc, char* argv[]);
void Stop();

DWORD StartTime;




int main(int argc, char *argv[]) {
    std::cout << "Welcome to the Simply KoolB Compiler!" << std::endl;
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
    if (argc != 2) {
       std::cout << "Usage: KoolB <filename>" << std::endl;
       exit(1);
    }

    Reading Read = Reading();

    // Open the file, pass it to OpenBook()
    Read.OpenBook(argv[1]);
    Read.GetNextWord();

    while (Read.WordType() != Read.None) {
        // Loop through each word and print out its type.
        switch (Read.WordType()) {
        case Read.Identifier:
            std::cout << "Found Identifier:\t\t" << Read.Word() << std::endl;
            break;
        case Read.String:
            std::cout << "Found String:\t\t" << Read.Word() << std::endl;
            break;
        case Read.Symbol:
            std::cout << "Found Symbol:\t\t" << Read.Word() << std::endl;
            break;
        case Read.EndOfLine:
            // std::cout << "Found EndOfLine:\t\t" << Read.Word() << std::endl;
            break;
        default:
            std::cout << "ERR: Unable to report what happened." << std::endl;
            break;
        }
        // Sleep for 150ms then get the next word.
        Sleep(150);
        Read.GetNextWord();
    }
    return ;
}


void Stop() {
    std::cout << "Compile Time:\t"
              << static_cast<float>((GetTickCount() - StartTime) / 1000.0)
              << std::endl;
    return ;
}