//====================================//
// インクルード
//====================================//
#include "courseAnalysis.h"
#include "fatfs.h"
//====================================//
// グローバル変数の宣
//====================================//
float   CurvatureRadiuses[100]; // マーカー区間ごとの曲率半径
bool    optimalTrace = false;
uint8_t boostSpeed;
uint8_t numMarkerLog = 0;
/////////////////////////////////////////////////////////////////////
// モジュール名 calcCurvatureRadius
// 処理概要     曲率半径の計算
// 引数         velo: エンコーダカウント angvelo: 角速度[rad/s]
// 戻り値       曲率半径[mm]
/////////////////////////////////////////////////////////////////////
float calcCurvatureRadius(float velo, float angvelo) {
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
// モジュール名 readLog
// 処理概要     CSVファイルの読み込み
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
uint8_t readLog(int logNumber) {
    // ファイル読み込み
    FIL         fil_Read;
    FRESULT     fresult;
    uint8_t     fileName[10];
    uint8_t     ret = 0;


    snprintf(fileName,sizeof(fileName),"%d",logNumber);   // 数値を文字列に変換
    strcat(fileName,".csv");                              // 拡張子を追加
    fresult = f_open(&fil_Read, fileName, FA_OPEN_ALWAYS | FA_READ);  // csvファイルを開く

    if (fresult == FR_OK) {
        // ヘッダーの取得
        TCHAR     header[256];
        uint8_t   formatLogRead[256] = "", *tmpStr;

        f_gets(header,256,&fil_Read);
        printf("%s",header);
        tmpStr = header;
        while(*tmpStr != '\0') {
            if (*tmpStr == (uint8_t)',') {
                strcat(formatLogRead,"%d,");
            }
            tmpStr++;
        }
        // printf("%s\n",formatLogRead);

        // ログデータの取得
        TCHAR     log[512];
        int32_t   time, marker,velo,angVelo,null;
        int32_t   i,numMarker=0,numCurRArry,cntCurR=0,cnt1 = 0;
        uint8_t   beforeMarker;
        float     CurRbuff[500];
        float*    sortCurR;

        // 前処理
        // 配列初期化
        for(i=0;i<sizeof(CurvatureRadiuses)/sizeof(CurvatureRadiuses[0]);i++) CurvatureRadiuses[i] = 0.0F;

        // 取得開始
        while (f_gets(log,256,&fil_Read) != NULL) {
            sscanf(log,"%d,%d,%d,%d",&time,&marker,&velo,&angVelo);
            // 解析処理
            // 曲率変化マーカーを通過したとき
            if (marker == 0 && beforeMarker == 2) {
                numCurRArry = cntCurR+1;  // マーカー間で曲率半径を計算した回数＝配列要素数
                sortCurR = (float*)malloc(sizeof(float) * numCurRArry); // 計算した曲率半径カウント分の配列を作成
                memcpy(sortCurR,CurRbuff,sizeof(float) * numCurRArry);  // 作成した配列に曲率半径をコピーする
                qsort(sortCurR, numCurRArry, sizeof(float), cmpfloat); // ソート

                if (numCurRArry % 2 == 0) {
                    // 中央値を記録(配列要素数が偶数のとき)
                    CurvatureRadiuses[numMarker] = (sortCurR[numCurRArry/2]+sortCurR[numCurRArry/2-1])/2;
                } else {
                    // 中央値を記録(配列要素数が奇数のとき)
                    CurvatureRadiuses[numMarker] = sortCurR[numCurRArry/2];
                }
                printf("%f\n",CurvatureRadiuses[numMarker]);
                cntCurR = 0;  // 曲率半径用配列のカウント初期化
                numMarker++;     // マーカーカウント加算
            }
            beforeMarker = marker;  // 前回マーカーを記録

            // 曲率半径の計算
            CurRbuff[cntCurR] = calcCurvatureRadius((float)velo, (float)angVelo/10000);
            cntCurR++;  // 曲率半径用配列のカウント
            cnt1++;
        //    printf("%s",log);
        }

        ret = numMarker;
    }
    f_close(&fil_Read);

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
