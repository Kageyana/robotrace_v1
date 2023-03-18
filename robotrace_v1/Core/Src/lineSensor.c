//====================================//
// インクルード
//====================================//
#include "lineSensor.h"
//====================================//
// グローバル変数の宣言
//====================================//

// ラインセンサ関連
uint32_t		lSensorInt[NUM_SENSORS] = {0};	// ラインセンサのAD値積算用
uint16_t		lSensor[NUM_SENSORS] = {0};		// ラインセンサの平均AD値
float			lSensorf[NUM_SENSORS] = {0};	// 正規化したラインセンサのAD値
uint16_t		cntls = 0;		// ラインセンサの積算回数カウント用
// 仮想センサステア関連
uint16_t		lineIndex = 0;
float        	angleSensor;
// キャリブレーション関連
uint16_t		lSensorOffset[NUM_SENSORS] = {0};
uint8_t			modeCalLinesensors = 0;
/////////////////////////////////////////////////////////////////////
// モジュール名 powerLinesensors
// 処理概要  	ラインセンサのON/OFF処理
// 引数     	0:OFF 1:ON
// 戻り値    	なし
/////////////////////////////////////////////////////////////////////
void powerLinesensors(uint8_t onoff) {
	if (onoff == 0) {
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	} else if ( onoff == 1 ) {
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 337);
	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getLineSensor
// 処理概要  	ラインセンサのAD値を取得し、平均値を計算する
// 引数     	なし
// 戻り値    	なし
/////////////////////////////////////////////////////////////////////
void getLineSensor(void) {
	uint8_t i;
	
	for ( i=0;i<NUM_SENSORS;i++) {
		lSensorInt[i] += analogVal[i];
		cntls++;

		if (cntls > 16) {
			lSensor[i] = lSensorInt[i] >> 4;		// 平均値算出
			// if (lSensorOffset[i] > 0 && modeCalLinesensors == 0) {
			// 	lSensorf[i] = (float)lSensor[i] / lSensorOffset[i];
			// }
			lSensorInt[i] = 0;					// 積算値リセット
			cntls = 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getAngleSensor
// 処理概要  	ラインセンサのAD値からステア角を算出する
// 引数     	なし
// 戻り値    	なし
/////////////////////////////////////////////////////////////////////
void getAngleSensor(void) {
	uint16_t index, sen1, sen2, i, min;
	float nsen1, nsen2, phi, dthita;

	min = 1000;
	for ( i=0;i<NUM_SENSORS;i++) {
		if (lSensor[i] < min) {
			min = lSensor[i];
			index = i;
		}
	}
	
	if (index >= 0 && index <= NUM_SENSORS-1 ) {// 両端のセンサが白線の上にあるときは無視
		// 白線に一番近いセンサの両隣のセンサ値を取得
		sen1 = lSensor[index-1];
		sen2 = lSensor[index+1];
		// 正規化
		nsen1 = (float)sen1 / (sen1 + sen2);
		nsen2 = (float)sen2 / (sen1 + sen2);
		if (index >= NUM_SENSORS/2) {
			phi = atan( (nsen1 - nsen2)/1 );		// 偏角φ計算
		} else{
			phi = atan( (nsen2 - nsen1)/1 );		// 偏角φ計算
		}
		dthita = (phi*THITA_SENSOR* (M_PI/180.0)/2) / (M_PI/4);	// 微小角度dθ計算

		// センサ角度と微小角度を足す
		if (index >= NUM_SENSORS/2) {
			angleSensor = ( (index-5.5)*THITA_SENSOR * (M_PI/180.0)) + dthita;
		} else { 
			angleSensor = -( ( (5.5-index)*THITA_SENSOR * (M_PI/180.0)) + dthita );
		}
		angleSensor = angleSensor*(180.0/M_PI);		// 弧度法に変換
		
	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名 calibrationLinesensor
// 処理概要  	ラインセンサのAD値を正規化する
// 引数     	なし
// 戻り値    	なし
/////////////////////////////////////////////////////////////////////
void calibrationLinesensor (void) {
	uint8_t i;
	
	for ( i=0;i<NUM_SENSORS;i++) {
		if (lSensor[i] > lSensorOffset[i]) {
			lSensorOffset[i] = lSensor[i];
		}
	}
}