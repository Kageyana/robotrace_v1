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
#define KP1		9
#define KI1		0
#define KD1		14

//====================================//
// グローバル変数の宣言
//====================================//
extern int16_t 	tracePwm;	// 白線トレースサーボPWM
extern double	Int;		// I成分積算値(白線トレース)
extern uint8_t  kp1_buff, ki1_buff, kd1_buff;
//====================================//
// プロトタイプ宣言
//====================================//
void motorControltrace( void );

#endif // LINETRACE_H_