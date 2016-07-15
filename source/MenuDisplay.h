#ifndef __MENUDISPLAYH_GUARD
#define __MENUDISPLAYH_GUARD

#include <string>
#include <vector>
#include <memory>

#include "IDisplay.h"

class MenuDisplay : public IDisplay
{
public:
    MenuDisplay();
    void draw();
private:
    class Structure
    {

    };
};

#endif // __MENUDISPLAYH_GUARD
