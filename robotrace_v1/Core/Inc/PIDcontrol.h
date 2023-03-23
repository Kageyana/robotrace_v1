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
#define KP1		20
#define KI1		0
#define KD1		80

#define KP2		20
#define KI2		40
#define KD2		0

#define KP3		4
#define KI3		55
#define KD3		3

#define KP4		1
#define KI4		0
#define KD4		0

typedef struct {
    int16_t kp;
    int16_t ki;
    int16_t kd;
    int16_t pwm;
} pidParam;

//====================================//
// グローバル変数の宣言
//====================================//
extern int16_t	targetSpeed;	// 目標速度
extern float 	targetAngle;
extern float    targetAngularVelocity;

extern pidParam lineTraceCtrl;
extern pidParam veloCtrl;
extern pidParam yawRateCtrl;
extern pidParam yawCtrl;
//====================================//
// プロトタイプ宣言
//====================================//
void motorControlTrace( void );
void motorControlSpeed( void );
void motorControlYawRate( void );
void motorControlYaw( void );

#endif // LINETRACE_H_
