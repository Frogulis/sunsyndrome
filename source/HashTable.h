#ifndef __HASHTABLEH_GUARD
#define __HASHTABLEH_GUARD

#include <functional>
#include <iostream>
#include <memory>

#include "JHException.h"

namespace JH
{
    class HashTableException : public JHException
    {
    public:
        HashTableException()
        {
            this->message = "Something went wrong in the HashTable. ";
        }
        explicit HashTableException(std::string message) : HashTableException()
        {
            this->message += message;
        }
    };

    class HTElementNotFoundException : public HashTableException
    {
    public:
        HTElementNotFoundException()
        {
            this->message += "No element could be found with the given key. ";
        }
        explicit HTElementNotFoundException(std::string message) : HTElementNotFoundException()
        {
            this->message += message;
        }
    };

    template <class KeyT, class DataT>
    class HashTable
    {
    public:
        HashTable();
        ~HashTable();
        void add(KeyT key, DataT unit);
        void del(KeyT key);
        DataT get(KeyT key);
        unsigned int length();
    private:
        std::hash<KeyT> h;
        std::unique_ptr<KeyT[]> keys;
        std::unique_ptr<DataT[]> data;
        std::unique_ptr<bool[]> taken_spots;
        unsigned int filled;
        unsigned int data_length;
        unsigned int section_size;
    };
}

template <class KeyT, class DataT>
JH::HashTable<KeyT,DataT>::HashTable()
{
    this->data_length = 0;
    this->filled = 0;
    this->section_size = 100;
}

template <class KeyT, class DataT>
JH::HashTable<KeyT,DataT>::~HashTable()
{
    /*if (this->data)
        delete [] this->data;
    if (this->taken_spots)
        delete [] this->taken_spots;
    if (this->keys)
        delete [] this->keys;*/
}

template <class KeyT, class DataT>
void JH::HashTable<KeyT,DataT>::add(KeyT key, DataT unit)
{
    /***
    Adds bucket associated with the given key.
    Reusing a key overwrites the data.
    ***/

    if (!this->data)
    {
        this->keys.reset(new KeyT[this->section_size]);
        this->data.reset(new DataT[this->section_size]);
        this->data_length = this->section_size;
        this->taken_spots.reset(new bool[this->section_size]);
        for (unsigned int i = 0; i < this->data_length; i++)
        {
            this->taken_spots[i] = false;
        }
    }
    else if (this->filled > this->data_length * 0.9) //extending when 90% or more
    {
        std::unique_ptr<KeyT[]> ktemp = std::move(this->keys);
        std::unique_ptr<DataT[]> temp = std::move(this->data);
        std::unique_ptr<bool[]> btemp = std::move(this->taken_spots);
        this->keys.reset(new KeyT[this->data_length + this->section_size]);
        this->data.reset(new DataT[this->data_length + this->section_size]);
        this->taken_spots.reset(new bool[this->data_length + this->section_size]);
        for (unsigned int i = 0; i < this->data_length + this->section_size; i++)
        {
            this->taken_spots[i] = false;
        }
        for (unsigned int i = 0; i < this->data_length; i++) //we now reallocate all of the existing elements
        {
            if (btemp[i])
            {
                size_t my_index = this->h(ktemp[i]) % (this->data_length + this->section_size);
                while (true)
                {
                    if (my_index < this->data_length + this->section_size)
                    {
                        if (!this->taken_spots[my_index])
                        {
                            this->data[my_index] = temp[i];
                            this->keys[my_index] = ktemp[i];
                            this->taken_spots[my_index] = true;
                            break;
                        }
                        my_index++;
                    }
                    else
                    {
                        my_index = 0;
                    }
                }
            }
        }
        this->data_length += this->section_size;
    }
    //now we have to actually add it
    size_t my_index = this->h(key) % this->data_length;
    bool overwritten = false; //just so we know whether it's an overwrite
    while (true) //so it will keep looking for a spot in the case of running off the end etc...
    {
        while (this->taken_spots[my_index])
        {
            if (this->keys[my_index] == key)
            {
                overwritten = true;
                break; //use this index and overwrite it
            }
            my_index++;
        }
        if (my_index < this->data_length)
        {
            this->data[my_index] = unit;
            this->keys[my_index] = key;
            this->taken_spots[my_index] = true;
            if (!overwritten) //shouldn't increase the 'length' if it's overwritten
            {
                this->filled++;
            }
            return;
        }
        else
        {
            my_index = 0; //jump back and keep looking... it must eventually find a spot
        }
    }
}

template <class KeyT, class DataT>
void JH::HashTable<KeyT,DataT>::del(KeyT key)
{
    /***
    Removes bucket for given key.
    Can safely delete at a non-existent key.
    ***/

    size_t my_index = this->h(key) % this->data_length;

    if (!this->taken_spots[my_index])
    {
        return;
    }

    while (true)
    {
        if (key == this->keys[my_index])
        {
            this->taken_spots[my_index] = false;
            this->filled--;
            return;
        }
        else
        {
            my_index++;
            if (my_index >= this->data_length)
            {
                my_index = 0;
            }
        }
    }
}

template <class KeyT, class DataT>
DataT JH::HashTable<KeyT,DataT>::get(KeyT key)
{
    /***
    Returns bucket for the given key.
    Throws HTElementNotFoundException if key is not found
    ***/

    if (!this->data_length)
    {
        throw JH::HTElementNotFoundException("HashTable empty!");
    }

    size_t my_index = this->h(key) % this->data_length;

    if (!this->taken_spots[my_index])
    {
        throw JH::HTElementNotFoundException();
    }

    while (true)
    {
        if (key == this->keys[my_index])
        {
            return this->data[my_index];
        }
        else
        {
            my_index++;
            if (my_index >= this->data_length)
            {
                my_index = 0;
            }
        }
    }
}

template <class KeyT, class DataT>
unsigned int JH::HashTable<KeyT,DataT>::length()
{
    /***
    Returns the number of filled buckets in the table.
    ***/
    return this->filled;
}

#endif // __HASHTABLEH_GUARD
