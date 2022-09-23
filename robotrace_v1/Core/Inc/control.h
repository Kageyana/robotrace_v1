#ifndef LINECHASE_H_
#define LINECHASE_H_
//====================================//
// インクルード
//====================================//
#include "main.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
//====================================//
// シンボル定義
//====================================//
// 機体諸元
#define WHELLBASE           149
#define TREAD               143
#define SENSORBAR           307
#define MAXDEG              42
#define PALSE_METER         60074
#define PALSE_MILLIMETER    60.07F

#define M_PI                3.141592
// 緊急停止
#define	STOPPING_METER		3		// 停止距離

// 各セクションでの目標速度　x/10[m/s]

// PIDゲイン関連
//白線トレース
#define KP1		7
#define KI1		0
#define KD1		14

// 速度制御
#define KP2		10
#define KI2		4
#define KD2		0

// 緊急停止関連
#define STOP_SENSOR1	60		// センサ全灯
#define STOP_SENSOR2	800		// センサ全消灯
#define STOP_ENCODER	100		// エンコーダ停止(ひっくり返った？)
#define STOP_GYRO		100		// マイナスの加速度検知(コースから落ちた？)
#define STOP_COUNT		1000	// 時間停止
//====================================//
// グローバル変数の宣言
//====================================//
// パターン、モード関連
extern uint8_t  pattern;		// パターン番号
extern uint8_t  modeLCD;		// LCD表示選択
extern uint8_t  modeSlope;		// 坂チェック		0:上り坂始め	1:上り坂終わり	2:下り坂始め	3:下り坂終わり
extern uint8_t	modeAngle;		// サーボPWM変更	0:白線トレース	1:角度制御
extern uint8_t	modePushcart;	// 手押しモード可否	0:自動走行	1:手押し
extern uint8_t	msdset;			// MicroSDが初期化されたか	0:初期化失敗	1:初期化成功
extern uint8_t	IMUSet;			// IMUが初期化されたか	0: 初期化失敗	1:初期化成功

// パラメータ関連
// 距離
extern int16_t	stopping_meter;			    // 停止距離
// タイマ関連

// デモ関連
extern bool demo;

// トレース制御
extern int16_t 	tracePwm;	// 白線トレースサーボPWM
extern double	Int;		// I成分積算値(白線トレース)

// 速度制御関連
extern int16_t  speedPwm;	    // モーター制御PWM
extern double	targetSpeed;	// 目標速度

// ゲイン関連
extern uint8_t  kp1_buff, ki1_buff, kd1_buff;
extern uint8_t  kp2_buff, ki2_buff, kd2_buff;
//====================================//
// プロトタイプ宣言
//====================================//
// マーカー関連
bool checkCrossLine( void );
bool checkRightLine( void );
bool checkLeftLine( void );

// エンコーダ関連
uint32_t encMM( short mm );

// 速度制御関連
void motorControlspeed( void );

// トレース制御関連
void motorControltrace( void );
void motorPwmOutSynth(int16_t tPwm, int16_t sPwm);

#endif // LINECHASE_H_