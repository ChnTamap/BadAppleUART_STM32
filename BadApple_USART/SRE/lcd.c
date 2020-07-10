/*
  ����˵��: CT117EǶ��ʽ������LCD��������
  �������: Keil uVision 4.10 
  Ӳ������: CT117EǶ��ʽ������
  ��    ��: 2011-8-9
*/
#include "lcd.h"
#include "fonts.h"

static  vu16 TextColor = 0x0000, BackColor = 0xFFFF;

/*******************************************************************************
* Function Name  : Delay_LCD
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
//���뼶����ʱ
void LCD_delay_ms(uint16_t time)
{    
   uint16_t i=0;  
   while(time--)
   {
      i=12000; 
      while(i--) ;    
   }
}

/*******************************************************************************
* Function Name  : STM3210B_LCD_Init
* Description    : Initializes the LCD.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void STM3210B_LCD_Init(void)
{ 
	 GPIO_InitTypeDef GPIO_InitStructure;
 	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_5;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
	 GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_5);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	 GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC
	 GPIO_SetBits(GPIOC,GPIO_Pin_All);
 
	 LCD_delay_ms(50); // delay 50 ms 
	 LCD_WriteReg(0x0000,0x0001); //�����ڲ�ʱ��
	 LCD_delay_ms(80); // delay 50 ms 
  //DeviceCode = LCD_ReadReg(0x0000);//����ĻID  
  	
	//0x9328������	 
	  LCD_WriteReg(0x00EC,0x108F);// internal timeing  
  	LCD_WriteReg(0x00EF,0x1234);// ADD 
	  LCD_WriteReg(0x0001,0x0100);     
    LCD_WriteReg(0x0002,0x0700);//��Դ����     
    LCD_WriteReg(0x0001,0x0100);     
    LCD_WriteReg(0x0002,0x0700);//��Դ����                    
  	//LCD_WriteReg(0x0003,(1<<3)|(1<<4) ); 	//65K RGB
   	//DRIVE TABLE(�Ĵ��� 03H)
	  //BIT3=AM BIT4:5=ID0:1
  	//AM ID0 ID1   FUNCATION
  	// 0  0   0	   R->L D->U
  	// 1  0   0	   D->U	R->L
	  // 0  1   0	   L->R D->U
	  // 1  1   0    D->U	L->R
  	// 0  0   1	   R->L U->D
	  // 1  0   1    U->D	R->L
	  // 0  1   1    L->R U->D �����������.
  	// 1  1   1	   U->D	L->R
    LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(0<<3) );//65K    
    LCD_WriteReg(0x0004,0x0000);                                   
    LCD_WriteReg(0x0008,0x0207);	           
    LCD_WriteReg(0x0009,0x0000);         
    LCD_WriteReg(0x000a,0x0000);//display setting         
    LCD_WriteReg(0x000c,0x0001);//display setting          
    LCD_WriteReg(0x000d,0x0000);//0f3c          
    LCD_WriteReg(0x000f,0x0000);
	//��Դ����
    LCD_WriteReg(0x0010,0x0000);   
    LCD_WriteReg(0x0011,0x0007);
    LCD_WriteReg(0x0012,0x0000);                                                                 
    LCD_WriteReg(0x0013,0x0000);                 
    LCD_delay_ms(50); 
    LCD_WriteReg(0x0010,0x1590);   
    LCD_WriteReg(0x0011,0x0227);
    LCD_delay_ms(50); 
    LCD_WriteReg(0x0012,0x009c);                  
    LCD_delay_ms(50); 
    LCD_WriteReg(0x0013,0x1900);   
    LCD_WriteReg(0x0029,0x0023);
    LCD_WriteReg(0x002b,0x000e);//
    LCD_delay_ms(50); 
    LCD_WriteReg(0x0020,0x0000);//ȷ������                                                            
    LCD_WriteReg(0x0021,0x013f);           
  	LCD_delay_ms(50); 
	//٤��У��
    LCD_WriteReg(0x0030,0x0007); 
    LCD_WriteReg(0x0031,0x0707);   
    LCD_WriteReg(0x0032,0x0006);
    LCD_WriteReg(0x0035,0x0704);
    LCD_WriteReg(0x0036,0x1f04); 
    LCD_WriteReg(0x0037,0x0004);
    LCD_WriteReg(0x0038,0x0000);        
    LCD_WriteReg(0x0039,0x0706);     
    LCD_WriteReg(0x003c,0x0701);
    LCD_WriteReg(0x003d,0x000f);
    LCD_delay_ms(50);
	//GRAM����
    LCD_WriteReg(0x0050,0x0000); //ˮƽGRAM��ʼλ�� 
    LCD_WriteReg(0x0051,0x00ef); //ˮƽGRAM��ֹλ��                    
    LCD_WriteReg(0x0052,0x0000); //��ֱGRAM��ʼλ��                    
    LCD_WriteReg(0x0053,0x013f); //��ֱGRAM��ֹλ��      
    LCD_WriteReg(0x0060,0xa700);        
    LCD_WriteReg(0x0061,0x0001); 
    LCD_WriteReg(0x006a,0x0000);
	 //������ʾ����
    LCD_WriteReg(0x0080,0x0000);
    LCD_WriteReg(0x0081,0x0000);
    LCD_WriteReg(0x0082,0x0000);
    LCD_WriteReg(0x0083,0x0000);
    LCD_WriteReg(0x0084,0x0000);
    LCD_WriteReg(0x0085,0x0000);
  	//������
    LCD_WriteReg(0x0090,0x0010);     
    LCD_WriteReg(0x0092,0x0600);  
    //����26��ɫ��ʾ����    
    LCD_WriteReg(0x0007,0x0133);  	  
	  LCD_Clear(White);
}

/*******************************************************************************
* Function Name  : LCD_SetTextColor
* Description    : Sets the Text color.
* Input          : - Color: specifies the Text color code RGB(5-6-5).
* Output         : - TextColor: Text color global variable used by LCD_DrawChar
*                  and LCD_DrawPicture functions.
* Return         : None
*******************************************************************************/
void LCD_SetTextColor(vu16 Color)
{
	TextColor = Color;
}
/*******************************************************************************
* Function Name  : LCD_SetBackColor
* Description    : Sets the Background color.
* Input          : - Color: specifies the Background color code RGB(5-6-5).
* Output         : - BackColor: Background color global variable used by 
*                  LCD_DrawChar and LCD_DrawPicture functions.
* Return         : None
*******************************************************************************/
void LCD_SetBackColor(vu16 Color)
{
	BackColor = Color;
}

/*******************************************************************************
* Function Name  : LCD_ClearLine
* Description    : Clears the selected line.
* Input          : - Line: the Line to be cleared.
*                    This parameter can be one of the following values:
*                       - Linex: where x can be 0..9
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_ClearLine(u8 Line)
{
	LCD_DisplayStringLine(Line, "                    ");
}



/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : Clears the hole LCD.
* Input          : Color: the color of the background.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Clear(u16 Color)
{
	uint32_t index=0;      
	LCD_SetCursor(0x00,0x0000);//���ù��λ�� 
	LCD_WriteRAM_Prepare(); //��ʼд��GRAM	
	for(index=0;index<76800;index++)//240 *320 
	{
		LCD_WR_DATA(Color);    
	}
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_SetCursor(u8 Xpos, u16 Ypos)
{ 
	LCD_WriteReg(R32, Xpos);
	LCD_WriteReg(R33, Ypos);	
 
}

//����
//x:0~239
//y:0~319
//TextColor:�˵����ɫ
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	LCD_WR_REG(R34); //��ʼд��GRAM
	LCD_WR_DATA(TextColor); 
} 	

/*******************************************************************************
* Function Name  : LCD_DrawChar �ַ�
* Description    : Draws a character on LCD.
* Input          : - Xpos: the Line where to display the character shape.
*                    This parameter can be one of the following values:
*                       - Linex: where x can be 0..9
*                  - Ypos: start column address.
*                  - c: pointer to the character data.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DrawChar(u8 Xpos, u16 Ypos, uc16 *c)
{
  u32 index = 0, i = 0;
	
	u8 Xaddress = 0;
	u16 temp; //�������浱ǰ�ı���
	uint16_t colortemp = TextColor;//������ʱ����pointcolor
	Xaddress = Xpos;
  LCD_SetCursor(Xpos,Ypos);
	for(index = 0; index < 24; index++)//24��
	{
		temp = c[index];  //���浱ǰ���� 16λ
		for(i = 0; i < 16; i++)//ÿ��16λ
		{
			if(temp & 0x01)//��֤ĳλ�Ƿ�Ϊ1
			{
				TextColor = colortemp;//ѡ��ӡ��ɫ��ӡ
			}
			else
			{
			  TextColor = BackColor;//ѡ������ɫ��ӡ
			}
			LCD_DrawPoint(Xaddress,Ypos);//�������
			temp>>=1;  //����ѭ��һλ
		  Xaddress++; //ͬһ����һ����
		}
		Xaddress = Xpos;//x�����λ
		Ypos++;  //��һ��
		LCD_SetCursor(Xaddress, Ypos);
	}
	
	TextColor = colortemp;//��ԭϵͳ�Ĵ�ӡ��ɫ
}

/*******************************************************************************
* Function Name  : LCD_DisplayChar  �ַ���
* Description    : Displays one character (16dots width, 24dots height).
* Input          : - Line: the Line where to display the character shape .
*                    This parameter can be one of the following values:
*                       - Linex: where x can be 0..9
*                  - Column: start column address.
*                  - Ascii: character ascii code, must be between 0x20 and 0x7E.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DisplayChar(u8 Line, u16 Column, u8 Ascii)
{
	Ascii -= 32;//�ո��ASC��
	LCD_DrawChar(Column, Line, &ASCII_Table[Ascii * 24]);
}

/*******************************************************************************
* Function Name  : LCD_DisplayStringLine
* Description    : Displays a maximum of 20 char on the LCD.
* Input          : - Line: the Line where to display the character shape .
*                    This parameter can be one of the following values:
*                       - Linex: where x can be 0..9
*                  - *ptr: pointer to string to display on LCD.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DisplayStringLine(u8 Line, u8 *ptr)
{
	u32 i = 0;
	u16 refcolumn = 0;

	while ((*ptr != 0) && (i < 15))//	һ��������ʾ15���ַ�
	{
		LCD_DisplayChar(Line, refcolumn, *ptr); 
		refcolumn += 16;
		ptr++;
		i++;
	}
}






/*******************************************************************************
* Function Name  : LCD_SetDisplayWindow
* Description    : Sets a display window
* Input          : - Xpos: specifies the X buttom left position.
*                  - Ypos: specifies the Y buttom left position.
*                  - Height: display window height.
*                  - Width: display window width.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_SetDisplayWindow(u8 Xpos, u16 Ypos, u8 Height, u16 Width)
{
	if(Xpos >= Height)
	{
		LCD_WriteReg(R80, (Xpos - Height + 1));
	}
	else
	{
		LCD_WriteReg(R80, 0);
	}
	LCD_WriteReg(R81, Xpos);
	if(Ypos >= Width)
	{
		LCD_WriteReg(R82, (Ypos - Width + 1));
	}  
	else
	{
		LCD_WriteReg(R82, 0);
	}
	/* Vertical GRAM End Address */
	LCD_WriteReg(R83, Ypos);
	LCD_SetCursor(Xpos, Ypos);
}
/*******************************************************************************
* Function Name  : LCD_WindowModeDisable
* Description    : Disables LCD Window mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WindowModeDisable(void)
{
	LCD_SetDisplayWindow(239, 0x13F, 240, 320);
	LCD_WriteReg(R3, 0x1018);    
}


/*******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Displays a line.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
*                  - Length: line length.
*                  - Direction: line direction.
*                    This parameter can be one of the following values: Vertical 
*                    or Horizontal.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DrawLine(u8 Xpos, u16 Ypos, u16 Length, u8 Direction)
{
	u32 i = 0;
	LCD_SetCursor(Xpos, Ypos);
	if(Direction == Horizontal)
	{
		LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
		for(i = 0; i < Length; i++)
		{
			LCD_WriteRAM(TextColor);
		}
	}
	else
	{
		for(i = 0; i < Length; i++)
		{
			LCD_WriteRAM_Prepare();  /* Prepare to write GRAM */
			LCD_WriteRAM(TextColor);
			Xpos++;
			LCD_SetCursor(Xpos, Ypos);
		}
	}
}


/*******************************************************************************
* Function Name  : LCD_DrawRect
* Description    : Displays a rectangle.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
*                  - Height: display rectangle height.
*                  - Width: display rectangle width.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DrawRect(u8 Xpos, u16 Ypos, u8 Height, u16 Width)
{
	LCD_DrawLine(Xpos, Ypos, Width, Horizontal);
	LCD_DrawLine((Xpos + Height), Ypos, Width, Horizontal);
  
	LCD_DrawLine(Xpos, Ypos, Height, Vertical);
	LCD_DrawLine(Xpos, (Ypos - Width + 1), Height, Vertical);
}
/*******************************************************************************
* Function Name  : LCD_DrawCircle
* Description    : Displays a circle.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
*                  - Height: display rectangle height.
*                  - Width: display rectangle width.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DrawCircle(u8 Xpos, u16 Ypos, u16 Radius)
{
	s32  D;
	u32  CurX;
	u32  CurY;
  
  	D = 3 - (Radius << 1);
  	CurX = 0;
  	CurY = Radius;
  
  	while (CurX <= CurY)
  	{
	    LCD_SetCursor(Xpos + CurX, Ypos + CurY);
	    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	    LCD_WriteRAM(TextColor);
	
	    LCD_SetCursor(Xpos + CurX, Ypos - CurY);
	    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	    LCD_WriteRAM(TextColor);
	
	    LCD_SetCursor(Xpos - CurX, Ypos + CurY);
	    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	    LCD_WriteRAM(TextColor);
	
	    LCD_SetCursor(Xpos - CurX, Ypos - CurY);
	    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	    LCD_WriteRAM(TextColor);
	
	    LCD_SetCursor(Xpos + CurY, Ypos + CurX);
	    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	    LCD_WriteRAM(TextColor);
	
	    LCD_SetCursor(Xpos + CurY, Ypos - CurX);
	    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	    LCD_WriteRAM(TextColor);
	
	    LCD_SetCursor(Xpos - CurY, Ypos + CurX);
	    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	    LCD_WriteRAM(TextColor);
	
	    LCD_SetCursor(Xpos - CurY, Ypos - CurX);
	    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	    LCD_WriteRAM(TextColor);
	
	    if (D < 0)
	    { 
	      D += (CurX << 2) + 6;
	    }
	    else
	    {
	      D += ((CurX - CurY) << 2) + 10;
	      CurY--;
	    }
	    CurX++;
  	}
}

/*******************************************************************************
* Function Name  : LCD_DrawMonoPict//��ɫͼƬ
* Description    : Displays a monocolor picture.
* Input          : - Pict: pointer to the picture array.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DrawMonoPict(uc32 *Pict)
{
	u32 index = 0, i = 0;

	LCD_SetCursor(0, 319); 

	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	for(index = 0; index < 2400; index++)
	{
		for(i = 0; i < 32; i++)
		{
			if((Pict[index] & (1 << i)) == 0x00)
			{
				LCD_WriteRAM(BackColor);
			}
			else
			{
				LCD_WriteRAM(TextColor);
			}
		}
	}
}

/*******************************************************************************
* Function Name  : LCD_WriteBMP
* Description    : Displays a bitmap picture loaded in the internal Flash.
* Input          : - BmpAddress: Bmp picture address in the internal Flash.
* Output         : None
* Return         : None
*******************************************************************************/


void LCD_WriteBMP(u32 BmpAddress)
{
	u32 index = 0, size = 0;
	size = *(vu16 *) (BmpAddress + 2);
	size |= (*(vu16 *) (BmpAddress + 4)) << 16;
	index = *(vu16 *) (BmpAddress + 10);
	index |= (*(vu16 *) (BmpAddress + 12)) << 16;
	size = (size - index)/2;
	BmpAddress += index;
	LCD_WriteReg(R3, 0x1008);
	LCD_WriteRAM_Prepare();
	for(index = 0; index < size; index++)
	{
		LCD_WriteRAM(*(vu16 *)BmpAddress);
		BmpAddress += 2;
	}
	LCD_WriteReg(R3, 0x1018);
}


/**********************************************************
 * @brief  LCD_WR_REG д�Ĵ�����ַ����  
 * @param  data  ���Ĵ�����ַ               
 * @retval  None            
 *********************************************************/
void LCD_WR_REG(uint8_t data)
{ 	
	LCD_CS_CLR;
	LCD_RS_CLR;//д��ַ  	 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}

//д�Ĵ���
/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	 
}

//���Ĵ���
/*******************************************************************************
* Function Name  : LCD_ReadReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
*******************************************************************************/
u16 LCD_ReadReg(u8 LCD_Reg)
{
	u16 temp;

	GPIOB->BRR = 1<<9;  
	GPIOB->BRR = 1<<8;  
	GPIOB->BSRR = 1<<5; 

	GPIOC->ODR = LCD_Reg; 
	GPIOB->BRR = 1<<5; 
	GPIOB->BSRR = 1<<5;
	GPIOB->BSRR = 1<<8;

	LCD_BusIn();
	GPIOB->BRR = 1<<10;  
	temp = GPIOC->IDR;    
	GPIOB->BSRR = 1<<10; 

	LCD_BusOut();
	GPIOB->BSRR = 1<<9; 

	return temp; 
}

//д�Դ�׼��
/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : Prepare to write to the LCD RAM.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{ 
	LCD_WR_REG(R34);
}
//д�Դ�
/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{
	LCD_WR_DATA(RGB_Code);//дʮ��λGRAM
}

/*******************************************************************************
* Function Name  : LCD_ReadRAM
* Description    : Reads the LCD RAM.
* Input          : None
* Output         : None
* Return         : LCD RAM Value.
*******************************************************************************/
u16 LCD_ReadRAM(void)
{
	u16 temp;

	GPIOB->BRR = 0x0200; 
	GPIOB->BRR = 0x0100; 
	GPIOB->BSRR = 0x0020; 

	GPIOC->ODR = R34;     
	GPIOB->BRR = 0x0020;  
	GPIOB->BSRR = 0x0020; 
	GPIOB->BSRR = 0x0100; 

	LCD_BusIn();
	GPIOB->BRR = 0x0400; 
	temp = GPIOC->IDR;  
	GPIOB->BSRR = 0x0400;

	LCD_BusOut();
	GPIOB->BSRR = 0x0200; 
                         
	return temp; 
}

/*******************************************************************************
* Function Name  : LCD_PowerOn
* Description    : Power on the LCD.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_PowerOn(void)
{
	/* LCD_WriteReg(R16, 0x0000);
	LCD_WriteReg(R17, 0x0000); 
	LCD_WriteReg(R18, 0x0000);
	LCD_WriteReg(R19, 0x0000); 
	Delay_LCD(20);             
	LCD_WriteReg(R16, 0x17B0); 
	LCD_WriteReg(R17, 0x0137);
	Delay_LCD(5);             
	LCD_WriteReg(R18, 0x0139); 
	Delay_LCD(5);             
	LCD_WriteReg(R19, 0x1d00); 
	LCD_WriteReg(R41, 0x0013); 
	Delay_LCD(5);             
	LCD_WriteReg(R7, 0x0173); */
}

/*******************************************************************************
* Function Name  : LCD_DisplayOn
* Description    : Enables the Display.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DisplayOn(void)
{
	LCD_WriteReg(R7, 0x0173);
}
/*******************************************************************************
* Function Name  : LCD_DisplayOff
* Description    : Disables the Display.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DisplayOff(void)
{
	LCD_WriteReg(R7, 0x0); 
}
/*******************************************************************************
* Function Name  : LCD_CtrlLinesConfig
* Description    : Configures LCD Control lines.
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/*******************************************************************************
* Function Name  : LCD_BusIn
* Description    : Configures the Parallel interface for LCD(PortC)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_BusIn(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : LCD_BusOut
* Description    : Configures the Parallel interface for LCD(PortC)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_BusOut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//16��ɫ��PIC
/*******************************************************************************
* Function Name  : LCD_DrawPicture
* Description    : Displays a 16 color picture.
* Input          : - picture: pointer to the picture array.
* Output         : None
* Return         : None
*******************************************************************************/
//picture ��һ���ֽ����飬ÿ�����ֽ���һ�����ص���ɫֵ��

void LCD_DrawPicture(const u8* picture)
{
	unsigned int index;
	LCD_SetCursor(0,40); //���ڴ洢���ޣ�ֻ��ʾ240*240
	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	for(index = 0; index < 240 * 240 ; index++)
	{
		LCD_WriteRAM(picture[2*index+1]<<8|picture[2*index]); //��λ��ǰ	
	}
}

//void LCD_DrawPicture(const u8* picture)
//{
//	unsigned int index;
//	LCD_SetCursor(0,0); //���ڴ洢���ޣ�ֻ��ʾ240*240
//	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
//	for(index = 0; index < 240 * 320 ; index++)
//	{
//		LCD_WriteRAM(picture[2*index+1]<<8|picture[2*index]); //��λ��ǰ	
//	}
//}



