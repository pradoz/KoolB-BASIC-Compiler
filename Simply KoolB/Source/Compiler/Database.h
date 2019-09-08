#ifndef DATABASE_HPP
#define DATABASE_HPP


#include <string>
#include <map>

// Stores literal data types (int, str, double)
struct SimpleDataInfo {
    std::string AsmName;
    int Type;
};


// Stores information about the array data type
struct ArrayDataInfo {
    std::string AsmName;
    int Type;
};


// Stores information about the relationship between types and their members
struct TypeDataInfo {
    std::string AsmName;
    int Size;
    std::map<std::string, SimpleDataInfo> Members;
};






#endif // DATABASE_HPP