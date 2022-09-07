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
void ledOut ( uint8_t rgb )
{
	if ( (rgb & 0x4) == 0x4 ) HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);

	if ( (rgb & 0x2) == 0x2 ) HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);

	if ( (rgb & 0x1) == 0x1 ) HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
}
