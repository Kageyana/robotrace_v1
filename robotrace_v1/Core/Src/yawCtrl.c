//====================================//
// インクルード
//====================================//
#include "yawCtrl.h"
//====================================//
// グローバル変数の宣言
//====================================//
float	 	targetAngle;
int32_t		yawPwm;		// 白線トレースサーボPWM
int32_t 	angleBefore;	// 1ms前のセンサ値
float		Int4;			// I成分積算値(白線トレース)
uint8_t		kp4_buff = KP4, ki4_buff = KI4, kd4_buff = KD4;

///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlYaw
// 処理概要     角速度制御時のPWMの計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlYaw(void) {
	float iP, iD, iI;
	int32_t Dev, Dif, kp, ki, kd, iRet;
	
	//サーボモータ用PWM値計算
	kp = kp4_buff;
	ki = ki4_buff;
	kd = kd4_buff;
	Dev = (angle[INDEX_Z] - targetAngle) * 10;	// 目標値-現在値
	// I成分積算
	Int4 += (float)Dev * 0.005;
	// if ( Int3 > 5000 ) Int3 = 5000;		// I成分リミット
	// else if ( Int3 < -5000 ) Int3 = -5000;s
	Dif = ( Dev - angleBefore ) * 1;	// dゲイン1/1000倍

	iP = (float)kp * Dev;	// 比例
	iI = (float)ki * Int4;	// 積分
	iD = (float)kd * Dif;	// 微分
	iRet = (int32_t)iP + iI + iD;
	iRet = iRet >> 2;				// PWMを0～100近傍に収める

	// PWMの上限の設定
	if ( iRet >  1000 ) iRet =  1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	yawPwm = iRet;
	angleBefore = Dev;				// 次回はこの値が1ms前の値となる
}
