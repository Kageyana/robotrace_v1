#ifndef MOTOR_H_
#define MOTOR_H_

//====================================//
// インクルード
//====================================//
#include "main.h"
//====================================//
// シンボル定義
//====================================//
#define MOTOR_COUNTERPERIOD htim1.Init.Period
//====================================//
// グローバル変数の宣言
//====================================//
extern int16_t motorpwmL;
extern int16_t motorpwmR;
//====================================//
// プロトタイプ宣言
//====================================//
void motorPwmOut(int16_t pwmL, int16_t pwmR);
void motorPwmOutSynth(int16_t tPwm, int16_t sPwm, int16_t yrPwm, int16_t yPwm);

#endif // MOTOR_H_
