//====================================//
// インクルード
//====================================//
#include "timer.h"
//====================================//
// グローバル変数の宣
//====================================//
uint32_t    cntRun = 0;
uint32_t    cnt5ms = 0;
uint32_t    cnt10ms = 0;
uint32_t    cntLog = 0;
// Emergency stop
uint16_t    cntEmc1 = 0;
uint16_t    cntAngleX = 0;
uint16_t    cntAngleY = 0;
uint16_t    cntEncStop = 0;
/////////////////////////////////////////////////////////////////////
// モジュール名 HAL_TIM_PeriodElapsedCallback
// 処理概要     タイマー割り込み(1ms)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6){
        cntRun++;
        cnt5ms++;
        cnt10ms++;
        cntLog++;
        if (patternTrace > 10 && patternTrace < 100) {
            if (fabs(angle[INDEX_X]) > 45.0) cntAngleX++;
            else    cntAngleX = 0;
            if (fabs(angle[INDEX_Y]) > 45.0) cntAngleY++;
            else    cntAngleY = 0;
            if (abs(encCurrentN) < 10) cntEncStop++;
            else    cntEncStop = 0;

        }
        if (patternTrace < 10 || patternTrace > 100) {
            getSwitches();  // スイッチの入力を取得
            cntSetup1++;
            cntSetup2++;
            cntSwitchUD++;
            cntSwitchLR++;
        }

        if (modeLCD == 1) lcdShowProcess();   // LCD

        // 仮想センサステア計算
        getAngleSensor();
        // Encoder
        getEncoder();
        // PWM
        motorControlTrace();
        motorControlSpeed();

        switch(cnt5ms) {
            case 1:
                // getCurrent();               // 電流計測
                // getBNO055Acceleration();    // 加速度取得       
                getBNO055Gyro();    // 角速度取得
                calcDegrees();
                motorControlYawRate();
                // motorControlYaw();
                break;
            case 2:
                cMarker = checkMarker();
                if (modeLOG) writeLogBuffer(
                    8,
                    cntLog,
                    patternTrace,
                    cMarker,
                    // encCurrentR,
                    // encCurrentL,
                    encCurrentN,
                    // encTotalR,
                    // encTotalL,
                    encTotalN,
                    (int32_t)angleSensor*10,
                    // lSensor[0],
                    // lSensor[1],
                    // lSensor[2],
                    // lSensor[3],
                    // lSensor[4],
                    // lSensor[5],
                    // lSensor[6],
                    // lSensor[7],
                    // lSensor[8],
                    // lSensor[9],
                    // lSensor[10],
                    // lSensor[11],
                    (int32_t)angularVelocity[INDEX_X]*10,
                    (int32_t)angularVelocity[INDEX_Y]*10,
                    (int32_t)angularVelocity[INDEX_Z]*10,
                    (int32_t)angle[INDEX_X]*10,
                    (int32_t)angle[INDEX_Y]*10,
                    (int32_t)angle[INDEX_Z]*10
                    // rawCurrentR,
                    // rawCurrentL,
                    );
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                cnt5ms = 0;
                break;
            default:
                break;
        }

        switch(cnt10ms) {
            case 10:
                
                cnt10ms = 0;
                break;
            default:
                break;
        }
    }
    if(htim->Instance == TIM7){
        if (modeLOG) writeLogPut();
    }
}
