#include <iostream>
#include <vector>
#include <memory>


int main() {
    std::vector<int> vec1;
    std::vector<int> vec2;
    std::unique_ptr<int> original_vector_ptr = vec(new int);
    // unique_ptr<std::vector<int>> new_vector_ptr = std::move(original_vector_ptr);
    return 0;
}