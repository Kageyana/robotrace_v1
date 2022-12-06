//====================================//
// インクルード
//====================================//
#include "lineSensor.h"
//====================================//
// グローバル変数の宣言
//====================================//

// ラインセンサ関連
uint32_t		lSensorInt[12] = {0};	// ラインセンサのAD値積算用
uint16_t		lSensor[12] = {0};		// ラインセンサの平均AD値

lSensors     	lSensorsList[12];
double        	angleSensor;

uint16_t		cntls = 0;		// ラインセンサの積算回数カウント用
/////////////////////////////////////////////////////////////////////
// モジュール名 cmpareLSensorsList
// 処理概要  	構造体のvalueを比較する qsort関数に渡す比較用の関数（昇順）
// 引数     	n1 n2
// 戻り値    	1:n1>n2 0:n1<n2
/////////////////////////////////////////////////////////////////////
int compareLSensorsList(const void *n1, const void *n2) {
    if (((lSensors *)n1)->value > ((lSensors *)n2)->value) {
        return 1;
    } else if (((lSensors *)n1)->value < ((lSensors *)n2)->value) {
        return -1;
    }
    return 0;
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
			lSensor[i] = lSensorInt[i]>>4;		// 平均値算出
			lSensorsList[i].value = lSensor[i];	// 構造体に格納
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
	uint16_t index, sen1, sen2;
	double nsen1, nsen2, phi, dthita;

	// 昇順ソート
	qsort(lSensorsList, NUM_SENSORS, sizeof(lSensors), compareLSensorsList);
	index = lSensorsList[0].index;
	
	if (index !=0 && index != NUM_SENSORS -1 ) {// 両端のセンサが白線の上にあるときは無視
		// 白線に一番近いセンサの両隣のセンサ値を取得
		sen1 = lSensor[index-1];
		sen2 = lSensor[index+1];
		// 正規化
		nsen1 = (double)sen1 / (sen1 + sen2);
		nsen2 = (double)sen2 / (sen1 + sen2);
		if (index >= NUM_SENSORS/2) phi = atan( (nsen1 - nsen2)/1 );		// 偏角φ計算
		else 						phi = atan( (nsen2 - nsen1)/1 );		// 偏角φ計算
		dthita = (phi*THITA_SENSOR* (M_PI/180.0)/2) / (M_PI/4);	// 微小角度dθ計算

		// センサ角度と微小角度を足す
		if (index >= NUM_SENSORS/2) angleSensor = ( (index-5.5)*THITA_SENSOR * (M_PI/180.0)) + dthita;
		else 						angleSensor = -( ( (5.5-index)*THITA_SENSOR * (M_PI/180.0)) + dthita );
		angleSensor = angleSensor*(180.0/M_PI);		// 弧度法に変換
		
	}

	// インデックスを初期化
	for (int i = 0; i < NUM_SENSORS; i ++) lSensorsList[i].index = i;
	
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
		lSensor[i]
	}
}