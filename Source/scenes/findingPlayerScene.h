#ifndef __FINDING_PPLAYER_SCENE_H__
#define __FINDING_PPLAYER_SCENE_H__

#include "axmol.h"

class FindingPlayerScene : public ax::Scene {
public:

    bool init();
    static FindingPlayerScene* create(){
        auto scene = new FindingPlayerScene();
        if(scene && scene -> init()){
            return scene;
        }
        else return nullptr;
    }
    void showServerError();
};

#endif
