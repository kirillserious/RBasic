#include <iostream>

#include "Parser/Parser.h"
#include "Interpreter/Interpreter.h"
#include "Exeption.h"

using namespace std;

void
inter (Parser &parser) {
        while (true) {
                try {
                        auto poliz = parser.get_line();
                        if (poliz.empty()) return;
                        Interpreter:: make_line(poliz);
                } catch (Exeption& e) {
                        cout << e.what() << endl;
                } catch (const char *s) {
                        cout << s << endl;
                }
        }
}

int
main (int argc, char* argv[])
{
        for (int i = 1; i < argc; ++i) {
                try {
                        Parser parser(argv[i]);
                        inter(parser);
                } catch (Exeption &e) {
                        cout << e.what() << endl;
                } catch (const char *s) {
                        cout << s << endl;
                }
        }

        try {
                Parser parser;
                inter(parser);
        } catch (Exeption &e) {
                cout << e.what() << endl;
        } catch (const char *s) {
                cout << s << endl;
        }

        return 0;
}