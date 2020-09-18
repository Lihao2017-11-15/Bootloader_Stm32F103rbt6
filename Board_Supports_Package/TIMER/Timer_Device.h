#ifndef __TIMER_DEVICE_H
#define __TIMER_DEVICE_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef void TimTaskCall(void);//�ص��������Ͷ���
/*******************************************************************************
��ע�ͣ��ṹ�嶨������
*******************************************************************************/
typedef struct{
	volatile uint32_t vu32Time_task_name;	//�������ƣ������ƻ���ID���룩
	volatile uint32_t vu32Time_Out;			//��ʱʱ��
	volatile uint32_t vu32Time_Count;		//��������Ϊֹ����ǰ��ʱ��
	TimTaskCall *pCallbackFunction;	        //��ʱ�ص�����ָ��
}struct_TaskData_t;							//�������ִ����������Ҫ������	
/*******************************************************************************
��ע�ͣ���ʱ������ӿ�
*******************************************************************************/
typedef enum
{
    No_Task_ID = 0,
    
    PowerUpClrZero_TASK,
    BEEP_TASK,
    LED_COMx_TASK,
    LED_GPRS_TASK,
    CalAlarmInfo_TASK,
    
    TIM_TASK_NUM,
}enum_TimTask_t;



extern void SystemTimer_Init(void);
extern uint32_t SysTimeGet(void);
extern uint32_t ReadTimerCount(void);				        // ��ȡ���붨ʱ���жϵĴ���,����ʱ�����
extern void SetTimerCount(uint32_t DesiredValue);		    // ���ý���������ڲ�����
//����ʱ������
extern bool Timer_AddTask(uint32_t Name, uint32_t timeout, TimTaskCall* callbackfunction);//���ĳ����ʱ������
extern bool Timer_TaskClr(uint32_t Name);		            // ���ĳ����ʱ������	
extern bool Timer_TaskReset(uint32_t Name);					// ��ʱ������λ����������	
extern bool Timer_TaskClrALL(void);							// ������ж�ʱ������

#endif
