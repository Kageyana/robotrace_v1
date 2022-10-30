#ifndef SWITCH_H_
#define SWITCH_H_

//====================================//
// インクルード
//====================================//
#include "main.h"
//====================================//
// シンボル定義
//====================================//
#define SW_NONE     0x0
#define SW_UP		0x1
#define SW_PUSH 	0x2
#define	SW_LEFT	    0x3
#define SW_RIGHT	0x4
#define SW_DOWN	    0x5

//====================================//
// グローバル変数の宣言
//====================================//
extern uint8_t		swValTact;
extern uint8_t		swValRotary;

// タイマ関連
extern uint16_t		cntSW;			// アナログ入力スイッチのチャタリング防止用
//====================================//
// プロトタイプ宣言
//====================================//
uint8_t getSWrotary( uint16_t ad );
uint8_t getSWtact( uint16_t ad );

#endif // SWITCH_H_
