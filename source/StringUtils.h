#ifndef __STRINGUTILSH_GUARD
#define __STRINGUTILSH_GUARD

#include <string>

namespace JH
{
    class StringUtils
    {
    public:
        static std::string intToString(long long int i);

        static long long int stringToInt(std::string s)
        {
            long long int result = 0;

            for (std::string::iterator i = s.begin(); i != s.end(); i++)
            {
                if (*i >= '0' && *i <= '9')
                {
                    result *= 10;
                    result += *i - '0';
                }
            }

            return result;
        }
    };
}


#endif // __STRINGUTILSH_GUARD
