//
// Created by Xu_Jinglong on 2018/3/15.
//

#include "store.h"
#include "json/rapidjson.h"
#include "json/document.h"

using namespace rapidjson;

bool store::init()
{
    if(!Layer::init())
        return false;

    windowSize=this->getContentSize();
    price=10;
    buyNumber=0;

    Sprite* backGround=Sprite::createWithSpriteFrameName("store.png");
    backGround->setAnchorPoint(Vec2(0,0));
    addChild(backGround);

    Sprite* superBullet=Sprite::create("tool2.png");
    superBullet->setPosition(Vec2(windowSize.width/2,500));
    addChild(superBullet,2);

    infLabel=Label::create();
    infLabel->setTextColor(Color4B::BLUE);
    infLabel->setSystemFontSize(32);
    infLabel->setString("");
    infLabel->setPosition(Vec2(600,windowSize.height-100));
    this->addChild(infLabel);

    moneyLabel=Label::create();
    moneyLabel->setTextColor(Color4B::BLUE);
    moneyLabel->setSystemFontSize(32);
    moneyLabel->setString(StringUtils::format("money:%d",money));
    moneyLabel->setPosition(Vec2(100,windowSize.height-100));
    this->addChild(moneyLabel);

    priceLabel=Label::create();
    priceLabel->setTextColor(Color4B::BLUE);
    priceLabel->setSystemFontSize(32);
    priceLabel->setString(StringUtils::format("%d",price));
    priceLabel->setPosition(Vec2(windowSize.width/2+50,770));
    this->addChild(priceLabel);

    toolNumberLabel=Label::create();
    toolNumberLabel->setTextColor(Color4B::BLUE);
    toolNumberLabel->setSystemFontSize(32);
    toolNumberLabel->setString(StringUtils::format("%d",haveNumber));
    toolNumberLabel->setPosition(Vec2(windowSize.width/2+50,850));
    this->addChild(toolNumberLabel);
    //a http request to get the data from sever of money and tool number.
    auto request = new HttpRequest();
    request->setUrl("http://2048f433q0.iok.la:11403?type=store&user="+userName);
    request->setTag("type_store");
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(store::useResponse,this));

    auto client = HttpClient::getInstance();
    client->setTimeoutForConnect(60);
    client->setTimeoutForRead(100);
    client->send(request);
    request->release();

    auto backNormal=Sprite::createWithSpriteFrameName("back.png");
    auto backSelect=Sprite::createWithSpriteFrameName("back.png");
    auto backItem=MenuItemSprite::create(backNormal,backSelect,CC_CALLBACK_1(store::gotoMainPage,this));
    auto menu=Menu::create(backItem,NULL);
    menu->setPosition(Vec2(600,100));
    this->addChild(menu);
    //input the number of tool user want to buy
    buyNumberText = ui::TextField::create("input number","Arial",30);
    buyNumberText->setMaxLength(10);
    buyNumberText->setTouchEnabled(true);
    buyNumberText->setBright(true);
    buyNumberText->setColor(Color3B::GREEN);
    buyNumberText->setContentSize(Size(500,50));
    buyNumberText->addEventListener([=](Ref* sender, ui::TextField::EventType type){
        switch(type)
        {
            case ui::TextField::EventType::ATTACH_WITH_IME:
                break;
            case ui::TextField::EventType::DETACH_WITH_IME:
            {
                //a little trick to transfer a c_str to int
                stringstream ss;
                ss<<buyNumberText->getString();
                ss>>this->buyNumber;
                break;
            }

            case ui::TextField::EventType::INSERT_TEXT:
                break;
            case ui::TextField::EventType::DELETE_BACKWARD:
                break;
        }
    });
    buyNumberText->setPosition(Vec2(445,680));
    this->addChild(buyNumberText,5);

    Sprite* inputPicture=Sprite::create("input1.png");
    inputPicture->setPosition(450,680);
    addChild(inputPicture,2);

    auto ok=Sprite::createWithSpriteFrameName("ok.png");
    auto okItem=MenuItemSprite::create(ok,ok,ok,CC_CALLBACK_1(store::finishBuy,this));
    okItem->setPosition(Vec2(windowSize.width/2,300));
    auto okMenu=Menu::create(okItem,NULL);
    okMenu->setPosition(Vec2(0,0));
    this->addChild(okMenu);


    return true;
}

Scene* store::creatStoreScene()
{
    auto scene=Scene::create();
    scene->addChild(store::create());
    return scene;
}

void store::gotoMainPage(Ref *object)
{
    Director::getInstance()->replaceScene(mainPage::creatMainPage());
}
//sent the informatin of a purchase to the sever for updating the database
void store::finishBuy(Ref* object)
{
    if(buyNumberText->getString()=="")
        this->infLabel->setString("no number");
    else
    {
        money=money-price*buyNumber;
        haveNumber+=buyNumber;
        toolNumberLabel->setString(StringUtils::format("%d",haveNumber));
        moneyLabel->setString(StringUtils::format("money:%d",money));

        auto request = new HttpRequest();
        request->setUrl("http://2048f433q0.iok.la:11403?type=buy&user="+userName+"&money="+StringUtils::format("%d",money)+"&toolNumber="+StringUtils::format("%d",haveNumber));
        request->setTag("type_buy");
        request->setRequestType(HttpRequest::Type::GET);
        request->setResponseCallback(CC_CALLBACK_2(store::useResponse,this));

        auto client = HttpClient::getInstance();
        client->setTimeoutForConnect(60);
        client->setTimeoutForRead(100);
        client->send(request);
        request->release();
    }
}

void store::useResponse(HttpClient *client,HttpResponse *response)
{
    if(response->isSucceed())
    {
        if(strcmp(response->getHttpRequest()->getTag(),"type_store")==0)
        {
            std::vector<char> * data = response->getResponseData();
            std::stringstream oss;
            for (int i = 0; i < data->size(); i++)
            {
                oss<<(*data)[i];
            }
            std::string str = oss.str();
           //return haveNumber and money in json form
            rapidjson::Document doc;
            doc.Parse<0>(str.c_str());
            if (doc.HasParseError())
            {
                log("json parse error : %d",doc.GetParseError());
            }else//解析成功之后的操作
            {
                log("parse success");
                if (doc.IsObject() && doc.HasMember("money"))
                {
                    rapidjson::Value &value = doc["money"];
                    if (value.IsInt()) {
                        this->money = value.GetInt();
                    }

                }

                if (doc.IsObject() && doc.HasMember("toolNumber"))
                {
                    if(doc["toolNumber"].IsInt())
                    this->haveNumber=doc["toolNumber"].GetInt();
                }

            }
            toolNumberLabel->setString(StringUtils::format("%d",haveNumber));
            moneyLabel->setString(StringUtils::format("money:%d",money));
        }

        else if(strcmp(response->getHttpRequest()->getTag(),"type_buy")==0)
        {
            std::vector<char> * data = response->getResponseData();
            std::stringstream oss;
            for (int i = 0; i < data->size(); i++)
            {
                oss<<(*data)[i];
            }
            std::string str = oss.str();

            if(strcmp(str.c_str(),"succeed")==0)
            {
                infLabel->setString("buy_secceed");
            }

        }
    }
    else
    {
        log("error msg is:%s",response->getErrorBuffer());
    }

}