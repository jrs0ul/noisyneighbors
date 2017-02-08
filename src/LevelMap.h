#ifndef _LEVEL_MAP_H
#define _LEVEL_MAP_H

#include "TextureLoader.h"
#include "Xml.h"

struct Tile{
    COLOR c;
    unsigned picture;
    Tile(){picture = 0;}
};

struct LevelMap{
    
    Tile ** map;
    bool ** bmap;
    unsigned width;
    unsigned height;
    unsigned backgroundPicIndex;
    char musicPath[255];
    
    LevelMap(){
        map = 0;
        width = 0;
        height = 0;
        backgroundPicIndex = 13;
    }
    void destroy();
    void generate(unsigned _width = 0, unsigned _height = 0);
    bool ** boolean();//updates bmap and returns pointer
    void boolean(bool *** _map); //creates external boolean map
    void loadFromXMLNode(XmlNode * node);
    void putToXMLNode(XmlNode * node);
    void draw(PicsContainer& pics, unsigned index, int x, int y, 
              float c = 1.0f, int screenWidth = 480,
              int screenHeight = 320);
    void setTile(unsigned x, unsigned y, Tile & t);
};

#endif //_LEVEL_MAP_H

