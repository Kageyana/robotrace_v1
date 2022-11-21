#ifndef YAWRATECTRL_H_
#define YAWRATECTRL_H_
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
#define KP3		4
#define KI3		110
#define KD3		3

//====================================//
// グローバル変数の宣言
//====================================//
extern double   anglevelocity;
extern int16_t 	yawPwm;	// 白線トレースサーボPWM
extern uint8_t  kp3_buff, ki3_buff, kd3_buff;

//====================================//
// プロトタイプ宣言
//====================================//
void motorControlYaw( void );

#endif // YAWRATECTRL_H_
