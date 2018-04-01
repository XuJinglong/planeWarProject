//
// Created by Xu_Jinglong on 2018/3/15.
//

#include "mainPage.h"
#include "iostream"

bool mainPage::init()
{
    if(Layer::init())
    {
        this->windowSize=this->getContentSize();

        Sprite* background=Sprite::create("bg2.png");
        background->setAnchorPoint(Vec2(0,0));
        this->addChild(background);

        Vector<MenuItem*> MenuItems;
        auto backLabel = Label::createWithSystemFont("Back", "Arial", 48);
        auto settingLabel= Label::createWithSystemFont("Setting", "Arial", 48);
        auto storeLabel= Label::createWithSystemFont("Store", "Arial", 48);
        auto infLabel= Label::createWithSystemFont("Rank", "Arial", 48);
        auto startGameLabel= Label::createWithSystemFont("StartGame", "Arial", 48);

        backLabel->setColor(Color3B::RED);
        settingLabel->setColor(Color3B::RED);
        storeLabel->setColor(Color3B::RED);
        infLabel->setColor(Color3B::RED);
        startGameLabel->setColor(Color3B::RED);

        auto backItem = MenuItemLabel::create(backLabel,CC_CALLBACK_1(mainPage::backToLog,this));
        backItem->setPosition(Vec2(windowSize.width/2,200));
        auto settingItem = MenuItemLabel::create(settingLabel,CC_CALLBACK_1(mainPage::intoSetting,this));
        settingItem->setPosition(Vec2(windowSize.width/2,400));
        auto storeItem = MenuItemLabel::create(storeLabel,CC_CALLBACK_1(mainPage::intoStore,this));
        storeItem->setPosition(Vec2(windowSize.width/2,600));
        auto infItem = MenuItemLabel::create(infLabel,CC_CALLBACK_1(mainPage::intoInf,this));
        infItem->setPosition(Vec2(windowSize.width/2,800));
        auto startGameItem = MenuItemLabel::create(startGameLabel,CC_CALLBACK_1(mainPage::intoGame,this));
        startGameItem->setPosition(Vec2(windowSize.width/2,1000));

        MenuItems.pushBack(backItem);
        MenuItems.pushBack(settingItem);
        MenuItems.pushBack(storeItem);
        MenuItems.pushBack(infItem);
        MenuItems.pushBack(startGameItem);

        auto menu = Menu::createWithArray(MenuItems);
        menu->setPosition(Vec2(0,0));
        this->addChild(menu, 2);

        auto closeItem = MenuItemImage::create(
                "CloseNormal.png",
                "CloseSelected.png",
                CC_CALLBACK_1(mainPage::menuCloseCallback, this));
        closeItem->setPosition(Vec2(650,100));
        auto menu2 = Menu::create(closeItem, NULL);
        menu2->setPosition(Vec2::ZERO);
        this->addChild(menu2, 3);

        return true;
    }
    return false;
}

Scene* mainPage::creatMainPage()
{
    Scene* scene=Scene::create();
    scene->addChild(mainPage::create());
    return scene;
}

void mainPage::backToLog(Ref* object)
{
    Director* dir=Director::getInstance();
    dir->replaceScene(logIn::creatLogInScene());
}

void mainPage::intoInf(Ref *object){
    Director* dir=Director::getInstance();
    dir->replaceScene(inf::creatInfScene());
}

void mainPage::intoSetting(Ref *object){
    Director* dir=Director::getInstance();
    dir->replaceScene(setting::creatSettingScene());
}

void mainPage::intoStore(Ref *object){
    Director* dir=Director::getInstance();
    dir->replaceScene(store::creatStoreScene());
}

void mainPage::intoGame(Ref *object)
{
    Director::getInstance()->replaceScene(gameMode1::creatgame1Scene());
}

void mainPage::menuCloseCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->end();
}