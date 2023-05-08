#include "Gimbal_task.h"
#include "cmsis_os.h"
#include "main.h"
#include "State.h"
#include "can_drv.h"

char mode = 0;
int16_t current[4];
char action = 0;

float angle_tar = 0;
float speed_tar = 0;
float output = 0;

float wspeed;
float wangle;
float woutput;
float wout;


uint16_t time;

void Start_gimbal_Task(void const * argument)
{
	GIMBAL_INIT();
	
	for(;;)
	{	
		if(mode == 0)time = HAL_GetTick();
			
		if(mode == 1){
		
		
			Gimb_Motor[GIMB_Y].pid->Mec_Out_Pid.target = angle_tar;
		
			motor_pid_t *pid = Gimb_Motor[GIMB_Y].pid;
		
			pid->out = PID_CAL(&pid->Mec_Out_Pid,
												 &pid->Mec_Inn_Pid,
												 Gimb_Motor[GIMB_Y].info->angle, Gimb_Motor[GIMB_Y].info->speed, 1);

			current[Gimb_Motor[GIMB_Y].driver->drv_id] = Gimb_Motor[GIMB_Y].pid->out;
		
			if(action)Send_Current(GIMBA_CAN, GIMBA_ID, current);		
		
		}
		else if(mode == 2){
		
			Gimb_Motor[GIMB_Y].pid->Mec_Inn_Pid.target = speed_tar;
		
			motor_pid_t *pid = Gimb_Motor[GIMB_Y].pid;
		
			pid->out = PID_CAL(&pid->Mec_Inn_Pid, NULL,
												 Gimb_Motor[GIMB_Y].info->speed, NULL, 1);

			current[Gimb_Motor[GIMB_Y].driver->drv_id] = Gimb_Motor[GIMB_Y].pid->out;
		
			if(action)Send_Current(GIMBA_CAN, GIMBA_ID, current);		
		
		}
		else if(mode == 3){
			
			output = 3000;
			
			current[Gimb_Motor[GIMB_Y].driver->drv_id] = output;
		
			if(action)Send_Current(GIMBA_CAN, GIMBA_ID, current);		
			
			if(HAL_GetTick() - time > 5000){
			
					mode = 0;
					output = 0;
			}
		
		}

		
		wangle = Gimb_Motor[GIMB_Y].info->angle;
		wspeed = Gimb_Motor[GIMB_Y].info->speed;
		woutput = output;
		wout = Gimb_Motor[GIMB_Y].pid->out;
		
		osDelay(1);
	}
}


