//
// Created by Xu_Jinglong on 2018/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_MAINPLANE_H
#define PROJ_ANDROID_STUDIO_MAINPLANE_H

#include "cocos2d.h"
USING_NS_CC;
//some special features may be added in the class in the future.Now,it is just a Sprite
class mainPlane:public cocos2d::Sprite
{
public:

    virtual bool init();

    CREATE_FUNC(mainPlane);
};


#endif //PROJ_ANDROID_STUDIO_MAINPLANE_H
