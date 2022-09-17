//====================================//
// インクルード
//====================================//
#include "timer.h"
//====================================//
// グローバル変数の宣言
//====================================//
uint8_t 	led = 0;
uint32_t 	cnt1 = 0;
uint16_t     encBufR = 0;
uint16_t     encBufL = 0;
uint32_t     encR = 0;
uint32_t     encL = 0;
uint32_t     encTotalR = 0;
uint32_t     encTotalL = 0;

/////////////////////////////////////////////////////////////////////
// モジュール名 HAL_TIM_PeriodElapsedCallback
// 処理概要     タイマー割り込み(1ms)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	cnt1++;

	 if (cnt1 % 1000 == 0) {
		 printf("IN14:%5d IN7:%5d IN6:%5d IN5:%5d IN4:%5d IN3:%5d IN2:%5d IN1:%5d IN0:%5d IN13:%5d IN12:%5d IN11:%5d \n",
				 analog[11],
				 analog[7],
				 analog[6],
				 analog[5],
				 analog[4],
				 analog[3],
				 analog[2],
				 analog[1],
				 analog[0],
				 analog[10],
				 analog[9],
				 analog[8]);
	 }
    
    // LCD
    lcdShowProcess();

    // Encoder
    getEncoder();

}
/////////////////////////////////////////////////////////////////////
// モジュール名 getEncoder
// 処理概要     1ms間のエンコーダカウントを算出する
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getEncoder(void) {
    uint16_t encRawR, encRawL;

    encRawR = TIM3 -> CNT;
	encRawL = TIM4 -> CNT;

    encR = encRawR - encBufR;
    encL = encRawL - encBufL;

    encTotalR += encR;
    encTotalL += encL;

    encBufR = encRawR;
    encBufL = encRawL;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getEncoder
// 処理概要     1ms間のエンコーダカウントを算出する
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void motorPwmOut(int16_t pwmL, int16_t pwmR) {

    if (pwmL > 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwmL);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    } else if (pwmL == 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 100);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 100);
    } else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, abs(pwmL));
    }
    
    if (pwmR > 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwmR);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
    } else if (pwmR == 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 100);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 100);
    } else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, abs(pwmR));
    }

}
