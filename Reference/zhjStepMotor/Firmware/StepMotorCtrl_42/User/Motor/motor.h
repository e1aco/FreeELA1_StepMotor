#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "motion_planner.h"

/* 电机常量 */
#define MOTOR_HARD_STEPS          200
#define SOFT_DIVIDE_NUM           256
#define MOTOR_SUBDIVIDE_STEPS     (MOTOR_HARD_STEPS * SOFT_DIVIDE_NUM)  /* 51200 */

/* 电机模式 */
typedef enum {
    MODE_STOP,
    MODE_COMMAND_POSITION,
    MODE_COMMAND_VELOCITY,
    MODE_COMMAND_CURRENT,
    MODE_COMMAND_TRAJECTORY,
    MODE_PWM_POSITION,
    MODE_PWM_VELOCITY,
    MODE_PWM_CURRENT,
    MODE_STEP_DIR
} Motor_Mode_t;

/* 电机状态 */
typedef enum {
    STATE_STOP,
    STATE_FINISH,
    STATE_RUNNING,
    STATE_OVERLOAD,
    STATE_STALL,
    STATE_NO_CALIB
} Motor_State_t;

/* PID 结构体 */
typedef struct {
    int32_t kp, ki, kd;
    int32_t vError, vErrorLast;
    int32_t outputKp, outputKi, outputKd;
    int32_t integralRound;
    int32_t integralRemainder;
    int32_t output;
} PID_t;

/* DCE 结构体 */
typedef struct {
    int32_t kp, kv, ki, kd;
    int32_t pError, vError;
    int32_t outputKp, outputKi, outputKd;
    int32_t integralRound;
    int32_t integralRemainder;
    int32_t output;
} DCE_t;

/* 控制器配置 */
typedef struct {
    PID_t pid;
    DCE_t dce;
    bool stallProtectSwitch;
} Controller_Config_t;

/* 电机配置 */
typedef struct {
    MotionPlanner_Config_t motionParams;
    Controller_Config_t ctrlParams;
} Motor_Config_t;

/* 初始化电机系统 */
void Motor_Init(void);

/* 设置配置指针 */
void Motor_SetConfig(Motor_Config_t* config);

/* 20kHz 中断中调用 */
void Motor_Tick20kHz(void);

/* 控制接口 */
void Motor_SetMode(Motor_Mode_t mode);
void Motor_SetPosition(int32_t pos);
void Motor_SetVelocity(int32_t vel);
void Motor_SetCurrent(int32_t cur);
void Motor_SetDisable(bool disable);
void Motor_SetBrake(bool brake);
void Motor_ClearStallFlag(void);

/* 状态读取 */
Motor_State_t Motor_GetState(void);
float Motor_GetPosition(bool isLap);
float Motor_GetVelocity(void);
float Motor_GetCurrent(void);
bool Motor_IsCalibrated(void);
uint8_t Motor_GetMode(void);

/* 编码器校准接口 */
bool Motor_IsCalibrated(void);
void Motor_TriggerCalibration(void);

void Motor_GetTelemetry(float *pos, float *vel, float *cur, uint8_t *mode, uint8_t *state);

void Motor_ZeroPosition(void);

#endif

