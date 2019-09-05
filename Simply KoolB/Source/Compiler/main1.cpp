#include <iostream>
#include <windows.h>


void Start();
void Compile();
void Stop();

DWORD StartTime;




int main(int argc, char *argv[]) {
    std::cout << "Welcome to the Simply KoolB Compiler!" << std::endl;
    Start();
    Compile();
    Stop();
    return 0;
}

void Start() {
    StartTime = GetTickCount();
    return ;
}

void Compile() {
    return ;
}
void Stop() {
    std::cout << "Compile Time:\t"
              << static_cast<float>((GetTickCount()-StartTime) / 1000.0)
              << std::endl;
    return ;
}