#ifndef TIMER_H_
#define TIMER_H_

//====================================//
// インクルード
//====================================//
#include "main.h"
#include "io.h"
#include "adc.h"
#include "AQM0802A.h"
#include <stdlib.h>
//====================================//
// シンボル定義
//====================================//

//====================================//
// グローバル変数の宣言
//====================================//
extern uint16_t		cntSW;
extern uint32_t 	cnt1;

extern int16_t      encR;
extern int16_t      encL;
extern int16_t      encN;
extern int32_t     encTotalR;
extern int32_t     encTotalL;
extern int32_t     encTotalN;

//====================================//
// プロトタイプ宣言
//====================================//
void ledOut( uint8_t rgb );
void getEncoder(void);
void motorPwmOut(int16_t pwmL, int16_t pwmR);

#endif // TIMER_H_
