#ifndef YAWCTRL_H_
#define YAWCTRL_H_
//====================================//
// インクルード
//====================================//
#include "main.h"
#include <stdint.h>
//====================================//
// シンボル定義
//====================================//
#define KP4		1
#define KI4		0
#define KD4		0

//====================================//
// グローバル変数の宣言
//====================================//
extern double 	targetAngle;
extern int32_t 	yawPwm;	// 白線トレースサーボPWM
extern uint8_t  kp4_buff, ki4_buff, kd4_buff;

//====================================//
// プロトタイプ宣言
//====================================//
void motorControlYaw( void );

#endif // YAWCTRL_H_
