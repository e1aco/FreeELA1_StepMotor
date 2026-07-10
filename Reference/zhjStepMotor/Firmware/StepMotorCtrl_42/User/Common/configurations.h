#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* ==================== 配置状态枚举 ==================== */
typedef enum {
    CONFIG_RESTORE = 0,
    CONFIG_OK,
    CONFIG_COMMIT
} configStatus_t;

/* ==================== 板卡配置结构体 ==================== */
typedef struct {
    configStatus_t configStatus;
    uint32_t canNodeId;
    int32_t encoderHomeOffset;
    uint32_t defaultMode;
    int32_t currentLimit;
    int32_t velocityLimit;
    int32_t velocityAcc;
    int32_t calibrationCurrent;
    int32_t dce_kp;
    int32_t dce_kv;
    int32_t dce_ki;
    int32_t dce_kd;
	  int32_t pid_kp;
    int32_t pid_ki;
    int32_t pid_kd;
    bool enableMotorOnBoot;
    bool enableStallProtect;
} BoardConfig_t;

/* ==================== 全局变量声明 ==================== */
extern BoardConfig_t boardConfig;

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATIONS_H */
