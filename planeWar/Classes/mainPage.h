//
// Created by Xu_Jinglong on 2018/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_MAINPAGE_H
#define PROJ_ANDROID_STUDIO_MAINPAGE_H

#include "cocos2d.h"
#include "logIn.h"
#include "store.h"
#include "setting.h"
#include "inf.h"
#include "gameMode1.h"
using namespace std;
USING_NS_CC;

class mainPage:public Layer {
private:
    Size windowSize;
public:
    bool init();
    static Scene* creatMainPage();
    void backToLog(Ref* object);
    void intoStore(Ref* object);
    void intoInf(Ref* object);
    void intoSetting(Ref* object);
    void intoGame(Ref* object);
    void menuCloseCallback(cocos2d::Ref* pSender);



    CREATE_FUNC(mainPage);

};


#endif //PROJ_ANDROID_STUDIO_MAINPAGE_H
