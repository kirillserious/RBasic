#include "Exeption.h"

#include <string>

Exeption :: Exeption (int code, const std::string &what)
        : _code(code), _what(what) {}

int
Exeption :: code() const
{
        return _code;
}

const std::string&
Exeption :: what() const
{
        return _what;
}