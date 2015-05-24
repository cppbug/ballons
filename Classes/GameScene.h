#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

#define VEC_BLUE_BALLOON 100
#define VEC_RED_BALLOON 150
#define VEC_YELLOW_BALLOON 200

#define BLUE_BALLOON 1
#define RED_BALLOON 2
#define YELLOW_BALLOON 3

class GameScene : public cocos2d::Layer
{
	cocos2d::Vector<cocos2d::Sprite*> m_balloons;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	void update(float dt);
    
    // implement the "static create()" method manually
	CREATE_FUNC(GameScene);
};

#endif // __HELLOWORLD_SCENE_H__
