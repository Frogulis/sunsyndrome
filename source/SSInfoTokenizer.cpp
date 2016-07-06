#include "SSInfoTokenizer.h"

void JH::SSInfoTokenizer::parse()
{
    this->stream->seekg(0);

    std::string buffer = ""; //for reading in numbers etc.
    Token temp;

    while (!this->stream->eof())
    {
        char c = this->stream->get();

        if (c == '\n')
        {
            temp.type = "newline";
            this->tokens.push_back(temp);
        }
        else if (c == '/')
        {
            temp.type = "next_section";
            this->tokens.push_back(temp);
        }
        else if (c == ',')
        {
            temp.type = "next_item";
            this->tokens.push_back(temp);
        }
        else if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9' && temp.type == "word"))) //so we have strings with digits
        {
            temp.type = "word";
            buffer += c;
            if (!(this->stream->peek() >= 'a' && this->stream->peek() <= 'z')
                && !(this->stream->peek() >= 'A' && this->stream->peek() <= 'Z')
                && this->stream->peek() != '_'
                && !(this->stream->peek() >= '0' && this->stream->peek() <= '9')) // end of word
            {
                temp.content = buffer;
                this->tokens.push_back(temp);
                buffer = "";
                temp.type = "";
            }
        }
        else if (c >= '0' && c <= '9')
        {
            temp.type = "number";
            buffer += c;
            if (this->stream->peek() < '0' || this->stream->peek() > '9') //if it's the last digit
            {
                temp.content = buffer;
                this->tokens.push_back(temp);
                buffer = "";
                temp.type = "";
            }
        }
    }
}
