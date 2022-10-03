﻿//====================================//
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
uint8_t paramSpeed[10] = {	PARAM_STRAIGHT, 
								PARAM_CURVEBREAK,
								PARAM_CURVEBREAK,
								PARAM_CURVE
							};

uint16_t analogVal[14];		// ADC結果格納配列

// ログ関連
uint16_t    logMarker[10000];
uint16_t    logEncoder[10000];
uint32_t	goalTime = 0;
uint32_t	j=0;

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

	for (int i = 0; i < NUM_SENSORS; i ++) lSensors_list[i].index = i;

	// lcdRowPrintf(UPROW, "who am i");
	// lcdRowPrintf(LOWROW, "    %#x",initBNO055());
	// HAL_Delay(700);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 systemLoop
// 処理概要     メインループ
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void systemLoop (void) {

	// if (pattern > 0 )
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
				targetSpeed = paramSpeed[INDEX_STRAIGHT]*PALSE_MILLIMETER/10;
			} else {
				targetSpeed = paramSpeed[INDEX_CURVE]*PALSE_MILLIMETER/10;
			}
			
			motorPwmOutSynth( tracePwm, speedPwm );
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
			// lcdRowPrintf(UPROW, "    %4d", encCurrentN);
			lcdRowPrintf(LOWROW, "   %d", SGmarker);

			// マーカー処理
			if ((lSensor[0] + lSensor[1] + lSensor[10] + lSensor[11]) < 11000) {
				encCross2 = encTotalN;
			}
			if (encTotalN - encCross2 >= encMM (200) ) {
				if (checkMarker() == RIGHTMARKER) {
					// ゴールマーカー処理
					if (SGmarker == 0) {
						SGmarker = STARTMARKER;
					} else if (SGmarker == STARTMARKER && encTotalN > encMM(500)) {
						SGmarker = GOALMARKER;
					}
				}
			}
			 
			// else if (checkMarker() == LEFTMARKER) {
			// 	// カーブマーカー処理
			// 	enc1 = 0;
			// 	modeCurve = 1;
			// }

			// カーブ処理
			if (angleSensor > 11 || angleSensor < -11) {
				modeCurve = 1;
			} else if (angleSensor < 11 && angleSensor > -11) {
				modeCurve = 0;
			}

			// ゴール
			if (SGmarker == GOALMARKER) {
				goalTime = cnt1;
				enc1 = 0;
				pattern = 101;
			}
			break;

      	case 101:
			targetSpeed = paramSpeed[INDEX_STOP]*PALSE_MILLIMETER/10;
			motorPwmOutSynth( tracePwm, speedPwm );

			if (enc1 >= encMM(500)) {
				pattern = 102;
			}
			break;

      	case 102:
			targetSpeed = 0;
			if (encCurrentN == 0) motorPwmOutSynth( 0, 0 );
			else                  motorPwmOutSynth( 0, speedPwm );
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);

			lcdRowPrintf(UPROW, " %7d",encMarker2);
			lcdRowPrintf(LOWROW, " %7d",cntmark);

			// if(swValTact == SW_PUSH) {
			// 	printf("cnt\tmarker\tencoder");
			// 	for (j=0;j<goalTime;j++) {
			// 		printf("%4d\t%d\t%d",j,logMarker[j],logEncoder[j]);
			// 	}
			// }
			break;
    
      	default:
        	break;
    } // switch case
}
