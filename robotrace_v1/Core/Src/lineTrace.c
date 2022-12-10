//====================================//
// インクルード
//====================================//
#include "lineTrace.h"
//====================================//
// グローバル変数の宣言
//====================================//
int16_t		tracePwm;		// 白線トレースサーボPWM
float 		traceBefore;	// 1ms前のセンサ値
float		Int;			// I成分積算値(白線トレース)
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
	float Dev, Dif, kp, ki, kd;
	
	//サーボモータ用PWM値計算
	// if (angleSensor > (float)paramAngle[INDEX_ANGLE_CURVE]) {
	// 	kp = kp1Curve_buff;
	// 	ki = ki1Curve_buff;
	// 	kd = kd1Curve_buff;
	// 	Dev = lSensorf[5] - lSensorf[8];
	// } else if (angleSensor < -(float)paramAngle[INDEX_ANGLE_CURVE]) {
	// 	kp = kp1Curve_buff;
	// 	ki = ki1Curve_buff;
	// 	kd = kd1Curve_buff;
	// 	Dev = lSensorf[3] - lSensorf[6];
	// } else {
		// kp = kp1_buff;
		// ki = ki1_buff;
		// kd = kd1_buff;
	// 	Dev = lSensorf[4] - lSensorf[7];
	// }
	Dev = ((lSensor[3]*0.2) + (lSensor[4]*0.8) + (lSensor[5])) - ((lSensor[6]) + (lSensor[7]*0.8) + (lSensor[8]*0.2));
	kp = kp1_buff;
	ki = ki1_buff;
	kd = kd1_buff;
	// Dev = angleSensor * 10;
	// Dev = Dev * 3000.0f;
	// I成分積算
	Int += (float)Dev * 0.001;
	if ( Int > 10000 ) Int = 10000;		// I成分リミット
	else if ( Int < -10000 ) Int = -10000;
	Dif = ( Dev - traceBefore ) * 1;	// dゲイン1/1000倍

	iP = kp * Dev;	// 比例
	iI = ki * Int;	// 積分
	iD = kd * Dif;	// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 6;				// PWMを0～1000近傍に収める

	// PWMの上限の設定
	if ( iRet >  1000 ) iRet =  1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	tracePwm = iRet;
	traceBefore = Dev;				// 次回はこの値が1ms前の値となる
}
