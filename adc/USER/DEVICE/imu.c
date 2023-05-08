#include "imu.h"
#include "gpio_drv.h"
void imu_init(struct imu_struct *self);
void imu_heart_beat(heart_t *heart);
void imu_updata(struct imu_struct *self);

struct bmi2_dev bmi270;

bmi_t bmi_client = {

	.dev = &bmi270,
	.init = &bmi_init,	

};


imu_t imu = {

	.bmi = &bmi_client,
	.driver.tpye = D_SPI2,
	.heart.state = DEV_OFFLINE,
	.id = DEV_ID_IMU,	
	.info.init_flag = 0,
	.heart.offline_max_cnt = 50,	
	
	.init = &imu_init,
	.updata = &imu_updata,
  .heart_beat = &imu_heart_beat,
};


void imu_init(struct imu_struct *self)
{
	int8_t rslt;
	uint32_t tickstart = HAL_GetTick();

	self->heart.state = DEV_OFFLINE;

	self->bmi->read = bmi2_get_regs;
  self->bmi->write = bmi2_set_regs;
	
	if(self->driver.tpye == D_SPI1 | self->driver.tpye == D_SPI2 | self->driver.tpye == D_SPI3){

		self->bmi->drive_type = BMI2_SPI_INTF;

	}
	else if(self->driver.tpye == D_IIC){
	
		self->bmi->drive_type = BMI2_I2C_INTF;
		
	}
	
	rslt = self->bmi->init(self->bmi->dev,self->bmi->drive_type);
	

	while(rslt)
	{
        self->heart.state = DEV_INIT_ERR;
        rslt = self->bmi->init(self->bmi->dev,self->bmi->drive_type);
	}	

	self->heart.state = DEV_ONLINE;
	
//	for(uint16_t i=0; i<250; i++) {
////		BMI_Get_GRO(&self->info.rate_pitch, &self->rate_roll, &self->rate_yaw);
//		self->info.acc_gyro.rate_pitch_offset += self->info.acc_gyro.rate_pitch_offset;
//		self->info.acc_gyro.rate_yaw_offset += self->info.acc_gyro.rate_yaw_offset;
//	}
//	self->info.acc_gyro.rate_pitch_offset/= 250.f;
//	self->info.acc_gyro.rate_yaw_offset /= 250.f;
	
	self->info.init_flag = 1;
}

void imu_updata(struct imu_struct *self){







}


void imu_heart_beat(heart_t *heart){


	heart->offline_cnt++;
	if(heart->offline_cnt > heart->offline_max_cnt) 
	{
		heart->offline_cnt = heart->offline_max_cnt;
		heart->state = DEV_OFFLINE;
	}
	else 
	{
		if(heart->state == DEV_OFFLINE)
			heart->state = DEV_ONLINE;
	}
	heart->state = DEV_ONLINE;

}

int16_t buff[6];
struct bmi2_sens_data imu_data;

int32_t t1,t2,tf;
float r_f;
extern SPI_HandleTypeDef hspi2;
void Get_IMU_Data(void){

	uint8_t len=12,reg;
//	

//	bmi2_get_sensor_data(&imu_data, imu.bmi->dev);
////int8_t bmi2_get_regs(uint8_t reg_addr, uint8_t *data, uint16_t len, struct bmi2_dev *dev)	

	
	
	
		uint8_t da[len + 1],data[12];
	
		BMI_CS_LOW();
	
	  reg = 0x0c|0x80;
		HAL_SPI_Transmit(&hspi2, &reg,  1, 1000);
		HAL_SPI_Receive(&hspi2, da, len+1, 1000);
		
		for(uint8_t i=0;i<len+1;i++){
		
			data[i] = da[i+1];
		
		}
		BMI_CS_HIG();
		
	buff[0] = (int16_t)data[0] | ( (int16_t)data[1] << 8);
	buff[1] = (int16_t)data[2] | ( (int16_t)data[3] << 8);
	buff[2] = (int16_t)data[4] | ( (int16_t)data[5] << 8);
	
	buff[3] = (int16_t)data[6] | ( (int16_t)data[7] << 8);
	buff[4] = (int16_t)data[8] | ( (int16_t)data[9] << 8);
	buff[5] = (int16_t)data[10] | ( (int16_t)data[11] << 8);		
		
}

volatile uint32_t start,end,fps;
volatile uint32_t stay_time = 100;
volatile uint32_t out,inn;
volatile int32_t stay;
uint32_t *addl;
uint8_t daa1,daa2;
void BMI_DATA(void)
{
//	uint8_t aaa = 0x2c,bbb = 0xac;
	
//	imu.bmi->write(BMI2_ACC_CONF_ADDR,&aaa,1,imu.bmi->dev);
//	imu.bmi->read(BMI2_ACC_CONF_ADDR,&daa1,1,imu.bmi->dev);


//	imu.bmi->write(BMI2_GYR_CONF_ADDR,&bbb,1,imu.bmi->dev);
//	imu.bmi->read(BMI2_GYR_CONF_ADDR,&daa2,1,imu.bmi->dev);
	

		inn = micros();
		Get_IMU_Data();



		
}


extern TIM_HandleTypeDef htim4;
//1->1us;
void TIM4_Init_Handle(uint16_t timeout)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = timeout * 84/2 - 1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 2 - 1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
//    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
//    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
//    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */
	
  /* USER CODE END TIM4_Init 2 */
	
}






