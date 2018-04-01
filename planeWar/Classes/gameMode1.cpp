//
// Created by Xu_Jinglong on 2018/3/15.
//

#include "gameMode1.h"

bool gameMode1::init()
{
    if(!Layer::init())
        return false;
    //初始化参数
    this->windowSize=this->getContentSize();
    this->planeIsTouch=false;
    this->mainBulletNumber=0;
    this->enemyNumber=0;
    this->startTime=clock();
    //a particle effect of snow
    auto snow = ParticleSnow::create();
    snow->setPosition(Vec2(windowSize.width / 2, windowSize.height));
    this->addChild(snow,9);

    //a http request to get the number of tool a user possesses
    auto request = new HttpRequest();
    request->setUrl("http://2048f433q0.iok.la:11403?type=getdata&user="+userName);
    request->setTag("type_getdata");
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(gameMode1::useResponse,this));

    auto client = HttpClient::getInstance();
    client->setTimeoutForConnect(60);
    client->setTimeoutForRead(100);
    client->send(request);
    request->release();

    //返回按钮
    auto backNormal=Sprite::createWithSpriteFrameName("back.png");
    auto backSelect=Sprite::createWithSpriteFrameName("back.png");
    auto backItem=MenuItemSprite::create(backNormal,backSelect,CC_CALLBACK_1(gameMode1::gotoMainPage,this));
    auto menu=Menu::create(backItem,NULL);
    menu->setPosition(Vec2(600,100));
    this->addChild(menu,9);

    //背景循环
    bgImg1=Sprite::create("star.png");
    bgImg2=Sprite::create("star.png");
    bgImg1->setAnchorPoint(Vec2(0,0));
    bgImg1->setPosition(Vec2(0,0));
    bgImg2->setAnchorPoint(Vec2(0,0));
    bgImg2->setPosition(Vec2(0,windowSize.height));
    this->addChild(bgImg1);
    this->addChild(bgImg2);
    this->schedule(schedule_selector(gameMode1::bgCircle),0.1);

    //计分计时
    startTime=clock();
    timeLabel=Label::create();
    scoreLabel=Label::create();
    timeLabel->setTextColor(Color4B::BLUE);
    timeLabel->setSystemFontSize(32);
    timeLabel->setString("time:0s");

    scoreLabel->setTextColor(Color4B::BLUE);
    scoreLabel->setSystemFontSize(32);
    scoreLabel->setString("scole:0");

    timeLabel->setPosition(Vec2(120,windowSize.height-100));
    scoreLabel->setPosition(Vec2(600,windowSize.height-100));
    this->addChild(timeLabel,2);
    this->addChild(scoreLabel,2);
    this->scheduleUpdate();

    //道具
    auto tool=Sprite::create("tool2.png");
    auto toolItem=MenuItemSprite::create(tool,tool,tool,CC_CALLBACK_1(gameMode1::toolUse,this));
    auto toolButton=Menu::create(toolItem,NULL);
    toolButton->setPosition(100,200);
    this->addChild(toolButton,9);

    toolLabel=Label::create();
    toolLabel->setTextColor(Color4B::BLUE);
    toolLabel->setSystemFontSize(32);
    toolLabel->setString(StringUtils::format("superBullet:%d",toolNumber));
    toolLabel->setPosition(Vec2(400,windowSize.height-100));
    this->addChild(toolLabel,2);

    //添加主机
    main_Plane=mainPlane::create();
    main_Plane->setTexture("mainPlane1.png");
    main_Plane->setPosition(Vec2(windowSize.width/2,200));
    this->addChild(main_Plane);

    //主机添加监听器
    auto listener1=EventListenerTouchOneByOne::create();

    listener1->onTouchBegan = [this](Touch* touch, Event* event){
        if(this->main_Plane->getBoundingBox().containsPoint(touch->getLocation()))
            this->planeIsTouch=true;
        return true;
    };

    listener1->onTouchMoved = [this](Touch* touch, Event* event){
        if(planeIsTouch)
        {
            this->main_Plane->setPosition(touch->getLocation());
        }
    };

    listener1->onTouchEnded = [this](Touch* touch, Event* event){
        this->planeIsTouch=false;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, this);

    //some timers
    //发射子弹
    this->schedule(schedule_selector(gameMode1::mainBulletManage),0.2);
    //产生敌机
    this->schedule(schedule_selector(gameMode1::enemyManage),0.1);
    //敌机子弹管理
    this->schedule(schedule_selector(gameMode1::enemyBulletManage),0.5);
    //碰撞检测
    this->schedule(schedule_selector(gameMode1::collisionManage),0.1);
    //游戏结束处理
    this->schedule(schedule_selector(gameMode1::passManage),0.1);

    return true;
}

Scene* gameMode1::creatgame1Scene(){
    auto scene=Scene::create();
    scene->addChild(gameMode1::create());
    return scene;
}
//sent data to sever before exit the game scene
void gameMode1::gotoMainPage(Ref *object){

    auto request = new HttpRequest();
    request->setUrl("http://2048f433q0.iok.la:11403?type=record&user="+userName+"&score="+
                            StringUtils::format("%d",score).c_str()+"&toolNumber="+
                            StringUtils::format("%d",toolNumber).c_str());
    request->setTag("type_record");
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(gameMode1::useResponse,this));

    auto client = HttpClient::getInstance();
    client->setTimeoutForConnect(60);
    client->setTimeoutForRead(100);
    client->send(request);
    request->release();
}

void gameMode1::bgCircle(float time)
{
    if(bgImg1->getPosition().y<-windowSize.height)
        bgImg1->setPosition(Vec2(0,windowSize.height));
    else
    {
        auto yy=bgImg1->getPosition().y;
        yy-=10;
        bgImg1->setPosition(Vec2(0,yy));
    }

    if(bgImg2->getPosition().y<-windowSize.height)
        bgImg2->setPosition(Vec2(0,windowSize.height));
    else
    {
        auto yy=bgImg2->getPosition().y;
        yy-=10;
        bgImg2->setPosition(Vec2(0,yy));
    }

}

void gameMode1::mainBulletManage(float time)
{
    if(mainBulletNumber<20)
    {
        bullet* mb=bullet::create();
        mb->bt=main_type;
        mb->setTexture("bullet1.png");
        mb->setPosition(Vec2(main_Plane->getPosition().x,main_Plane->getPosition().y+50));
        this->addChild(mb);
        mainBulletNumber++;
        auto by=MoveBy::create(1,Vec2(0,600));
        CallFuncN* fun=CallFuncN::create(CC_CALLBACK_1(gameMode1::removeBullet,this));
        mb->runAction(Sequence::create(by,fun,NULL));

    }

}
void gameMode1::removeBullet(Ref* object)
{
    bullet* bl=(bullet*)object;
    if(bl->bt==main_type)
        this->mainBulletNumber--;
    bl->stopAllActions();
    this->removeChild(bl);
}
//produce 4 enemy planes at most having random textures and speed
void gameMode1::enemyManage(float time)
{
    int enemyTag=random(1,4);
    if(enemyNumber<4)
    {
        enemy* e=enemy::create();
        char enemyPicture[30];
        sprintf(enemyPicture,"enemy%d.png",enemyTag);
        e->setTexture(enemyPicture);
        e->setPosition(Vec2(random(50,650),windowSize.height+100));
        this->addChild(e);
        this->enemyNumber++;
        e->tag=enemyNumber;
        auto to=MoveTo::create(random(6,10),Vec2(e->getPosition().x,0));
        e->runAction(Sequence::create(to,CallFunc::create([=]() {
            this->enemyNumber--;
            e->stopAllActions();
            this->removeChild(e);
        }),NULL));

    }
}
//select the enemy plane with right tag randomly produced to emit a bullet
void gameMode1::enemyBulletManage(float time)
{
    Vector<Node*> allNodes=this->getChildren();
    int rand=random(1,4);
    for(auto & object:allNodes)
    {
        if(strcmp(typeid(enemy).name(), typeid(*object).name())==0)
        {
            enemy* en=(enemy*)object;
            if(en->tag==rand)
            {
                bullet* eb=bullet::create();
                eb->bt=enemy_type;
                eb->setTexture("bullet3.png");
                eb->setPosition(Vec2(en->getPosition().x,en->getPosition().y-50));
                this->addChild(eb);
                auto to=MoveTo::create(4,Vec2(en->getPosition().x,0));
                CallFuncN* fun=CallFuncN::create(CC_CALLBACK_1(gameMode1::removeBullet,this));
                eb->runAction(Sequence::create(to,fun,NULL));
                break;
            }
        }
    }

}
//deal with all collision events
void gameMode1::collisionManage(float time)
{
    Vector<Node*> allNodes=this->getChildren();
    //decide if a mian bullet hit a enemy
    for(int i=0;i<allNodes.size();i++)
    {
        Node* object1=allNodes.at(i);
        if(strcmp(typeid(enemy).name(), typeid(*object1).name())==0)
        {
            enemy* en=(enemy*)object1;
            for(int j=0;j<allNodes.size();j++)
            {
                Node* object2=allNodes.at(j);
                if(strcmp(typeid(bullet).name(), typeid(*object2).name())==0)
                {
                    bullet* mainBullet=(bullet*)object2;
                    if(mainBullet->bt==main_type)
                    {
                        if(mainBullet->getBoundingBox().intersectsRect(en->getBoundingBox()))
                        {
                            this->specialEffect(en->getPosition());
                            en->stopAllActions();
                            en->removeFromParent();
                            this->enemyNumber--;
                            this->score+=10;
                            break;
                        }
                    }
                }

            }

        }
    }


    //decide if an enemy hit the main plane
    for(int i=0;i<allNodes.size();i++)
    {
        Node* object=allNodes.at(i);
        if(strcmp(typeid(enemy).name(), typeid(*object).name())==0)
        {
            enemy* en=(enemy*)object;
            if(this->main_Plane->getBoundingBox().intersectsRect(en->getBoundingBox()))
            {
                this->specialEffect(en->getPosition());
                this->specialEffect(main_Plane->getPosition());
                en->stopAllActions();
                en->removeFromParent();
                this->enemyNumber--;
                this->showGameOver();

                break;
            }
        }
    }
    //decide if the main plane is hitted by enemy bullet
    for(int i=0;i<allNodes.size();i++)
    {
        Node* object1=allNodes.at(i);
        if(strcmp(typeid(bullet).name(), typeid(*object1).name())==0)
        {
            bullet* enemyBullet=(bullet*)object1;
            if(enemyBullet->bt==enemy_type)
            {
                if(this->main_Plane->getBoundingBox().intersectsRect(enemyBullet->getBoundingBox()))
                {
                    this->specialEffect(main_Plane->getPosition());
                    this->showGameOver();
                }
            }
        }
    }

}
//pass the game
void gameMode1::passManage(float time)
{
    long offset = clock()-startTime;
    if(offset/1000000>=10)
        this->showVictory();

}

void gameMode1::showGameOver()
{
    this->pause();
    auto failPicture=Sprite::createWithSpriteFrameName("GAMEOVER1.png");
    failPicture->setPosition(windowSize/2);
    this->addChild(failPicture);
    auto finalScole=Label::create();
    finalScole->setString(StringUtils::format("%d",score));
    finalScole->setColor(Color3B::BLUE);
    auto finalTime=Label::create();
    finalTime->setString(StringUtils::format("%d",(int)(clock()-startTime)/1000000));
    finalTime->setColor(Color3B::BLUE);
    failPicture->addChild(finalScole,9);
    failPicture->addChild(finalTime,9);
    finalScole->setPosition(250,180);
    finalTime->setPosition(250,135);
    //restart the game scene when touch the ok button
    auto ok=Sprite::createWithSpriteFrameName("ok.png");
    auto okItem=MenuItemSprite::create(ok,ok,ok,CC_CALLBACK_1(gameMode1::restart,this));
    auto okMenu=Menu::create(okItem,NULL);
    okMenu->setPosition(Vec2(failPicture->getContentSize().width/2,40));
    failPicture->addChild(okMenu,9);
}

void gameMode1::showVictory()
{
    this->pause();
    auto vicPicture=Sprite::createWithSpriteFrameName("victory1.png");
    vicPicture->setPosition(windowSize/2);
    this->addChild(vicPicture);
    auto finalScore=Label::create();
    finalScore->setString(StringUtils::format("%d",score));
    finalScore->setColor(Color3B::BLUE);
    auto finalTime=Label::create();
    finalTime->setString(StringUtils::format("%d",(int)(clock()-startTime)/CLOCKS_PER_SEC));
    finalTime->setColor(Color3B::BLUE);
    vicPicture->addChild(finalScore,9);
    vicPicture->addChild(finalTime,9);
    finalScore->setPosition(250,180);
    finalTime->setPosition(250,125);
    //return to main page after pass the game
    auto ok=Sprite::create("ok.png");
    auto okItem=MenuItemSprite::create(ok,ok,ok,CC_CALLBACK_1(gameMode1::gotoMainPage,this));
    auto okMenu=Menu::create(okItem,NULL);
    okMenu->setPosition(Vec2(vicPicture->getContentSize().width/2,40));
    vicPicture->addChild(okMenu);
}

void gameMode1::update(float time)
{
    long offset = clock()-startTime;
    timeLabel->setString( StringUtils::format("time:%ds",((int)offset)/1000000));
    scoreLabel->setString(StringUtils::format("scole:%d",score));
}

void gameMode1::toolUse(Ref* object)
{
    if(toolNumber<=0)
    {
        return;
    }
    toolNumber-=1;
    toolLabel->setString(StringUtils::format("toolNumber:%d",toolNumber));
    auto tool=bullet::create();
    tool->bt=main_type;
    tool->setTexture("tool2.png");
    tool->setPosition(Vec2(main_Plane->getPosition().x,main_Plane->getPosition().y+50));
    this->addChild(tool);
    //the tool hit every enemy in the game
    auto nodes=this->getChildren();
    Vector<FiniteTimeAction*> vt;
    for(auto &it:nodes)
    {
        if(strcmp(typeid(*it).name(),typeid(enemy).name())==0)
        {
            enemy* en=(enemy*)it;
            auto to=MoveTo::create(0.1,en->getPosition());
            en->stopAllActions();
            this->removeChild(en);
            this->enemyNumber--;
            this->score+=10;
            this->specialEffect(en->getPosition());
            vt.pushBack(to);
        }
    }
    vt.pushBack(CallFunc::create([=](){
        tool->stopAllActions();
        this->removeChild(tool);
    }));
    tool->runAction(Sequence::create(vt));
}

void gameMode1::specialEffect(Vec2 v)
{
    auto audio=CocosDenshion::SimpleAudioEngine::getInstance();
    audio->setEffectsVolume(effectVolum);
    if(musicEffect)
        audio->playEffect("boom1.wav",false,1.0f,1.0f,1.0f);

    auto boom=Sprite::createWithSpriteFrameName("boom1.png");
    boom->setPosition(v);
    this->addChild(boom);
    auto change1=ScaleTo::create(0.1,0.1);
    auto change2=ScaleTo::create(0.3,2);
    auto seq=Sequence::create(change1,change2,CallFunc::create([=](){
        boom->stopAllActions();
        this->removeChild(boom);
    }), nullptr);
    boom->runAction(seq);

}

void gameMode1::restart(Ref* object)
{
    Director::getInstance()->replaceScene(gameMode1::creatgame1Scene());
}

void gameMode1::useResponse(HttpClient *client,HttpResponse *response)
{
    if(response->isSucceed())
    {
        if(strcmp(response->getHttpRequest()->getTag(),"type_record")==0)
            Director::getInstance()->replaceScene(mainPage::creatMainPage());
        else if(strcmp(response->getHttpRequest()->getTag(),"type_getdata")==0)
        {
            std::vector<char> * data = response->getResponseData();
            std::stringstream oss;
            for (int i = 0; i < data->size(); i++)
            {
                oss<<(*data)[i];
            }
            oss>>this->toolNumber;
            toolLabel->setString(StringUtils::format("toolNumber:%d",toolNumber));
        }

    }

}