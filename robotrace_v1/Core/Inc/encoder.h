#ifndef ENCODER_H_
#define ENCODER_H_

//====================================//
// インクルード
//====================================//
#include "main.h"
//====================================//
// シンボル定義
//====================================//
#define PALSE_METER         60074
#define PALSE_MILLIMETER    60
//====================================//
// グローバル変数の宣言
//====================================//
extern int16_t      encCurrentN;
extern int32_t      encTotalR;
extern int32_t      encTotalL;
extern int32_t      encTotalN;
extern int32_t      enc1;
//====================================//
// プロトタイプ宣言
//====================================//
void getEncoder(void);
int32_t encMM( int16_t mm );

#endif // ENCODER_H_
