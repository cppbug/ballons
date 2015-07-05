#include "GameScene.h"
#include "SimpleAudioEngine.h"

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

	// Gán hàm onScoreChanged vào biến OnScoreChanged của scoreSystem
	scoreSystem.OnScoreChanged = CC_CALLBACK_1(GameScene::onScoreChanged, this);
	highestScoreOfLastGame = UserDefault::getInstance()->getIntegerForKey("HighestScore", 0);

	// Xoá đoạn test label vừa thêm
	// Gọi hàm init score label
	this->initScoreLabel();	
	// gọi onScoreChanged để cập nhật lại label
	this->onScoreChanged(&scoreSystem);

	m_gameTime = GAME_TIME;
	m_lblTime = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	//Canh lbl theo top-right
	m_lblTime->setAnchorPoint(Point(1, 1));
	m_lblTime->setPosition(getContentSize());
	//Set màu đỏ cho dễ nhìn
	m_lblTime->setColor(Color3B::RED);
	addChild(m_lblTime);

	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::spawnClock), 30);

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);

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

		// trừ điểm:
		scoreSystem.decreaseScore(5);
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

	//Countdown
	m_gameTime -= dt;
	//In thời gian lên màn hình, in ra kiểu int(làm tròn đến giây)
	int minute = (int) floorf(m_gameTime) / 60;
	int second = (int) floorf(m_gameTime) % 60;
	m_lblTime->setString(StringUtils::format("%02d : %02d", minute, second));
	
	//Dừng game
	if (m_gameTime <= 0) {
		this->finishGame();
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

				// cộng thêm điểm nào
				scoreSystem.increaseScore(10);

				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tap.wav");
			}
		}
	}

	for (auto clock : m_clocks) {
		if (clock->getOpacity() > 0)
		{
			if (clock->getBoundingBox().containsPoint(pos)) {
				log("Bonus time");
				clock->stopAllActions();
				clock->removeFromParent();

				// cộng thêm thời gian chứ :3
				m_gameTime += BONUS_TIME;
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

// Viết nội dung cho hàm thêm label
void GameScene::initScoreLabel()
{
	// Kích thước màn hìh
	Size winSize = getContentSize();

	lbScore = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	// canh lại anchor point
	lbScore->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	// đặt ở góc trên bên trái
	lbScore->setPosition(30, winSize.height);

	// thêm vào game
	addChild(lbScore);
}

// Khi điểm số thay đổi thì ta cập nhật lại nội dung label
void GameScene::onScoreChanged(ScoreSystem *scoreSys)
{
	// cập nhật text trong label
	// tạo chuỗi từ điểm số
	std::string sScore = StringUtils::format("%d | Highest: %d",
		scoreSystem.getScore(), MAX(highestScoreOfLastGame, scoreSystem.getScore()));
	// hàm set string dùng để thay đổi đoạn text trong label
	lbScore->setString(sScore);
}

void GameScene::spawnClock(float dt) {
	Sprite *clock = Sprite::create("clock.png");
	// đặt giá trị position x ngẫu nhiên và đồng hồ dưới màn hình.
	clock->setPosition(Point(rand() % 900, -100));
	this->addChild(clock);
	m_clocks.pushBack(clock);

	//Chúng ta set hành động cho đồng hồ tương tự bong bóng
	// lây kích thước màn hình
	Size winSize = Director::getInstance()->getWinSize();

	// random tốc độ (từ 300 - 600)
	float speed = 300 + rand() % 300;

	// random quãng đường, ít nhất là 500
	float len = 500 + rand() % ((int)winSize.height);
	float duration = len / speed;
	MoveBy *moveUpAction = MoveBy::create(duration, Vec2(0, len));
	// tạo action mờ dần
	FadeTo *fadeOutAction = FadeTo::create(duration, 0);

	// tạo spawn để kết hợp song song 2 action trên
	Spawn *moveAndFadeAction = Spawn::create(moveUpAction, fadeOutAction, nullptr);
	// tạo hàm callback sau khi thực hiện xong action
	CallFunc *callfunc = CallFunc::create([=]
	{
		//xóa khỏi vector
		m_clocks.eraseObject(clock);
		// xóa khỏi game
		clock->removeFromParent();
	});

	// tạo sequence để liên kết 2 action này
	Sequence *sequence = Sequence::create(moveAndFadeAction, callfunc, nullptr);
	// thực thi sequence
	clock->runAction(sequence);
}

void GameScene::finishGame()
{
	//nghỉ chạy update
	unscheduleUpdate();

	std::string message = "";
	//Nếu điểm lớn hơn 0 thì thắng, ngược lại thua
	if (scoreSystem.getScore() > 0) {
		message = "Win";
	}
	else {
		message = "Failed!";
	}
	Label *lblFinish = Label::createWithTTF(message, "fonts/Marker Felt.ttf", 200);
	lblFinish->setColor(Color3B::ORANGE);
	addChild(lblFinish);
	lblFinish->setPosition(getContentSize() / 2.0f);

	if (highestScoreOfLastGame < scoreSystem.getScore())
	{
		UserDefault::getInstance()->setIntegerForKey("HighestScore", scoreSystem.getScore());
	}
}