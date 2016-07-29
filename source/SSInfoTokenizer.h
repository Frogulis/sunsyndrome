#ifndef __SSINFOTOKENIZERH_GUARD
#define __SSINFOTOKENIZERH_GUARD

#include <sstream>

#include "Tokenizer.h"
#include "StringUtils.h"

namespace JH
{
    class SSInfoTokenizer : public Tokenizer
    {
    public:
        void parse();
    };
}


#endif // __SSINFOTOKENIZERH_GUARD
