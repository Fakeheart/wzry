#ifndef __HROCKER_H__
#define __HROCKER_H__
#include "cocos2d.h"  
#include <functional>
using namespace cocos2d;
//ö���ͣ����ڱ�ʶҡ����ҡ�˵ı���  
typedef enum{
	tag_rocker,
	tag_rockerBG,
}tagForHRocker;
typedef enum {
	up, down, left, right, up_right, down_right, up_left, down_left,noDir
}Direction;
class HRocker : public Layer
{
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
	//�Ƿ�ɲ���ҡ��  
	bool isCanMove;
	//�õ�ҡ�����û�������ĽǶ�  
	float getRad(Point pos1, Point pos2);
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
	Direction _direction;
	//�����¼�
	virtual bool onPressKey(EventKeyboard::KeyCode keyCode, Event*envet);
	virtual bool onReleaseKey(EventKeyboard::KeyCode keyCode, Event*envet);
	virtual bool updateState(EventKeyboard::KeyCode keyCode,int type);
	virtual bool updateDirection();
	CREATE_FUNC(HRocker);
};

#endif  