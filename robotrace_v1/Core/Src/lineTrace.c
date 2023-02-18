//====================================//
// インクルード
//====================================//
#include "lineTrace.h"
//====================================//
// グローバル変数の宣言
//====================================//
int16_t		tracePwm;		// 白線トレースサーボPWM
uint8_t		kp1_buff = KP1, ki1_buff = KI1, kd1_buff = KD1;
uint8_t		kp1Curve_buff = KP1CURVE, ki1Curve_buff = KI1CURVE, kd1Curve_buff = KD1CURVE;

///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlTrace
// 処理概要     ライントレース時サーボのPWMの計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlTrace( void ) {
	int32_t 		iP, iD, iI, iRet, Dev, Dif, kp, ki, kd;
	static float 	Int;
	static int32_t 	traceBefore;
	
	//サーボモータ用PWM値計算
	// if (modeCurve == 1 ) {
	// 	Dev = ((lSensor[4]*0.2) + (lSensor[5]*0.8) + (lSensor[6])) - ((lSensor[7]) + (lSensor[8]*0.8) + (lSensor[9]*0.2));
	// } else if (modeCurve == 2 ) {
	// 	Dev = ((lSensor[2]*0.2) + (lSensor[3]*0.8) + (lSensor[4])) - ((lSensor[5]) + (lSensor[6]*0.8) + (lSensor[7]*0.2));
	// } else {
	// 	Dev = ((lSensor[3]*0.2) + (lSensor[4]*0.8) + (lSensor[5])) - ((lSensor[6]) + (lSensor[7]*0.8) + (lSensor[8]*0.2));
	// }
	// Dev = ((lSensor[3]*0.8) + (lSensor[4]) + (lSensor[5]*0.5)) - ((lSensor[6]*0.5) + (lSensor[7]) + (lSensor[8]*0.8));
	Dev = ((lSensor[3]*0.5) + (lSensor[4]*0.8) + (lSensor[5])) - ((lSensor[6]) + (lSensor[7]*0.8) + (lSensor[8]*0.5));
	// Dev = ((lSensor[0]*0.2) + (lSensor[1]*0.3) + (lSensor[2]*0.4) + (lSensor[3]*0.5) + (lSensor[4]) + (lSensor[5]*0.9)) - ((lSensor[6]*0.9) + (lSensor[7]) + (lSensor[8]*0.5) + (lSensor[9]*0.4) + (lSensor[10]*0.3) + (lSensor[11]*0.2));
	kp = kp1_buff;
	ki = ki1_buff;
	kd = kd1_buff;
	// Dev = angleSensor * 10;
	// Dev = Dev * 3000.0f;
	// I成分積算
	Int += (float)Dev * 0.001;
	if ( Int > 10000 ) Int = 10000;		// I成分リミット
	else if ( Int < -10000 ) Int = -10000;
	Dif = ( Dev - traceBefore ) * 2;	// dゲイン1/500倍

	iP = kp * Dev;	// 比例
	iI = ki * Int;	// 積分
	iD = kd * Dif;	// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 6;				// PWMを0～1000近傍に収める

	// PWMの上限の設定
	if ( iRet >  1000 ) iRet = 1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	tracePwm = iRet;
	traceBefore = Dev;				// 次回はこの値が1ms前の値となる
}
