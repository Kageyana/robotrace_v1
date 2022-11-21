//====================================//
// インクルード
//====================================//
#include "yawRatectrl.h"
//====================================//
// グローバル変数の宣言
//====================================//
double 		anglevelocity;
double		anglevelocityBefore;
int16_t		yawPwm;		// 白線トレースサーボPWM
int16_t 	angleZBefore;	// 1ms前のセンサ値
double		Int3;			// I成分積算値(白線トレース)
uint8_t		kp3_buff = KP3, ki3_buff = KI3, kd3_buff = KD3;

///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlYaw
// 処理概要     角速度制御時のPWMの計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlYaw(void) {
	double iP, iD, iI;
	int32_t Dev, Dif, kp, ki, kd, iRet;
	
	//サーボモータ用PWM値計算
	kp = kp3_buff;
	ki = ki3_buff;
	kd = kd3_buff;
	Dev = (gyroValZ - (int32_t)anglevelocity);	// 目標値-現在値
	// I成分積算
	Int3 += (double)Dev * 0.005;
	// if ( Int3 > 5000 ) Int3 = 5000;		// I成分リミット
	// else if ( Int3 < -5000 ) Int3 = -5000;
	Dif = ( Dev - angleZBefore ) * 1;	// dゲイン1/1000倍

	iP = (double)kp * Dev;	// 比例
	iI = (double)ki * Int3;	// 積分
	iD = (double)kd * Dif;	// 微分
	iRet = (int32_t)iP + iI + iD;
	iRet = iRet >> 2;				// PWMを0～100近傍に収める

	// PWMの上限の設定
	if ( iRet >  1000 ) iRet =  1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	yawPwm = iRet;
	angleZBefore = Dev;				// 次回はこの値が1ms前の値となる
	anglevelocityBefore = gyroValZ;
}
