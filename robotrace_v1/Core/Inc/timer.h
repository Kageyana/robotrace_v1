#ifndef TIMER_H_
#define TIMER_H_

//====================================//
// インクルード
//====================================//
#include "main.h"
//====================================//
// シンボル定義
//====================================//

//====================================//
// グローバル変数の宣言
//====================================//
extern uint16_t		cntSW;
extern uint32_t     cntRun;
extern uint32_t     cntLog;
// Emergency stop
extern uint16_t     cntEmc1;
extern uint16_t     cntAngleX;
extern uint16_t     cntAngleY;
extern uint16_t     cntEncStop;
//====================================//
// プロトタイプ宣言
//====================================//
void Interrupt1ms(void);
void Interrupt100us(void);

#endif // TIMER_H_
