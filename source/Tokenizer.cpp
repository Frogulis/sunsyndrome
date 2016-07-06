#include "Tokenizer.h"

JH::Tokenizer::Tokenizer()
{

}

JH::Tokenizer::Tokenizer(std::string &str)
: JH::Tokenizer::Tokenizer()
{
    this->input(str);
}

JH::Tokenizer::~Tokenizer()
{
    //
}

void JH::Tokenizer::input(std::string &str)
{
    std::stringstream ss(str);
    this->stream = &ss;
}

void JH::Tokenizer::input(std::ifstream &file)
{
    this->stream = &file;
}

std::vector<JH::Tokenizer::Token>& JH::Tokenizer::getTokens()
{
    return tokens;
}
