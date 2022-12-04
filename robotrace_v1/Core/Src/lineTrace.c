//====================================//
// インクルード
//====================================//
#include "lineTrace.h"
//====================================//
// グローバル変数の宣言
//====================================//
int16_t		tracePwm;		// 白線トレースサーボPWM
int16_t 	traceBefore;	// 1ms前のセンサ値
double		Int;			// I成分積算値(白線トレース)
uint8_t		kp1_buff = KP1, ki1_buff = KI1, kd1_buff = KD1;
uint8_t		kp1Curve_buff = KP1CURVE, ki1Curve_buff = KI1CURVE, kd1Curve_buff = KD1CURVE;

///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlTrace
// 処理概要     ライントレース時サーボのPWMの計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlTrace( void ) {
	int32_t iP, iD, iI, iRet;
	int32_t Dev, Dif, kp, ki, kd;
	
	//サーボモータ用PWM値計算
	if (angleSensor > paramAngle[INDEX_ANGLE_CURVE]) {
		kp = kp1Curve_buff;
		ki = ki1Curve_buff;
		kd = kd1Curve_buff;
		Dev = lSensor[5] - lSensor[8];
	} else if (angleSensor < -paramAngle[INDEX_ANGLE_CURVE]) {
		kp = kp1Curve_buff;
		ki = ki1Curve_buff;
		kd = kd1Curve_buff;
		Dev = lSensor[3] - lSensor[6];
	} else {
		kp = kp1_buff;
		ki = ki1_buff;
		kd = kd1_buff;
		Dev = lSensor[4] - lSensor[7];
	}
	// kp = kp1_buff;
	// ki = ki1_buff;
	// kd = kd1_buff;
	// Dev = angleSensor * 10;
	// I成分積算
	Int += (double)Dev * 0.001;
	if ( Int > 10000 ) Int = 10000;		// I成分リミット
	else if ( Int < -10000 ) Int = -10000;
	Dif = ( Dev - traceBefore ) * 1;	// dゲイン1/1000倍

	iP = (int32_t)kp1_buff * Dev;	// 比例
	iI = (double)ki1_buff * Int;	// 積分
	iD = (int32_t)kd1_buff * Dif;	// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 6;				// PWMを0～100近傍に収める

	// PWMの上限の設定
	if ( iRet >  1000 ) iRet =  1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	tracePwm = iRet;
	traceBefore = Dev;				// 次回はこの値が1ms前の値となる
}