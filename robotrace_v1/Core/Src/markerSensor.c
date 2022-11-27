//====================================//
// インクルード
//====================================//
#include "markerSensor.h"
//====================================//
// グローバル変数の宣言
//====================================//
uint8_t     SGmarker = 0;
uint8_t		nowMarker = 0, existMarker = 0, crossLine = 0, cntMarkerNone = 0;

int32_t		encMarker = 0;
uint8_t     stateMarker = 0, checkStart = 0;
/////////////////////////////////////////////////////////////////////
// モジュール名 getMarksensor
// 処理概要     マーカーセンサの値を取得
// 引数         なし
// 戻り値       0x1:右センサ反応 0x2:左センサ反応
/////////////////////////////////////////////////////////////////////
uint8_t getMarkerSensor ( void ) {
	uint8_t r=1, l=1, ret=0;

	r = HAL_GPIO_ReadPin(Sidesensor1_GPIO_Port,Sidesensor1_Pin);
	l = HAL_GPIO_ReadPin(Sidesensor2_GPIO_Port,Sidesensor2_Pin);

	if (r == 0) ret += RIGHTMARKER;
	if (l == 0) ret += LEFTMARKER;

	return ret;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 checkMarker
// 処理概要     クロスラインの読み飛ばし処理を含むマーカー検知
// 引数         なし
// 戻り値       0:マーカなし 0x1:右 0x2:左 0x3:クロスライン
///////////////////////////////////////////////////////////////////////////
uint8_t checkMarker( void ) {
	uint8_t ret = 0;

	nowMarker = getMarkerSensor();

	// if (encTotalN - encCross >= encMM(30)) encCross = 0;

	// if ( getMarkerSensor() != 0) {
	// 	if (encMarker == 0) {
	// 		mark = getMarkerSensor();
	// 		encMarker = encTotalN;
	// 	} else if (encTotalN - encMarker <= encth && encTotalN - encCross <= encMM(40)) {

	// 	} else if (encTotalN - encMarker >= encth) {
	// 		ret = getMarkerSensor();
	// 		encMarker = 0;
	// 		mark = 0;
	// 	}
	// }

	if (crossLine == 1 && encTotalN - encMarker >= encMM(90)) {
		crossLine = 0;
		encMarker = 0;
	}

	if (nowMarker != 0 && checkStart == 0 && crossLine == 0) {
		existMarker = nowMarker;
		checkStart = 1;
		cntMarkerNone = 0;
		encMarker = encTotalN;
	}
	if (checkStart == 1) {
		if (encTotalN - encMarker <= encMM(50)) {
			if (nowMarker != 0 && nowMarker != existMarker) {
				// クロスライン
				stateMarker = 1;
				checkStart = 0;
				return CROSSLINE;
			} else if ( nowMarker == existMarker ) {
				if (cntMarkerNone == 0 && encTotalN - encMarker >= encMM(19)) {
					checkStart = 0;
					stateMarker = 4;
					return existMarker;
				}
			} else if ( nowMarker == 0 ) {
				cntMarkerNone++;
				stateMarker = 2;
			}
		} else {
			checkStart = 0;
			stateMarker = 3;
			return existMarker;
		}
	}

	return ret;
}
