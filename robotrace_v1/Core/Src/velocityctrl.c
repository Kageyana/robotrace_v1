//====================================//
// インクルード
//====================================//
#include "velocityctrl.h"
//====================================//
// グローバル変数の宣言
//====================================//
int16_t 	speedPwm;			// モーター制御PWM
double		targetSpeed;		// 目標速度
int16_t		encoderBefore;		// 1ms前の速度
double 		targetSpeedBefore;	// 1ms前の目標速度	
double 		Int2;				// I成分積算値(速度制御)
bool 		demo;
uint8_t		kp2_buff = KP2, ki2_buff = KI2, kd2_buff = KD2;

///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlSpeed
// 処理概要     モーターのPWM計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlSpeed( void ) {
	int32_t i, j, iRet, Dif, iP, iI, iD, Dev;
	int8_t kp2, ki2, kd2;
	
	i = (int32_t)targetSpeed;		// 目標値
	j = encCurrentN;				// 現在値 targetSpeedはエンコーダのパルス数*10のため

	// デモモードのときゲイン変更
	if ( demo ) {
		kp2 = kp2_buff;
		ki2 = ki2_buff;
		kd2 = kd2_buff;
	} else {
		kp2 = kp2_buff;
		ki2 = ki2_buff;
		kd2 = kd2_buff;
	}
	
	// 駆動モーター用PWM値計算
	Dev = i - j;	// 偏差
	// 目標値を変更したらI成分リセット
	if ( i != targetSpeedBefore ) Int2 = 0;
	
	Int2 += (double)Dev * 0.001;	// 時間積分
	Dif = Dev - encoderBefore;		// 微分　dゲイン1/1000倍
	
	iP = (int32_t)kp2 * Dev;		// 比例
	iI = (double)ki2 * Int2;		// 積分
	iD = (int32_t)kd2 * Dif;		// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 1;
	
	// PWMの上限の設定
	if ( iRet >  1000 ) iRet = 1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	speedPwm = iRet;
	encoderBefore = Dev;
	targetSpeedBefore = i;
}
