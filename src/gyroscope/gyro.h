#pragma once
#include <common/utils.h>

using namespace std;

#define GYRO_DEVICE_ADDR 0x68
#define PWR_MGMT_1 0x6B

// ==== MODULE ====
void gyro_init();
void gyro_cleanup();
bool gyro_isActive();

// ==== RAW VALUES =====
float get_xGyro_raw();
float get_yGyro_raw();
float get_zGyro_raw();

// ======= YAW =========
float getYaw();
//float getAbsYaw();
//static float prev_yaw = 0;
void calculateAngle();
//bool is90();

// reset yaw to 0
void resetYaw();