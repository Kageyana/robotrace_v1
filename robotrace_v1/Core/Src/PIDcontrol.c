//====================================//
// インクルード
//====================================//
#include "PIDcontrol.h"
//====================================//
// グローバル変数の宣言
//====================================//
pidParam 	lineTraceCtrl = { KP1, KI1, KD1, 0, 0};
pidParam 	veloCtrl = { KP2, KI2, KD2, 0, 0};
pidParam 	yawRateCtrl = { KP3, KI3, KD3, 0, 0};
pidParam 	yawCtrl = { KP4, KI4, KD4, 0, 0};

int16_t		targetSpeed;			// 目標速度
float 		targetAngle;			// 目標角速度
float   	targetAngularVelocity;	// 目標角度

///////////////////////////////////////////////////////////////////////////
// モジュール名 setTargetSpeed
// 処理概要     目標速度の設定
// 引数         目標速度の整数倍値
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void setTargetSpeed (float speed) {
	targetSpeed = (int16_t)(speed*PALSE_MILLIMETER);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 setTargetAngularVelocity
// 処理概要     目標角速度の設定
// 引数         目標角速度[rad/s]
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void setTargetAngularVelocity (float angularVelocity) {
	targetAngularVelocity = angularVelocity;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 setTargetAngle
// 処理概要     目標角度の設定
// 引数         目標角度[rad]
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void setTargetAngle (float angle) {
	targetAngle = angle;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlTrace
// 処理概要     ライントレース時の制御量の計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlTrace( void ) {
	int32_t 		iP, iD, iI, iRet, Dev, Dif, senL, senR;
	static int32_t 	traceBefore;
	
	//サーボモータ用PWM値計算
	if (lSensorOffset[0] > 0)	{
		senL = (lSensorCari[5]) + (lSensorCari[4]*0.6) + (lSensorCari[3]*0.2) + (lSensorCari[2]*0.2) + (lSensorCari[1]*0.1) + (lSensorCari[0]*0.2);
		senR = (lSensorCari[6]) + (lSensorCari[7]*0.6) + (lSensorCari[8]*0.2) + (lSensorCari[9]*0.2) + (lSensorCari[10]*0.1) + (lSensorCari[11]*0.2);
	} else {
		senL = (lSensor[3]*0.5) + (lSensor[4]*0.8) + (lSensor[5]);
		senR = (lSensor[8]*0.5) + (lSensor[7]*0.8) + (lSensor[6]);
	}
	Dev = senL - senR;
		
	// I成分積算
	lineTraceCtrl.Int += (float)Dev * 0.001;
	if ( lineTraceCtrl.Int > 10000.0 ) lineTraceCtrl.Int = 10000.0;		// I成分リミット
	else if ( lineTraceCtrl.Int < -10000.0 ) lineTraceCtrl.Int = -10000.0;
	Dif = ( Dev - traceBefore ) * 2;	// dゲイン1/500倍

	iP = lineTraceCtrl.kp * Dev;	// 比例
	iI = lineTraceCtrl.ki * lineTraceCtrl.Int;	// 積分
	iD = lineTraceCtrl.kd * Dif;	// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 6;				// PWMを0～1000近傍に収める

	// PWMの上限の設定
	if ( iRet >  900 ) iRet = 900;
	if ( iRet <  -900 ) iRet = -900;
	
	lineTraceCtrl.pwm = iRet;
	traceBefore = Dev;				// 次回はこの値が1ms前の値となる
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlSpeed
// 処理概要     モーターの制御量の計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlSpeed( void ) {
	int32_t 		iP, iI, iD, iRet, Dev, Dif;
	static int16_t 	targetSpeedBefore, encoderBefore;
	
	// 駆動モーター用PWM値計算
	Dev = targetSpeed - encCurrentN;	// 偏差
	// 目標値を変更したらI成分リセット
	if ( targetSpeed != targetSpeedBefore ) veloCtrl.Int = 0;
	
	veloCtrl.Int += (float)Dev * 0.001;	// 時間積分
	Dif = Dev - encoderBefore;		// 微分　dゲイン1/1000倍
	
	iP = veloCtrl.kp * Dev;		// 比例
	iI = veloCtrl.ki * veloCtrl.Int;		// 積分
	iD = veloCtrl.kd * Dif;		// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 1;
	
	// PWMの上限の設定
	if ( iRet >  900 ) iRet = 900;
	if ( iRet <  -900 ) iRet = -900;
	
	veloCtrl.pwm = iRet;
	encoderBefore = Dev;				// 次回はこの値が1ms前の値となる
	targetSpeedBefore = targetSpeed;	// 前回の目標値を記録
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlYaw
// 処理概要     角速度制御時の制御量の計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlYawRate(void) {
	float 			iP, iD, iI, Dev, Dif;
	static float	angularVelocityBefore;
	static float 	targetAngularVelocityBefore;
	int32_t 		iRet;
	
	Dev = (BNO055val.gyro.z - targetAngularVelocity) * 20;	// 目標値-現在値
	// I成分積算
	yawRateCtrl.Int += Dev * 0.005;
	// 目標値を変更したらI成分リセット
	if ( targetAngularVelocity != targetAngularVelocityBefore ) yawRateCtrl.Int = 0;
	Dif = ( Dev - angularVelocityBefore ) * 2;	// dゲイン1/500倍

	iP = yawRateCtrl.kp * Dev;	// 比例
	iI = yawRateCtrl.ki * yawRateCtrl.Int;	// 積分
	iD = yawRateCtrl.kd * Dif;	// 微分
	iRet = (int32_t)iP + iI + iD;
	iRet = iRet >> 2;				// PWMを0～1000近傍に収める

	// PWMの上限の設定
	if ( iRet >  900 ) iRet =  900;
	if ( iRet <  -900 ) iRet = -900;
	
	yawRateCtrl.pwm = iRet;
	angularVelocityBefore = Dev;	// 次回はこの値が1ms前の値となる
	targetAngularVelocityBefore = targetAngularVelocity;	// 前回の目標値を記録
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlYaw
// 処理概要     角速度制御時の制御量の計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlYaw(void) {
	float 			iP, iD, iI, Dev, Dif;
	static float 	angleBefore;
	static float 	targetAngleBefore;
	int32_t 		iRet;
	
	Dev = (BNO055val.angle.z - targetAngle) * 20;	// 目標値-現在値
	// I成分積算
	yawCtrl.Int += Dev * 0.005;
	// 目標値を変更したらI成分リセット
	if ( targetAngle != targetAngleBefore ) yawCtrl.Int = 0;
	Dif = ( Dev - angleBefore ) * 1;	// dゲイン1/1000倍

	iP = yawCtrl.kp * Dev;	// 比例
	iI = yawCtrl.ki * yawCtrl.Int;	// 積分
	iD = yawCtrl.kd * Dif;	// 微分
	iRet = (int32_t)iP + iI + iD;
	iRet = iRet >> 2;				// PWMを0～1000近傍に収める

	// PWMの上限の設定
	if ( iRet >  900 ) iRet =  900;
	if ( iRet <  -900 ) iRet = -900;
	
	yawCtrl.pwm = iRet;
	angleBefore = Dev;					// 次回はこの値が1ms前の値となる
	targetAngleBefore = targetAngle;	// 前回の目標値を記録
}
