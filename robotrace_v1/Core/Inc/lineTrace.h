#ifndef LINETRACE_H_
#define LINETRACE_H_
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
#define KP1		19
#define KI1		0
#define KD1		71

#define KP1CURVE		8
#define KI1CURVE		0
#define KD1CURVE		40

//====================================//
// グローバル変数の宣言
//====================================//
extern int16_t 	tracePwm;	// 白線トレースサーボPWM
extern uint8_t  kp1_buff, ki1_buff, kd1_buff;
extern uint8_t	kp1Curve_buff, ki1Curve_buff, kd1Curve_buff;
extern uint8_t	modeCalLinesensors;
//====================================//
// プロトタイプ宣言
//====================================//
void motorControlTrace( void );

#endif // LINETRACE_H_
