#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <list>
void MakeBegin(sf::RectangleShape& shape) {
    shape.setFillColor(sf::Color::Red);
}
void MakeTarget(sf::RectangleShape& shape) {
    shape.setFillColor(sf::Color::Blue);
}
void MakeObstacle(sf::RectangleShape& shape) {
    shape.setFillColor(sf::Color::Green);
}

class Block {
public:
    bool bObstacle = false;
    bool bVisited = false;
    float fGlobalGoal;
    float fLocalGoal;
    int x;
    int y;
    std::vector<Block*> vecNeighbours;
    Block* parent;
    sf::RectangleShape shape;
};
class NPC {
public: 
    int size = 10;
    Block* Next;
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(size,size));
    std::list<sf::Vector2f> path;
};

sf::VertexArray Path(sf::LinesStrip);

int main()
{
    int nMapWidth = 16;
    int nMapHeight = 16;
    int nSize = 20;
    int nBorder = 0;
    sf::Color cColorDefault = sf::Color::Magenta;
    Block* cNodes = nullptr;
    cNodes = new Block[nMapWidth * nMapHeight];
    for (int x = 0; x < nMapWidth; x++) 
        for (int y = 0; y < nMapHeight; y++){
            
            cNodes[y * nMapWidth + x].x = x;
            cNodes[y * nMapWidth + x].y = y;
            cNodes[y * nMapWidth + x].bObstacle = false;
            cNodes[y * nMapWidth + x].parent = nullptr;
            cNodes[y * nMapWidth + x].bVisited = false;
            cNodes[y * nMapWidth + x].shape.setSize(sf::Vector2f(nSize,nSize));
            cNodes[y * nMapWidth + x].shape.setFillColor(cColorDefault);
            cNodes[y * nMapWidth + x].shape.setPosition(sf::Vector2f(x * nSize + nBorder * x, y * nSize + nBorder * y));
        }
    Block* Start = nullptr;
    Block *End = nullptr;
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(nSize / 2, nSize / 2));
    shape.setFillColor(sf::Color::Transparent);
    NPC npc;
    
    for (int x = 0; x < nMapWidth; x++)
        for (int y = 0; y < nMapHeight; y++) {
            if (y>0)
                cNodes[y * nMapWidth + x].vecNeighbours.push_back(&cNodes[(y - 1) * nMapWidth + (x + 0)]);
            if (y<nMapHeight -1 )
                cNodes[y * nMapWidth + x].vecNeighbours.push_back(&cNodes[(y + 1) * nMapWidth + (x + 0)]);
            if (x>0)
                cNodes[y * nMapWidth + x].vecNeighbours.push_back(&cNodes[(y - 0) * nMapWidth + (x - 1)]);
            if (x<nMapWidth-1)
                cNodes[y * nMapWidth + x].vecNeighbours.push_back(&cNodes[(y - 0) * nMapWidth + (x + 1)]);

            if (x < nMapWidth - 1 && nMapHeight-1 > y)
                cNodes[y * nMapWidth + x].vecNeighbours.push_back(&cNodes[(y + 1) * nMapWidth + (x + 1)]);
            if (x > 0 && nMapHeight -1 > y)
                cNodes[y * nMapWidth + x].vecNeighbours.push_back(&cNodes[(y + 1) * nMapWidth + (x - 1)]);
            if (x > 0 && y > 0)
                cNodes[y * nMapWidth + x].vecNeighbours.push_back(&cNodes[(y - 1) * nMapWidth + (x - 1)]);
            if (x < nMapWidth - 1 && y > 0)
                cNodes[y * nMapWidth + x].vecNeighbours.push_back(&cNodes[(y - 1) * nMapWidth + (x + 1)]);
        }
    

    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
    
    
    

    Start = &cNodes[1];
    End = &cNodes[0];
    MakeBegin(Start->shape);
    MakeTarget(End->shape);
    bool RecalcAstar = true;
    sf::Clock elapsedTime;
    /// **************************************************************************************
    /// Start of Loop
    /// ****************************************************************************************
    /// <returns></returns>
    while (window.isOpen())
    {
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased) {


                sf::Vector2f Mouse_Pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                for (int i = 0; i < nMapHeight * nMapWidth; i++) {
                    if (cNodes[i].shape.getGlobalBounds().contains((sf::Vector2f(Mouse_Pos)))) {
                        if (cNodes[i].shape.getFillColor() == sf::Color::Magenta) {

                            MakeObstacle(cNodes[i].shape);
                            cNodes[i].bObstacle = true;
                            RecalcAstar = true;
                        }
                        else if (cNodes[i].shape.getFillColor() == sf::Color::Green) {
                            cNodes[i].shape.setFillColor(sf::Color::Magenta);
                            cNodes[i].bObstacle = false;
                            RecalcAstar = true;
                        }
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                sf::Vector2f Mouse_Pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    for (int x = 0; x < nMapHeight * nMapWidth; x++)
                    {
                        if (cNodes[x].shape.getGlobalBounds().contains((sf::Vector2f(Mouse_Pos)))) {
                            End->shape.setFillColor(sf::Color::Magenta);
                            End->bObstacle = false;
                            MakeTarget(cNodes[x].shape);
                            cNodes[x].bObstacle = false;
                            End = &cNodes[x];
                            npc.shape.setPosition(sf::Vector2f(End->shape.getPosition().x + nSize/4, End->shape.getPosition().y + nSize /4));
                            RecalcAstar = true;
                        }
                    }
            }
        
                
            }
            if (event.type == sf::Event::MouseButtonReleased && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                sf::Vector2f Mouse_Pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                for (int x = 0; x < nMapHeight * nMapWidth; x++)
                {
                    if (cNodes[x].shape.getGlobalBounds().contains((sf::Vector2f(Mouse_Pos)))) {
                        Start->shape.setFillColor(sf::Color::Magenta);
                        Start->bObstacle = false;
                        MakeBegin(cNodes[x].shape);
                        
                        Start = &cNodes[x];
                        npc.shape.setPosition(sf::Vector2f(End->shape.getPosition().x + nSize / 4, End->shape.getPosition().y + nSize / 4));
                        RecalcAstar = true;
                    }
                }
            }
            
        

        //Solve A*
            if (RecalcAstar) {
                for (int x = 0; x < nMapWidth; x++)
                    for (int y = 0; y < nMapHeight; y++) {
                        cNodes[y * nMapWidth + x].bVisited = false;
                        cNodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
                        cNodes[y * nMapWidth + x].fLocalGoal = INFINITY;
                        cNodes[y * nMapWidth + x].parent = nullptr;

                    }
                auto distance = [](Block* a, Block* b) {
                    return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
                };
                auto heuristic = [distance](Block* a, Block* b) {
                    return distance(a, b);
                };
                Block* nodeCurrent = Start;
                Start->fLocalGoal = 0.0f;
                Start->fGlobalGoal = heuristic(Start, End);
                std::list<Block*> untestedNodes;
                npc.path.clear();
                untestedNodes.push_back(Start);
                while (!untestedNodes.empty() && nodeCurrent != End) {
                    untestedNodes.sort([](const Block* lhs, const Block* rhs) {return lhs->fGlobalGoal < rhs->fGlobalGoal; });
                    while (!untestedNodes.empty() && untestedNodes.front()->bVisited)
                        untestedNodes.pop_front();
                    if (untestedNodes.empty())
                        break;
                    nodeCurrent = untestedNodes.front();
                    nodeCurrent->bVisited = true;

                    for (auto nodeNeighbor : nodeCurrent->vecNeighbours) {
                        if (!nodeNeighbor->bVisited && nodeNeighbor->bObstacle == 0)
                            untestedNodes.push_back(nodeNeighbor);
                        float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbor);
                        if (fPossiblyLowerGoal < nodeNeighbor->fLocalGoal) {
                            
                            nodeNeighbor->parent = nodeCurrent;
                            nodeNeighbor->fLocalGoal = fPossiblyLowerGoal;
                            nodeNeighbor->fGlobalGoal = nodeNeighbor->fLocalGoal + heuristic(nodeNeighbor, End);
                            
                        }
                    }
                    Start->shape.setFillColor(sf::Color::Red);
                    End->shape.setFillColor(sf::Color::Yellow);
                }
                
                
            }
            if (End != nullptr && RecalcAstar) {
                Block* p = End;
                Path.clear();
                while (p != NULL) {
                    Path.append(sf::Vector2f(p->x * nSize + nBorder * p->x + nSize / 2, p->y * nSize + nBorder * p->y + nSize / 2));
                    npc.path.push_back(sf::Vector2f(p->shape.getPosition().x + nSize / 4, p->shape.getPosition().y + nSize / 4));
                    p = p->parent;
                   
                }
                
                shape.setPosition(npc.path.front());
                RecalcAstar = false;
            
            }

            if (elapsedTime.getElapsedTime().asMilliseconds() > 50.0f && !npc.path.empty()) {
                npc.shape.move(sf::Vector2f((npc.path.front().x - npc.shape.getPosition().x) / 4, (npc.path.front().y - npc.shape.getPosition().y) / 4));
                if (shape.getGlobalBounds().intersects(npc.shape.getGlobalBounds())) {
                    npc.path.pop_front();
                    if (!npc.path.empty())
                        shape.setPosition(npc.path.front());
                }
                    
                elapsedTime.restart();

        window.clear();

        }
        for (int i = 0; i < nMapHeight * nMapWidth; i++) {
            window.draw(cNodes[i].shape);

        }
        window.draw(npc.shape);
        window.draw(Path);
        window.display();

    }
    
    cNodes = NULL;
    delete cNodes;
    
    
    return 0;
}