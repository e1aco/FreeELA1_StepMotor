#ifndef MOTION_PLANNER_H
#define MOTION_PLANNER_H

#include <stdint.h>
#include <stdbool.h>

/* 控制频率 */
#define CONTROL_FREQUENCY  20000   /* Hz */
#define CONTROL_PERIOD_US  50      /* 微秒 */

/* ==================== 配置 ==================== */
typedef struct {
    int32_t encoderHomeOffset;  // 编码器零位偏移量
    int32_t caliCurrent; // 校准电流
    int32_t ratedCurrent; // 额定电流
    int32_t ratedVelocity; // 额定速度
    int32_t ratedVelocityAcc; // 速度变化加速度
    int32_t ratedCurrentAcc; // 电流变化加速度
} MotionPlanner_Config_t;

/* 全局配置指针（需要在c文件中赋值）*/
extern MotionPlanner_Config_t* g_motion_config;

/* ==================== CurrentTracker ==================== */
extern int32_t g_go_current;

void CurrentTracker_Init(void);
void CurrentTracker_SetCurrentAcc(int32_t currentAcc);
void CurrentTracker_NewTask(int32_t realCurrent);
void CurrentTracker_CalcSoftGoal(int32_t goalCurrent);

/* ==================== VelocityTracker ==================== */
extern int32_t g_go_velocity;

void VelocityTracker_Init(void);
void VelocityTracker_SetVelocityAcc(int32_t velocityAcc);
void VelocityTracker_NewTask(int32_t realVelocity);
void VelocityTracker_CalcSoftGoal(int32_t goalVelocity);

/* ==================== PositionTracker ==================== */
extern int32_t g_go_location;
extern int32_t g_go_location_velocity;

void PositionTracker_Init(void);
void PositionTracker_SetVelocityAcc(int32_t value);
void PositionTracker_NewTask(int32_t realLocation, int32_t realSpeed);
void PositionTracker_CalcSoftGoal(int32_t goalPosition);

/* ==================== PositionInterpolator ==================== */
extern int32_t g_interp_go_position;
extern int32_t g_interp_go_velocity;

void PositionInterpolator_Init(void);
void PositionInterpolator_NewTask(int32_t realPosition, int32_t realVelocity);
void PositionInterpolator_CalcSoftGoal(int32_t goalPosition);

/* ==================== TrajectoryTracker ==================== */
extern int32_t g_traj_go_position;
extern int32_t g_traj_go_velocity;

void TrajectoryTracker_Init(int32_t updateTimeout);
void TrajectoryTracker_SetSlowDownVelocityAcc(int32_t value);
void TrajectoryTracker_NewTask(int32_t realLocation, int32_t realSpeed);
void TrajectoryTracker_CalcSoftGoal(int32_t goalPosition, int32_t goalVelocity);

#endif

