//
// Created by Xu_Jinglong on 2018/3/15.
//

#include "enemy.h"

bool enemy::init()
{
    if(!Sprite::init())
        return false;
    else
        tag=0;
        return true;
}