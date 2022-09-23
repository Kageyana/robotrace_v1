﻿//====================================//
// インクルード
//====================================//
#include "control.h"
//====================================//
// グローバル変数の宣言
//====================================//
// モード関連
uint8_t		pattern = 0;
uint8_t 	modeLCD = 1;		// LCD表示可否		1:表示		0:消灯		
uint8_t 	modeSlope;			// 坂チェック		0:上り坂始め	1:上り坂終わり	2:下り坂始め
uint8_t 	modeAngle;			// サーボPWM変更	0:白線トレース	1:角度制御
uint8_t		modePushcart;		// 手押しモード可否	0:自動走行	1:手押し
uint8_t		msdset;				// MicroSDが初期化されたか	0:初期化失敗	1:初期化成功
uint8_t		IMUSet = 0;			// IMUが初期化されたか		0: 初期化失敗	1:初期化成功

// パラメータ関連
// 距離
int16_t	stopping_meter;			// 停止距離

// デモ関連
bool 	demo;

// サーボ関連
// 白線トレース
int16_t		tracePwm;		// 白線トレースサーボPWM
int16_t 	traceBefore;	// 1ms前のセンサ値
int16_t		devBefore;		// I成分リセット用
double		Int;			// I成分積算値(白線トレース)

// モーター関連
int16_t 	speedPwm;			// モーター制御PWM
double		targetSpeed;		// 目標速度
int16_t		encoderBefore;		// 1ms前の速度
double 		targetSpeedBefore;	// 1ms前の目標速度	
double 		Int2;				// I成分積算値(速度制御)

// ゲイン関連
uint8_t	kp1_buff = KP1, ki1_buff = KI1, kd1_buff = KD1;
uint8_t	kp2_buff = KP2, ki2_buff = KI2, kd2_buff = KD2;

///////////////////////////////////////////////////////////////////////////
// モジュール名 checkCrossLine
// 処理概要     クロスライン検知
// 引数         なし
// 戻り値       0:クロスラインなし 1:あり
///////////////////////////////////////////////////////////////////////////
bool checkCrossLine( void )
{
	if ( sensor_inp() == 0x7 ) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 checkRightLine
// 処理概要     右ハーフライン検出処理
// 引数         なし
// 戻り値       0:右ハーフラインなし 1:あり
///////////////////////////////////////////////////////////////////////////
bool checkRightLine( void )
{
	if ( sensor_inp() == 0x3 ) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 checkLeftLine
// 処理概要     左ハーフライン検出処理
// 引数         なし
// 戻り値       0:左ハーフラインなし 1:あり
///////////////////////////////////////////////////////////////////////////
bool checkLeftLine( void )
{
	if ( sensor_inp() == 0x6 ) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 encMM
// 処理概要     mmをエンコーダのパルス数に変換して返す
// 引数         mm:変換する長さ[mm]
// 戻り値       変換したパルス数
///////////////////////////////////////////////////////////////////////////
uint32_t encMM( int16_t mm )
{
	return PALSE_MILLIMETER * abs(mm);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlTrace
// 処理概要     ライントレース時サーボのPWMの計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlTrace( void )
{
	int32_t iP, iD, iI, iRet;
	int32_t Dev, Dif;
	
	//サーボモータ用PWM値計算
	// Dev = (lsensor[4]+lsensor[5]) - (lsensor[6]+lsensor[7]);
	Dev = (lsensor[5]) - (lsensor[6]);
	// I成分積算
	Int += (double)Dev * 0.001;
	if ( Int > 10000 ) Int = 10000;		// I成分リミット
	else if ( Int < -10000 ) Int = -10000;
	Dif = ( Dev - traceBefore ) * 1;	// dゲイン1/1000倍

	iP = (int32_t)kp1_buff * Dev;		// 比例
	iI = (double)ki1_buff * Int;	// 積分
	iD = (int32_t)kd1_buff * Dif;		// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 6;				// PWMを0～100近傍に収める

	// PWMの上限の設定
	if ( iRet >  1000 ) iRet =  1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	if ( Dev >= 0 )	devBefore = 0;
	else			devBefore = 1;
	tracePwm = iRet;
	traceBefore = Dev;				// 次回はこの値が1ms前の値となる
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlSpeed
// 処理概要     モーターのPWM計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlSpeed( void )
{
	int32_t i, j, iRet, Dif, iP, iI, iD, Dev;
	int8_t kp2, ki2, kd2;
	
	i = (int32_t)targetSpeed;		// 目標値
	j = encN;			// 現在値 targetSpeedはエンコーダのパルス数*10のため
							// 現在位置も10倍する

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
	
	iP = (int32_t)kp2 * Dev;			// 比例
	iI = (double)ki2 * Int2;		// 積分
	iD = (int32_t)kd2 * Dif;			// 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 1;
	
	// PWMの上限の設定
	if ( iRet >  1000 ) iRet = 1000;
	if ( iRet <  -1000 ) iRet = -1000;
	
	speedPwm = iRet;
	encoderBefore = Dev;
	targetSpeedBefore = i;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorPwmOutSynth
// 処理概要     トレースと速度制御のPID制御量をPWMとしてモータに出力する
// 引数         tPwm: トレースのPID制御量 sPwm: 速度のPID制御量
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorPwmOutSynth(int16_t tPwm, int16_t sPwm) {
	int16_t pwmR, pwmL;

	if (tPwm > 0) {
		pwmR = sPwm - abs(tPwm);
		pwmL = sPwm + abs(tPwm);
	} else {
		pwmR = sPwm + abs(tPwm);
		pwmL = sPwm - abs(tPwm);
	}
	motorPwmOut(pwmL, pwmR);
}
