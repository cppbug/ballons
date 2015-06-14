#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	srand(time(0));
	
	// Vẫn giữ background cũ
	Sprite *sprBackground = Sprite::create("background.jpg");
	addChild(sprBackground);
	sprBackground->setAnchorPoint(Point::ZERO);

	//Chỉ xử lý 1 touch tại một thời điểm (sigle-touch)
	auto listener = EventListenerTouchOneByOne::create();

	//Set callback: báo cho cc2x biết cần trả về cho lớp GameScene khi có sự kiện touch xảy ra.
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	//Add cái listener mới tạo vào Dispatcher.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// Lại xóa đoạn thêm 10 bong bóng

	// Gọi hàm schedule upate
	this->scheduleUpdate();
	this->spawnTime = 0;
	return true;
}

void GameScene::runBalloonAction(Sprite *balloon)
{
	// Đâu tiên, đặt vị trí bong bóng dưới màn hình
	balloon->setPositionY(-balloon->getContentSize().width);

	// lây kích thước màn hình
	Size winSize = Director::getInstance()->getWinSize();

	// random tốc độ (từ 100 - 200)
	float speed = 100 + rand() % 100;

	// random quãng đường, ít nhất là 500
	float len = 500 + rand() % ((int) winSize.height);
	float duration = len / speed;

	// tạo action di chuyển
	MoveBy *moveUpAction = MoveBy::create(duration, Vec2(0, len));
	// tạo action mờ dần
	FadeTo *fadeOutAction = FadeTo::create(duration, 0);

	// tạo spawn để kết hợp song song 2 action trên
	Spawn *moveAndFadeAction = Spawn::create(moveUpAction, fadeOutAction, nullptr);

	// tạo hàm callback sau khi thực hiện xong action
	CallFunc *callfunc = CallFunc::create([=]
	{
		//xóa khỏi vector
		m_balloons.eraseObject(balloon);

		// xóa khỏi game
		balloon->removeFromParent();
	});

	// tạo sequence để liên kết 2 action này
	Sequence *sequence = Sequence::create(moveAndFadeAction, callfunc, nullptr);
	// thực thi sequence
	balloon->runAction(sequence);
}

// Hàm này tạm thời không còn dùng nữa
// Disable từ Lesson4
void GameScene::update(float dt)
{
	this->spawnTime += dt;
	if (spawnTime > 0.5)
	{
		// spawn logic
		int _rand = rand() % 3;// tag và tên file sprite của bong bóng

		std::string imgName;

		// đặt giá trị cho tag và tên sprite
		switch (_rand % 3)
		{
		case 0:
			imgName = "balloon_blue.png";
			break;
		case 1:
			imgName = "balloon_red.png";
			break;
		case 2:
			imgName = "balloon_yellow.png";
			break;
		}

		// tạo bong bóng
		Sprite *balloon = Sprite::create(imgName);
		// đặt giá trị position x ngẫu nhiên
		balloon->setPositionX(rand() % 900);
		this->addChild(balloon);
		// bỏ bong bóng vào vector để quản lý
		m_balloons.pushBack(balloon);

		// gọi hàm runBalloonAction cho bong bóng nàu
		this->runBalloonAction(balloon);

		// reset spawn time
		spawnTime = 0;
	}
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point pos = touch->getLocation();
	for (auto balloon : m_balloons) {
		if (balloon->getOpacity() > 0)
		{
			if (balloon->getBoundingBox().containsPoint(pos)) {
				log("Bingo at (%d, %d)", (int)pos.x, (int)pos.y);
				balloon->stopAllActions();
				balloon->removeFromParent();
				balloon->setOpacity(0);
			}
		}
	}
	return true;
}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

}