//====================================//
// インクルード
//====================================//
#include "velocityCtrl.h"
//====================================//
// グローバル変数の宣言
//====================================//
int16_t 	speedPwm;			// モーター制御PWM
int16_t		targetSpeed;		// 目標速度
uint8_t		kp2_buff = KP2, ki2_buff = KI2, kd2_buff = KD2;

///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlSpeed
// 処理概要     モーターのPWM計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlSpeed( void ) {
	int32_t 		iP, iI, iD, iRet, Dev, Dif, kp, ki, kd;
	static float 	Int;
	static int16_t 	targetSpeedBefore, encoderBefore;
	
	
	kp = kp2_buff;
	ki = ki2_buff;
	kd = kd2_buff;

	// 駆動モーター用PWM値計算
	Dev = targetSpeed - encCurrentN;	// 偏差
	// 目標値を変更したらI成分リセット
	if ( targetSpeed != targetSpeedBefore ) Int = 0;
	
	Int += (float)(Dev * 0.001);	// 時間積分
	Dif = Dev - encoderBefore;		// 微分　dゲイン1/1000倍
	
	iP = kp * Dev;		// 比例
	iI = ki * Int;		// 積分
	iD = kd * Dif;		// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 1;
	
	// PWMの上限の設定
	if ( iRet >  1000 ) iRet = 1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	speedPwm = iRet;
	encoderBefore = Dev;
	targetSpeedBefore = targetSpeed;
}
