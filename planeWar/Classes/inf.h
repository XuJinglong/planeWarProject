//
// Created by Xu_Jinglong on 2018/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_DATARECORD_H
#define PROJ_ANDROID_STUDIO_DATARECORD_H

#include "cocos2d.h"
#include "mainPage.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "network/HttpResponse.h"
#include "commonVar.h"
USING_NS_CC;
using namespace std;
using namespace cocos2d::network;
extern string userName;


class inf:public Layer {
public:
    bool init();
    static Scene* creatInfScene();
    void gotoMainPage(Ref* object);
    void useResponse(HttpClient *client,HttpResponse *response);

    CREATE_FUNC(inf);

};


#endif //PROJ_ANDROID_STUDIO_DATARECORD_H
