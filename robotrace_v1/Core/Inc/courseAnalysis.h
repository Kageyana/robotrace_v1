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
#define CALCDISTANCE        50
#define MACHINEACCELE       2.7F
#define MACHINEDECREACE     3.3F
#define BOOST_MARKER        1
#define BOODT_DISTANCE      2

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
// 2次走行関係
extern uint8_t  optimalTrace;
extern uint16_t optimalIndex;
extern uint16_t numOptimalArry;
extern float    boostSpeed;
extern int32_t  distanceStart, distanceEnd; 

// 解析関係
extern AnalysisData analysisMarker[ANALYSISBUFFSIZE];
extern AnalysisData analysisDistance[ANALYSISBUFFSIZE];
extern float    ROCmarker[ANALYSISBUFFSIZE];
extern float    ROCdistance[ANALYSISBUFFSIZE];
//====================================//
// プロトタイプ宣言
//====================================//
float       calcROC(float velo, float angvelo);
uint16_t    readLogMarker(int logNumber);
uint16_t    readLogDistance(int logNumber);
float     asignVelocity(float ROC);
int         cmpfloat(const void * n1, const void * n2);
#endif // COURSEANALYSIS_H_
