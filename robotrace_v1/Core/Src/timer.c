//====================================//
// インクルード
//====================================//
#include "timer.h"
//====================================//
// グローバル変数の宣
//====================================//
uint32_t    cnt1 = 0;
uint16_t    encBufR = 0;
uint16_t    encBufL = 0;
int16_t     encR = 0;
int16_t     encL = 0;
int16_t     encN = 0;
int32_t    encTotalR = 0;
int32_t    encTotalL = 0;
int32_t    encTotalN = 0;

/////////////////////////////////////////////////////////////////////
// モジュール名 HAL_TIM_PeriodElapsedCallback
// 処理概要     タイマー割り込み(1ms)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	cnt1++;
    cntSW++;
    cntSetup1++;
    cntSetup2++;
    cntSetup3++;

    // スイッチの入力を取得
    if (cntSW >= 100) {
        swValTact = getSWtact();
        swValRotary = getSWrotary();
        cntSW = 0;
    }

	// if (cnt1 % 500 == 0) {
		// printf("IN14:%5d IN7:%5d IN6:%5d IN5:%5d IN4:%5d IN3:%5d IN2:%5d IN1:%5d IN0:%5d IN13:%5d IN12:%5d IN11:%5d \n",
        //         analog[0],
        //         analog[1],
        //         analog[2],
        //         analog[3],
        //         analog[4],
        //         analog[5],
        //         analog[6],
        //         analog[7],
        //         analog[8],
        //         analog[9],
        //         analog[10],
        //         analog[11]);

	//  }
    
    // LCD
    lcdShowProcess();
    // Encoder
    getEncoder();
    // PWM
    motorControlTrace();
    motorControlSpeed();

}
/////////////////////////////////////////////////////////////////////
// モジュール名 getEncoder
// 処理概要     1ms間のエンコーダカウントを算出する
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void getEncoder(void) {
    uint16_t encRawR, encRawL;

    // エンコーダカウントを取得
    encRawR = TIM4 -> CNT;
	encRawL = TIM3 -> CNT;

    // 1msあたりのカウント
    encR = encBufR - encRawR;
    encL = encBufL - encRawL;
    encN = (encR + encL ) / 2;

    // カウントの積算(回転方向が逆なのでマイナスで積算)
    encTotalR += encR;
    encTotalL += encL;
    encTotalN += encN;

    // 前回値を更新
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
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwmL);
    } else if (pwmL == 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1000);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 1000);
    } else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, abs(pwmL));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    }
    
    if (pwmR > 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwmR);
    } else if (pwmR == 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 1000);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 1000);
    } else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, abs(pwmR));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
    }

}
