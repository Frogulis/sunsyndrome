#include "AStar.h"

JH::UndirectedGraph2D::UndirectedGraph2D(DirFlags flags)
{
    if (flags & cardinal)
    {
        dirs.push_back(std::pair<int,int>(-1, 0));
        dirs.push_back(std::pair<int,int>(1,  0));
        dirs.push_back(std::pair<int,int>( 0,-1));
        dirs.push_back(std::pair<int,int>( 0, 1));
    }
    else if (flags & diagonal)
    {
        dirs.push_back(std::pair<int,int>(-1,-1));
        dirs.push_back(std::pair<int,int>( 1,-1));
        dirs.push_back(std::pair<int,int>( 1, 1));
        dirs.push_back(std::pair<int,int>(-1, 1));
    }

}

JH::UndirectedGraph2D::UndirectedGraph2D(bool** a, int size_x, int size_y, DirFlags flags) : JH::UndirectedGraph2D::UndirectedGraph2D(flags)
{
    this->generateFromArray(a, size_x, size_y);
}

JH::UndirectedGraph2D::~UndirectedGraph2D()
{
    for (std::vector<Node*>::iterator i = this->nodes.begin(); i != nodes.end(); i++)
    {
        delete *i;
    }
}

void JH::UndirectedGraph2D::generateFromArray(bool** a, int size_x, int size_y)
{
    this->size_x = size_x;
    this->size_y = size_y;
    for (int x = 0; x < size_x; x++) //loop once to generate the nodes
    {
        for (int y = 0; y < size_y; y++)
        {
            if (!a[x][y])
            {
                continue;
            }
            Node* cur = this->getNode(x, y);
            if (!cur) // if the current space isn't filled
            {
                cur = new Node;
                cur->grid_coords.first = x;
                cur->grid_coords.second = y;
                this->nodes.push_back(cur);
            }
            for (std::vector<std::pair<int,int> >::iterator i = dirs.begin(); i != dirs.end(); i++)
            {
                std::pair<int,int> new_coords = addPairs(cur->grid_coords, *i);
                if (validRel(a, new_coords))
                {
                    Node* child = this->getNode(new_coords.first, new_coords.second);
                    if (!child)
                    {
                        child = new Node;
                        child->grid_coords = new_coords;
                        this->nodes.push_back(child);
                    }
                    cur->relations.push_back(std::pair<Node*, float>(child, std::sqrt(std::pow(i->first, 2) + std::pow(i->second, 2))));
                }
            }

        }
    }
}

JH::UndirectedGraph2D::Node* JH::UndirectedGraph2D::operator()(int x, int y)
{
    return this->getNode(x, y);
}

JH::UndirectedGraph2D::Node* JH::UndirectedGraph2D::getNode(int x, int y)
{
    if (this->nodes.size() == 0)
    {
        return nullptr;
    }
    for (std::vector<Node*>::iterator i = this->nodes.begin(); i != this->nodes.end(); i++)
    {
        if ((*i)->grid_coords == std::pair<int,int>(x,y))
        {
            return *i;
        }
    }
    return nullptr;
}

bool JH::UndirectedGraph2D::validRel(bool** a, std::pair<int,int> sum)
{
    if (sum.first < 0 || sum.second < 0
        || sum.first >= this->size_x || sum.second >= this->size_y
        || !a[sum.first][sum.second])
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::pair<int,int> JH::UndirectedGraph2D::addPairs(std::pair<int,int> a, std::pair<int,int> b)
{
    return std::pair<int,int>(a.first + b.first, a.second + b.second);
}

std::pair<int,int> JH::UndirectedGraph2D::differencePairs(std::pair<int,int> a, std::pair<int,int> b)
{
    return std::pair<int,int>(std::abs(a.first-b.first), std::abs(a.second-b.second));
}

JH::AStar::AStar(bool** a, int size_x, int size_y) :
    network(a, size_x, size_y, JH::UndirectedGraph2D::DirFlags(JH::UndirectedGraph2D::cardinal)),
    start(0,0),
    goal(0,0)
{

}

float hPythag(std::pair<int,int> a, std::pair<int,int> b)
{
    return std::sqrt(std::pow(std::abs(a.first-b.first), 2) + std::pow(std::abs(a.second-b.second), 2));
}

float hManhattan(std::pair<int,int> a, std::pair<int,int> b)
{
    return std::abs(a.first-b.first) + std::abs(a.second-b.second);
}

void JH::AStar::generatePath(std::pair<int,int> start, std::pair<int,int> goal)
{
    typedef JH::UndirectedGraph2D::Node GNode;
    struct PNode //path node
    {
        PNode* parent;
        float f, g, h;
        GNode* gnode;
    };

    std::vector<PNode*> open;
    std::vector<PNode*> closed;

    result.clear();

    PNode* first = new PNode;
    first->parent = nullptr;
    first->gnode = this->network.getNode(start.first, start.second);
    first->g = 0;
    first->h = hManhattan(first->gnode->grid_coords, goal);
    first->f = first->g + first->h;
    open.push_back(first);

    while (!open.empty())
    {
        std::vector<PNode*>::iterator lowest_f = open.begin();
        for (std::vector<PNode*>::iterator i = open.begin(); i != open.end(); i++)
        {
            if ((*i)->f < (*lowest_f)->f)
            {
                lowest_f = i;
            }
        }
        PNode* q = (*lowest_f);
        open.erase(lowest_f);

        for (std::vector<std::pair<GNode*, float> >::iterator i = q->gnode->relations.begin(); i != q->gnode->relations.end(); i++)
        {
            PNode* child = new PNode;
            child->parent = q;
            child->gnode = i->first;
            child->g = q->g + i->second; //i->second is the weight of that edge
            child->h = hManhattan(child->gnode->grid_coords, goal);
            child->f = child->g + child->h;
            if (child->gnode->grid_coords == goal)
            {
                PNode* cur = child;
                while (cur)
                {
                    result.push_back(cur->gnode->grid_coords);
                    for (std::vector<PNode*>::iterator i = open.begin(); i != open.end(); i++)
                    {
                        //delete (*i);
                    }
                    for (std::vector<PNode*>::iterator i = closed.begin(); i != closed.end(); i++)
                    {
                        //delete (*i);
                    }
                    cur = cur->parent;
                }
                return;
            }
            try
            {
                for (std::vector<PNode*>::iterator i = open.begin(); i != open.end(); i++)
                {
                    if ((*i)->gnode->grid_coords == child->gnode->grid_coords)
                    {
                        throw std::exception();
                    }
                }
                for (std::vector<PNode*>::iterator i = closed.begin(); i != closed.end(); i++)
                {
                    if ((*i)->gnode->grid_coords == child->gnode->grid_coords)
                    {
                        throw std::exception();
                    }
                }
            }
            catch (std::exception &e) //there are better paths that go through this spot so we discard it lol
            {
                delete child;
                continue;
            }
            open.push_back(child);
        }
        closed.push_back(q);
    }
}

std::vector<std::pair<int,int> >& JH::AStar::getPath()
{
    return this->result;
}
