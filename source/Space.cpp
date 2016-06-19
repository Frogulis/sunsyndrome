#include "Space.h"

Space::Space()
{
    this->image_list = nullptr;
    this->id_array = nullptr;
    this->w = 0;
    this->d = 0;
    this->h = 0;
}

bool Space::loadFromFile(std::string filename)
{
    int mw, md, mh;

    filename += "/map";

    std::ifstream f;
    f.open(filename);
    if (!f.is_open())
    {
        std::cout << "Couldn't load level from " << filename << ".\n";
        return false;
    }
    f >> mw >> md >> mh;

    this->id_array = new int**[mw];
    for (int i = 0; i < mw; i++)
    {
        this->id_array[i] = new int*[md];
        for (int j = 0; j < md; j++)
        {
            this->id_array[i][j] = new int[mh];
        }
    }

    this->w = mw;
    this->d = md;
    this->h = mh;

    int current_number = 0;
    int cw = 0;
    int cd = -1;
    int ch = -1;

    f.ignore(256, '\n');
    //f.get(); //discarding the space
    std::cout << "Starting read.";

    while (!f.eof())
    {
        char c = 0;
        c = f.get();
        if (c == '\\')
        {
            cw = 0;
            cd = -1;
            ch += 1;
        }
        else if (c == '\n')
        {
            cw = 0;
            cd += 1;
        }
        else if (c == '-')
        {
            if (cw >= mw || cd >= md || ch >= mh)
            {
                std::cout << "Level at " << filename << " has incorrect formatting.\n";
                std::cout << cw << cd << ch;
                return false;
            }
            this->id_array[cw][cd][ch] = current_number;
            cw += 1;
            current_number = 0;
        }
        else if (c >= '0' && c <= '9')
        {
            current_number *= 10; //shift one digit to left
            current_number += c - '0'; //add the int value of digit
            //std::cout << "-*" << int(current_number) < "\n";
        }
    }
    std::cout << "Successfully loaded from " << filename << "\n";
    /*for (int i = 0; i < mh; i++)
    {
        for (int j = 0; j < mw; j++)
        {
            for (int k = 0; k < md; k++)
            {
                std::cout << this->id_array[j][k][i] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\\\n";
    }*/
    return true;
}

void Space::setImageList(ImageList* il)
{
    this->image_list = il;
}

ALLEGRO_BITMAP* Space::getImageFromLocation(int w, int d, int h)
{
    if (w >= this->w || d >= this->d || h >= this->h)
    {
        return nullptr;
    }
    return this->image_list->getImageFromID(this->id_array[w][d][h]);
}

int Space::getWidth()
{
    return this->w;
}

int Space::getDepth()
{
    return this->d;
}

int Space::getHeight()
{
    return this->h;
}
