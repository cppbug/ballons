#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
// Include file vừa tạo
#include "ScoreSystem.h"

#define GAME_TIME 300

#define SPAWN_TIME_CLOCK 30
#define BONUS_TIME 5


class GameScene : public cocos2d::Layer
{
	// Khai báo thêm hệ thống tính điểm
	ScoreSystem scoreSystem;

	// Khai báo label hiển thị điểm
	cocos2d::Label *lbScore;

	//Sử dụng lại vector để quản lý các balloons.
	cocos2d::Vector<cocos2d::Sprite *> m_balloons;

	// Khai báo thêm hàm thêm label vào game, sẽ gọi hàm này
	// trong hàm init, mục tiêu là đỡ dài hàm init thôi
	void initScoreLabel();

	// khai báo thêm một hàm
	void runBalloonAction(cocos2d::Sprite *balloon);

	float spawnTime;

	//Gọi khi có chạm lên màn hình
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	//Gọi khi ngón tay (điểm tiếp xúc) di chuyển trong lúc vẫn còn nhấn trên màn hình
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	//Gọi khi ngón tay (điểm tiếp xúc) rời màn hình
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	
	void onScoreChanged(ScoreSystem *scoreSys);

	float m_gameTime;
	cocos2d::Label *m_lblTime;

	//Clock
	void spawnClock(float dt);
	//Tạo vector quản lý đồng hồ
	cocos2d::Vector<cocos2d::Sprite *> m_clocks;
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
