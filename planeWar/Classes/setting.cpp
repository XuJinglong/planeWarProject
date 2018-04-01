//
// Created by Xu_Jinglong on 2018/3/15.
//

#include "setting.h"

bool setting::init()
{
    if(!Layer::init())
        return false;
    //background
    Sprite* backGround=Sprite::createWithSpriteFrameName("bg1.png");
    backGround->setAnchorPoint(Vec2(0,0));
    addChild(backGround);
    //add back menu
    auto backNormal=Sprite::createWithSpriteFrameName("back.png");
    auto backSelect=Sprite::createWithSpriteFrameName("back.png");
    auto backItem=MenuItemSprite::create(backNormal,backSelect,CC_CALLBACK_1(setting::gotoMainPage,this));
    auto menu=Menu::create(backItem,NULL);
    menu->setPosition(Vec2(600,100));
    this->addChild(menu,9);
    //add label of the settings
    auto effectLabel = Label::createWithSystemFont("Effect", "Arial", 32);
    auto volumLabel = Label::createWithSystemFont("Volum", "Arial", 32);
    effectLabel->setPosition(Vec2(200,800));
    volumLabel->setPosition(Vec2(200,600));
    this->addChild(effectLabel,1);
    this->addChild(volumLabel,1);
    //a checkbox  used for turn on/off the music effect.Note:the pictures can't be transparent usually
    auto checkbox = ui::CheckBox::create("check_box_normal.png",
                                     "check_box_normal_press.png",
                                     "check_box_active.png",
                                     "check_box_normal_disable.png",
                                     "check_box_active_disable.png");

    checkbox->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                musicEffect=!musicEffect;
                break;
            default:
                break;
        }
    });
    checkbox->setPosition(Vec2(500,800));
    this->addChild(checkbox);
    //add a slider to adjust the volum of the music effect
    auto slider = ui::Slider::create();
    slider->loadBarTexture("Slider_Back.png"); // what the slider looks like
    slider->loadSlidBallTextures("SliderNode_Normal.png", "SliderNode_Press.png", "SliderNode_Disable.png");
    slider->loadProgressBarTexture("Slider_PressBar.png");
    slider->setPercent(static_cast<int>(100*effectVolum));
    //Note:not using a "=" instead of a "&" int the lambda expression
    slider->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                effectVolum= static_cast<float >(slider->getPercent())/100;//the return of getPercent is int.  100.0 not 100
                break;
            default:
                break;
        }
    });

    slider->setPosition(Vec2(500,600));
    this->addChild(slider);

    return true;
}

Scene* setting::creatSettingScene()
{
    auto scene=Scene::create();
    scene->addChild(setting::create());
    return scene;
}

void setting::gotoMainPage(Ref *object)
{
    Director::getInstance()->replaceScene(mainPage::creatMainPage());
}
