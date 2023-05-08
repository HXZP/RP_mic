#include "Chassis_task.h"
#include "cmsis_os.h"
#include "main.h"


/*
	底盘控制任务
	获取电容状态信息
*/
int16_t chas[4] = {500,0,0,0};
void Start_chassis_task(void const * argument)
{
	for(;;)
	{	
		if(CHASSIS_MD)
		CHASSIS_CTRL();
		
		osDelay(2);
	}
}


/*
	电容控制任务
*/
uint16_t cishu;
void Start_Super_Task(void const * argument)
{	
	for(;;)
	{
	
		if(CAP_MD)
		{
			CAP_RP_2022.ctrl();
		}
		
		
		osDelay(4);
	}
}



