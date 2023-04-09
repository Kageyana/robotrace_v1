//====================================//
// インクルード
//====================================//
#include "courseAnalysis.h"
#include "fatfs.h"
//====================================//
// グローバル変数の宣
//====================================//
float       ROCmarker[ANALYSISBUFFSIZE] = {0}; // マーカー区間ごとの曲率半径 Radius Of Curvature
float       ROCdistance[ANALYSISBUFFSIZE] = {0}; // 一定距離ごとの曲率半径 Radius Of Curvature
uint8_t     optimalTrace = 0;
uint16_t    optimalIndex;
uint16_t    numOptimalArry;
float       boostSpeed;
int32_t     distanceStart, distanceEnd; 

AnalysisData analysisMarker[ANALYSISBUFFSIZE];
AnalysisData analysisDistance[ANALYSISBUFFSIZE];
/////////////////////////////////////////////////////////////////////
// モジュール名 calcROC
// 処理概要     曲率半径の計算
// 引数         velo: エンコーダカウント angvelo: 角速度[rad/s]
// 戻り値       曲率半径[mm]
/////////////////////////////////////////////////////////////////////
float calcROC(float velo, float angvelo) {
    float dl, drad, ret;
    dl = velo / PALSE_MILLIMETER * 1000 * DELTATIME; // [pilse] → [mm/s] → [mm] 
    drad = angvelo * DELTATIME;            // [rad/s] → [rad]
    ret = dl / drad;
    // 曲率半径が大きい＝直線の場合は極大にする
    if (fabs(ret) > 1500.0F) {
        ret = 10000.0;
    }

    return ret;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 readLogMarker
// 処理概要     マーカー基準2次走行
// 引数         なし
// 戻り値       最適速度配列の最大要素数
/////////////////////////////////////////////////////////////////////
uint16_t readLogMarker(int logNumber) {
    // ファイル読み込み
    FIL         fil_Read;
    FRESULT     fresult;
    uint8_t     fileName[10];
    uint16_t    ret = 0;


    snprintf(fileName,sizeof(fileName),"%d",logNumber);   // 数値を文字列に変換
    strcat(fileName,".csv");                              // 拡張子を追加
    fresult = f_open(&fil_Read, fileName, FA_OPEN_ALWAYS | FA_READ);  // csvファイルを開く

    if (fresult == FR_OK) {
        printf("Analysis marker start\n");

        // // ヘッダーの取得
        // TCHAR     header[256];
        // uint8_t   formatLogRead[256] = "", *tmpStr;

        // f_gets(header,256,&fil_Read);
        // tmpStr = header;
        // while(*tmpStr != '\0') {
        //     if (*tmpStr == (uint8_t)',') {
        //         strcat(formatLogRead,"%d,");
        //     }
        //     tmpStr++;
        // }

        // ログデータの取得
        TCHAR     log[512];
        int32_t   time, marker,velo,angVelo,distance,null;
        int32_t   i,numM=0,cntCurR=0,cnt1 = 0;
        uint8_t   beforeMarker;
        float     ROCbuff[500];
        float*    sortROC;

        // 取得開始
        while (f_gets(log,256,&fil_Read) != NULL) {
            sscanf(log,"%d,%d,%d,%d,%d",&time,&marker,&velo,&angVelo,&distance);
            // 解析処理
            // 曲率変化マーカーを通過したとき
            if (marker == 0 && beforeMarker == 2) {
                sortROC = (float*)malloc(sizeof(float) * cntCurR); // 計算した曲率半径カウント分の配列を作成
                memcpy(sortROC,ROCbuff,sizeof(float) * cntCurR);  // 作成した配列に曲率半径をコピーする
                qsort(sortROC, cntCurR, sizeof(float), cmpfloat); // ソート
                // 曲率半径を記録する
                if (cntCurR % 2 == 0) {
                    // 中央値を記録(配列要素数が偶数のとき) 中央2つの平均値
                    analysisMarker[numM].ROC = (sortROC[cntCurR/2]+sortROC[cntCurR/2-1])/2;
                } else {
                    // 中央値を記録(配列要素数が奇数のとき)
                    analysisMarker[numM].ROC = sortROC[cntCurR/2];
                }
                // printf("%f\n",analysisMarker[numM].ROC);
                cntCurR = 0;  // 曲率半径用配列のカウント初期化

                analysisDistance[numM].time = time;
                analysisDistance[numM].marker = marker;
                analysisDistance[numM].velocity = (float)velo/PALSE_MILLIMETER;
                analysisDistance[numM].angularVelocity = (float)angVelo/10000;
                analysisDistance[numM].distance = distance;
                analysisDistance[numM].boostSpeed = (float)asignVelocity(ROCmarker[numM])/10;   // 曲率半径ごとの速度を記録する
                numM++;     // マーカーカウント加算
            }
            beforeMarker = marker;  // 前回マーカーを記録

            // 曲率半径の計算
            ROCbuff[cntCurR] = calcROC((float)velo, (float)angVelo/10000);
            cntCurR++;  // 曲率半径用配列のカウント
            cnt1++;
        //    printf("%s",log);
        }

        ret = numM-1;
    }
    f_close(&fil_Read);

    printf("Analysis marker end\n");

    return ret;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 readLogDistance
// 処理概要     距離基準2次走行の解析
// 引数         なし
// 戻り値       最適速度配列の最大要素数
/////////////////////////////////////////////////////////////////////
uint16_t readLogDistance(int logNumber) {
    // ファイル読み込み
    FIL         fil_Read;
    FRESULT     fresult;
    uint8_t     fileName[10];
    uint16_t     ret = 0;


    snprintf(fileName,sizeof(fileName),"%d",logNumber);   // 数値を文字列に変換
    strcat(fileName,".csv");                              // 拡張子を追加
    fresult = f_open(&fil_Read, fileName, FA_OPEN_ALWAYS | FA_READ);  // csvファイルを開く

    if (fresult == FR_OK) {
        printf("Analysis distance start\n");

        // // ヘッダーの取得
        // TCHAR     header[256];
        // uint8_t   formatLogRead[256] = "", *tmpStr;

        // f_gets(header,256,&fil_Read);
        // tmpStr = header;
        // while(*tmpStr != '\0') {
        //     if (*tmpStr == (uint8_t)',') {
        //         strcat(formatLogRead,"%d,");
        //     }
        //     tmpStr++;
        // }

        // ログデータの取得
        TCHAR     log[512];
        int32_t   time, marker,velo,angVelo,distance,null;
        int32_t   i, startEnc=0, numD=0, cntCurR=0,beforeMarker=0;
        bool      analysis=false;
        float     ROCbuff[500] = {0};
        float*    sortROC;

        // 前処理
        // 配列初期化
        for(i=0;i<ANALYSISBUFFSIZE;i++) {
            ROCmarker[i] = 0.0F;
        }

        // ログデータ取得開始
        while (f_gets(log,256,&fil_Read) != NULL) {
            sscanf(log,"%d,%d,%d,%d,%d",&time,&marker,&velo,&angVelo,&distance);
            // 解析処理
            // ゴールマーカーを通過したときにフラグ反転
            if (marker == 1 && beforeMarker == 0) {
                analysis = !analysis;
                startEnc = distance;
            }
            beforeMarker = marker;  // 前回マーカーを記録
            
            if (analysis == true) {
                // スタートマーカー通過後から解析開始
                // 一定距離ごとに処理
                if ( distance-startEnc >= encMM(CALCDISTANCE)) {
                    sortROC = (float*)malloc(sizeof(float) * cntCurR); // 計算した曲率半径カウント分の配列を作成
                    memcpy(sortROC,ROCbuff,sizeof(float) * cntCurR);  // 作成した配列に曲率半径をコピーする
                    qsort(sortROC, cntCurR, sizeof(float), cmpfloat); // ソート

                    // 曲率半径を記録する
                    if (cntCurR % 2 == 0) {
                        // 中央値を記録(配列要素数が偶数のとき) 中央2つの平均値
                        analysisDistance[numD].ROC = (sortROC[cntCurR/2]+sortROC[cntCurR/2-1])/2;
                    } else {
                        // 中央値を記録(配列要素数が奇数のとき)
                        analysisDistance[numD].ROC = sortROC[cntCurR/2];
                    }
                    // printf("%f\n",analysisDistance[numD].ROC);
                    cntCurR = 0;  // 曲率半径用配列のカウント初期化

                    analysisDistance[numD].time = time;
                    analysisDistance[numD].marker = marker;
                    analysisDistance[numD].velocity = (float)velo/PALSE_MILLIMETER;
                    analysisDistance[numD].angularVelocity = (float)angVelo/10000;
                    analysisDistance[numD].distance = distance;
                    analysisDistance[numD].boostSpeed = asignVelocity(analysisDistance[numD].ROC);   // 曲率半径ごとの速度を計算する

                    printf("%f\n",analysisDistance[numD].ROC);

                    startEnc = distance;    // 距離計測開始位置を更新
                    numD++;          // インデックス更新
                }
                // 曲率半径の計算
                ROCbuff[cntCurR] = calcROC((float)velo, (float)angVelo/10000);
                cntCurR++;  // 曲率半径用配列のカウント
            }
        }
        numD--; // インデックスが1多くなるので調整

        printf("fix velocity\n");
        // 目標速度配列の整形 加減速が間に合うように距離を調整する
        float acceleration, elapsedTime, dv, dl;

        // 最初の要素は調整しない
        dl = (float)CALCDISTANCE / 1000;
        for (i=2;i<=numD;i++) {
            dv = (analysisDistance[i].boostSpeed - analysisDistance[i-1].boostSpeed);
            elapsedTime = fabs(dl /dv);
            acceleration = dv / elapsedTime;
            if (acceleration > MACHINEACCELE) {
                analysisDistance[i].boostSpeed = analysisDistance[i-1].boostSpeed + (MACHINEACCELE*dl);
            }
            // printf("%f\n",analysisDistance[i].boostSpeed);
        }

        for (i=numD-1;i>=1;i--) { 
            dv = (analysisDistance[i].boostSpeed - analysisDistance[i+1].boostSpeed);
            elapsedTime = fabs(dl /dv);
            acceleration = dv / elapsedTime;
            if (acceleration > MACHINEDECREACE) {
                analysisDistance[i].boostSpeed = analysisDistance[i+1].boostSpeed + (MACHINEDECREACE*dl);
            }
            // printf("%f\n",analysisDistance[i].boostSpeed);
        }

        for (i=0;i<=numD;i++) {
            printf("%f\n",analysisDistance[i].boostSpeed);
        }
        
        ret = numD;
    }
    f_close(&fil_Read);

    printf("Analysis distance end\n");

    return ret;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 asignVelocity
// 処理概要     曲率半径ごとの最適速度を割り当てる
// 引数         
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
float asignVelocity(float ROC) {
    float absROC;
    float ret; 

    absROC = fabs(ROC);
    if ( absROC > 1500.0F ) ret = targetParam.boostStraight;
    if ( absROC <= 1500.0F ) ret = targetParam.boost1500;
    if ( absROC <= 800.0F )  ret = targetParam.boost800;
    if ( absROC <= 600.0F )  ret = targetParam.boost600;
    if ( absROC <= 400.0F )  ret = targetParam.boost400;
    if ( absROC <= 200.0F )  ret = targetParam.boost200;

    return ret;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 cmpfloat
// 処理概要     float型の比較
// 引数         
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
int cmpfloat(const void * n1, const void * n2) {
	if (*(float *)n1 > *(float *)n2) return 1;
	else if (*(float *)n1 < *(float *)n2) return -1;
	else return 0;
}
