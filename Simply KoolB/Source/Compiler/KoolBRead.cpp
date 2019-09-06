#include "KoolBRead.hpp"
#include <iostream>


void Reading::OpenBook(std::string FileName){
    std::ifstream File(FileName.c_str(), std::ios::in);
    long FileStart;
    long FileEnd;
    char* Spoon;

    // Check to see if the file was opened successfully
    if (File.is_open() == false) {
        std::cout << "Error! Could not open file: " << FileName << std::endl;
        exit(1);
    }
    // Get the current position of the file
    FileStart = File.tellg();
    File.seekg(0, std::ios::end);

    // Set the current position of the file
    FileEnd = File.tellg();
    File.seekg(0, std::ios::beg);

    // Create a Spoon to temporarily hold the file contents
    Spoon = new char[FileEnd - FileStart];

    File.read(Spoon, FileEnd - FileStart);
    Book = Spoon;
    BookLength = Book.length();

    // Clean up
    delete[] Spoon;
    File.close();

    return ;
}

// --------------------------------------------------------
// GetNextWord() 
// --------------------------------------------------------
void Reading::GetNextWord(){
    if (SkipWhiteSpace() == false){
        return ;
    }
    if (isalpha(Book[BookMark])){
        GetIdentifier();
        return ;
    }
    if (isdigit(Book[BookMark]) or Book[BookMark] == '.'){
        GetNumber();
        return ;
    }
    if (Book[BookMark] == '\"'){
        GetString();
        return ;
    }
    if (ispunct(Book[BookMark])){
        GetSymbol(); 
        return ;
    }
    TypeOfWord = None;
    return ;
}

// SkipWhiteSpace()
// Returns false if it found an end of line, otherwise it will return true
// because it skipped all exisitng white space. 
bool Reading::SkipWhiteSpace(){
    CurrentWord = "";
    while (isspace(Book[BookMark]) or Book[BookMark] == '\'') {
        if (Book[BookMark] == '\'') {
            ++BookMark;
            while (Book[BookMark] != '\n' and BookMark < BookLength) {
                ++BookMark;
            }
        }
        if (Book[BookMark] == '\n') {
            CurrentWord = Book[BookMark];
            TypeOfWord = EndOfLine;
            ++BookMark;
            ++CurrentLine;
            return false;
        }
        ++BookMark;
    }
    return true;
}


// GetIdentifier() adds the next character to our word while the next character
// is part of the alphabet or a digit. When the do statement conditions to
// exit, it stops and assigns Identifier to TypeOfWord.
void Reading::GetIdentifier(){ 
    do {
        CurrentWord += toupper(Book[BookMark]);
        ++BookMark;
    } while (isalpha(Book[BookMark]) or isdigit(Book[BookMark]));

    TypeOfWord = Identifier;
}


// GetNumber()
// A number is an integer or floating point number (i.e. has one decimal)
void Reading::GetNumber() {
    bool ReachedDot = false;
    do {
        if (Book[BookMark] == '.') {

            // Each number can contain at most one decimal point
            if (ReachedDot == true) {
                std::cout << "SyntaxError on line: " << CurrentLine << std::endl;
                std::cout << "ERR: Found two consecutive decimal points."
                          << std::endl;
                exit(0); // Exit sucessfully
            }
            ReachedDot = true;
        }
            CurrentWord += Book[BookMark];
            ++BookMark;
    } while (isdigit(Book[BookMark]) or Book[BookMark] == '.');

    // If both are true, we have at least one decimal point
    if (CurrentWord.length() == 1 and ReachedDot == true) {
        TypeOfWord = Symbol;
    }
    else {
        TypeOfWord = Number;
    }
} 


// GetString()
// A string (denoted "" is anything that comes between two double quotes)
// We only want the contents of the string, which is what is between the quotes
void Reading::GetString() {
    ++BookMark;
    while (Book[BookMark] != '\"') {
        CurrentWord += Book[BookMark];
        ++BookMark;
        if (BookMark > BookLength or Book[BookMark] == '\n') {
            std::cout << "SyntaxError on line: " << CurrentLine << ", at "<< CurrentWord << std::endl;

            std::cout << "ERR: Closing quotation mark not found on string." << std::endl;
            exit(0);
        }
    }
    ++BookMark;
    TypeOfWord = String;
} 


// GetSymbol()
// Cases:
// 1. '_' (underscore) will tell the compiler to continue on the next line
// 2. ':' (colon) will tell the compiler to split the statement into two lines
void Reading::GetSymbol() {
    // Case 1: underscore
    if (Book[BookMark] == '_') {
        ++BookMark;
        if (SkipWhiteSpace() == true) {
            std::cout << "Error on line: " << CurrentLine << std::endl;
            std::cout << "ERR: Newline not found after \'_\'" << std::endl;
            exit(1);
        }
        GetNextWord();
        return ;
    }

    // Case 2: colon
    if (Book[BookMark] == ':') {
        CurrentWord = '\n';
        ++BookMark;
        TypeOfWord = EndOfLine;
        return ;
    }
    CurrentWord = Book[BookMark];
    ++BookMark;
    TypeOfWord = Symbol;
}


// CheckWordLength()
// Maximum character limit per word is 128 characters (half a byte)
void Reading::CheckWordLength() {
    if (CurrentWord.length() > 128 and TypeOfWord != String) {
        std::cout << "Error on line: " << CurrentLine << std::endl;
        std::cout << "ERR: A word exceeds the 128 character limit" << std::endl;
        exit(1);
    }
    return ;
}

std::string Reading::Word() {
    CheckWordLength();
    return CurrentWord;
}

int Reading::WordType() {
    return TypeOfWord;
}