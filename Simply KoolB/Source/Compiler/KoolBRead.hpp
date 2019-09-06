#ifndef KOOLBREAD_HPP
#define KOOLBREAD_HPP

#include <fstream>
#include <string>


class Reading {
public:
    Reading() {
        BookMark = 0;
        CurrentLine = 1;
    }

    void OpenBook(std::string FileName);

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

#endif // KOOLBREAD_HPP