//====================================//
// インクルード
//====================================//
#include "yawRateCtrl.h"
//====================================//
// グローバル変数の宣言
//====================================//
float	 	targetAngularVelocity;
int32_t		yawRatePwm;				// 制御量
int32_t 	angularVelocityBefore;	// 1ms前のセンサ値
float		Int3;					// I成分積算値(白線トレース)
uint8_t		kp3_buff = KP3, ki3_buff = KI3, kd3_buff = KD3;

///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlYaw
// 処理概要     角速度制御時のPWMの計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlYawRate(void) {
	float iP, iD, iI;
	int32_t Dev, Dif, kp, ki, kd, iRet;
	
	//サーボモータ用PWM値計算
	kp = kp3_buff;
	ki = ki3_buff;
	kd = kd3_buff;
	Dev = angularVelocity[INDEX_Z] - targetAngularVelocity;	// 目標値-現在値
	// I成分積算
	Int3 += (float)Dev * 0.005;
	// if ( Int3 > 5000 ) Int3 = 5000;		// I成分リミット
	// else if ( Int3 < -5000 ) Int3 = -5000;s
	Dif = ( Dev - angularVelocityBefore ) * 1;	// dゲイン1/1000倍

	iP = (float)kp * Dev;	// 比例
	iI = (float)ki * Int3;	// 積分
	iD = (float)kd * Dif;	// 微分
	iRet = (int32_t)iP + iI + iD;
	iRet = iRet >> 2;				// PWMを0～100近傍に収める

	// PWMの上限の設定
	if ( iRet >  1000 ) iRet =  1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	yawRatePwm = iRet;
	angularVelocityBefore = Dev;				// 次回はこの値が1ms前の値となる
}
