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
        // Interrupt 1ms
        cntRun++;
        cnt5ms++;
        cnt10ms++;
        cntLog++;
        if (patternTrace > 10 && patternTrace < 100) {
            if (fabs(angularVelocity[INDEX_X]) > 200.0f) cntAngleX++;
            else    cntAngleX = 0;
            if (fabs(angularVelocity[INDEX_Y]) > 200.0f) cntAngleY++;
            else    cntAngleY = 0;
            if (abs(encCurrentN) < 10) cntEncStop++;
            else    cntEncStop = 0;
          
        }
        // if (trace_test) 
        if (patternTrace < 10 || patternTrace > 100) {
            getSwitches();  // スイッチの入力を取得
            countDown();
            cntSetup1++;
            cntSetup2++;
            cntSwitchUD++;
            cntSwitchLR++;
        }

        if (modeLCD == 1) lcdShowProcess();   // LCD
        // if ( modeCalLinesensors == 1) calibrationLinesensor();
        // 仮想センサステア計算
        getAngleSensor();
        // Encoder
        getEncoder();
        // PWM
        motorControlTrace();
        motorControlSpeed();
        

        switch(cnt5ms) {
            case 1:
                
                // getBNO055Acceleration();    // 加速度取得       
                getBNO055Gyro();    // 角速度取得
                calcDegrees();              // 角速度制御
                motorControlYawRate();      // 角度制御
                // motorControlYaw();

                checkCurve();
                break;
            case 2:
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
                cMarker = checkMarker();    // マーカー検知
                checkGoalMarker();  // ゴールマーカー処理

                getCurrent();               // 電流計測
                if (modeLOG == 1) writeLogBuffer(
                    11,
                    cntLog,
                    patternTrace,
                    getMarkerSensor(),
                    // encCurrentR,
                    // encCurrentL,
                    encCurrentN,
                    // encTotalR,
                    // encTotalL,
                    encTotalN,
                    (int32_t)(angleSensor*100),
                    modeCurve,
                    // tracePwm,
                    // speedPwm,
                    // (int32_t)lSensorf[0],
                    // (int32_t)lSensorf[1],
                    // (int32_t)lSensorf[2],
                    // (int32_t)lSensorf[3],
                    // (int32_t)lSensorf[4],
                    // (int32_t)lSensorf[5],
                    // (int32_t)lSensorf[6],
                    // (int32_t)lSensorf[7],
                    // (int32_t)lSensorf[8],
                    // (int32_t)lSensorf[9],
                    // (int32_t)lSensorf[10],
                    // (int32_t)lSensorf[11],
                    // (int32_t)(angularVelocity[INDEX_X]*10),
                    // (int32_t)(angularVelocity[INDEX_Y]*10),
                    (int32_t)(angularVelocity[INDEX_Z]*10),
                    // (int32_t)(angle[INDEX_X]*10),
                    // (int32_t)(angle[INDEX_Y]*10),
                    (int32_t)(angle[INDEX_Z]*10),
                    rawCurrentR,
                    rawCurrentL
                    );
                cnt10ms = 0;
                break;
            default:
                break;
        }
    }
    if(htim->Instance == TIM7){
        // Interrupt 0.1ms
        if (modeLOG) writeLogPut();
    }
}
