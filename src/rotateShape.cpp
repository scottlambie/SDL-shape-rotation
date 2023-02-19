#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>
#include <vector>
#define PI 3.14159265

const int WINDOW_LENGTH = 640, WINDOW_HEIGHT = 480, X_CENTER = WINDOW_LENGTH/2, Y_CENTER = WINDOW_HEIGHT/2;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

struct coord
{
    float x, y, z;
};

struct edge
{
    coord start, end;
};

void initSDL()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WINDOW_LENGTH,WINDOW_HEIGHT,0,&window,&renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,255,255,255); 
}

void renderObject(int x1, int y1, int x2, int y2)
{
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderDrawLine(renderer,x1,y1,x2,y2);
    SDL_RenderPresent(renderer);
}

void renderObject(std::vector<edge> edges)
{

    //Draw X axis
    SDL_SetRenderDrawColor(renderer,0,255,255,255);
    SDL_RenderDrawLine(renderer,0,Y_CENTER,WINDOW_LENGTH,Y_CENTER);
    //Draw Y axis
    SDL_SetRenderDrawColor(renderer,255,0,255,255);
    SDL_RenderDrawLine(renderer,X_CENTER,0,X_CENTER,WINDOW_HEIGHT);

    //Draw shape from user-defined edges
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    for(auto &itr:edges)
    {
        float x1,x2,y1,y2;
        x1 = itr.start.x+X_CENTER;
        y1 = itr.start.y+Y_CENTER;
        x2 = itr.end.x+X_CENTER;
        y2 = itr.end.y+Y_CENTER;
        SDL_RenderDrawLine(renderer,x1,y1,x2,y2);
    }
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_Delay(10);
}

std::vector<edge> drawShape(std::vector<edge> edges)
{
    std::vector<coord> coords;
    std::string input = "";
    std::cout << "Enter pairs of XYZ coordinates. These pairs become edges on the shape\n";
    do
    {
        printf("\nEnter coordinate 1\nX:");
        std::cin >> input;
        float x = stof(input);
        printf("Y:");
        std::cin >> input;
        float y = stof(input);
        printf("Z:");
        std::cin >> input;
        float z = stof(input);
        coords.push_back({x,y,z});
        
        printf("\nEnter coordinate 2\nX:");
        std::cin >> input;
        x = stof(input);
        printf("Y:");
        std::cin >> input;
        y = stof(input);
        printf("Z:");
        std::cin >> input;
        z = stof(input);
        coords.push_back({x,y,z});
        edges.push_back({coords[0],coords[1]});
        
        //Translate points to isometric view for reference
        float x1 = (coords.at(0).x + X_CENTER) + (coords.at(0).z/2);
        float y1 = (coords.at(0).y + Y_CENTER) + (coords.at(0).z/2);
        float x2 = (coords.at(1).x + X_CENTER) + (coords.at(1).z/2);
        float y2 = (coords.at(1).y + Y_CENTER) + (coords.at(1).z/2);
        coords.clear();

        renderObject(x1,y1,x2,y2);

        printf("\n'done' to finish:");
        std::cin >> input;
    }
    while(input!="done");
    return edges;
}

void rotateVector(std::vector<edge> edges)
{
    int theta = 1;
    float cosTheta = cos(theta*PI/180);
    float sinTheta = sin(theta*PI/180);

    for(int j=0;j<720;++j){
        SDL_RenderClear(renderer);
        //Find rotation of points by theta(rotation by 1 degree)
        std::vector<edge>::iterator itr = edges.begin();
        for(auto &itr:edges)
        {
            //TODO remove repetition
            //X Rotation
            itr.start.x = itr.start.z*sinTheta + itr.start.x*cosTheta;
            itr.start.x = itr.start.x*cosTheta - itr.start.y*sinTheta;
            itr.end.x = itr.end.z*sinTheta + itr.end.x*cosTheta;
            itr.end.x = itr.end.x*cosTheta - itr.end.y*sinTheta;
        
            //Y Rotation
            itr.start.y = itr.start.y*cosTheta - itr.start.z*sinTheta;
            itr.start.y = itr.start.x*sinTheta + itr.start.y*cosTheta;
            itr.end.y = itr.end.y*cosTheta - itr.end.z*sinTheta;
            itr.end.y = itr.end.x*sinTheta + itr.end.y*cosTheta;

            //Z Rotation
            itr.start.z = itr.start.y*sinTheta + itr.start.z*cosTheta;
            itr.start.z = itr.start.z*cosTheta - itr.start.x*sinTheta;
            itr.end.z = itr.end.y*sinTheta + itr.end.z*cosTheta;
            itr.end.z = itr.end.z*cosTheta - itr.end.x*sinTheta;
        }   
        renderObject(edges);
    }
}

int main()
{
    initSDL();
    std::vector<edge> edges;
    edges = drawShape(edges);
    rotateVector(edges);
    return 0;
}
