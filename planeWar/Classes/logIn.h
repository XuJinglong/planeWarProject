//
// Created by Xu_Jinglong on 2018/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_LOGIN_H
#define PROJ_ANDROID_STUDIO_LOGIN_H

#include "cocos2d.h"
#include "mainPage.h"
#include "ui/CocosGUI.h"
#include "string"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "network/HttpResponse.h"
#include "commonVar.h"
USING_NS_CC;
using namespace std;
using namespace cocos2d::network;

extern  string userName;

class logIn:public Layer {
private:
    Size windowSize;
    string passWord;
    Label* infLabel;
public:
    bool init();
    static Scene* creatLogInScene();
    void gotoMainPage();
    void newRegister();
    //be called to deal with the data from game sever.
    void useResponse(HttpClient *client,HttpResponse *response);

    CREATE_FUNC(logIn);

};


#endif //PROJ_ANDROID_STUDIO_LOGIN_H
