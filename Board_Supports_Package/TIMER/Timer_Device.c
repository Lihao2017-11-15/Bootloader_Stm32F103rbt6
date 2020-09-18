#include "Timer_Device.h"
#include "Timer_Driver.h"

/****************************** PRIVATE VARIABLES *****************************/
static volatile uint32_t vu32_TimerTick = 0;				    // ϵͳʱ��(�����޸�)
static volatile uint32_t vu32_TimerCounter = 0;	   			// ϵͳʱ��(���޸�)
static struct_TaskData_t timer_task[TIM_TASK_NUM];	 	// ��ʱ��������¼�����

void SystemTimer_Init(void)
{
    TIM3_PortInit(9, 7199);//1ms��׼
}


void TIM3_IRQHandler(void)   //TIM3�ж�
{
    uint8_t i=0;
    
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
    {
        vu32_TimerTick++;
        vu32_TimerCounter++;

		for(i=0;i<TIM_TASK_NUM;i++) 	//�������
		{
			if(timer_task[i].vu32Time_Out != 0)	//����������
			{
				timer_task[i].vu32Time_Count++;	//��Ӧ����ʱ�����
				//�������趨��ʱ��㵽�ˣ�ִ�лص�����
				if(timer_task[i].vu32Time_Out == timer_task[i].vu32Time_Count)
				{
					timer_task[i].vu32Time_Count=0;     //��λ��ʱʱ��
					if(timer_task[i].pCallbackFunction != NULL)  //�ص���������
						timer_task[i].pCallbackFunction();     //ִ�лص�����
				}
			}
		}
        
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
    }
}

//================================ ����ʱ������ =================================
/*************************************************************
��������:	bool Timer_AddTask(uint32_t , uint32_t ,  TimTaskCall*  )
��    ��:	���ĳ����ʱ������
��ڲ���:	uint32_t ����ID��  uint32_t ��ʱʱ��
			FUNCTION* �ص�����ָ��
���ó���:
�� �� ֵ:	bool��ӳɹ�����TRUE
��    ��:	Jason			         ʱ    ��:   2011-7-28
��    ��:                            ʱ    ��:
*************************************************************/
bool Timer_AddTask(uint32_t fu32_Name,  uint32_t timeout, TimTaskCall* callbackfunction)
{
	bool lb_AddSuccess=false;
	
    //Ϊ��ֹ�ظ���������������ǰ���������ͬһ���ֵ�����
	Timer_TaskClr(fu32_Name);
	//�������б����������
	if((fu32_Name!=0)&&(timeout!=0))
	{
        if(timer_task[fu32_Name].vu32Time_task_name==0)  //���������Ĵ������������������
        {
            timer_task[fu32_Name].vu32Time_task_name = fu32_Name;
            timer_task[fu32_Name].vu32Time_Out=timeout;
            timer_task[fu32_Name].vu32Time_Count=0;
            timer_task[fu32_Name].pCallbackFunction=callbackfunction;
            lb_AddSuccess=true;
        }
	}
	return(lb_AddSuccess);
}
/*************************************************************
��������:	bool Timer_TaskClr(uint32_t ID)
��    ��:	���ĳ����ʱ������	
��ڲ���:	uint32_t ����ID�� 
���ó���:
�� �� ֵ:	bool����ɹ�����TRUE
��    ��:	Jason			         ʱ    ��:   2011-7-28
��    ��:                            ʱ    ��:
*************************************************************/
bool Timer_TaskClr(uint32_t fu32_Name)
{
	bool lb_ClrSuccess=false;
    
	if(fu32_Name!=0)
	{   
        //�����������Ʋ�����Ӧ�������
        timer_task[fu32_Name].vu32Time_task_name=0;
        timer_task[fu32_Name].vu32Time_Out=0;
        timer_task[fu32_Name].vu32Time_Count=0;
        timer_task[fu32_Name].pCallbackFunction=NULL;
        lb_ClrSuccess=true;
	}
	return (lb_ClrSuccess);
}
/*************************************************************
��������:	bool Timer_TaskRest(uint32_t  )
��    ��:	��ʱ������λ����������
��ڲ���:	uint32_t ����ID�� 
���ó���:
�� �� ֵ:	bool��λ�ɹ�����TRUE
��    ��:	Jason			         ʱ    ��:   2011-7-28
��    ��:                            ʱ    ��:
*************************************************************/
bool Timer_TaskReset(uint32_t fu32_Name)
{
	bool lb_RstSuccess=false;
    
	if(fu32_Name!=0)
    {
        if(timer_task[fu32_Name].vu32Time_Out!=0)
        {
            timer_task[fu32_Name].vu32Time_Out=0;		
            lb_RstSuccess=true;
        }				
	}
	return(lb_RstSuccess);
}
/*************************************************************
��������:	bool Timer_TaskClrALL()
��    ��:	������ж�ʱ������
��ڲ���:	
���ó���:
�� �� ֵ:	bool����ɹ�����TRUE
��    ��:	Jason			         ʱ    ��:   2011-7-28
��    ��:                            ʱ    ��:
*************************************************************/
bool Timer_TaskClrALL(void)
{
	uint8_t i;
    
	for(i=0;i<TIM_TASK_NUM;i++)
	{
		timer_task[i].vu32Time_task_name=0;
		timer_task[i].vu32Time_Out=0;
		timer_task[i].vu32Time_Count=0;
		timer_task[i].pCallbackFunction=NULL;
	}
	return(true);
}
//============================== ��ʱ���������� ===============================
/*************************************************************
��������:	SysTimeGet
��    ��:	��ȡϵͳʱ��
��ڲ���:	
���ó���:
�� �� ֵ:	ϵͳʱ��
��    ��:	Jason			         ʱ    ��:   2011-7-28
��    ��:                            ʱ    ��:
*************************************************************/
uint32_t SysTimeGet(void)
{
    return vu32_TimerTick;
}
/*************************************************************
��������:	uint32_t ReadTimerCount(void)
��    ��:	��ȡ���붨ʱ���жϵĴ���,����ʱ�����
��ڲ���:	void 
���ó���:
�� �� ֵ:	uint32_t ���붨ʱ���Ĵ���
��    ��:			         ʱ    ��:   2016-6-28
��    ��:                    ʱ    ��:
*************************************************************/
uint32_t ReadTimerCount(void)   
{
	return vu32_TimerCounter; //���� ��ʱ��ʱ�����
}
/*************************************************************
��������:	void SetTimerCount(uint32_t DesiredValue)
��    ��:	���ý���������ڲ�����
��ڲ���:	uint32_t �趨ֵ 
���ó���:
�� �� ֵ:	
��    ��:			         ʱ    ��:   2016-6-28
��    ��:                    ʱ    ��:
*************************************************************/
void SetTimerCount(uint32_t fu32_DesiredValue)
{
	vu32_TimerCounter = fu32_DesiredValue; //���� ��ʱ��ʱ�����
}

