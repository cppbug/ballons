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
	Sprite *sprBackground = Sprite::create("background.jpg");
	addChild(sprBackground);
	sprBackground->setAnchorPoint(Point::ZERO);

	//Tạo 10 cái bong bóng
	for (int i = 0; i < 10; i++) {
		//Chỉ là để vẽ đủ các hình
		int imgIndex = i % 3;
		//Khởi tạo đối tượng bong bóng
		Sprite *balloon = nullptr;
		//Random vị trí của bong bóng trong khoảng Size(900, 600)
		Point pos = Point(rand() % 900, rand()%600);
		switch (imgIndex){
		case 0: {
					balloon = Sprite::create("balloon_blue.png");
					balloon->setTag(BLUE_BALLOON);
					break;
		}
		case 1: {
					balloon = Sprite::create("balloon_red.png");
					balloon->setTag(RED_BALLOON);
					break;
		}
		case 2: {
					balloon = Sprite::create("balloon_yellow.png");
					balloon->setTag(YELLOW_BALLOON);
					break;
		}
		}
		//Nếu đối tượng balloon khác nullptr thì vẽ lên màn hình
		if (balloon) {
			addChild(balloon);
			balloon->setPosition(pos);
			m_balloons.pushBack(balloon);
		}
	}

	scheduleUpdate();
	return true;
}

void GameScene::update(float dt)  {
	//Lấy kích thước window
	Size ws = Director::getInstance()->getWinSize();
	//Duyệt từng đối tượng bong bóng
	for (auto balloon : m_balloons) {
		//Nếu bong bóng bay vượt qua khỏi màn hình thì set lại ví trí ban đầu
		if (balloon->getPositionY() > ws.height + 200) {
			balloon->setPositionY(-200);
		}
		//Ngược lại, cho bong bóng di chuyển lên với vận tốc đã định nghĩa
		else {
			switch (balloon->getTag())
			{
					case BLUE_BALLOON: {
										   balloon->setPositionY(balloon->getPositionY() + VEC_BLUE_BALLOON*dt);
										   break;
					}
					case RED_BALLOON: {
										   balloon->setPositionY(balloon->getPositionY() + VEC_RED_BALLOON*dt);
										   break;
					}
					case YELLOW_BALLOON:{
											balloon->setPositionY(balloon->getPositionY() + VEC_YELLOW_BALLOON*dt);
											break;
					}
					default:
						break;
			}
		}
	}
}
