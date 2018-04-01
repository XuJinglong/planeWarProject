//
// Created by Xu_Jinglong on 2018/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_BULLET_H
#define PROJ_ANDROID_STUDIO_BULLET_H
#include "cocos2d.h"
USING_NS_CC;

typedef enum BULLETTYPE
{
    def_type,
    main_type,
    enemy_type
}bullet_type;

class bullet :public cocos2d::Sprite {

public:
    bullet_type bt;
    virtual bool init();

    CREATE_FUNC(bullet);

};


#endif //PROJ_ANDROID_STUDIO_BULLET_H
