#ifndef __ASTARH_GUARD
#define __ASTARH_GUARD

#include <vector>
#include <iostream>
#include <list>

#include <cmath>

namespace JH
{
    class UndirectedGraph2D
    {
    public:
        enum DirFlags {cardinal = 1, diagonal = 1 << 1};
        struct Node
        {
            Node()
            {
                this->grid_coords.first = 0;
                this->grid_coords.second = 0;
            }
            Node(std::pair<int,int> coords) //we must populate relations manually
            {
                this->grid_coords = coords;
            }
            std::vector<std::pair<Node*, float> > relations; //float is edge weight
            std::pair<int, int> grid_coords;
        };
        UndirectedGraph2D(DirFlags flags);
        UndirectedGraph2D(bool** a, int size_x, int size_y, DirFlags flags);
        ~UndirectedGraph2D();
        void generateFromArray(bool** a, int size_x, int size_y);
        Node* operator()(int x, int y);
        Node* getNode(int x, int y);
    private:
        bool validRel(bool** a, std::pair<int,int> sum);
        std::pair<int,int> addPairs(std::pair<int,int> a, std::pair<int,int> b);
        std::pair<int,int> differencePairs(std::pair<int,int> a, std::pair<int,int> b);
        std::vector<Node*> nodes;
        std::vector<std::pair<int,int> > dirs; //cardinal and diagonal directions in (-1..1,-1..1) format
        int size_x, size_y;
    };

    class AStar
    {
    public:
        AStar(bool** a, int size_x, int size_y);
        void generatePath(std::pair<int,int> start, std::pair<int,int> goal);
        std::vector<std::pair<int, int> >& getPath();
    private:
        UndirectedGraph2D network;
        std::vector<std::pair<int, int> > result;
        std::pair<int,int> start, goal;
    };
}

#endif // __ASTARH_GUARD
