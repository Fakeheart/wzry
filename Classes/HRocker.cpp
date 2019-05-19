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
	//���ҡ�˱���ͼ  
	Sprite* spRockerBG = Sprite::create(rockerBGImageName);
	spRockerBG->setVisible(false);
	spRockerBG->setPosition(position);
	addChild(spRockerBG, 0, tag_rockerBG);
	//���ҡ���Ϸ��Ǹ����ƶ���ͼ  
	Sprite* spRocker = Sprite::create(rockerImageName);
	spRocker->setVisible(false);
	spRocker->setPosition(position);
	addChild(spRocker, 1, tag_rocker);
	spRocker->setOpacity(180);
	//ҡ�˱���ͼ����  
	rockerBGPosition = position;
	//ҡ�˱���ͼ�뾶  
	rockerBGR = spRockerBG->getContentSize().width*0.5;
	listener = EventListenerTouchOneByOne::create();
	listenerKeyBoard = EventListenerKeyboard::create();
	// �󶨼����¼�
	listener->onTouchBegan = CC_CALLBACK_2(HRocker::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HRocker::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HRocker::onTouchEnded, this);

	listenerKeyBoard->onKeyPressed = CC_CALLBACK_2(HRocker::onPressKey, this);
	listenerKeyBoard->onKeyReleased = CC_CALLBACK_2(HRocker::onReleaseKey, this);

	listener->setSwallowTouches(true);
}
//����ҡ��  
void HRocker::startRocker(bool _isStopOther)
{
	//�������ÿɼ������ü���  
	Sprite* rocker = (Sprite*)getChildByTag(tag_rocker);
	rocker->setVisible(true);
	Sprite* rockerBG = (Sprite*)getChildByTag(tag_rockerBG);
	rockerBG->setVisible(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKeyBoard, this);
}
//ֹͣҡ��  
void HRocker::stopRocker()
{
	//�������ò��ɼ���ȡ������  
	Sprite* rocker = (Sprite*)getChildByTag(tag_rocker);
	rocker->setVisible(false);
	Sprite* rockerBG = (Sprite*)getChildByTag(tag_rockerBG);
	rockerBG->setVisible(false);
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
}
//�õ�������ĽǶ�ֵ  
float HRocker::getRad(Point pos1, Point pos2)
{
	//�õ����������x,y����ֵ  
	float px1 = pos1.x;
	float py1 = pos1.y;
	float px2 = pos2.x;
	float py2 = pos2.y;
	//������߳���  
	float x = px2 - px1;
	float y = py1 - py2;
	//����   ��  ���η� ��ʽ  
	float xie = sqrt(pow(x, 2) + pow(y, 2));
	float cos = x / xie;
	//�����Ҷ���֪�����߳���Ƕȣ�cos = �ڱ�/б��  
	float rad = acos(cos);
	//������Y���� <ҡ�˵�Y����ʱ��ȡ��ֵ  
	if (py1 > py2)
	{
		rad = -rad;
	}
	return rad;
}
//�õ���Ƕȶ�Ӧ�İ뾶Ϊr��Բ��һ�����  
Point HRocker::getAnglePosition(float r, float angle)
{
	return ccp(r*cos(angle), r*sin(angle));
}
bool HRocker::onTouchBegan(Touch *touch, Event *event)
{

	Sprite* sp = (Sprite*)getChildByTag(tag_rocker);
	//�õ�����������  
	Point point = touch->getLocation();
	//�ж��Ƿ�����sp������飺boundingBox()�����С֮�ڵ���������  
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
	//�ж�����Բ�ĵľ����Ƿ����ҡ�˱����İ뾶  
	if (sqrt(pow(point.x - rockerBGPosition.x, 2) + pow(point.y - rockerBGPosition.y, 2)) >= rockerBGR)
	{
		//�õ�������ҡ�˱���Բ���γɵĽǶ�  
		float angle = getRad(rockerBGPosition, point);
		//ȷ��СԲ�˶���Χ�ڱ���Բ��  
		sp->setPosition(ccpAdd(getAnglePosition(rockerBGR, angle), ccp(rockerBGPosition.x, rockerBGPosition.y)));
	}
	else
	{
		//�����ڱ���Բ������津���˶�  
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
