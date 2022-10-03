﻿#ifndef LINECHASE_H_
#define LINECHASE_H_
//====================================//
// インクルード
//====================================//
#include "main.h"
//====================================//
// シンボル定義
//====================================//
// 機体諸元

#define M_PI                3.141592

// 速度パラメータ関連
// 配列インデックス
#define INDEX_STRAIGHT            0
#define INDEX_CURVEBREAK          1
#define INDEX_STOP                2
#define INDEX_CURVE               3

// パラメータ
#define PARAM_STRAIGHT            6
#define PARAM_CURVE               4
#define PARAM_CURVEBREAK          4
#define PARAM_STOP                4

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
extern uint8_t pattern;		// パターン番号
extern uint8_t modeLCD;		// LCD表示選択
extern uint8_t modeCurve;	// カーブ判断 0:直線 1:カーブ進入

// 速度パラメータ関連
extern uint8_t paramSpeed[10];

extern uint16_t     analogVal[14];         // ADC結果格納配列

// ログ関連
extern uint16_t     logMarker[10000];
extern uint16_t     logEncoder[10000];

//====================================//
// プロトタイプ宣言
//====================================//
void systemInit (void);
void systemLoop (void);

#endif // LINECHASE_H_