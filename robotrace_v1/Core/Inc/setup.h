﻿#ifndef SETUP_H_
#define SETUP_H_
//======================================//
// インクルード
//======================================//
#include "main.h"
#include <stdio.h>
//======================================//
// グローバル変数の宣言
//======================================//
#define UD	0
#define LR	1

#define START_COUNT	    1
#define START_GATE		2

//======================================//
// グローバル変数の宣言
//======================================//
// パターン関連
extern uint8_t		start;

// タイマ関連
extern uint16_t 	cntSetup1;
extern uint16_t 	cntSetup2;
extern uint16_t 	cntSetup3;	
extern uint16_t		cntSwitchUD;	// スイッチ長押し判定用右
extern uint16_t		cntSwitchLR;	// スイッチ長押し判定用左

// パラメータ関連
extern uint8_t      fixSpeed;

// フラグ関連
extern uint8_t trace_test;

//======================================//
// プロトタイプ宣言
//======================================//
void setup(void);
uint8_t fixSpeedSetting ( void );

#endif /* SETUP_H_ */