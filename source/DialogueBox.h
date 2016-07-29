#ifndef __DIALOGUEBOXH_GUARD
#define __DIALOGUEBOXH_GUARD

#include <iostream>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

class DialogueBox
{
public:
    DialogueBox();
    bool load(std::string boxname);
    void setDimensions(float x, float y, float tile_size=10);
    void setText(std::string text);
    ALLEGRO_BITMAP* getBoxBitmap();
private:
    ALLEGRO_BITMAP* tiles[9];
    std::string text;
    ALLEGRO_FONT* font;
    float tile_size;
    float x, y;
};

#endif // __DIALOGUEBOXH_GUARD


