#ifndef ENCODER_H_
#define ENCODER_H_

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
extern int16_t      encCurrentN;
extern int32_t      encTotalR;
extern int32_t      encTotalL;
extern int32_t      encTotalN;
extern int32_t      enc1;
//====================================//
// プロトタイプ宣言
//====================================//
void getEncoder(void);
int32_t encMM( short mm );

#endif // ENCODER_H_
