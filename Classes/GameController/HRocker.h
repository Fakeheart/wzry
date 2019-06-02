#ifndef __HROCKER_H__
#define __HROCKER_H__

#include "cocos2d.h" 
#include "Const/Constant.h"
#include <functional>

USING_NS_CC;
//ö���ͣ����ڱ�ʶҡ����ҡ�˵ı���  
#define TAG_ROCKER     0
#define TAG_ROCKERBG   1

class HRocker : public Layer
{
	CC_SYNTHESIZE(bool, isCanMove, IsCanMove);
	CC_SYNTHESIZE(float, _angle, Angle);

public:
	//����ҡ�ˣ�ҡ��ͼƬ��ҡ�˱���ͼƬ����ʼ����  
	static HRocker* createHRocker(const char* rockerImageName, const char* rockerBGImageName, Point position);
	//����ҡ��  
	void startRocker(bool _isStopOther);
	//ֹͣҡ��  
	void stopRocker();
private:
	EventListenerTouchOneByOne* listener;
	EventListenerKeyboard* listenerKeyBoard;
	void rockerInit(const char* rockerImageName, const char* rockerBGImageName, Point position);
	//�õ��뾶Ϊr��Բ���˶���һ���Ƕ�Ӧ��Ӧ��x,y����  
	Point getAnglePosition(float r, float angle);
	//ҡ�˱���������  
	Point rockerBGPosition;
	//ҡ�˱����İ뾶  
	float rockerBGR;
	//�����¼�  
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	//����״̬
	bool _wState, _aState, _sState, _dState;
	EDirection _direction;
	//�����¼�
	virtual bool onPressKey(EventKeyboard::KeyCode keyCode, Event* envet);
	virtual bool onReleaseKey(EventKeyboard::KeyCode keyCode, Event* envet);
	virtual bool updateState(EventKeyboard::KeyCode keyCode, int type);
	virtual bool updateDirection();
	CREATE_FUNC(HRocker);
};

#endif  