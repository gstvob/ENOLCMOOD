#include "Map.h"

// DA DE JOGAR A DEFINIÇÃO DO MAPA EM SI EM ALGUM ARQUIVO EXTERNO. (O ARRAY COM OS NÚMEROS NO CASO)
Map::Map(SDL_Renderer& renderer)
{
    mapDefinition = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 3, 3, 3, 3, 0, 0, 0, 2, 2, 2, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 2, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 2, 0, 0, 1},
        {1, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 3, 1, 3, 1, 1, 1, 3, 0, 0, 3, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 3, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 3, 1, 1, 1},
        {1, 1, 3, 1, 1, 1, 1, 1, 1, 3, 0, 0, 3, 1, 1, 1},
        {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 2, 0, 0, 0, 0, 0, 3, 4, 0, 4, 3, 0, 1},
        {1, 0, 0, 5, 0, 0, 0, 0, 0, 0, 3, 0, 3, 0, 0, 1},
        {1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 1},
        {1, 1, 3, 3, 0, 0, 3, 3, 1, 3, 3, 1, 3, 1, 1, 1},
        {1, 1, 1, 3, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 3, 3, 4, 0, 0, 4, 3, 3, 3, 3, 3, 3, 3, 3, 1},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 3},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    };
    wallTexturesMap = {
            { 1, LoadTexture("./assets/textures/1.png", renderer) },
            { 2, LoadTexture("./assets/textures/2.png", renderer) },
            { 3, LoadTexture("./assets/textures/3.png", renderer) },
            { 4, LoadTexture("./assets/textures/4.png", renderer) },
            { 5, LoadTexture("./assets/textures/5.png", renderer) },
    };

    getMap();
    BuildMapGraph();
}

Map::~Map()
{
    for (auto i = wallTexturesMap.begin(); i != wallTexturesMap.end(); i++) {
        if (i->second != nullptr) {
            SDL_DestroyTexture(i->second);
            i->second = nullptr;
        }
    }
}

void Map::getMap()
{
    for (auto row = mapDefinition.begin(); row != mapDefinition.end(); row++) {
        for (auto column = row->begin(); column != row->end(); column++) {
            if (*column != 0) {
                int j = std::distance(mapDefinition.begin(), row);
                int i = std::distance(row->begin(), column);
                mapPositions[{i, j}] = *column;
            }
        }
    }
}


void Map::Draw(SDL_Renderer& renderer)
{
    for (const auto& [key, value] : mapPositions) {
        const auto [x, y] = key;
        SDL_Rect outlineRect = { x*100, y*100, 100, 100};
        SDL_SetRenderDrawColor(&renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawRect(&renderer, &outlineRect);
    }
}

void Map::Reset()
{
    mapGraph.clear();
    getMap();
    BuildMapGraph();
}


std::vector<std::tuple<int, int>> Map::getAdjacentCells(float x, float y)
{
    std::vector<std::tuple<int, int>> adjacentCells;
    std::tuple<int, int> paths[] = {
            {-1, 0}, {0, -1}, {1, 0}, {0, 1}, {-1, -1}, {1, -1}, {1, 1}, {-1, 1}
    };

    for (const auto& [dx, dy] : paths) {
        float nextX = x + dx;
        float nextY = y + dy;
        if (mapPositions.find({ nextX, nextY }) == mapPositions.end()) {
            adjacentCells.push_back({ nextX, nextY });
        }
    }
    return adjacentCells;
}

void Map::BuildMapGraph()
{
    for (auto row = mapDefinition.begin(); row != mapDefinition.end(); row++) {
        for (auto column = row->begin(); column != row->end(); column++) {
            if (*column == 0) {
                int x = std::distance(row->begin(), column);
                int y = std::distance(mapDefinition.begin(), row);
                std::vector<std::tuple<int, int>> adjacentCells = getAdjacentCells(x, y);
                mapGraph[{x, y}].insert(mapGraph[{x, y}].end(), adjacentCells.begin(), adjacentCells.end());
            }
        }
    }
}

// ESSA FUNÇAO AQUI PODERIA IR PRA UM ARQUIVO QUE CONTENHA FUNÇÕES GENERICAS E AS CONSTANTES.

SDL_Texture* LoadTexture(std::string path, SDL_Renderer& renderer)
{
    SDL_Texture* texture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr)
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    else {
        texture = SDL_CreateTextureFromSurface(&renderer, loadedSurface);
        if (texture == nullptr)
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        SDL_FreeSurface(loadedSurface);
    }
    return texture;
}
