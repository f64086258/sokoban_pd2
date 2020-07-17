#include "map.h"

#include <iostream>
#include <fstream>
Map::Map(int stagenum)
{
    setmapname(stagenum);
    setmapdata();
}
Map::~Map(){
    for(int i = 0; i < height; i++)
    {
        delete [] mapdata[i];
    }
    delete [] mapdata;
    for(int i = 0; i < height; i++)
    {
        delete [] mapbackup[i];
    }
    delete [] mapbackup;
}
void Map::setmapname(int n)
{
    switch (n) {
    case 1:
        name = "/home/pd2/2020-pd2-sokoban/stage1.txt";
        break;
    case 2:
        name = "/home/pd2/2020-pd2-sokoban/stage2.txt";
        break;
    default:
        name = "/home/pd2/2020-pd2-sokoban/stage1.txt";
        break;
    }
}
void Map::setwidth(int w)
{
    width = w;
}
void Map::setheight(int h)
{
   height = h;
}
void Map::setmapdata()
{
    ifstream fin(name, ios::in);
    if(fin.is_open())
    {
        int w, h;
        fin >> w >> h >> cratenum;
        setwidth(w);
        setheight(h);
        mapdata = new int*[height];
        for(int i = 0; i < height; i++)
        {
            mapdata[i] = new int[width];
        }
        mapbackup = new int*[height];
        for(int i = 0; i < height; i++)
        {
            mapbackup[i] = new int[width];
        }
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                fin >> mapdata[i][j];
                mapbackup[i][j] = mapdata[i][j];
                if(mapdata[i][j] == PLAYER)
                {
                    start ={j,i};
                }
            }
        }
        fin.close();
    }
    else {
        setwidth(5);
        setheight(5);
        cratenum = 1;
        mapdata = new int*[height];
        for(int i = 0; i < height; i++)
        {
            mapdata[i] = new int[width];
        }
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                mapdata[i][j] = 1;
            }
        }
        for(int i = 1; i < 4; i++)
        {
            for(int j = 1; j < 4; j++)
            {
                mapdata[i][j] = 0;
            }
        }
        mapdata[2][2] = 2;
        mapdata[2][1] = 3;
        mapdata[3][2] = 4;
        cout << "wrong";
    }
}



