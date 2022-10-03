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

    if (pwmL > 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwmL);
    } else if (pwmL == 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1000);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 1000);
    } else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, abs(pwmL));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    }
    
    if (pwmR > 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwmR);
    } else if (pwmR == 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 1000);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 1000);
    } else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, abs(pwmR));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
    }

}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorPwmOutSynth
// 処理概要     トレースと速度制御のPID制御量をPWMとしてモータに出力する
// 引数         tPwm: トレースのPID制御量 sPwm: 速度のPID制御量
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorPwmOutSynth(int16_t tPwm, int16_t sPwm) {
	int16_t pwmR, pwmL;

	if (tPwm > 0) {
		pwmR = sPwm - abs(tPwm);
		pwmL = sPwm + abs(tPwm);
	} else {
		pwmR = sPwm + abs(tPwm);
		pwmL = sPwm - abs(tPwm);
	}
	motorPwmOut(pwmL, pwmR);
}