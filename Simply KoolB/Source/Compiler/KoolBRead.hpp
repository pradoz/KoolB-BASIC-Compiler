#include <fstream>
#include <string>


class Reading {
public:
    Reading() {
        BookMark = 0;
        CurrentLine = 1;
    }

    void OpenBook(string FileName);

    void GetNextWord();

    bool SkipWhiteSpace();

    void GetIdentifier();

    void GetNumber();

    void GetString();

    void GetSymbol();

    void CheckWordLength();

    std::string Word();

    int WordType();

    enum WordTypes{EndOfLine, Identifier, Number, String, Symbol, None};


private:
    std::string Book;
    long BookLength;
    long BookMark;
    long CurrentLine;
    std::string CurrentWord;
    int TypeOfWord;
};



void Reading::OpenBook(std::string FileName){
    ifstream File(FileName.c_str(), ios::in);
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
    File.seekg(0, ios::end);

    // Set the current position of the file
    FileEnd = File.tellg();
    File.seekg(0, ios::beg);

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












