//====================================//
// インクルード
//====================================//
#include "timer.h"
//====================================//
// グローバル変数の宣
//====================================//
uint32_t    cnt5ms = 0;
uint32_t    cnt10ms = 0;
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

    // LCD表示
    if (modeLCD && initLCD) lcdShowProcess();

    // 仮想センサステア計算
    getAngleSensor();

    // Encoder
    getEncoder();

    // PID制御処理
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
                if (!optimalTrace) checkCurve();
                if (crossLine == 1) {
                    BNO055val.gyro.z = 0.01;
                }
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

    // 走行中に処理
    if (patternTrace > 10 && patternTrace < 100) {
        // 緊急停止処理
        if (cntEmcStopAngleX()) emcStop = STOP_ANGLE_X;
        if (cntEmcStopAngleY()) emcStop = STOP_ANGLE_Y;
        if (cntEmcStopEncStop()) emcStop = STOP_ENCODER_STOP;
        if (cntEmcStopLineSensor()) emcStop = STOP_LINESENSOR;
        
        courseMarker = checkMarker();   // マーカー検知
        checkGoalMarker();              // ゴールマーカー処理

        // マーカーを通過した時
        if (courseMarker == 2 && beforeCourseMarker == 0) {
            cntMarker++;    // マーカーカウント
            if (optimalTrace == BOODT_DISTANCE) {
                int32_t i, j, errorDistance, upperLimit, lowerLimit;

                for(i=0;i<=numPPAMarry;i++) {
                    // 現在地から一番近いマーカーを探す
                    if (abs(encTotalN - markerPos[i].distance) < encMM(50)) {
                        errorDistance = encTotalN - distanceStart;  // 現在の差を計算
                        encTotalN = markerPos[i].distance;               // 距離を補正
                        distanceStart = encTotalN - errorDistance;  // 補正後の現在距離からの差分
                        optimalIndex = markerPos[i].indexPPAD;      // インデックス更新
                        break;
                    }
                }
            }
        }
        beforeCourseMarker = courseMarker;
        
    }
    
    // 走行前に処理
    if (patternTrace < 10 || patternTrace > 100) {
        getSwitches();  // スイッチの入力を取得
        countDown();
        cntSetup1++;
        cntSetup2++;
        cntSwitchUD++;
        cntSwitchLR++;

        motorControlYawRate();
        motorControlYaw();
    }

    switch(cnt10ms) {
        case 10:
            if (initCurrent) {
                // getCurrent();               // 電流計測
            }

            if (modeLOG) {
                writeLogBuffer(
                    12,
                    cntLog,
                    getMarkerSensor(),
                    encCurrentN,
                    (int32_t)(BNO055val.gyro.z*10000),
                    encTotalN,
                    targetSpeed,
                    // encCurrentR,
                    // encCurrentL,
                    // encTotalR,
                    // encTotalL,
                    // (int32_t)(angleSensor*10),
                    // modeCurve,
                    motorpwmR,
                    motorpwmL,
                    // lineTraceCtrl.pwm,
                    // veloCtrl.pwm,
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
                    // (int32_t)(BNO055val.gyro.x*10000),
                    // (int32_t)(BNO055val.gyro.y*10000),
                    // (int32_t)(BNO055val.angle.x*10000),
                    // (int32_t)(BNO055val.angle.y*10000),
                    (int32_t)(BNO055val.angle.z*10000),
                    // rawCurrentR,
                    // rawCurrentL,
                    // (int32_t)(calcROC((float)encCurrentN, BNO055val.gyro.z) * 100)
                    cntMarker,
                    optimalIndex,
                    (int32_t)PPAD[optimalIndex].ROC
                );
            }
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