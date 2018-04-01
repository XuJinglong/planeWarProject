//
// Created by Xu_Jinglong on 2018/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_STORE_H
#define PROJ_ANDROID_STUDIO_STORE_H
#include "cocos2d.h"
#include "mainPage.h"
#include "iostream"
#include "tool.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "network/HttpResponse.h"
#include "commonVar.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace std;
using namespace cocos2d::network;
using namespace ui;
extern string userName;

class store:public Layer {
private:
    int money;
    int price;
    int haveNumber;
    int buyNumber;
    Size windowSize;
    Label* moneyLabel;
    Label* infLabel;
    Label* toolNumberLabel;
    Label* priceLabel;
    TextField* buyNumberText;

public:
    bool init();
    static Scene* creatStoreScene();
    void gotoMainPage(Ref* object);
    void finishBuy(Ref* object);
    void useResponse(HttpClient *client,HttpResponse *response);

    CREATE_FUNC(store);

};


#endif //PROJ_ANDROID_STUDIO_STORE_H
