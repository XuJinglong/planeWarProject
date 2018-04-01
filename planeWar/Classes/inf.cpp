//
// Created by Xu_Jinglong on 2018/3/15.
//

#include "inf.h"
#include "iostream"
#include "json/rapidjson.h"
#include "json/document.h"
using namespace rapidjson;

bool inf::init()
{
    if(!Layer::init())
        return false;
    Sprite* backGround=Sprite::createWithSpriteFrameName("bg3.png");
    backGround->setAnchorPoint(Vec2(0,0));
    addChild(backGround);

    auto backNormal=Sprite::createWithSpriteFrameName("back.png");
    auto backSelect=Sprite::createWithSpriteFrameName("back.png");
    auto backItem=MenuItemSprite::create(backNormal,backSelect,CC_CALLBACK_1(inf::gotoMainPage,this));
    auto menu=Menu::create(backItem,NULL);
    menu->setPosition(Vec2(600,100));
    this->addChild(menu);
    //a http request to get the rank information
    auto request = new HttpRequest();
    request->setUrl("http://2048f433q0.iok.la:11403?type=rank&user="+userName);
    request->setTag("type_rank");
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(inf::useResponse,this));

    auto client = HttpClient::getInstance();
    client->setTimeoutForConnect(60);
    client->setTimeoutForRead(100);
    client->send(request);
    request->release();

    auto nameLabel = Label::createWithSystemFont("name", "Arial", 32);
    auto scoreLabel = Label::createWithSystemFont("score", "Arial", 32);
    auto rankLabel = Label::createWithSystemFont("rank", "Arial", 32);
    nameLabel->setPosition(Vec2(150,1200));
    scoreLabel->setPosition(Vec2(350,1200));
    rankLabel->setPosition(Vec2(550,1200));
    this->addChild(nameLabel,1);
    this->addChild(scoreLabel,1);
    this->addChild(rankLabel,1);

    return true;
}

Scene* inf::creatInfScene()
{
    auto scene=Scene::create();
    scene->addChild(inf::create());
    return scene;
}

void inf::gotoMainPage(Ref *object)
{
    Director::getInstance()->replaceScene(mainPage::creatMainPage());
}

void inf::useResponse(HttpClient *client,HttpResponse *response)
{
    if(response->isSucceed())
    {
            std::vector<char> * data = response->getResponseData();
            std::stringstream oss;
            for (int i = 0; i < data->size(); i++)
            {
                oss<<(*data)[i];
            }
            std::string str = oss.str();
            //return data in json
            rapidjson::Document doc;
            doc.Parse<0>(str.c_str());
            if (doc.HasParseError())
            {
                log("json parse error : %d",doc.GetParseError());
            }else//解析成功之后的操作
            {
                log("parse success");
                if (doc.IsObject() && doc.HasMember("myRank"))
                {
                    rapidjson::Value &value = doc["myRank"];
                    if (value.IsObject())
                    {
                        auto myName = value.GetObject()["user"].GetString();
                        auto myScore=value.GetObject()["scole"].GetInt();//int cant't use getstring
                        auto myRank=value.GetObject()["rank"].GetString();

                        auto myNameLabel=Label::create();
                        myNameLabel->setTextColor(Color4B::BLUE);
                        myNameLabel->setSystemFontSize(32);
                        myNameLabel->setString(myName);
                        myNameLabel->setPosition(Vec2(150,1100));
                        this->addChild(myNameLabel);

                        auto myScoreLabel=Label::create();
                        myScoreLabel->setTextColor(Color4B::BLUE);
                        myScoreLabel->setSystemFontSize(32);
                        myScoreLabel->setString(StringUtils::format("%d",myScore));
                        myScoreLabel->setPosition(Vec2(350,1100));
                        this->addChild(myScoreLabel);

                        auto myRankLabel=Label::create();
                        myRankLabel->setTextColor(Color4B::BLUE);
                        myRankLabel->setSystemFontSize(32);
                        myRankLabel->setString(myRank);
                        myRankLabel->setPosition(Vec2(550,1100));
                        this->addChild(myRankLabel);

                    }

                }

                if (doc.IsObject() && doc.HasMember("rankFirst"))
                {
                    if(doc["rankFirst"].IsArray())
                    {
                        auto rankArray=doc["rankFirst"].GetArray();
                        auto rankNumber=rankArray.Size();
                        for(int i=0;i<rankNumber;i++)
                        {
                            if(rankArray[i].IsObject()&&rankArray[i].HasMember("user"))
                            {
                                auto nameLabel=Label::create();
                                nameLabel->setTextColor(Color4B::RED);
                                nameLabel->setSystemFontSize(32);
                                nameLabel->setString(rankArray[i].GetObject()["user"].GetString());
                                nameLabel->setPosition(Vec2(150,100*(10-i)));
                                this->addChild(nameLabel);
                            }

                            if(rankArray[i].IsObject()&&rankArray[i].HasMember("scole"))
                            {
                                auto scoreLabel=Label::create();
                                scoreLabel->setTextColor(Color4B::RED);
                                scoreLabel->setSystemFontSize(32);
                                int temp=rankArray[i].GetObject()["scole"].GetInt();
                                scoreLabel->setString(StringUtils::format("%d",temp));
                                scoreLabel->setPosition(Vec2(350,100*(10-i)));
                                this->addChild(scoreLabel);
                            }

                            if(rankArray[i].IsObject()&&rankArray[i].HasMember("rank"))
                            {
                                auto rankLabel=Label::create();
                                rankLabel->setTextColor(Color4B::RED);
                                rankLabel->setSystemFontSize(32);
                                rankLabel->setString(rankArray[i].GetObject()["rank"].GetString());
                                rankLabel->setPosition(Vec2(550,100*(10-i)));
                                this->addChild(rankLabel);
                            }
                        }
                    }

                }

            }
    }
    else
    {
        log("error msg is:%s",response->getErrorBuffer());
    }
}