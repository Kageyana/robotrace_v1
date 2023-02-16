#ifndef CONTROL_H_
#define CONTROL_H_
//====================================//
// インクルード
//====================================//
#include "main.h"
//====================================//
// シンボル定義
//====================================//
// 機体諸元

// 速度パラメータ関連
// 配列インデックス
#define INDEX_STRAIGHT              0
#define INDEX_CURVEBREAK            1
#define INDEX_STOP                  2
#define INDEX_CURVE                 3

// パラメータ
#define PARAM_STRAIGHT              20
#define PARAM_CURVEBREAK            4
#define PARAM_STOP                  4
#define PARAM_CURVE                 13

#define PARAM_ANGLE_CURVE           17
#define INDEX_ANGLE_CURVE           0

// ゴール
#define COUNT_GOAL              2       // ゴールマーカーを読む回数
// 緊急停止関連
#define STOP_ANGLE_X            1
#define STOP_ANGLE_Y            2
#define STOP_ENCODER_CURRENT    3

#define STOP_COUNT_ENCODER_CURRENT	100		// エンコーダ停止
#define STOP_COUNT_ANGLE_X	100		// X方向の角速度変化
#define STOP_COUNT_ANGLE_Y	100		// Y方向の角速度変化
#define STOP_COUNT_TIME		1000	// 時間停止
//====================================//
// グローバル変数の宣言
//====================================//
// パターン、モード関連
extern uint8_t  patternTrace;	// パターン番号
extern uint8_t  modeLCD;		// LCD表示選択
extern uint8_t  modeLOG;        // ログ取得状況
extern uint8_t  modeCurve;	    // カーブ判断 0:直線 1:カーブ進入
extern int16_t  countdown;

// パラメータ関連
extern uint8_t  paramSpeed[10];
extern uint8_t  paramAngle[10];

extern uint16_t analogVal[12];         // ADC結果格納配列

// マーカー関連
extern uint8_t  cMarker;

// ログ関連
extern uint16_t logMarker[10000];
extern uint16_t logEncoder[10000];

//====================================//
// プロトタイプ宣言
//====================================//
void initSystem (void);
void loopSystem (void);
void emargencyStop (uint8_t modeStop);
void countDown (void);
void checkCurve(void);
void setTargetSpeed (uint8_t paramSpeed);

#endif // CONTROL_H_
