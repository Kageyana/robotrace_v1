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
#define KP1		5
#define KI1		0
#define KD1		10

#define KP1CURVE		3
#define KI1CURVE		0
#define KD1CURVE		14

//====================================//
// グローバル変数の宣言
//====================================//
extern int16_t 	tracePwm;	// 白線トレースサーボPWM
extern double	Int;		// I成分積算値(白線トレース)
extern uint8_t  kp1_buff, ki1_buff, kd1_buff;
extern uint8_t	kp1Curve_buff, ki1Curve_buff, kd1Curve_buff;

//====================================//
// プロトタイプ宣言
//====================================//
void motorControltrace( void );

#endif // LINETRACE_H_