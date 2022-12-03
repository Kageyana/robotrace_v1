#ifndef LINECHASE_H_
#define LINECHASE_H_
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
#define PARAM_STRAIGHT              10
#define PARAM_CURVEBREAK            4
#define PARAM_STOP                  4
#define PARAM_CURVE                 6

#define PARAM_ANGLE_CURVE           17
#define INDEX_ANGLE_CURVE           0

// 緊急停止関連
#define STOP_ANGLE_X          1
#define STOP_ANGLE_Y          2
#define STOP_ENCODER_CURRENT         3

#define STOP_COUNT_SENSOR1	60		// センサ全灯
#define STOP_COUNT_SENSOR2	800		// センサ全消灯
#define STOP_COUNT_ENCODER_CURRENT	100		// エンコーダ停止
#define STOP_COUNT_ANGLE_X	200		// X方向の角度変化
#define STOP_COUNT_ANGLE_Y	200		// Y方向の角度変化
#define STOP_COUNT_TIME		1000	// 時間停止
//====================================//
// グローバル変数の宣言
//====================================//
// パターン、モード関連
extern uint8_t patternTrace;		// パターン番号
extern uint8_t modeLCD;		// LCD表示選択
extern uint8_t modeLOG;     // ログ取得状況
extern uint8_t modeCurve;	// カーブ判断 0:直線 1:カーブ進入

// パラメータ関連
extern uint8_t paramSpeed[10];
extern uint8_t paramAngle[10];

extern uint16_t     analogVal[12];         // ADC結果格納配列

// マーカー関連
extern uint8_t cMarker;

// ログ関連
extern uint16_t     logMarker[10000];
extern uint16_t     logEncoder[10000];

//====================================//
// プロトタイプ宣言
//====================================//
void systemInit (void);
void systemLoop (void);
void emargencyStop (uint8_t modeStop);

#endif // LINECHASE_H_
