#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>

class IGraph {
public:
    IGraph() {};
    IGraph(const IGraph& _oth) {};
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;
    virtual int VerticesCount() const = 0;
    virtual std::vector<int> Vertices() const = 0;
    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;

    virtual void printGraph() = 0;
};

class ListGraph : public IGraph
{
public:
    ListGraph(){}
    ListGraph(const IGraph& oth) 
    {
        verticesIn.clear();
        verticesOut.clear();

        for (const auto& vertex : oth.Vertices())
        {
            verticesOut.insert(std::pair<int, std::vector<int>>(vertex, oth.GetNextVertices(vertex)));
            verticesIn.insert(std::pair<int, std::vector<int>>(vertex, oth.GetPrevVertices(vertex)));
        }
    }
    ~ListGraph() override{}

    void AddEdge(int from, int to) override
    {
        checkVertex(from);
        checkVertex(to);

        addLink(verticesIn, to, from);
        addLink(verticesOut, from, to);
    }
    int VerticesCount() const override
    {
        return verticesIn.size();
    }
    std::vector<int> Vertices() const override
    {
        std::vector<int> vertices;
        for (const auto& el : verticesIn)
        {
            vertices.push_back(el.first);
        }
        return vertices;
    }
    std::vector<int> GetNextVertices(int vertex) const override
    {
        return (verticesOut.find(vertex)->second);
    }
    std::vector<int> GetPrevVertices(int vertex) const override
    {
        return (verticesIn.find(vertex)->second);
    }

    void printGraph() override
    {
        for (auto& elem : verticesOut)
        {
            std::sort(elem.second.begin(), elem.second.end());
            std::cout << std::setw(4) << elem.first << ": ";
            for (const auto& vertices : elem.second)
            {
                std::cout << std::setw(4) << vertices;
            }  
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

private:
    std::map<int, std::vector<int>> verticesOut;
    std::map<int, std::vector<int>> verticesIn;

    void checkVertex(int vertex)
    {
        if (verticesIn.find(vertex) == verticesIn.end())
        {
            verticesIn.insert(std::pair<int, std::vector<int>>(vertex, 0));
        }  
        if (verticesOut.find(vertex) == verticesOut.end())
        {
            verticesOut.insert(std::pair<int, std::vector<int>>(vertex, 0));
        }        
    }

    void addLink(std::map<int, std::vector<int>>& inMap, int to, int from)
    {
        std::map<int, std::vector<int>>::iterator itf = inMap.find(to);
        bool found = false;
        for (int i = 0; !found && (i < itf->second.size()); ++i)
        {
           if (itf->second[i] == from)
           {
              found = true;
           }        
        } 

        if (!found)
        {
           itf->second.push_back(from);
        }      
    }
};

class MatrixGraph : public IGraph
{
public:
    MatrixGraph(){}
    MatrixGraph(const IGraph& oth)
    {
        vertex2index.clear();
        verticesQueue.clear();
        matrix.clear();

        for (const auto& vertex : oth.Vertices())
        {
            for (const auto& next : oth.GetNextVertices(vertex))
            {
                AddEdge(vertex, next);
            }
        }          
    }

    ~MatrixGraph() override{}

    void AddEdge(int from, int to) override
    {
        checkVertex(from);
        checkVertex(to);

        size_t verticesCount = verticesQueue.size();
        if (matrix.size() != verticesCount)
        {
            matrix.resize(verticesCount);
            for (auto& row : matrix)
            {
                row.resize(verticesCount);
            }        
        }

        int indexFrom = vertex2index.find(from)->second;
        int indexTo = vertex2index.find(to)->second;

        matrix[indexFrom][indexTo] = true;
    }

    int VerticesCount() const override
    {
        return verticesQueue.size();
    }

    std::vector<int> Vertices() const override
    {
        return verticesQueue;
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        std::vector<int> nextVertices;
        if (vertex2index.find(vertex) == vertex2index.end())
        {
            return nextVertices;
        }
            
        int index = vertex2index.find(vertex)->second;

        for (int col = 0; col < matrix[index].size(); ++col)
        {
            if (matrix[index][col] == 1)
            {
                nextVertices.push_back(verticesQueue[col]);
            }       
        }  
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int> prevVertices;
        if (vertex2index.find(vertex) == vertex2index.end())
        {
            return prevVertices;
        }
            
        int index = vertex2index.find(vertex)->second;

        for (int row = 0; row < matrix.size(); ++row)
        {
            if (matrix[row][index] == 1)
            {
                prevVertices.push_back(verticesQueue[row]);
            }       
        }   
        return prevVertices;
    }

    void printGraph() override
    {
        std::cout << std::setw(4) << ' ';
        for (const auto& vertex : verticesQueue)
        {
            std::cout << std::setw(4) << vertex;
        }
            
        std::cout << std::endl;

        for (int i = 0; i < matrix.size(); ++i)
        {
            std::cout << std::setw(4) << verticesQueue[i];
            for (int j = 0; j < matrix[i].size(); ++j)
            {
                std::cout << std::setw(4) << matrix[i][j];
            }
                
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

private:
    std::map<int, int> vertex2index;
    std::vector <int> verticesQueue;
    std::vector<std::vector<bool>> matrix;

    void checkVertex(int vertex)
    {
        if (vertex2index.find(vertex) == vertex2index.end())
        {
            size_t pos = verticesQueue.size();
            vertex2index.insert(std::pair <int, int>(vertex, pos));
            verticesQueue.push_back(vertex);
        }
    }
};


int main()
{
    MatrixGraph m1;
    m1.AddEdge(0, 4);
    m1.AddEdge(0, 3);
    m1.AddEdge(1, 2);
    m1.AddEdge(1, 4);
    m1.AddEdge(1, 5);
    m1.AddEdge(2, 3);
    m1.AddEdge(2, 5);
    m1.AddEdge(5, 3);
    m1.AddEdge(5, 4);
    m1.printGraph();

    ListGraph l1 = m1;
    l1.printGraph();

    MatrixGraph m2 = l1;
    m2.printGraph();
}