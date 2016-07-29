#ifndef __TOKENIZERH_GUARD
#define __TOKENIZERH_GUARD

#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "JHException.h"

namespace JH
{
    class Tokenizer
    {
        /***
        Base Tokenizer class.
        Children will have parse() defined
        for whatever specific grammar.
        ***/
    public:
        class Token
        {
        public:
            Token()
            {
                this->type = "";
                this->content = "";
            }
            std::string type;
            std::string content;
        };
        Tokenizer();
        Tokenizer(std::string &str);
        ~Tokenizer();
        void input(std::string &str);
        void input(std::ifstream &file);
        virtual void parse() = 0;
        std::vector<Token>& getTokens();
    protected:
        std::istream* stream;
        std::vector<Token> tokens;
    };
}

#endif // __TOKENIZERH_GUARD
