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
#define PARAM_STRAIGHT              17
#define PARAM_CURVE                 13
#define PARAM_STOP                  8
#define PARAM_BOOST_STRAIGHT        23
#define PARAM_BOOST_1500            20
#define PARAM_BOOST_800             18
#define PARAM_BOOST_600             14
#define PARAM_BOOST_400             14
#define PARAM_BOOST_200             12

// ゴール
#define COUNT_GOAL              2       // ゴールマーカーを読む回数

typedef struct {
    int16_t straight;
    int16_t curve;
    int16_t stop;
    int16_t boostStraight;
    int16_t boost1500;
    int16_t boost800;
    int16_t boost600;
    int16_t boost400;
    int16_t boost200;
} speedParam;
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
// extern uint8_t  paramSpeed[20];
extern speedParam targetParam;

extern uint16_t analogVal[12];         // ADC結果格納配列

// マーカー関連
extern uint8_t  courseMarker;
extern uint8_t  beforeCourseMarker;
extern uint32_t cntMarker;

//====================================//
// プロトタイプ宣言
//====================================//
void initSystem (void);
void loopSystem (void);
void emargencyStop (void);
void countDown (void);
void checkCurve(void);
void setTargetSpeed (uint8_t speed);

#endif // CONTROL_H_
