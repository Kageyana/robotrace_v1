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
// モジュール名 Interrupt1ms
// 処理概要     タイマー割り込み(1ms)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void Interrupt1ms(void) {
    // Interrupt 1ms

    // countup
    cntRun++;
    cnt5ms++;
    cnt10ms++;
    cntLog++;
    if (patternTrace > 10 && patternTrace < 100) {
        // 緊急停止処理
        if (cntEmcStopAngleX()) emcStop = STOP_ANGLE_X;
        if (cntEmcStopAngleY()) emcStop = STOP_ANGLE_Y;
        if (cntEmcStopEncStop()) emcStop = STOP_ENCODER_STOP;
        if (cntEmcStopLineSensor()) emcStop = STOP_LINESENSOR;
        
        courseMarker = checkMarker();   // マーカー検知
        checkGoalMarker();              // ゴールマーカー処理

        if (courseMarker == 2 && beforeCourseMarker == 0) cntMarker++;    // マーカーカウント
        beforeCourseMarker = courseMarker;
        
    }
    
    if (patternTrace < 10 || patternTrace > 100) {
        getSwitches();  // スイッチの入力を取得
        countDown();
        cntSetup1++;
        cntSetup2++;
        cntSwitchUD++;
        cntSwitchLR++;
    }

    if (modeLCD && initLCD) lcdShowProcess();   // LCD表示

    if ( modeCalLinesensors == 1) calibrationLinesensor();
    // 仮想センサステア計算
    getAngleSensor();
    // Encoder
    getEncoder();
    // PWM
    
    motorControlTrace();
    motorControlSpeed();

    switch(cnt5ms) {
        case 1:
            if (initIMU && useIMU) {
                // getBNO055Acceleration();    // 加速度取得       
                getBNO055Gyro();            // 角速度取得
                calcDegrees();              // 角度計算
                // motorControlYawRate();  // 角度制御
                // motorControlYaw();
                checkCurve();
            }
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
            if (initCurrent) {
                // getCurrent();               // 電流計測
            }

            if (modeLOG) writeLogBuffer(
                14,
                cntLog,
                getMarkerSensor(),
                encCurrentN,
                (int32_t)(angularVelocity[INDEX_Z]*10000),
                encCurrentR,
                encCurrentL,
                // encTotalR,
                // encTotalL,
                encTotalN,
                (int32_t)(angleSensor*10),
                modeCurve,
                motorpwmR,
                motorpwmL,
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
                // (int32_t)(angularVelocity[INDEX_X]*10000),
                // (int32_t)(angularVelocity[INDEX_Y]*10000),
                // (int32_t)(angle[INDEX_X]*10000),
                // (int32_t)(angle[INDEX_Y]*10000),
                (int32_t)(angle[INDEX_Z]*10000),
                // rawCurrentR,
                // rawCurrentL,
                // (int32_t)(calcCurvatureRadius((float)encCurrentN, angularVelocity[INDEX_Z]) * 100)
                targetSpeed,
                cntMarker
                );
            cnt10ms = 0;
            break;
        default:
            break;
    }
}
/////////////////////////////////////////////////////////////////////
// モジュール名 Interrupt100us
// 処理概要     タイマー割り込み(0.1ms)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void Interrupt100us(void) {
    // Interrupt 0.1ms
    if (modeLOG) writeLogPut();
}