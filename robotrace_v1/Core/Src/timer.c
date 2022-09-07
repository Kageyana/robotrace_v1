//====================================//
// インクルード
//====================================//
#include "timer.h"
//====================================//
// グローバル変数の宣言
//====================================//
uint8_t 	led = 0;
uint32_t 	cnt1 = 0;

/////////////////////////////////////////////////////////////////////
// モジュール名 HAL_TIM_PeriodElapsedCallback
// 処理概要     タイマー割り込み(1ms)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	cnt1++;

	// if (cnt1 % 500 == 0) {
	// 	ledOut(led);
	// 	led++;
	// 	if (led > 0x7) led = 0;
	// }
    
    // LCD
    lcdShowProcess();

	if (getSWtact() == 0) {
		 __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);
		 __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
		 __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 500);
 		 __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 500);
	} else if (getSWtact() == RIGHT) {
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 100);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 100);
	} else if (getSWtact() == LEFT) {
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 100);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 100);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
	}


	// if (cnt1 % 500 == 0) {
	// 	printf("AD15: %5d, HEX: 0x%x,  AD10: %5d, HEX: 0x%x\n",analog[13], getSWrotary(), analog[12], getSWtact());
	// 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
	// 	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 600);
	// }

}
