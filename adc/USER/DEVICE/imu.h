#ifndef __IMU_H
#define __IMU_H

#include "BMI.h"

typedef enum {
		DEV_ID_NONE,
    DEV_ID_IMU, 

		DEV_ID_CNT,
} dev_id_t;

typedef enum {
    DEV_ONLINE,
    DEV_OFFLINE,
	
		DEV_TYPE_ERR,
    DEV_ID_ERR,
    DEV_INIT_ERR,	
    DEV_DATA_ERR,
} dev_state_t;



typedef enum{
	D_CAN1,
	D_CAN2,
	
	D_SPI1,
	D_SPI2,
	D_SPI3,
	
	D_IIC,
	
	D_USART1,
	D_USART2,
	D_USART3,	
	
}drive_t;

typedef struct drive_str{
	drive_t tpye;
	int8_t (*send)(struct drive_str *self,uint8_t *Txbuff,uint16_t len);
	int8_t (*read)(struct drive_str *self,uint8_t *Rxbuff,uint16_t len);
}drv_t;


typedef struct heart_struct {

	dev_state_t	 state;
	
	uint8_t   offline_cnt;
	uint8_t   offline_max_cnt;
	
} heart_t;







typedef struct{
	float imu_center;
	float imu_angle_sum;
	float imu_angle_pre;
}flag_longtime_t;

typedef enum{
	pitch_longtime,
	yaw_longtime,
	IMU_CNT,
}imu_cnt_t;

typedef struct{
	float yaw;
	float pitch;
	float roll;
	
	short rate_yaw;
	short rate_pitch;
	short rate_roll;	
	
	float rate_yaw_offset;
	float rate_pitch_offset;	
	float rate_roll_offset;

}acc_gyro_t;


typedef struct imu_info_struct {

	acc_gyro_t acc_gyro;

	flag_longtime_t Longtime_imu_Array[IMU_CNT];
	
	uint8_t		init_flag;

} imu_info_t;



typedef struct imu_struct {
	
  bmi_t       *bmi;	
	imu_info_t	info;
	drv_t			  driver;
	
	void				(*init)(struct imu_struct *self);
	void				(*updata)(struct imu_struct *self);
	void				(*heart_beat)(struct heart_struct *self);
	
	heart_t      heart;
	
	dev_id_t		 id;	
} imu_t;

void Get_IMU_Data(void);
void TIM4_Init_Handle(uint16_t timeout);
void BMI_DATA(void);

extern imu_t imu;

#endif


