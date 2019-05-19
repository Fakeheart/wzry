#pragma once
#ifndef _Constant_H_
#define _Constant_H_

#define ORIGIN_ATTACK 100
#define ORIGIN_DEFENSE 10
#define ORIGIN_RADIUS 500
#define ORIGIN_INTERVAL 0.2
#define PRESS 1
#define RELEASE 2
#define MAPTAG 100
#define MAPWIDTH 3200
#define MAPHEIGHT 955

#define PI 3.1415926


//飞行物速度
#define SPEED_FLY 10

//TAG部分
//技能精灵TAG>=10000
#define TAG_JINENG 10000
#define TAG_DAJI_1 14010
#define TAG_DAJI_2 14020
#define TAG_DAJI_3 14030

#define TAG_HOUYI_1 15010
#define TAG_HOUYI_2 15020
#define TAG_HOUYI_3 15030

#define TAG_YASE_1 16010
#define TAG_YASE_2 16020
#define TAG_YASE_3 16030

//英雄TAG>=5000&&<10000
#define TAG_HERO 5000
#define TAG_HOUYI 5001
#define TAG_YASE 6000
#define TAG_DAJI 7000

//移动物TAG>=2000&&<5000
#define TAG_MOVINGACTOR 2000
#define TAG_JIN 2001
#define TAG_ZHONG 2010
#define TAG_YUAN 2020
#define TAG_CHE 2030

//静止物TAG>=1000&&<2000
#define TAG_ACTOR 1000
#define TAG__TA 1001
#define TAG_JIA 1010

enum ECamp{
	BLUE, RED, MID
};


#endif // !Constant_H
