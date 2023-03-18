#ifndef COURSEANALYSIS_H_
#define COURSEANALYSIS_H_
//====================================//
// インクルード
//====================================//
#include "main.h"

//====================================//
// シンボル定義
//====================================//
#define ANALYSISBUFFSIZE    256
#define DELTATIME           0.01F
#define CALCDISTANCE        30
#define MACHINEACCELE       2.5F
#define MACHINEDECREACE     -2.5F

typedef struct {
    int32_t time;
    uint8_t marker;
    float   velocity;
    float   angularVelocity;
    int32_t distance;
    float   ROC;
    float   boostSpeed;
} AnalysisData;
//====================================//
// グローバル変数の宣言
//====================================//
extern float    ROCmarker[ANALYSISBUFFSIZE];
extern float    ROCdistance[ANALYSISBUFFSIZE];
extern uint8_t  boostSpeeds[ANALYSISBUFFSIZE];
extern bool     optimalTrace;
extern uint8_t  boostSpeed;
extern uint8_t  numMarkerLog;
//====================================//
// プロトタイプ宣言
//====================================//
float   calcROC(float velo, float angvelo);
uint8_t readLogMarker(int logNumber);
uint8_t readLogDistance(int logNumber);
uint8_t asignVelocity(float ROC);
int     cmpfloat(const void * n1, const void * n2);
#endif // COURSEANALYSIS_H_
