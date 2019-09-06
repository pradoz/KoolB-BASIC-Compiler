#include <fstream>
#include <string>


// Run() will continue to try to open results.txt for 30 seconds. If 30 seconds passes
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
        if ((clock() / CLK_TCK) - StartTime > 30000) {
            std::cout << "Time out running: " + Command << std::endl;
            exit(1);
        }
    }
    
    FileStart = File.tellg();
    File.seekg(0, std::ios::end);
    FileEnd = File.tellg();
    File.seekg(0, std::ios::beg);

    if (FileEnd - FileStart > 0) {
        std::cout << "Error: nasm, GoRC, or GoLink failed. Please attach error
            messages to bug repots:" << std::endl; 
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