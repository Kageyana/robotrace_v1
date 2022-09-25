//====================================//
// インクルード
//====================================//
#include "control.h"
//====================================//
// グローバル変数の宣言
//====================================//
// モード関連
uint8_t	pattern = 0;
uint8_t modeLCD = 1;		// LCD表示可否		0:消灯		1:表示
uint8_t modeCurve = 0;		// カーブ判断		0:直線 		1:カーブ進入

// 速度パラメータ関連
double parameterSpeed[10];

///////////////////////////////////////////////////////////////////////////
// モジュール名 systemInit
// 処理概要     初期化処理
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void systemInit (void) {
	// Encoder count
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);

	// ADC
	if (HAL_ADC_Start_DMA(&hadc1, (uint16_t *) analogVal, 14) != HAL_OK)	Error_Handler();
	// PWM
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)			Error_Handler();
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2) != HAL_OK)			Error_Handler();
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3) != HAL_OK)			Error_Handler();
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4) != HAL_OK)			Error_Handler();
	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)			Error_Handler();

	// MAX22201 sleepmode ON
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 500);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 500);
	// Line sensor OFF
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);

	HAL_Delay(500);

	intiLcd();  	// LCD initialize
	//initIMU();	// IMU initialize

	// Timer interrupt
	HAL_TIM_Base_Start_IT(&htim6);

	// while(1) {
	//   lcdRowPrintf(UPROW, "5ax %4d",analogVal[12]);
	//   lcdRowPrintf(LOWROW, "dip %4d",analogVal[13]);
	// }

	lcdRowPrintf(UPROW, "who am i");
	lcdRowPrintf(LOWROW, "    %#x",initBNO055());
	HAL_Delay(700);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 systemLoop
// 処理概要     メインループ
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void systemLoop (void) {
	uint32_t    i = 0;

	switch (pattern) {
      	case 0:
			setup();

			if (start) {
				lcdRowPrintf(UPROW, "ready   ");
				lcdRowPrintf(LOWROW, "        ");

				HAL_Delay(2000);
				lcdRowPrintf(LOWROW, "      Go");
				encTotalN = 0;
				cnt1 = 0;
				pattern = 1;
			}
			break;

      	case 1:
			if (!modeCurve) {
				targetSpeed = 1.0*PALSE_MILLIMETER;
			} else {
				targetSpeed = 0.4*PALSE_MILLIMETER;
			}
			
			motorPwmOutSynth( tracePwm, speedPwm );
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
			// lcdRowPrintf(UPROW, "    %4d", encCurrentN);
			// lcdRowPrintf(LOWROW, "    %4d", SGmarker);

			// マーカー処理
			if (checkmarker() == RIGHTMARKER) {
				// ゴールマーカー処理
				if (SGmarker == 0) {
					SGmarker = STARTMARKER;
				} else if (SGmarker == STARTMARKER && encTotalN > encMM(1000)) {
					SGmarker = GOALMARKER;
				}
			} else if (checkmarker() == LEFTMARKER) {
				// カーブマーカー処理
				enc1 = 0;
				modeCurve = 1;
			}

			if (modeCurve == 1 && enc1 >= encMM(60)) modeCurve = 0;

			// ゴール
			if (SGmarker == GOALMARKER) {
				i = cnt1;
				enc1 = 0;
				pattern = 2;
			}
			break;

      	case 2:
			targetSpeed = 0.4*PALSE_MILLIMETER;
			motorPwmOutSynth( tracePwm, speedPwm );

			if (enc1 >= encMM(500)) {
				pattern = 3;
			}
			break;

      	case 3:
			targetSpeed = 0;
			if (encCurrentN == 0) motorPwmOutSynth( 0, 0 );
			else                  motorPwmOutSynth( 0, speedPwm );
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);

			lcdRowPrintf(UPROW, "   %5d",i);
			lcdRowPrintf(LOWROW, "     End");
			break;
    
      	default:
        	break;
    } // switch case
}