#ifndef READ_HPP
#define READ_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>


//Holds information for the $Const directive
struct ConstData{
    std::string Value;
    int Type;
};

class Reading {
public:
    Reading() {
        CurrentLine = 1;
        Uppercase = true;
    }
    ~Reading() {}

    void OpenBook(std::string FileName);
    void GetNextWord();
    bool SkipWhiteSpace();
    void GetIdentifier();
    void GetNumber();
    void GetString();
    void GetSymbol();

    void SetUppercase(bool isUpperCase);
    void CheckWordLength();
    std::string Word();
    int WordType();

    long GetBookMark();
    long GetCurrentLine();
    std::string GetBookName();
    std::string GetBook();
    std::string GetWholeLine();

    void AddConstData(std::string Name, std::string Value, int Type);
    bool IsConstData(std::string Name);

    enum WordTypes{EndOfLine, Identifier, Number, String, Symbol, None};


private:
    // The entire file
    std::string Book;
    
    // Stores our current location within the file
    int BookMark;
    
    //  The size of the entire book
    int BookSize;
    
    // The file name
    std::string BookName;
    
    // Length of the file
    long BookLength;
    
    // The current line we are on within the file
    long CurrentLine;
    
    // The current word to be analyzed
    std::string CurrentWord;
    
    // The data type of word the current word
    int TypeOfWord;
    
    // True if we should convert the word to uppercase
    bool Uppercase;
    
    // A hash of BASIC syntax replacements for const ($Const)
    std::map<std::string, ConstData> Const;
};


void Reading::OpenBook(std::string FileName) {
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
    BookSize = FileEnd - FileStart;
    Spoon = new char[BookSize];

    // Read the file into Spoon
    File.read(Spoon, BookSize);

    // Add character to signal the end of the char buffer
    Spoon[BookSize] = '\0';

    // Copy Spoon into Book
    Book = Spoon;

    // Update the new BookLength
    BookLength = Book.length();

    // Clean up manually allocated memory, close file
    delete[] Spoon;
    File.close();

    return ;
}


// GetNextWord()
void Reading::GetNextWord() {
    // Clear contents of CurrentWord
    CurrentWord = "";

    // Check for end of file
    if (!Book[BookMark] or BookMark >= BookSize) {
        // No type can exist past the end of the file
        TypeOfWord = None;
        return ;
    }

    if (SkipWhiteSpace() == false) {
        return ;
    }

    // Check for end of file again now that we skipped white space
    if (!Book[BookMark]) {
        // No type can exist past the end of the file
        TypeOfWord = None;
        return ;
    }

    if (isalpha(Book[BookMark])) {
        GetIdentifier();
        CheckWordLength();
        return ;
    }
    if (isdigit(Book[BookMark]) or Book[BookMark] == '.') {
        GetNumber();
        CheckWordLength();
        return ;
    }
    if (Book[BookMark] == '\"') {
        GetString();
        CheckWordLength();
        return ;
    }
    if (ispunct(Book[BookMark])) {
        GetSymbol(); 
        CheckWordLength();
        return ;
    }
    TypeOfWord = None;
    return ;
}

// SkipWhiteSpace()
// Returns false if it found an end of line, otherwise it will return true
// because it skipped all exisitng white space. 
bool Reading::SkipWhiteSpace() {
    CurrentWord = "";

    // Skip over comments and whitespace
    while (isspace(Book[BookMark]) or Book[BookMark] == '\'') {

        // If we find a comment, move to the end of the line
        if (Book[BookMark] == '\'') { 
            ++BookMark;
            while (Book[BookMark] != '\n' and BookMark < BookLength) {
                ++BookMark;
            }
        }

        // If we hit an end of line character, return false.
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
void Reading::GetIdentifier() { 
    do {
        if (!Book[BookMark]) {
            break;
        }

        // Convert to uppercase if flag is set
        if (!Uppercase) {
            // Preserve case (this will usually be the case)
            CurrentWord += Book[BookMark];
        }
        else {
            CurrentWord += toupper(Book[BookMark]);
        }
        ++BookMark;
    } while (isalpha(Book[BookMark]) or isdigit(Book[BookMark]) or Book[BookMark] == '_');

    // If we find a BASIC Character, then add it to CurrentWord
    if (Book[BookMark] == '&' or Book[BookMark] == '#' or Book[BookMark] == '$') {
        CurrentWord += Book[BookMark];
        ++BookMark;
    }
    
    TypeOfWord = Identifier;

    // Check for CONSTness
    if (IsConstData(CurrentWord)){
        TypeOfWord = Const[CurrentWord].Type;
        CurrentWord = Const[CurrentWord].Value; 
    }
    return ;
}


// GetNumber()
// A number is an integer or floating point number (i.e. has one decimal)
void Reading::GetNumber() {
    bool ReachedDot = false;
    do {
        if (Book[BookMark] == '.') {

            // Each number can contain at most one decimal point
            if (ReachedDot) {
                std::cout << "SyntaxError on line: " << CurrentLine << std::endl;
                std::cout << "ERR: Found two consecutive decimal points."
                          << std::endl;
                exit(1); // Exit
            }
            ReachedDot = true;
        }
            CurrentWord += Book[BookMark];
            ++BookMark;
    } while (isdigit(Book[BookMark]) or Book[BookMark] == '.');

    // If both are true, we have at least one decimal point
    if (CurrentWord.length() == 1 and ReachedDot) {
        TypeOfWord = Symbol;
    }
    else {
        TypeOfWord = Number;
    }
    return ;
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
            exit(1);
        }
    }
    ++BookMark;
    TypeOfWord = String;
    return ;
}


// GetSymbol()
// Cases:
// 1. '_' (underscore) will tell the compiler to continue on the next line
// 2. ':' (colon) will tell the compiler to split the statement into two lines
void Reading::GetSymbol() {
    // Case 1: underscore, which means continue the word to the next line.
    if (Book[BookMark] == '_') {
        ++BookMark;
        if (SkipWhiteSpace()) {
            std::cout << "Error on line: " << CurrentLine << std::endl;
            std::cout << "ERR: Newline not found after \'_\'" << std::endl;
            exit(1);
        }
        GetNextWord();
        return ;
    }

    // Case 2: colon, which is an end of line marker.
    if (Book[BookMark] == ':') {
        CurrentWord = '\n';
        ++BookMark;
        TypeOfWord = EndOfLine;
        return ;
    }
    CurrentWord = Book[BookMark];
    ++BookMark;

    // We need at least two words to process these operations
    if (CurrentWord == '<') {
        if (Book[BookMark] == '=' or Book[BookMark] == '>') {
            CurrentWord += Book[BookMark];
            ++BookMark;
        }
    }
    if (CurrentWord == '>') {
        if (Book[BookMark] == '=') {
            CurrentWord += Book[BookMark];
            ++BookMark;
        }
    }

    TypeOfWord = Symbol;
    return ;
}


// CheckWordLength()
// Maximum character limit per word is 128 characters (half a byte)
void Reading::CheckWordLength() {
    if (CurrentWord.length() > 128 and TypeOfWord != String) {
        std::cout << "Error on line: " << CurrentLine << std::endl;
        std::cout << "ERR: A word exceeded the 128 character limit" << std::endl;
        exit(1);
    }
    return ;
}


// Word() returns the CurrentWord
std::string Reading::Word() {
    return CurrentWord;
}


// WordType() returns the Type of the CurrentWord 
int Reading::WordType() {
    return TypeOfWord;
}


// GetBookMark() returns the current position in the file
long Reading::GetBookMark() {
    return BookMark;
}


// GetCurrentLine() returns the current line
long Reading::GetCurrentLine() {
    return CurrentLine;
}


// GetBookName() returns the name of the source file being compiled
std::string Reading::GetBookName() {
    return BookName;
}


// GetBook() returns contents of the entire source file
std::string Reading::GetBook() {
    return Book;
}


// SetUppercase() will toggle uppercase formatting on or off
void Reading::SetUppercase(bool isUpperCase) {
    Uppercase = isUpperCase;
    return ;
}


// GetWholeLine() gets a the current line for error reporting
std::string Reading::GetWholeLine() {
    std::string Line = "";
    while (Book[BookMark]) {
        if (Book[BookMark] == '\n') {
            BookMark++;
            break;
        }
        Line += Book[BookMark];
        ++BookMark;
    }
    return Line;
}


// AddConstData() adds a constant as defined by $Const
void Reading::AddConstData(std::string Name, std::string Value, int Type) {
    Const[Name].Value = Value;
    Const[Name].Type  = Type;
    return ;
}


// IsConstData() checks to see if a word is defined as a constant
bool Reading::IsConstData(std::string Name) {
  if (Const.find(Name) != Const.end()){
    return true;
  }
  return false;
}


#endif // READ_HPP