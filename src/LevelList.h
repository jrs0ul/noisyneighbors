#ifndef _LEVEL_LIST
#define _LEVEL_LIST

#include "DArray.h"
#include "LevelMap.h"
#include "RobotArmy.h"

struct LevelDescription{
    bool locked;
    char path[255];
    char name[50];
};


class LevelList{
    
    DArray<LevelDescription> levels;
public:
    unsigned firstToShow;
    
    LevelList(){firstToShow = 0;}
    
    bool load(const char * path);
    bool loadLevel(unsigned long index, LevelMap & map, RobotArmy& army);
    bool write(const char * path);
    void unlock(unsigned long index);
    unsigned long count(){return levels.count();}
    LevelDescription * get(unsigned long index){if (index < levels.count()) return &levels[index]; return 0;}
    void destroy(){levels.destroy();}
    void add(LevelDescription& desc){levels.add(desc);}
    void remove(unsigned index){levels.remove(index);}
};



#endif //_LEVEL_LIST
