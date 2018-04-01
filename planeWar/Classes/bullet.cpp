//
// Created by Xu_Jinglong on 2018/3/15.
//

#include "bullet.h"

bool bullet::init()
{
    if(Sprite::init())
    {
        bt=def_type;
        return true;
    }

    return false;
}
