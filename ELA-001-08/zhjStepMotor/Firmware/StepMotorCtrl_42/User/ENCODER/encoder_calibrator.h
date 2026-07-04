#ifndef ENCODER_CALIBRATOR_H
#define ENCODER_CALIBRATOR_H

#include <stdint.h>
#include <stdbool.h>

/* 初始化校准器 */
void EncoderCalibrator_Init(void);

/* 20kHz中断中调用 */
void EncoderCalibrator_Tick20kHz(void);

/* 主循环中调用 */
void EncoderCalibrator_TickMainLoop(void);

/* 触发校准（外部调用）*/
void EncoderCalibrator_Trigger(void);

/* 检查是否已校准 */
bool EncoderCalibrator_IsCalibrated(void);

/* 获取校准后的角度（查表）*/
uint16_t EncoderCalibrator_GetRectifiedAngle(uint16_t raw_angle);

bool EncoderCalibrator_IsTriggered(void);

#endif
