//====================================//
// インクルード
//====================================//
#include "motor.h"
//====================================//
// グローバル変数の宣言
//====================================//

/////////////////////////////////////////////////////////////////////
// モジュール名 motorPwmOut
// 処理概要     左右のモータにPWMを出力する
// 引数         pwmL: 左モータへの出力(1~1000) pwmR: 右モータへの出力(1~1000)
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void motorPwmOut(int16_t pwmL, int16_t pwmR) {

    if (abs(pwmL) < 10) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 10);
    } else if (pwmL > 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwmL);
    } else if (pwmL < 0){
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, abs(pwmL));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    }
    
    if (abs(pwmR) < 10) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 10);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
    } else if (pwmR > 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwmR);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
    } else if (pwmR < 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, abs(pwmR));
    }

}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorPwmOutSynth
// 処理概要     トレースと速度制御のPID制御量をPWMとしてモータに出力する
// 引数         tPwm: トレースのPID制御量 sPwm: 速度のPID制御量
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorPwmOutSynth(int16_t tPwm, int16_t sPwm) {
	int16_t pwmR, pwmL, overpwm;

    if (sPwm >= 900) {
        sPwm = 900;
    } else if (sPwm <= -900) {
        sPwm = -900;
    }

    if (sPwm + tPwm > 1000 || sPwm - tPwm < -1000) {
        // 合計制御量が1000を超えたとき
        overpwm = abs(sPwm) + abs(tPwm) - 1000; // 1000を超えた分の制御量を計算
        if (tPwm > 0) {
            pwmR = sPwm - tPwm - (overpwm * (tPwm/abs(tPwm)));
            pwmL = 1000 * (tPwm/abs(tPwm));
        } else {
            pwmR = 1000 * (tPwm/abs(tPwm));
            pwmL = sPwm + tPwm + (overpwm * (tPwm/abs(tPwm)));
        }
    } else {
        pwmR = sPwm - tPwm;
	    pwmL = sPwm + tPwm;
    }
	motorPwmOut(pwmL, pwmR);
}