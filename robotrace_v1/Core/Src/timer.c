//====================================//
// インクルード
//====================================//
#include "timer.h"
//====================================//
// グローバル変数の宣
//====================================//
uint32_t    cnt1 = 0;

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

    // LCD
    lcdShowProcess();
    // Encoder
    getEncoder();
    // PWM
    motorControlTrace();
    motorControlSpeed();

}