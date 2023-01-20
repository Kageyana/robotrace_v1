//====================================//
// インクルード
//====================================//
#include "control.h"
//====================================//
// グローバル変数の宣言
//====================================//
// モード関連
uint8_t	patternTrace = 0;
uint8_t modeLCD = 1;		// LCD表示可否		0:消灯			1:表示
uint8_t modeLOG = 0;		// ログ取得状況		0:ログ停止		 1:ログ取得中
uint8_t modeCurve = 0;		// カーブ判断		0:直線			1:カーブ進入
uint8_t modeEMC = 0;

// 速度パラメータ関連
uint8_t paramSpeed[10] = {	PARAM_STRAIGHT, 
							PARAM_CURVEBREAK,
							PARAM_STOP,
							PARAM_CURVE
							};

uint8_t paramAngle[10] = {	PARAM_ANGLE_CURVE
							};

uint16_t analogVal[12];		// ADC結果格納配列

int16_t countdown;

// マーカー関連
uint8_t cMarker;

// ログ関連
uint32_t goalTime = 0;
uint32_t j=0;

///////////////////////////////////////////////////////////////////////////
// モジュール名 systemInit
// 処理概要     初期化処理
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void initSystem (void) {
	// Encoder count
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
	powerLinesensors(0);

	HAL_Delay(500);

	intiLcd();  	// character display initialize
	//initIMU();	// IMU initialize
	initINA260();	// Current sensor initialize
	initBNO055();	// BNO055(IMU) initialize

	// Timer interrupt
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);

}
///////////////////////////////////////////////////////////////////////////
// モジュール名 systemLoop
// 処理概要     メインループ
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void loopSystem (void) {

	if (cntAngleX > STOP_COUNT_ANGLE_Y) {
		emargencyStop(STOP_ANGLE_X);	// X角度が一定値以上
	} else if (cntAngleY > STOP_COUNT_ANGLE_Y) {
		emargencyStop(STOP_ANGLE_Y);	// Y角度が一定値以上
	} else if (cntEncStop > STOP_COUNT_ENCODER_CURRENT) {
		emargencyStop(STOP_ENCODER_CURRENT);	// エンコーダ停止
	}

	// if (encTotalN >= encMM(19) ) {
	// 	emargencyStop(STOP_DISTANCE);
	// }
	
	switch (patternTrace) {
      	case 0:
			setup();

			if (start) {
				cntRun = 0;
				countdown = 5000;		// カウントダウンスタート
				patternTrace = 1;
			}
			break;
		case 1:
			// カウントダウンスタート
			lcdRowPrintf(UPROW, "ready   ");
			lcdRowPrintf(LOWROW, "       %d",countdown/1000);
			if ( countdown == 0 ) {
				motorPwmOut(0,0);	// モータドライバICのスリープモードを解除
				modeLCD = 0;		// LCD OFF
				// Logファイル作成
				if (insertMSD == 1) {
					initLog();
				}
				powerLinesensors(1);	// ラインセンサ ON
				// 変数初期化
				encTotalN = 0;
				encRightMarker = encMM(600);
				cntRun = 0;
				angle[INDEX_X] = 0.0f;
				angle[INDEX_Y] = 0.0f;
				angle[INDEX_Z] = 0.0f;

				modeLOG = 1;    // log start
				patternTrace = 11;
			}
			break;

		case 2:
			// 予備加速
			setTargetSpeed(5);
			motorPwmOutSynth( tracePwm, speedPwm );
			if (encTotalN > encMM(200)) {
				patternTrace = 11;
			}
			break;
      	case 11:
			// 目標速度
			if (modeCurve == 0) {
				setTargetSpeed(paramSpeed[INDEX_STRAIGHT]);
			} else {
				setTargetSpeed(paramSpeed[INDEX_CURVE]);
			}
			// ライントレース
			motorPwmOutSynth( tracePwm, speedPwm );
	 
			// カーブ処理
			// if (fabs(angleSensor) > (float)paramAngle[INDEX_ANGLE_CURVE]) {
			// 	modeCurve = 0;
			// } else {
			// 	modeCurve = 1;
			// }

			// ゴール
			if (SGmarker >= COUNT_GOAL ) {
				goalTime = cntRun;
				enc1 = 0;
				patternTrace = 101;
			}
			break;

      	case 101:
			if (enc1 >= encMM(500)) {
				setTargetSpeed(0);
			} else {
				setTargetSpeed(paramSpeed[INDEX_STOP]);
			}
			motorPwmOutSynth( tracePwm, speedPwm );

			
			if (encCurrentN == 0 && enc1 >= encMM(500)) {
				endLog();
				modeLCD = 1;
				patternTrace = 102;
			}
			
			break;

      	case 102:
			motorPwmOutSynth( 0, 0 );
			powerLinesensors(0);

			lcdRowPrintf(UPROW, "TIME   %d",modeEMC);
			lcdRowPrintf(LOWROW, "  %5ds",goalTime);
			break;
    
      	default:
        	break;
    } // switch case
}

///////////////////////////////////////////////////////////////////////////
// モジュール名 emargencyStop
// 処理概要     緊急停止処理
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void emargencyStop (uint8_t modeStop) { 
	endLog();
	modeLCD = 1;
	modeEMC = modeStop;
	patternTrace = 102;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 countDown
// 処理概要     カウントダウン
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void countDown (void) { 
	countdown--;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 checkCurve
// 処理概要     カーブとストレートの判定
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void checkCurve(void) {
	static uint8_t 	checkStraight, checkRight, checkLeft;
	static float	angleCurve;

	angleCurve += angularVelocity[INDEX_Z] * DEFF_TIME;

	if (fabs(angularVelocity[INDEX_Z]) < 50.0f) {
		// ストレート時
		checkRight = 0;
		checkLeft = 0;
		if (checkStraight == 0) {
			encCurve = 0;
			checkStraight = 1;
		}
		if(checkStraight == 1 && encCurve > encMM(100)){
			modeCurve = 0;
			angleCurve = 0;
		}
	} else if (angularVelocity[INDEX_Z] > 250.0f) {
		// 左カーブ時
		checkStraight = 0;
		checkRight = 0;
		if (checkLeft == 0) {
			encCurve = 0;
			checkLeft = 1;
		}
		if(checkLeft == 1 && encCurve > encMM(20)){
			modeCurve = 2;
		}
	} else if (angularVelocity[INDEX_Z] < -250.0f) {
		// 右カーブ時
		checkStraight = 0;
		checkLeft = 0;
		if (checkRight == 0) {
			encCurve = 0;
			checkRight = 1;
		}
		if(checkRight == 1 && encCurve > encMM(20)){
			modeCurve = 1;
		}
	}
	
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 setTargetSpeed
// 処理概要     目標速度の設定
// 引数         目標速度の整数倍値
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void setTargetSpeed (uint8_t paramSpeed) {
	targetSpeed = (float)paramSpeed*PALSE_MILLIMETER/10;
}