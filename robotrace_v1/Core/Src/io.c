//====================================//
// インクルード
//====================================//
#include "io.h"
//====================================//
// グローバル変数の宣言
//====================================//

/////////////////////////////////////////////////////////////////////
// モジュール名 ledOut
// 処理概要     フルカラーLEDのON/OFF
// 引数         rgb 下位3ビットがそれぞれ赤緑青に対応している
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void ledOut ( uint8_t rgb ) {
	if ( (rgb & 0x4) == 0x4 ) HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);

	if ( (rgb & 0x2) == 0x2 ) HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);

	if ( (rgb & 0x1) == 0x1 ) HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getMarksensor
// 処理概要     マーカーセンサの値を取得
// 引数         なし
// 戻り値       0x1:右センサ反応 0x2:左センサ反応
/////////////////////////////////////////////////////////////////////
uint8_t getMarksensor ( void ) {
	uint8_t r = 1, l = 1, ret = 0;

	r = HAL_GPIO_ReadPin(Sidesensor1_GPIO_Port,Sidesensor1_Pin);
	l = HAL_GPIO_ReadPin(Sidesensor2_GPIO_Port,Sidesensor2_Pin);

	if (r == 0) ret += 0x01;
	if (l == 0) ret += 0x02;

	return ret;
}