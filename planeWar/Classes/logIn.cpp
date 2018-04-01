//
// Created by Xu_Jinglong on 2018/3/15.
//

#include <iostream>
#include "logIn.h"

bool logIn::init()
{
    if(Layer::init())
    {
        this->windowSize=this->getContentSize();

        Sprite* backGround=Sprite::createWithSpriteFrameName("bg1.png");
        backGround->setAnchorPoint(Vec2(0,0));
        addChild(backGround);

        Sprite* inputPicture1=Sprite::createWithSpriteFrameName("input2.png");
        inputPicture1->setPosition(480,800);
        addChild(inputPicture1,2);
        Sprite* inputPicture2=Sprite::createWithSpriteFrameName("input2.png");
        inputPicture2->setPosition(480,600);
        addChild(inputPicture2,2);

        auto nameLabel = Label::createWithSystemFont("username:", "Arial", 32);
        auto passwordLabel = Label::createWithSystemFont("password:", "Arial", 32);
        nameLabel->setPosition(Vec2(150,800));
        passwordLabel->setPosition(Vec2(150,600));
        this->addChild(nameLabel,1);
        this->addChild(passwordLabel,1);

        auto nameText = ui::TextField::create("input name","Arial",30);
        nameText->setMaxLength(10);
        nameText->setTouchEnabled(true);
        nameText->setBright(true);
        nameText->setColor(Color3B::GREEN);
        nameText->setContentSize(Size(500,50));
        nameText->addEventListener([=](Ref* sender, ui::TextField::EventType type){
            switch(type)
            {
                case ui::TextField::EventType::ATTACH_WITH_IME:
                    break;
                case ui::TextField::EventType::DETACH_WITH_IME:
                    userName=nameText->getString();
                    break;
                case ui::TextField::EventType::INSERT_TEXT:
                    break;
                case ui::TextField::EventType::DELETE_BACKWARD:
                    break;
            }
        });
        nameText->setPosition(Vec2(450,800));
        this->addChild(nameText,5);

        auto passText = ui::TextField::create("input password","Arial",30);
        passText->setMaxLength(10);
        passText->setTouchEnabled(true);
        passText->setBright(true);
        passText->setColor(Color3B::GREEN);
        passText->setContentSize(Size(500,50));
        passText->addEventListener([=](Ref* sender, ui::TextField::EventType type){
            switch(type)
            {
                case ui::TextField::EventType::ATTACH_WITH_IME:
                    break;
                case ui::TextField::EventType::DETACH_WITH_IME:
                    this->passWord=passText->getString();
                    break;
                case ui::TextField::EventType::INSERT_TEXT:
                    break;
                case ui::TextField::EventType::DELETE_BACKWARD:
                    break;
            }
        });
        passText->setPosition(Vec2(450,600));
        this->addChild(passText,5);

        auto registerButton = ui::Button::create("register.png", "register.png", "register.png");
        registerButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type){
            switch (type)
            {
                case ui::Widget::TouchEventType::BEGAN:
                    break;
                case ui::Widget::TouchEventType::ENDED:
                    if(userName!=""&&passWord!="")
                        this->newRegister();
                    else
                        this->infLabel->setString("user name or password can't be null");
                    break;
                default:
                    break;
            }
        });
        registerButton->setPosition(Vec2(220,400));
        this->addChild(registerButton,1);

        auto logInButton =ui::Button::create("login.png", "login.png", "login.png");
        logInButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type){
            switch (type)
            {
                case ui::Widget::TouchEventType::BEGAN:
                    break;
                case ui::Widget::TouchEventType::ENDED:
                    if(userName!=""&&passWord!="")
                        this->gotoMainPage();
                    else
                        this->infLabel->setString("user name or password can't be null");
                    break;
                default:
                    break;
            }
        });
        logInButton->setPosition(Vec2(550,400));
        this->addChild(logInButton,1);


        infLabel = Label::createWithSystemFont("", "Arial", 32);
        infLabel->setPosition(Vec2(300,200));
        this->addChild(infLabel,1);

        return true;
    }
    return false;
}

Scene* logIn::creatLogInScene()
{
    Scene* scene=Scene::create();
    scene->addChild(logIn::create());
    return scene;
}

//sent data to sever and use the response to decide whether enter into the main page.
void logIn::gotoMainPage()
{
    //a http request
    //the address of sever:http://2048f433q0.iok.la:11403
    //sent request type ,user name and password
    auto request = new HttpRequest();
    request->setUrl("http://2048f433q0.iok.la:11403?type=enter&user="+userName+"&password="+passWord);
    request->setTag("type_enter");
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(logIn::useResponse,this));

    auto client = HttpClient::getInstance();
    client->setTimeoutForConnect(60);
    client->setTimeoutForRead(100);
    client->send(request);
    request->release();
}

void logIn::newRegister()
{
    //http request
    auto request = new HttpRequest();
    request->setUrl("http://2048f433q0.iok.la:11403?type=register&user="+userName+"&password="+passWord);
    request->setTag("type_register");
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(logIn::useResponse,this));

    auto client = HttpClient::getInstance();
    client->setTimeoutForConnect(60);
    client->setTimeoutForRead(100);
    client->send(request);
    request->release();

}

//deal with http response
void logIn::useResponse(HttpClient *client, HttpResponse *response)
{
    if(response->isSucceed())
    {
        //note:c风格字符串没有定义==
        if(strcmp(response->getHttpRequest()->getTag(),"type_enter")==0)
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
                Director::getInstance()->replaceScene(mainPage::creatMainPage());
            }
            else if(strcmp(str.c_str(),"noUser")==0)
            {
                infLabel->setString("noUser");
            }
            else if(strcmp(str.c_str(),"passWordWrong")==0)
            {
                infLabel->setString("passWordWrong");
            }
        }
        else if(strcmp(response->getHttpRequest()->getTag(),"type_register")==0)
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
                infLabel->setString("succeed");
            }
            else if(strcmp(str.c_str(),"userExist")==0)
            {
                infLabel->setString("userExist");
            }
        }

    }
    else
    {
        log("error msg is:%s",response->getErrorBuffer());
    }

}

