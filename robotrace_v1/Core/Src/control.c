//====================================//
// インクルード
//====================================//
#include "control.h"
//====================================//
// グローバル変数の宣言
//====================================//
// モード関連
uint8_t	patternTrace = 0;
uint8_t modeLCD = 1;		// LCD表示可否		0:消灯		1:表示
uint8_t modeLOG = 0;		// ログ取得状況		0:ログ停止	1:ログ取得中
uint8_t modeCurve = 0;		// カーブ判断		0:直線 		1:カーブ進入

// 速度パラメータ関連
uint8_t paramSpeed[10] = {	PARAM_STRAIGHT, 
							PARAM_CURVEBREAK,
							PARAM_STOP,
							PARAM_CURVE
							};

uint8_t paramAngle[10] = {	PARAM_ANGLE_CURVE
							};

uint16_t analogVal[12];		// ADC結果格納配列

// ログ関連
uint16_t logMarker[10000];
uint16_t logEncoder[10000];
uint32_t goalTime = 0;
uint32_t j=0;

///////////////////////////////////////////////////////////////////////////
// モジュール名 systemInit
// 処理概要     初期化処理
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void systemInit (void) {
	// Encoder count
	int16_t val;

	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);

	// ADC
	if (HAL_ADC_Start_DMA(&hadc1, (uint16_t *)analogVal, 12) != HAL_OK)	Error_Handler();

	// PWM
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)			Error_Handler();
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2) != HAL_OK)			Error_Handler();
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3) != HAL_OK)			Error_Handler();
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4) != HAL_OK)			Error_Handler();
	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)			Error_Handler();

	// MAX22201 sleep mode ON
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 500);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 500);
	// Line sensor OFF
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);

	HAL_Delay(500);

	intiLcd();  	// character display initialize
	//initIMU();	// IMU initialize
	initINA260();	// Current sensor initialize
	initBNO055();	// BNO055(IMU) initialize

	// Timer interrupt
	HAL_TIM_Base_Start_IT(&htim6);

	// while(1) {
	// 	// lcdRowPrintf(UPROW, "   %5d",lSensorBright[0]);
	// 	val = readINA260(0x01);
	// 	if (val > 32767) val = ~val+0x8000;
	// 	lcdRowPrintf(LOWROW, "   %5d",val);
	// 	HAL_Delay(500);
	// }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 systemLoop
// 処理概要     メインループ
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void systemLoop (void) {

	
	switch (patternTrace) {
      	case 0:
			setup();

			if (start) {
				lcdRowPrintf(UPROW, "ready   ");
				lcdRowPrintf(LOWROW, "        ");

				HAL_Delay(5000);
				lcdRowPrintf(LOWROW, "      Go");
				modeLCD = 0;
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 500);
				encTotalN = 0;
				cntRun = 0;
				initLog();
				patternTrace = 11;
			}
			break;

      	case 11:
			if (!modeCurve) {
				targetSpeed = paramSpeed[INDEX_STRAIGHT]*PALSE_MILLIMETER/10;
			} else {
				targetSpeed = paramSpeed[INDEX_CURVE]*PALSE_MILLIMETER/10;
			}
			motorPwmOutSynth( tracePwm, speedPwm );

			// マーカー処理
			if ((lSensor[0] + lSensor[1] + lSensor[10] + lSensor[11]) < 6000) {
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
			 
			// カーブ処理
			if (angleSensor < paramAngle[INDEX_ANGLE_CURVE] && angleSensor > -paramAngle[INDEX_ANGLE_CURVE]) {
				modeCurve = 0;
			} else {
				modeCurve = 1;
			}

			// ゴール
			if (SGmarker == GOALMARKER) {
				goalTime = cntRun;
				enc1 = 0;
				patternTrace = 101;
			}
			break;

      	case 101:
			targetSpeed = paramSpeed[INDEX_STOP]*PALSE_MILLIMETER/10;
			motorPwmOutSynth( tracePwm, speedPwm );

			if (enc1 >= encMM(500)) {
				endLog();
				modeLCD = 1;
				patternTrace = 102;
			}
			break;

      	case 102:
			targetSpeed = 0;
			if (encCurrentN == 0) motorPwmOutSynth( 0, 0 );
			else                  motorPwmOutSynth( 0, speedPwm );
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);

			lcdRowPrintf(UPROW, "TIME    ");
			lcdRowPrintf(LOWROW, "  %5ds",goalTime);

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
