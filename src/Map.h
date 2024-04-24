#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <map>
#include <tuple>

SDL_Texture* LoadTexture(std::string path, SDL_Renderer& renderer);

class Map {

public: 
    std::map<std::tuple<int, int>, int> mapPositions;
    std::map<int, SDL_Texture*> wallTexturesMap;
    std::map < std::tuple<int, int>, std::vector<std::tuple<int, int>>> mapGraph;
public:
    Map(SDL_Renderer& renderer);
    ~Map();

    void Draw(SDL_Renderer& renderer);
    void BuildMapGraph();
    void Reset();
private: 
    std::vector<std::vector<int>> mapDefinition;
private:
    void getMap();
    std::vector<std::tuple<int, int>> getAdjacentCells(float x, float y);
};

#endif