#ifndef __MENUDISPLAYH_GUARD
#define __MENUDISPLAYH_GUARD

#include <string>
#include <vector>

#include "IDisplay.h"

class MenuDisplay : public IDisplay
{
public:
    MenuDisplay();
    void draw();
private:
    //
};

#endif // __MENUDISPLAYH_GUARD
