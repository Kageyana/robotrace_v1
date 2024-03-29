//====================================//
// インクルード
//====================================//
#include "motor.h"
//====================================//
// グローバル変数の宣言
//====================================//
int16_t motorpwmL = 0;
int16_t motorpwmR = 0;
/////////////////////////////////////////////////////////////////////
// モジュール名 motorPwmOut
// 処理概要     左右のモータにPWMを出力する
// 引数         pwmL: 左モータへの出力(1~1000) pwmR: 右モータへの出力(1~1000)
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void motorPwmOut(int16_t pwmL, int16_t pwmR) {

    if (abs(pwmL) < 10) {
        // スリープモードへの移行を防ぐためにduty比を10にする
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (int16_t)((float)MOTOR_COUNTERPERIOD*0.02));
    } else if (pwmL > 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (int16_t)((float)pwmL/1000*MOTOR_COUNTERPERIOD));
    } else if (pwmL < 0){
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (int16_t)((float)abs(pwmL)/1000*MOTOR_COUNTERPERIOD));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    }
    
    if (abs(pwmR) < 10) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, (int16_t)((float)MOTOR_COUNTERPERIOD*0.02));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
    } else if (pwmR > 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, (int16_t)((float)pwmR/1000*MOTOR_COUNTERPERIOD));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
    } else if (pwmR < 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (int16_t)((float)abs(pwmR)/1000*MOTOR_COUNTERPERIOD));
    }

}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorPwmOutSynth
// 処理概要     トレースと速度制御のPID制御量をPWMとしてモータに出力する
// 引数         tPwm: トレースのPID制御量 sPwm: 速度のPID制御量
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorPwmOutSynth(int16_t tPwm, int16_t sPwm, int16_t yrPwm, int16_t yPwm) {
	int16_t overpwm;

    if (abs(sPwm + tPwm) > 1000 || abs(sPwm - tPwm) > 1000) {
        // ライントレースと速度制御の合計制御量が1000を超えたとき
        overpwm = abs(sPwm) + abs(tPwm) - 1000; // 1000を超えた分の制御量を計算

        // トレースの内輪側から越えた分の制御量を引く 正負はtPwmと同じ
        if (tPwm > 0) {
            if (sPwm > 0) {
                motorpwmR = sPwm - tPwm - (overpwm * (tPwm/abs(tPwm))) - yrPwm - yPwm;
                motorpwmL = 1000;
            } else {
                motorpwmR = -1000;
                motorpwmL = sPwm + tPwm + (overpwm * (tPwm/abs(tPwm))) + yrPwm + yPwm;
            }
        } else {
            if (sPwm > 0) {
                motorpwmR = 1000;
                motorpwmL = sPwm + tPwm + (overpwm * (tPwm/abs(tPwm))) + yrPwm + yPwm;
            } else {
                motorpwmR = sPwm - tPwm - (overpwm * (tPwm/abs(tPwm))) - yrPwm - yPwm;
                motorpwmL = -1000;
            }
            
        }
    } else {
        motorpwmR = sPwm - tPwm - yrPwm - yPwm;
	    motorpwmL = sPwm + tPwm + yrPwm + yPwm;
    }
    
	motorPwmOut(motorpwmL, motorpwmR);
}