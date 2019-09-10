#include <iostream>

int main() {
    bool flag = false;
    int arr[5] = {0};
    for (int i = 0; i < 15; ++i) {
        std::cout << arr[i] << std::endl;
    }
    flag
    ? std::cout << "True" << std::endl
    : std::cout << "False" << std::endl;
    
    
    return 0;
}