#ifndef COURSEANALYSIS_H_
#define COURSEANALYSIS_H_
//====================================//
// インクルード
//====================================//
#include "main.h"

//====================================//
// シンボル定義
//====================================//
#define DELTATIME 0.01F
//====================================//
// グローバル変数の宣言
//====================================//
extern float CurvatureRadiuses[100];
extern bool  optimalTrace;
//====================================//
// プロトタイプ宣言
//====================================//
float calcCurvatureRadius(float velo, float angvelo);
void readLog(int logNumber);
int cmpfloat(const void * n1, const void * n2);
#endif // COURSEANALYSIS_H_
