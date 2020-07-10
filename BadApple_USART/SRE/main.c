/*******************************************************************************  
* 文件名称：LCD图片
* 实验目的：1.掌握LCD资源的使用方法
*           2.掌握LCD驱动库的使用方法
* 程序说明：使用程序前，确认LCD相关引脚已经通过跳线正确连接
      现象：LCD显示QQ企鹅图像
* 日期版本：2015-NUAA
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "lcd.h"
//#include "vvvvv.h"
//#include "553.h"
//#include "picture.h"
//#include "badappleL.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t TimingDelay = 0;
uint32_t TimingOut = 0;
uint8_t Flag_VedioRxE = 0;
uint8_t Flag_Buffer = 0;
uint8_t RxBuffer1[2048];
uint8_t RxBuffer2[2048];
uint8_t *RxBuffer = RxBuffer1;
uint8_t *RxBufferNow = RxBuffer1;
#define BufferSize 2048
/* Private function prototypes -----------------------------------------------*/
void Delay_Ms(uint32_t nTime);
/* Private functions ---------------------------------------------------------*/
void USART_Config(void);

uint16_t r, g, b;
void RGB(void)
{
	if (r == 0 && g != 0)
	{
		g -= 2;
		b++;
		if (b == 31)
		{
			r++;
			b--;
		}
	}
	else if (g == 0 && b != 0)
	{
		r++;
		b--;
		if (r == 31)
		{
			g += 2;
			r--;
		}
	}
	else if (b == 0 && r != 0)
	{
		g += 2;
		r--;
		if (g == 62)
		{
			g -= 2;
			b++;
		}
	}
}

void LCD_Clear_Half(u16 Color)
{
	uint32_t index = 0;
	LCD_SetCursor(0, 0);										//设置光标位置
	LCD_WriteRAM_Prepare();									//开始写入GRAM
	for (index = 0; index < 16800; index++) //240 * 180
	{
		LCD_WR_DATA(Color);
	}
	LCD_SetCursor(0, 250);									//设置光标位置
	LCD_WriteRAM_Prepare();									//开始写入GRAM
	for (index = 0; index < 16800; index++) //240 * 180
	{
		LCD_WR_DATA(Color);
	}
}

void LCD_DrawMyPicture(const uint8_t *img)
{
	uint8_t buf;
	uint16_t MyColor = (r << 11) | (g << 5) | b;
	uint32_t i, j;
	i = 5400;								//240*180/8
	LCD_SetCursor(0, 70);		//设置光标位置
	LCD_WriteRAM_Prepare(); //开始写入GRAM
	while (i--)
	{
		buf = *img;
		for (j = 0; j < 8; j++)
		{
			LCD_WR_DATA((buf & 0x80) ? ~MyColor : MyColor);
			buf <<= 1;
		}
		img++;
	}
}

typedef struct
{
	uint16_t Length;
	uint8_t StartColor;
	uint8_t x0;
	uint8_t x1;
	uint8_t x2;
	uint8_t x3;
	uint8_t x4;
} TurnColor_TypeDef;
#define HeadBitMap(bmap) ((TurnColor_TypeDef *)bmap)
#define TEST_COLOR(pimg) (pimg->Red > 128)
#define CMD_MODE_BIT 0x01
#define CMD_MODE_BIT_EXIT0 0x00
#define CMD_MODE_BIT_EXIT1 0xFF
#define CMD_MODE_LONG 0x02
#define MODE_LEN 0
#define MODE_BIT 1
void LCD_DrawTurnPicture(const uint8_t *datas)
{
	TurnColor_TypeDef *pHead;
	uint32_t i, j;
	uint32_t count;
	uint8_t colorNow;
	uint8_t colorLine;
	uint8_t mode;
	uint16_t Color;
	//uint16_t MyColor = (r<<11) | (g<<5) | b;
	//uint16_t NMyColor = ~MyColor;

	pHead = HeadBitMap(datas);
	i = 240 * 180;
	j = pHead->Length;
	mode = MODE_LEN;
	count = 0;
	colorLine = colorNow = pHead->StartColor;

	datas += sizeof(TurnColor_TypeDef);

	LCD_SetCursor(0, 70);		//设置光标位置
	LCD_WriteRAM_Prepare(); //开始写入GRAM

	do
	{
		if (mode == MODE_LEN)
		{
			if (*datas == CMD_MODE_BIT)
			{
				mode = MODE_BIT;
				count = 8;
			}
			else
			{
				if (*datas == CMD_MODE_LONG)
				{
					datas++;
					count = *((uint16_t *)datas);
					datas++;
					j += 2;
				}
				else
				{
					count = *datas;
				}
				if (i % 240 == 0)
					colorLine = colorNow;

				Color = colorNow ? 0xFFFF : 0x0000;
				while (count-- && i)
				{
					LCD_WR_DATA(Color);
					i--;
					if (i % 240 == 0)
					{
						colorNow = colorLine;
						Color = colorNow ? 0xFFFF : 0x0000;
					}
				}

				colorNow = !colorNow;
			}
		}
		else if (mode == MODE_BIT)
		{
			if (*datas == CMD_MODE_BIT_EXIT0)
			{
				mode = MODE_LEN;
				colorNow = 0;
				count = 0;
				if ((i % 240) < 8)
					colorLine = 0;
			}
			else if (*datas == CMD_MODE_BIT_EXIT1)
			{
				mode = MODE_LEN;
				colorNow = 1;
				count = 0;
				if ((i % 240) < 8)
					colorLine = 1;
			}
			else
			{
				colorNow = *datas;
				count = 8;

				while (count-- && i)
				{
					if (i % 240 == 0)
					{
						colorLine = (colorNow & 0x80) != 0;
					}
					LCD_WR_DATA((colorNow & 0x80) ? 0xFFFF : 0x0000);
					colorNow <<= 1;
					i--;
				}
			}
		}
		datas++;
		j++;
	} while (i && j);
}

uint8_t LED_Status = 0xFF;
uint32_t Timer_Frame = 0;
uint32_t Timer_Real = 0;
uint32_t Timer_Tick = 40;

void LED_Out(void)
{
	GPIOC->ODR = (uint16_t)~LED_Status << 8;
	GPIOD->BSRR = GPIO_BSRR_BS2;
	GPIOD->BRR = GPIO_BRR_BR2;
}
void LED_Move(void)
{
	if ((LED_Status & 0x7F) == 0)
		LED_Status = 1;
	else
		LED_Status <<= 1;
	LED_Out();
}

/**
  * @说明     主函数
  * @参数     None 
  * @返回值   None
  */
int main(void)
{
	uint32_t i;
	int32_t j;
	GPIO_InitTypeDef GPIO_InitStructure;
	//LCD工作模式配置
	SysTick_Config(SystemCoreClock / 1000); //1ms中断一次
	//Usart_DMA
	USART_Config();

	//LCD
	LCD_delay_ms(10);
	STM3210B_LCD_Init();
	LCD_delay_ms(100); //这个延时比较重要！100以上

	LCD_SetTextColor(Red);
	LCD_SetBackColor(White);
	LCD_Clear(White);
	LCD_Clear_Half(Black);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); //GPIOD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //GPIOA

	r = 31;
	g = 0;
	b = 0;
	i = 0;
	j = 1;
	//USART2->DR = 0xAA;
	while (1)
	{
		if(TimingOut == 0)
		{
			//等待接收超时
			Flag_VedioRxE = 0;
			//超时重发重置实时时间
			Timer_Frame = 0;
			Timer_Real = 0;
		}
		if (TimingDelay == 0)
		{
			if (Timer_Frame < Timer_Real)
				TimingDelay = 0;
			else
				TimingDelay = 40;

			RGB();
			GPIOA->BRR = GPIO_BRR_BR1;
			//LCD_Clear_Half((r<<11) | (g<<5) | b);
			GPIOC->ODR = 0xFF00;
			GPIOD->BSRR = GPIO_BSRR_BS2;
			GPIOD->BRR = GPIO_BRR_BR2;

			if (Flag_VedioRxE == 0)
			{
				//重置超时时间1s
				TimingOut = 1000;
				//交换Buffer
				Flag_Buffer = !Flag_Buffer;
				if (Flag_Buffer)
				{
					RxBuffer = RxBuffer1;
					RxBufferNow = RxBuffer2;
				}
				else
				{
					RxBuffer = RxBuffer2;
					RxBufferNow = RxBuffer1;
				}
				USART2->DR = (Timer_Frame / 1000);
				Flag_VedioRxE = 1;
				LCD_DrawTurnPicture(RxBuffer);
				LED_Move();
				Timer_Frame += 40;
			}

			if (i > 203)
				j = -1;
			else if (i == 0)
				j = 1;
			i += j;
			GPIOA->BSRR = GPIO_BSRR_BS1;
		}
	}
}

/**
  * @说明     延时函数
  * @参数     nTime: 延时时间
  * @返回值   None
  */
void Delay_Ms(uint32_t nTime)
{
	TimingDelay = nTime;
	while (TimingDelay != 0)
		;
}

void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USARTy RX DMA1 Channel (triggered by USARTy Rx event) Config */
	//  DMA_DeInit(DMA1_Channel6);
	//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
	//  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBufferNow;
	//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//  DMA_InitStructure.DMA_BufferSize = BufferSize;
	//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	//  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	//  DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	//  USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	//  DMA_Cmd(DMA1_Channel6, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = 460800;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	//空闲中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);

	//Clear RxBuffer
	*((uint16_t *)RxBuffer1) = 0x0A;
	RxBuffer1[2] = 0x01;
	RxBuffer1[8] = 0x02;
	RxBuffer1[9] = 0xC0;
	RxBuffer1[10] = 0xA7;
	*((uint16_t *)RxBuffer2) = 0x0A;
	RxBuffer1[2] = 0x01;
	RxBuffer2[8] = 0x02;
	RxBuffer2[9] = 0xC0;
	RxBuffer2[10] = 0xA7;
}

void ReceiveVideo(void)
{
	//uint32_t length = 0;
	//length = BufferSize - DMA1_Channel6->CNDTR;
	//if(Flag_VedioRxE)
	{
		//if(HeadBitMap(RxBufferNow)->Length <= length || length == 0)
		{
			//Stop DMA
			DMA1_Channel6->CCR &= (uint16_t)(~DMA_CCR1_EN);
			USART_DMACmd(USART2, USART_DMAReq_Rx, DISABLE);
			DMA_Cmd(DMA1_Channel6, DISABLE);
			//设置Buffer
			//DMA1_Channel6->CPAR = (uint32_t)RxBufferNow;
			//DMA_SetCurrDataCounter(DMA1_Channel6, BufferSize);
			DMA1_Channel6->CNDTR = BufferSize;
			//Start DMA
			USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
			//DMA_Cmd(DMA1_Channel6, ENABLE);
			DMA1_Channel6->CCR |= (uint16_t)(DMA_CCR1_EN);
			//USART2->DR = 0xAA;
			Flag_VedioRxE = 0;
		}
	}
}

void USART2_IRQHandler(void)
{
	if (USART2->SR & USART_SR_IDLE)
	{
		*(RxBufferNow++) = USART2->DR;
		//Receive Data
		//ReceiveVideo();
		Flag_VedioRxE = 0;
	}
	if (USART2->SR & USART_SR_RXNE)
	{
		*(RxBufferNow++) = USART2->DR;
	}
}

/******************************************END OF FILE*************************/
