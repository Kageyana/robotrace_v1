//====================================//
// インクルード
//====================================//
#include "switch.h"
//====================================//
// グローバル変数の宣言
//====================================//
// スイッチ関連
uint8_t			swValTact;
uint8_t			swValRotary;

// タイマ関連
uint16_t		cntSW = 0;		// 5方向タクトスイッチのチャタリング防止用
/////////////////////////////////////////////////////////////////////
// モジュール名 getSWrotary
// 処理概要  ロータリスイッチのアナログ入力から2進数への変換
// 引数     なし
// 戻り値    ロータリスイッチのカウント
/////////////////////////////////////////////////////////////////////
uint8_t getSWrotary( uint16_t ad ) {
	uint8_t ret = 0;

	if ( ad > 3900 ) 					ret = 0x0;
	else if ( ad < 3681 && ad > 3653 ) 	ret = 0x1;
	else if ( ad < 2784 && ad > 2746 ) 	ret = 0x2;
	else if ( ad < 2592 && ad > 2543 ) 	ret = 0x3;
	else if ( ad < 2360 && ad > 2310 ) 	ret = 0x4;
	else if ( ad < 2215 && ad > 2164 ) 	ret = 0x5;
	else if ( ad < 1859 && ad > 1809 ) 	ret = 0x6;
	else if ( ad < 1768 && ad > 1718 ) 	ret = 0x7;
	else if ( ad < 1626 && ad > 1576 ) 	ret = 0x8;
	else if ( ad < 1556 && ad > 1407 ) 	ret = 0x9;
	else if ( ad < 1373 && ad > 1325 ) 	ret = 0xa;
	else if ( ad < 1323 && ad > 1275 ) 	ret = 0xb;
	else if ( ad < 1261 && ad > 1213 ) 	ret = 0xc;
	else if ( ad < 1209 && ad > 1162 ) 	ret = 0xd;
	else if ( ad < 1104 && ad > 1058 ) 	ret = 0xe;
	else if ( ad < 1057 && ad > 1016 ) 	ret = 0xf;

	return ret;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getSWtact
// 処理概要  5方向タクトスイッチのアナログ入力から5方向の入力に変換
// 引数      なし
// 戻り値     スイッチの方向
/////////////////////////////////////////////////////////////////////
uint8_t getSWtact( uint16_t ad ) {
	uint8_t ret = SW_NONE;

	if ( ad > 3900 ) 					ret = 0x0;
	else if ( ad < 3541 && ad > 3116 ) 	ret = SW_DOWN;
	else if ( ad < 3038 && ad > 2603 ) 	ret = SW_RIGHT;
	else if ( ad < 2391 && ad > 1950 ) 	ret = SW_LEFT;
	else if ( ad < 1440 && ad > 1006 ) 	ret = SW_PUSH;
	else if ( ad < 150 && ad >= 0 ) 	ret = SW_UP;

	return ret;
}