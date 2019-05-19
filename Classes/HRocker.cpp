#include "HRocker.h"
#include "Constant.h"
#include <functional>
USING_NS_CC;

HRocker* HRocker::createHRocker(const char *rockerImageName, const char *rockerBGImageName, Point position)
{
	HRocker* layer = HRocker::create();
	if (layer)
	{
		layer->rockerInit(rockerImageName, rockerBGImageName, position);
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}
void HRocker::rockerInit(const char *rockerImageName, const char *rockerBGImageName, Point position)
{
	//添加摇杆背景图  
	Sprite* spRockerBG = Sprite::create(rockerBGImageName);
	spRockerBG->setVisible(false);
	spRockerBG->setPosition(position);
	addChild(spRockerBG, 0, tag_rockerBG);
	//添加摇杆上方那个可移动的图  
	Sprite* spRocker = Sprite::create(rockerImageName);
	spRocker->setVisible(false);
	spRocker->setPosition(position);
	addChild(spRocker, 1, tag_rocker);
	spRocker->setOpacity(180);
	//摇杆背景图坐标  
	rockerBGPosition = position;
	//摇杆背景图半径  
	rockerBGR = spRockerBG->getContentSize().width*0.5;
	listener = EventListenerTouchOneByOne::create();
	listenerKeyBoard = EventListenerKeyboard::create();
	// 绑定监听事件
	listener->onTouchBegan = CC_CALLBACK_2(HRocker::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HRocker::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HRocker::onTouchEnded, this);

	listenerKeyBoard->onKeyPressed = CC_CALLBACK_2(HRocker::onPressKey, this);
	listenerKeyBoard->onKeyReleased = CC_CALLBACK_2(HRocker::onReleaseKey, this);

	listener->setSwallowTouches(true);
}
//启动摇杆  
void HRocker::startRocker(bool _isStopOther)
{
	//精灵设置可见，设置监听  
	Sprite* rocker = (Sprite*)getChildByTag(tag_rocker);
	rocker->setVisible(true);
	Sprite* rockerBG = (Sprite*)getChildByTag(tag_rockerBG);
	rockerBG->setVisible(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKeyBoard, this);
}
//停止摇杆  
void HRocker::stopRocker()
{
	//精灵设置不可见，取消监听  
	Sprite* rocker = (Sprite*)getChildByTag(tag_rocker);
	rocker->setVisible(false);
	Sprite* rockerBG = (Sprite*)getChildByTag(tag_rockerBG);
	rockerBG->setVisible(false);
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
}
//得到两坐标的角度值  
float HRocker::getRad(Point pos1, Point pos2)
{
	//得到两点的坐标x,y坐标值  
	float px1 = pos1.x;
	float py1 = pos1.y;
	float px2 = pos2.x;
	float py2 = pos2.y;
	//求出两边长度  
	float x = px2 - px1;
	float y = py1 - py2;
	//开方   与  几次方 公式  
	float xie = sqrt(pow(x, 2) + pow(y, 2));
	float cos = x / xie;
	//反余弦定理，知道两边长求角度：cos = 邻边/斜边  
	float rad = acos(cos);
	//当触屏Y坐标 <摇杆的Y坐标时，取反值  
	if (py1 > py2)
	{
		rad = -rad;
	}
	return rad;
}
//得到与角度对应的半径为r的圆上一坐标点  
Point HRocker::getAnglePosition(float r, float angle)
{
	return ccp(r*cos(angle), r*sin(angle));
}
bool HRocker::onTouchBegan(Touch *touch, Event *event)
{

	Sprite* sp = (Sprite*)getChildByTag(tag_rocker);
	//得到触屏点坐标  
	Point point = touch->getLocation();
	//判断是否点击到sp这个精灵：boundingBox()精灵大小之内的所有坐标  
	if (sp->boundingBox().containsPoint(point))
	{
		isCanMove = true;
	}
	return true;
}
void HRocker::onTouchMoved(Touch *touch, Event *event)
{
	if (!isCanMove)
	{
		return;
	}
	Sprite* sp = (Sprite*)getChildByTag(tag_rocker);
	Point point = touch->getLocation();
	//判断两个圆心的距离是否大于摇杆背景的半径  
	if (sqrt(pow(point.x - rockerBGPosition.x, 2) + pow(point.y - rockerBGPosition.y, 2)) >= rockerBGR)
	{
		//得到触点与摇杆背景圆心形成的角度  
		float angle = getRad(rockerBGPosition, point);
		//确保小圆运动范围在背景圆内  
		sp->setPosition(ccpAdd(getAnglePosition(rockerBGR, angle), ccp(rockerBGPosition.x, rockerBGPosition.y)));
	}
	else
	{
		//触点在背景圆内则跟随触点运动  
		sp->setPosition(point);
	}
}
void HRocker::onTouchEnded(Touch *touch, Event *event)
{
	if (!isCanMove)
	{
		return;
	}
	Sprite* rocker = (Sprite*)getChildByTag(tag_rocker);
	Sprite* rockerBG = (Sprite*)getChildByTag(tag_rockerBG);
	rocker->stopAllActions();
	rocker->runAction(MoveTo::create(0.08, rockerBG->getPosition()));

	isCanMove = false;
}
bool HRocker::onPressKey(EventKeyboard::KeyCode keyCode, Event * envet)
{
	updateState(keyCode, PRESS);
	Sprite* rocker = (Sprite*)getChildByTag(tag_rocker);
	Sprite* rockerBG = (Sprite*)getChildByTag(tag_rockerBG);
	rocker->stopAllActions();
	isCanMove = true;
	Point point;
	switch (_direction)
	{
	case Direction::down:
	{
		point = Point(rockerBGPosition.x, rockerBGPosition.y-rockerBGR);
		break;
	}
	case Direction::down_left:
	{
		point = Point(rockerBGPosition.x- rockerBGR, rockerBGPosition.y- rockerBGR);
		break;
	}
	case Direction::down_right:
	{
		point = Point(rockerBGPosition.x+ rockerBGR, rockerBGPosition.y- rockerBGR);
		break;
	}
	case Direction::up:
	{
		point = Point(rockerBGPosition.x, rockerBGPosition.y+ rockerBGR);
		break;
	}
	case Direction::up_left:
	{
		point = Point(rockerBGPosition.x- rockerBGR, rockerBGPosition.y+ rockerBGR);
		break;
	}
	case Direction::up_right:
	{
		point = Point(rockerBGPosition.x+ rockerBGR, rockerBGPosition.y+ rockerBGR);
		break;
	}
	case Direction::left:
	{
		point = Point(rockerBGPosition.x- rockerBGR, rockerBGPosition.y);
		break;
	}
	case Direction::right:
	{
		point = Point(rockerBGPosition.x+ rockerBGR, rockerBGPosition.y);
		break;
	}
	case Direction::noDir:
	{
		point = Point(rockerBGPosition.x, rockerBGPosition.y);
		break;
	}
	default:
	{
		point = Point(rockerBGPosition.x, rockerBGPosition.y);
		break;
	}
	}
	float angle = getRad(rockerBGPosition, point);
	rocker->setPosition(ccpAdd(getAnglePosition(rockerBGR, angle), ccp(rockerBGPosition.x, rockerBGPosition.y)));
	return true;
}

bool HRocker::onReleaseKey(EventKeyboard::KeyCode keyCode, Event * envet)
{
	updateState(keyCode, RELEASE);
	if (!isCanMove)
	{
		return true;
	}
	if (_direction == noDir)
	{
		Sprite* rocker = (Sprite*)getChildByTag(tag_rocker);
		Sprite* rockerBG = (Sprite*)getChildByTag(tag_rockerBG);
		rocker->stopAllActions();
		rocker->runAction(MoveTo::create(0.08, rockerBG->getPosition()));
	}
	else
	{
		Sprite* rocker = (Sprite*)getChildByTag(tag_rocker);
		Sprite* rockerBG = (Sprite*)getChildByTag(tag_rockerBG);
		rocker->stopAllActions();
		isCanMove = true;
		Point point;
		switch (_direction)
		{
		case Direction::down:
		{
			point = Point(rockerBGPosition.x, rockerBGPosition.y - rockerBGR);
			break;
		}
		case Direction::down_left:
		{
			point = Point(rockerBGPosition.x - rockerBGR, rockerBGPosition.y - rockerBGR);
			break;
		}
		case Direction::down_right:
		{
			point = Point(rockerBGPosition.x + rockerBGR, rockerBGPosition.y - rockerBGR);
			break;
		}
		case Direction::up:
		{
			point = Point(rockerBGPosition.x, rockerBGPosition.y + rockerBGR);
			break;
		}
		case Direction::up_left:
		{
			point = Point(rockerBGPosition.x - rockerBGR, rockerBGPosition.y + rockerBGR);
			break;
		}
		case Direction::up_right:
		{
			point = Point(rockerBGPosition.x + rockerBGR, rockerBGPosition.y + rockerBGR);
			break;
		}
		case Direction::left:
		{
			point = Point(rockerBGPosition.x - rockerBGR, rockerBGPosition.y);
			break;
		}
		case Direction::right:
		{
			point = Point(rockerBGPosition.x + rockerBGR, rockerBGPosition.y);
			break;
		}
		case Direction::noDir:
		{
			point = Point(rockerBGPosition.x, rockerBGPosition.y);
			break;
		}
		default:
		{
			point = Point(rockerBGPosition.x, rockerBGPosition.y);
			break;
		}
		}
		float angle = getRad(rockerBGPosition, point);
		rocker->setPosition(ccpAdd(getAnglePosition(rockerBGR, angle), ccp(rockerBGPosition.x, rockerBGPosition.y)));
	}

	return true;
}

bool HRocker::updateState(EventKeyboard::KeyCode keyCode, int type)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
	{
		if (type == PRESS)
		{
			_wState = true;
			_sState = false;
		}
		else if (type == RELEASE)
		{
			_wState = false;
		}
		break;
	}
	case EventKeyboard::KeyCode::KEY_A:
	{
		if (type == PRESS)
		{
			_aState = true;
			_dState = false;
		}
		else if (type == RELEASE)
		{
			_aState = false;
		}
		break;
	}
	case EventKeyboard::KeyCode::KEY_S:
	{
		if (type == PRESS)
		{
			_sState = true;
			_wState = false;
		}
		else if (type == RELEASE)
		{
			_sState = false;
		}
		break;
	}
	case EventKeyboard::KeyCode::KEY_D:
	{
		if (type == PRESS)
		{
			_dState = true;
			_aState = false;
		}
		else if (type == RELEASE)
		{
			_dState = false;
		}
		break;
	}
	default:
		break;
	}
	updateDirection();
	return true;
}

bool HRocker::updateDirection()
{
	if (_wState)
	{
		if (_aState)
		{
			_direction = Direction::up_left;
		}
		else if (_dState)
		{
			_direction = Direction::up_right;
		}
		else
		{
			_direction = Direction::up;
		}
	}
	else if(_sState)
	{
		if (_aState)
		{
			_direction = Direction::down_left;
		}
		else if (_dState)
		{
			_direction = Direction::down_right;
		}
		else
		{
			_direction = Direction::down;
		}
	}
	else
	{
		if (_aState)
		{
			_direction = Direction::left;
		}
		else if (_dState)
		{
			_direction = Direction::right;
		}
		else
		{
			_direction = Direction::noDir;
		}
	}
	return true;
}
