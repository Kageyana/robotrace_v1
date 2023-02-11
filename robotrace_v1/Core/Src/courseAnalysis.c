//====================================//
// インクルード
//====================================//
#include "courseAnalysis.h"
//====================================//
// グローバル変数の宣
//====================================//
float CurvatureRadius = 0;
/////////////////////////////////////////////////////////////////////
// モジュール名 calcCurvatureRadius
// 処理概要     タイマー割り込み(1ms)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void calcCurvatureRadius(void) {
    float dl,drad;
    dl = (float)encCurrentN / PALSE_MILLIMETER * 1000 * DELTATIME; // [pilse] → [mm/s] → [mm] 
    drad = angularVelocity[INDEX_Z] * DELTATIME;            // [rad/s] → [rad]
    CurvatureRadius = dl / drad;
    if (fabs(CurvatureRadius) > 1000.0F) {
        CurvatureRadius = 0.0;
    }
}