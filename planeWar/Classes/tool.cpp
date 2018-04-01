//
// Created by Xu_Jinglong on 2018/3/26.
//

#include "tool.h"

bool tool::init()
{
    if(!bullet::init())
        return false;
    this->bt=main_type;
    return true;
}
