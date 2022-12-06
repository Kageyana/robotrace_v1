#ifndef YAWRATECTRL_H_
#define YAWRATECTRL_H_
//====================================//
// インクルード
//====================================//
#include "main.h"
#include <stdint.h>
//====================================//
// シンボル定義
//====================================//
#define KP3		4
#define KI3		110
#define KD3		3

//====================================//
// グローバル変数の宣言
//====================================//
extern float   targetAngularVelocity;
extern int32_t 	yawRatePwm;	// 白線トレースサーボPWM
extern uint8_t  kp3_buff, ki3_buff, kd3_buff;

//====================================//
// プロトタイプ宣言
//====================================//
void motorControlYawRate( void );

#endif // YAWRATECTRL_H_
