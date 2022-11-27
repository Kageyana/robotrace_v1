//====================================//
// インクルード
//====================================//
#include "timer.h"
//====================================//
// グローバル変数の宣
//====================================//
uint32_t    cntRun = 0;
uint32_t    cnt5ms = 0;
uint32_t    cntLog = 0;
// Emergency stop
uint16_t    cntEmc1 = 0;
uint16_t    cntAngleX = 0;
uint16_t    cntAngleY = 0;
/////////////////////////////////////////////////////////////////////
// モジュール名 HAL_TIM_PeriodElapsedCallback
// 処理概要     タイマー割り込み(1ms)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	cntRun++;
    cnt5ms++;
    cntLog++;
    // if (trace_test == 1 || (patternTrace > 10 && patternTrace < 100) ) {
    //     if (abs(yawPwm) > 400) {
    //         cntEmc1++;
    //     }
    // }
    if (trace_test == 1 || (patternTrace > 10 && patternTrace < 100) ) {
        if (fabs(angle[INDEX_X]) > 45.0) cntAngleX++;
        else    cntAngleX = 0;
        if (fabs(angle[INDEX_Y]) > 45.0) cntAngleY++;
        else    cntAngleY = 0;
    }
    if (patternTrace < 10 ||	 patternTrace > 100) {
        cntSW++;
        cntSetup1++;
        cntSetup2++;
        cntSetup3++;
    }

    // スイッチの入力を取得
    if( cntSW >= 100 ) {
        HAL_ADC_Start(&hadc2);
        HAL_ADC_PollForConversion(&hadc2, 1);
        swValTact = getSWtact(HAL_ADC_GetValue(&hadc2));

        HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 1);
        swValRotary = getSWrotary(HAL_ADC_GetValue(&hadc2));
        // HAL_ADC_Stop(&hadc2);
        cntSW = 0;
    }
    if (modeLCD == 1) lcdShowProcess();   // LCD
    
    cMarker = checkMarker();
    switch(cnt5ms) {
        case 1:

            break;
        case 2:
            // getCurrent();               // 電流計測
            // getBNO055Acceleration();    // 加速度取得       
            getBNO055Gyro();    // 角速度取得
            calcDegrees();
            motorControlYaw();
            break;
        case 3:
            
            if (modeLOG) writeLog();
            break;
        case 4:
            break;
        case 5:
            cnt5ms = 0;
            break;
        default:
            break;
    }

    // if (cntRun % 800 == 0) {
    //     printf("acceleValX %d\t acceleValY %d\t acceleValZ %d\n",acceleValX, acceleValY, acceleValZ);
        
    // }

    // 仮想センサステア計算
    getAngleSensor();
    // Encoder
    getEncoder();
    // PWM
    motorControlTrace();
    motorControlSpeed();
    
    
}
