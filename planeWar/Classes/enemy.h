//
// Created by Xu_Jinglong on 2018/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_ENEMY_H
#define PROJ_ANDROID_STUDIO_ENEMY_H

#include "cocos2d.h"
USING_NS_CC;

class enemy:public cocos2d::Sprite {
public:
    //the tag of enemy which is used to decide weather it should emit a bullet
    int tag;
    bool init();


    CREATE_FUNC(enemy);

};


#endif //PROJ_ANDROID_STUDIO_ENEMY_H
