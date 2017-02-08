#include <wchar.h>
#include "LevelList.h"
#include "Xml.h"


//------------------------------
bool LevelList::write(const char * path){
    Xml data;
    XmlNode _Levels;
    _Levels.setName(L"Levels");
    for (unsigned long i = 0; i < levels.count(); i++){
        XmlNode _Level;
        _Level.setName(L"Level");
        XmlNode _name;
        _name.setName(L"Name");
        wchar_t buf[100];
        mbstowcs(buf, levels[i].name, 100);
        _name.setValue(buf);
        XmlNode _locked;
        _locked.setName(L"Locked");
        swprintf(buf,
        #ifndef __MINGW32__
                100,
        #endif
                L"%d", (int)levels[i].locked);
        _locked.setValue(buf);
        XmlNode _path;
        _path.setName(L"Path");
        mbstowcs(buf, levels[i].path, 100);
        _path.setValue(buf);
        _Level.addChild(_name);
        _Level.addChild(_locked);
        _Level.addChild(_path);
        _Levels.addChild(_Level);
    }
    data.root.addChild(_Levels);
    bool result = false;
    if (data.write(path))
        result = true;
    data.destroy();
    return result;
}


//-------------------------------
bool LevelList::load(const char * path){
    
    Xml data;
    levels.destroy();
    
    if (!data.load(path))
        return false;
    XmlNode * _levels = 0;
    _levels = data.root.getNode(L"Levels");
    //printf("%lu\n", data.root.childrenCount());
    if (_levels){
        //puts("levels OK");
        
        for (unsigned long i = 0; i < _levels->childrenCount(); i++){
            XmlNode * levelDesc = 0;
            levelDesc = _levels->getNode(i);
            if (levelDesc){
                LevelDescription l;
                char tmp[255];
                wcstombs(tmp, levelDesc->getNode(L"Locked")->getValue(), 255);
                l.locked = (atoi(tmp))? true : false;
                wcstombs(tmp, levelDesc->getNode(L"Name")->getValue(), 255);
                strcpy(l.name, tmp);
                wcstombs(tmp, levelDesc->getNode(L"Path")->getValue(), 255);
                strcpy(l.path, tmp);
                levels.add(l);
            }
        }
            
    }
    
    data.destroy();
    printf("levels: %lu\n", levels.count());
    
    return true;
}
//-----------------------------------
bool LevelList::loadLevel(unsigned long index, LevelMap & map, RobotArmy& army){
    
    if (index >= levels.count())
        return false;
    
    Xml data;
    puts(levels[index].path);
    if (!data.load(levels[index].path))
        return false;
    
    map.destroy();
    army.destroy();
    XmlNode * level = 0;
    level = data.root.getNode(L"Level");
    if (level){
        
        XmlNode * entities = 0;
        XmlNode * tiles = 0;
        entities = level->getNode(L"Entities");
        if (entities) {
            army.loadFromXMLNode(entities);
        }
        tiles = level->getNode(L"Tiles");
        if (tiles) {
            map.loadFromXMLNode(tiles);
        }
    }
    
    data.destroy();
    
    return true;
    
}
