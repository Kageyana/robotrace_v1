#ifndef VELOCITYCTRL_H_
#define VELOCITYCTRL_H_
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
#define KP2		20
#define KI2		4
#define KD2		0

//====================================//
// グローバル変数の宣言
//====================================//
extern bool     demo;
extern int16_t  speedPwm;	    // モーター制御PWM
extern float	targetSpeed;	// 目標速度
extern uint8_t  kp2_buff, ki2_buff, kd2_buff;

//====================================//
// プロトタイプ宣言
//====================================//
void motorControlspeed( void );

#endif // VELOCITYCTRL_H_
