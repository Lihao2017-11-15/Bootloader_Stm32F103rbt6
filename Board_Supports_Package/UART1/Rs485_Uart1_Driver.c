#include "Rs485_Uart1_Driver.h"


void Uart1_Init(uint32_t BaudRate, uint16_t DataBits, uint16_t Parity, uint16_t StopBits)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

    //USART1_485EN	  GPIOA.8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    //USART1_485TX    GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_485RX	  GPIOA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,ENABLE);//��λ����1
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);//ֹͣ��λ

    //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = BaudRate;//���ڲ�����
    USART_InitStructure.USART_WordLength = DataBits;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = StopBits;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = Parity;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure);//��ʼ������1
    
    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;  //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		    //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			    //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    
    USART_Cmd(USART1, ENABLE);//ʹ�ܴ���1
}



/********************************************************************************
* ������ : Uart1_SendData
* ��  �� : ���ڷ�������
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart1_SendData(uint8_t *buf,uint8_t len)
{
	uint8_t i=0;

    GPIO_SetBits(GPIOA, GPIO_Pin_8);    //����Ϊ����ģʽ
    
  	for(i=0;i<len;i++)		//ѭ����������
	{
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); //�ȴ����ͽ���
        USART_SendData(USART1, buf[i]); //��������
	}
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET){} //�ȴ����ͽ���

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);  //����Ϊ����ģʽ
}
/********************************************************************************
* ������ : Uart1_PutChar
* ��  �� : ���ڷ���1byte����
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart1_PutChar(uint8_t c)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_8);    //����Ϊ����ģʽ
    
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET){} //�ȴ����ͽ���
    USART_SendData(USART1, c); //��������
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET){} //�ȴ����ͽ���

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);  //����Ϊ����ģʽ    
}
/********************************************************************************
* ������ : Uart1_PutString
* ��  �� : ���ڷ����ַ�������
* ˵  �� : ������Ҫ����̡��������Ϊ�ַ�����������ַ������飬ע��ĩβ'\0'��ֹ�����ܷ�
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.06.22
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart1_PutString(uint8_t *s)
{
    while (*s != '\0')
    {
        Uart1_PutChar(*s);
        s++;
    }
}
