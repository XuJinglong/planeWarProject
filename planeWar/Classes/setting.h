//
// Created by Xu_Jinglong on 2018/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_SETTING_H
#define PROJ_ANDROID_STUDIO_SETTING_H

#include "cocos2d.h"
#include "iostream"
#include "mainPage.h"
#include "ui/CocosGUI.h"
#include "commonVar.h"

USING_NS_CC;
using namespace std;
using namespace ui;

class setting:public Layer {
public:
    bool init();
    static Scene* creatSettingScene();
    void gotoMainPage(Ref* object);

    CREATE_FUNC(setting);

};


#endif //PROJ_ANDROID_STUDIO_SETTING_H
