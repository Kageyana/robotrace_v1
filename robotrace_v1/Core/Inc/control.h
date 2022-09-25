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

// 速度パラメータ関連
#define STRAIGHT            0
#define CURVEBREAK          1
#define STOP                2

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
extern uint8_t modeCurve;		// カーブ判断 0:直線 1:カーブ進入

// 速度パラメータ関連
extern double parameterSpeed[10];

//====================================//
// プロトタイプ宣言
//====================================//
void systemInit (void);
void systemLoop (void);

#endif // LINECHASE_H_