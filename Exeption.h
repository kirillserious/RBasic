#ifndef EXEPTION_H_
#define EXEPTION_H_

#include <string>

class Exeption
{
        int             _code;
        std::string     _what;
public:
        Exeption(int, const std::string&);

        virtual int code() const;
        virtual const std::string& what() const;
};

class ScannerExeption : public Exeption
{
public:
        ScannerExeption(const std::string& s)
            : Exeption(1, s) {}
};

class ParserExeption : public Exeption
{
public:
    ParserExeption (const std::string &what) : Exeption(2, what) {}
};

class InterExeption : public Exeption
{
public:
    InterExeption (const std::string &what) : Exeption(3, what) {}
};

#endif