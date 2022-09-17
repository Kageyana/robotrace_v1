#ifndef TIMER_H_
#define TIMER_H_

//====================================//
// インクルード
//====================================//
#include "main.h"
#include "io.h"
#include "adc.h"
#include "AQM0802A.h"
//====================================//
// シンボル定義
//====================================//

//====================================//
// グローバル変数の宣言
//====================================//
extern uint32_t 	cnt1;

extern uint32_t     encR;
extern uint32_t     encL;
extern uint32_t     encTotalR;
extern uint32_t     encTotalL;

//====================================//
// プロトタイプ宣言
//====================================//
void ledOut( uint8_t rgb );
void getEncoder(void);
void motorPwmOut(int16_t pwmL, int16_t pwmR);

#endif // TIMER_H_
