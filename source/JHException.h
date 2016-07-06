#ifndef __JHEXCEPTIONH_GUARD
#define __JHEXCEPTIONH_GUARD

class JHException
{
public:
    JHException()
    {

    }
    std::string what()
    {
        return this->message;
    }
protected:
    std::string message;
};

#endif // __JHEXCEPTIONH_GUARD
