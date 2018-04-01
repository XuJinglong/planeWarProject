//
// Created by Xu_Jinglong on 2018/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_GAMEMODE1_H
#define PROJ_ANDROID_STUDIO_GAMEMODE1_H
#include "cocos2d.h"
#include "iostream"
#include "mainPage.h"
#include "mainPlane.h"
#include "bullet.h"
#include "enemy.h"
#include "typeinfo"
#include "SimpleAudioEngine.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "network/HttpResponse.h"
#include "commonVar.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;
using namespace cocos2d::network;
extern string userName;

class gameMode1:public Layer {
private:
    Sprite* bgImg1;
    Sprite* bgImg2;
    Size windowSize;
    mainPlane* main_Plane;
    bool planeIsTouch;
    int mainBulletNumber;
    int enemyNumber;
    int score;
    long startTime;
    Label* timeLabel;
    Label* scoreLabel;
    int toolNumber;
    Label* toolLabel;

public:
    bool init();

    static Scene* creatgame1Scene();

    void gotoMainPage(Ref* object);

    void bgCircle(float time);

    void mainBulletManage(float time);

    void removeBullet(Ref* object);

    void enemyManage(float time);

    void enemyBulletManage(float time);

    void collisionManage(float time);

    void passManage(float time);

    void showGameOver();

    void showVictory();
    //display the time,tool numbers and scole
    void update(float time);

    void toolUse(Ref* object);

    void specialEffect(Vec2);

    void restart(Ref* object);

    void useResponse(HttpClient *client,HttpResponse *response);

    CREATE_FUNC(gameMode1);
};


#endif //PROJ_ANDROID_STUDIO_GAMEMODE1_H
