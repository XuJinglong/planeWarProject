//
// Created by Xu_Jinglong on 2018/3/26.
//

#ifndef PROJ_ANDROID_STUDIO_TOOL_H
#define PROJ_ANDROID_STUDIO_TOOL_H

#include "cocos2d.h"
#include "bullet.h"
USING_NS_CC;
//a special bullet class
class tool:public bullet {
public:
    virtual bool init();

    CREATE_FUNC(tool);

};


#endif //PROJ_ANDROID_STUDIO_TOOL_H
