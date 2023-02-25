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
#define INDEX_BOOST_STRAIGHT        4
#define INDEX_BOOST_1500            5
#define INDEX_BOOST_800             6
#define INDEX_BOOST_600             7
#define INDEX_BOOST_400             8
#define INDEX_BOOST_200             9

// パラメータ
#define PARAM_STRAIGHT              17
#define PARAM_CURVEBREAK            8
#define PARAM_STOP                  8
#define PARAM_CURVE                 13
#define PARAM_BOOST_STRAIGHT        25
#define PARAM_BOOST_1500            22
#define PARAM_BOOST_800             20
#define PARAM_BOOST_600             16
#define PARAM_BOOST_400             14
#define PARAM_BOOST_200             12

#define PARAM_ANGLE_CURVE           17
#define INDEX_ANGLE_CURVE           0

// ゴール
#define COUNT_GOAL              2       // ゴールマーカーを読む回数
//====================================//
// グローバル変数の宣言
//====================================//
// パターン、モード関連
extern uint8_t  patternTrace;	// パターン番号
extern bool     modeLCD;		// LCD表示選択
extern bool     modeLOG;        // ログ取得状況
extern bool     initMSD;        // microSD初期化状況
extern bool     initLCD;        // LCD初期化状況
extern bool     initIMU;        // IMU初期化状況
extern bool     useIMU;         // IMU使用状況
extern bool     initCurrent;    // 電流センサ初期化状況
extern uint8_t  modeCurve;	    // カーブ判断 0:直線 1:カーブ進入
extern int16_t  countdown;

// パラメータ関連
extern uint8_t  paramSpeed[20];
extern uint8_t  paramAngle[10];

extern uint16_t analogVal[12];         // ADC結果格納配列

// マーカー関連
extern uint8_t  courseMarker;
extern uint8_t  beforeCourseMarker;
extern uint32_t cntMarker;

// ログ関連
extern uint16_t logMarker[10000];
extern uint16_t logEncoder[10000];

//====================================//
// プロトタイプ宣言
//====================================//
void initSystem (void);
void loopSystem (void);
void emargencyStop (void);
void countDown (void);
void checkCurve(void);
void setTargetSpeed (uint8_t paramSpeed);

#endif // CONTROL_H_
