//====================================//
// インクルード
//====================================//
#include "lineSensor.h"
//====================================//
// グローバル変数の宣言
//====================================//

// ラインセンサ関連
uint32_t		lSensorInt[12] = {0};	// ラインセンサのAD値積算用
uint16_t		lSensor[12] = {0};	// ラインセンサのAD値積算用
// lSensors 		*lSensors_list = (lSensors *)malloc(NUM_SENSORS * sizeof(lSensors));
lSensors     	lSensors_list[12];
double        	angleSensor;

uint16_t		cntls = 0;		// ラインセンサの積算回数カウント用
/////////////////////////////////////////////////////////////////////
// モジュール名 getLineSensor
// 処理概要  	ラインセンサのAD値を取得し、平均値を計算する
// 引数     	なし
// 戻り値    	なし
/////////////////////////////////////////////////////////////////////
void getLineSensor(void) {
	uint8_t i;
	cntls++;
	for ( i=0;i<NUM_SENSORS;i++) {
		lSensorInt[i] += analogVal[i];
	}

	if (cntls > 16) {
		for ( i=0;i<NUM_SENSORS;i++) {
			lSensor[i] = lSensorInt[i]>>4;
			lSensors_list[i].value = lSensor[i];
			lSensorInt[i] = 0;
		}
		cntls = 0;
	}
}
/* qsort関数に渡す比較用の関数（昇順） */
int cmpare_lSensors(const void *n1, const void *n2) {
    if (((lSensors *)n1)->value > ((lSensors *)n2)->value) {
        return 1;
    } else if (((lSensors *)n1)->value < ((lSensors *)n2)->value) {
        return -1;
    }
    return 0;
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
	qsort(lSensors_list, NUM_SENSORS, sizeof(lSensors), cmpare_lSensors);
	index = lSensors_list[0].index;
	
	if (index !=0 && index != NUM_SENSORS -1 ) {
		// 白線に一番近いセンサの両隣のセンサ値を取得
		sen1 = lSensor[index-1];
		sen2 = lSensor[index+1];

		// 正規化
		nsen1 = (double)sen1 / (sen1 + sen2);
		nsen2 = (double)sen2 / (sen1 + sen2);

		if (index >= NUM_SENSORS/2) phi = atan( (nsen1 - nsen2)/1 );		// 偏角φ計算
		else 						phi = atan( (nsen2 - nsen1)/1 );		// 偏角φ計算
		
		dthita = (phi*THITA_SENSOR* (M_PI/180.0)/2) / (M_PI/4);	// 微小角度dθ計算

		
		if (index >= NUM_SENSORS/2) {
			angleSensor = ( (index-5.5)*THITA_SENSOR * (M_PI/180.0)) + dthita;
		} else {
			angleSensor = -( ( (5.5-index)*THITA_SENSOR * (M_PI/180.0)) + dthita );
		}
		angleSensor = angleSensor*(180.0/M_PI);
		
	}

	for (int i = 0; i < NUM_SENSORS; i ++) lSensors_list[i].index = i;

	
}

void print_lSensors_list(const lSensors *list, const uint16_t n) {
	uint8_t i;
    printf("index\tvalue\n");
    for (i = 0; i < n; i ++) {
        printf("  %d\t%d\n", list[i].index, list[i].value);
    }
}
