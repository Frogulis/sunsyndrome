#include "Space.h"

Space::Space()
{
    this->id_array = nullptr;
    this->w = 0;
    this->d = 0;
    this->h = 0;
    this->arena_height = 0;
}

Space::~Space()
{
    this->deleteIDArray();
}

bool Space::loadFromFile(std::string filename)
{
    filename = "resources/levels/" + filename;

    if (!this->image_list.loadImagesFromIndexFile(filename))
    {
        return false;
    }

    int mw, md, mh;

    filename += "/map";

    std::ifstream f;
    f.open(filename);
    if (!f.is_open())
    {
        std::cout << "Can't open file at " << filename << ".\n";
        return false;
    }
    f >> mw >> md >> mh;

    this->id_array = new Tile**[mw];
    for (int i = 0; i < mw; i++)
    {
        this->id_array[i] = new Tile*[md];
        for (int j = 0; j < md; j++)
        {
            this->id_array[i][j] = new Tile[mh];
        }
    }

    this->w = mw;
    this->d = md;
    this->h = mh;

    int current_number = 0;
    int cw = 0;
    int cd = -1;
    int ch = -1;

    bool has_walkable = false;

    f.ignore(256, '\n');
    //f.get(); //discarding the space
    std::cout << "Starting read of " << filename << ".\n";

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
        else if (c == '-' || c == '+')
        {
            if (cw >= mw || cd >= md || ch >= mh)
            {
                std::cout << "Level at " << filename << " has incorrect formatting.\n";
                std::cout << cw << cd << ch;
                this->deleteIDArray();
                return false;
            }
            this->id_array[cw][cd][ch].id = current_number;
            if (c == '+')
            {
                this->id_array[cw][cd][ch].walkable = true;
                has_walkable = true;
                this->arena_height = ch;
            }
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

    if (!has_walkable)
    {
        std::cout << "Level at " << filename << " has no walkable tiles.\n";
        this->deleteIDArray();
        return false;
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

ALLEGRO_BITMAP* Space::getImageFromLocation(int w, int d, int h)
{
    if (!this->within_array_bounds(w, d, h))
    {
        return nullptr;
    }
    return this->image_list.getImageFromID(this->id_array[w][d][h].id);
}

bool Space::getWalkableFromLocation(int w, int d, int h)
{
    if (!this->within_array_bounds(w, d, h))
    {
        return false;
    }
    return this->id_array[w][d][h].walkable;
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

int Space::getArenaHeight()
{
    return this->arena_height;
}

bool Space::within_array_bounds(int w, int d, int h)
{
    return !(w >= this->w || d >= this->d || h >= this->h);
}

void Space::deleteIDArray()
{
    if (this->id_array)
    {
        for (int i = 0; i < this->w; i++)
        {
            for (int j = 0; j < this->d; j++)
            {
                delete [] this->id_array[i][j];
            }
            delete [] this->id_array[i];
        }
        delete [] this->id_array;
    }
    this->id_array = nullptr;
}
