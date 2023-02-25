//====================================//
// インクルード
//====================================//
#include "PIDcontrol.h"
//====================================//
// グローバル変数の宣言
//====================================//
pidParam 	lineTraceCtrl = { KP1, KI1, KD1, 0};
pidParam 	veloCtrl = { KP2, KI2, KD2, 0};
pidParam 	yawRateCtrl = { KP3, KI3, KD3, 0};
pidParam 	yawCtrl = { KP4, KI4, KD4, 0};

int16_t		targetSpeed;			// 目標速度
float 		targetAngle;			// 目標角速度
float   	targetAngularVelocity;	// 目標角度
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
	Dev = ((lSensor[3]*0.5) + (lSensor[4]*0.8) + (lSensor[5])) - ((lSensor[6]) + (lSensor[7]*0.8) + (lSensor[8]*0.5));
	// Dev = ((lSensor[0]*0.2) + (lSensor[1]*0.3) + (lSensor[2]*0.4) + (lSensor[3]*0.5) + (lSensor[4]) + (lSensor[5]*0.9)) - ((lSensor[6]*0.9) + (lSensor[7]) + (lSensor[8]*0.5) + (lSensor[9]*0.4) + (lSensor[10]*0.3) + (lSensor[11]*0.2));
	
	// PIDゲイン
	kp = lineTraceCtrl.kp;
	ki = lineTraceCtrl.ki;
	kd = lineTraceCtrl.kd;

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
	
	lineTraceCtrl.pwm = iRet;
	traceBefore = Dev;				// 次回はこの値が1ms前の値となる
}
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
	
	// PIDゲイン
	kp = veloCtrl.kp;
	ki = veloCtrl.ki;
	kd = veloCtrl.kd;

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
	if ( iRet >  900 ) iRet = 900;
	if ( iRet <  -900 ) iRet = -900;
	
	veloCtrl.pwm = iRet;
	encoderBefore = Dev;
	targetSpeedBefore = targetSpeed;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlYaw
// 処理概要     角速度制御時のPWMの計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlYawRate(void) {
	float 			iP, iD, iI, Dev, Dif, kp, ki, kd;
	static float 	Int;
	static float	angularVelocityBefore;
	int32_t 		iRet;
	
	// PIDゲイン
	kp = yawRateCtrl.kp;
	ki = yawRateCtrl.ki;
	kd = yawRateCtrl.kd;

	Dev = angularVelocity[INDEX_Z] - targetAngularVelocity;	// 目標値-現在値
	// I成分積算
	Int += Dev * 0.005;
	// if ( Int3 > 5000 ) Int3 = 5000;		// I成分リミット
	// else if ( Int3 < -5000 ) Int3 = -5000;s
	Dif = ( Dev - angularVelocityBefore ) * 2;	// dゲイン1/500倍

	iP = kp * Dev;	// 比例
	iI = ki * Int;	// 積分
	iD = kd * Dif;	// 微分
	iRet = (int32_t)iP + iI + iD;
	iRet = iRet >> 2;				// PWMを0～100近傍に収める

	// PWMの上限の設定
	if ( iRet >  1000 ) iRet =  1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	yawRateCtrl.pwm = iRet;
	angularVelocityBefore = Dev;				// 次回はこの値が1ms前の値となる
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlYaw
// 処理概要     角速度制御時のPWMの計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlYaw(void) {
	float 			iP, iD, iI, Dev, Dif, kp, ki, kd;
	static float	Int;
	static float 	angleBefore;
	int32_t 		iRet;
	
	// PIDゲイン
	kp = yawCtrl.kp;
	ki = yawCtrl.ki;
	kd = yawCtrl.kd;

	Dev = (angle[INDEX_Z] - targetAngle) * 10;	// 目標値-現在値
	// I成分積算
	Int += Dev * 0.005;
	// if ( Int3 > 5000 ) Int3 = 5000;		// I成分リミット
	// else if ( Int3 < -5000 ) Int3 = -5000;s
	Dif = ( Dev - angleBefore ) * 1;	// dゲイン1/1000倍

	iP = kp * Dev;	// 比例
	iI = ki * Int;	// 積分
	iD = kd * Dif;	// 微分
	iRet = (int32_t)iP + iI + iD;
	iRet = iRet >> 2;				// PWMを0～100近傍に収める

	// PWMの上限の設定
	if ( iRet >  1000 ) iRet =  1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	yawCtrl.pwm = iRet;
	angleBefore = Dev;				// 次回はこの値が1ms前の値となる
}