#include "CLOCK.h"
#include "USART.h"
#include "GPIO.h"
#include "SYS_INIT.h"

//vertical GPIOA
//horizontal GPIOB



//GPIOA
#define UD_red 5
#define UD_yellow 6
#define UD_green 7


//GPIOA
#define U_1 13 
#define U_2 14
#define U_3 15

//GPIOA 
#define D_1 10
#define D_2 11
#define D_3 12

//GPIOB
#define LR_red 3
#define LR_yellow 4
#define LR_green 5

//GPIOA 
#define R_1 13
#define R_2 14
#define R_3 15

#define L_1 10
#define L_2 11
#define L_3 12



#define UDMOVE GPIOA
#define LRMOVE GPIOB


const int32_t Red_to_Green=10; //actual 60
const int32_t Green_to_Yellow=5; //actual 5
const int32_t Yellow_to_Red=2; //actual 2

static GPIO_InitTypeDef gpio_LRlight;
static GPIO_InitTypeDef gpio_UDlight;

void reset_led(GPIO_TypeDef* GPIOx)	
{
	for(uint16_t i=10; i<=15; i++)
	{
		if(GPIOx==GPIOB && i==11) GPIO_WritePin(GPIOx,1,GPIO_PIN_RESET);
		if(GPIOx==GPIOB && i==15) GPIO_WritePin(GPIOx,2,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOx,i,GPIO_PIN_RESET);
	}
}

void traffic_move_UD(int n,GPIO_TypeDef* GPIOx)
{
	uint16_t st1=12;
	uint16_t st2=15;
	
	for(int i=0; i<n; i++)
	{
		reset_led(GPIOx);
		ms_delay(250);
		for(uint16_t j=0; j<3; j++)
		{
			if(j>0)
			{
				
				if(GPIOx==GPIOB && st1-j+1==11) GPIO_WritePin(GPIOx,1,GPIO_PIN_RESET);
				else GPIO_WritePin(GPIOx,st1-j+1,GPIO_PIN_RESET);
				if(GPIOx==GPIOB && st2-j+1==15) GPIO_WritePin(GPIOx,2,GPIO_PIN_RESET);
				else GPIO_WritePin(GPIOx,st2-j+1,GPIO_PIN_RESET);
			}
			if(GPIOx==GPIOB && st1-j==11) GPIO_WritePin(GPIOx,1,GPIO_PIN_SET);	
			else GPIO_WritePin(GPIOx,st1-j,GPIO_PIN_SET);
			if(GPIOx==GPIOB && st2-j==15) GPIO_WritePin(GPIOx,2,GPIO_PIN_SET);
			else GPIO_WritePin(GPIOx,st2-j,GPIO_PIN_SET);
			ms_delay(250);
		}
	
	} 
	reset_led(GPIOx);
}
void traffic_LR_go()
{
	GPIO_WritePin(GPIOB,LR_red,GPIO_PIN_RESET);
	GPIO_WritePin(GPIOB,LR_green,GPIO_PIN_SET);
}
void traffic_LR_stop()
{
		ms_delay(Green_to_Yellow);
		GPIO_WritePin(GPIOB,LR_green,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOB,LR_yellow,GPIO_PIN_SET);
		traffic_move_UD(Green_to_Yellow,GPIOB);
		GPIO_WritePin(GPIOB,LR_yellow,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOB,LR_red,GPIO_PIN_SET);
}
void traffic_UD_go()
{
		GPIO_WritePin(GPIOA,UD_red,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOA,UD_green,GPIO_PIN_SET);
}

void traffic_UD_stop()
{
		GPIO_WritePin(GPIOA,UD_green,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOA,UD_yellow,GPIO_PIN_SET);
		traffic_move_UD(Green_to_Yellow,GPIOA);
		GPIO_WritePin(GPIOA,UD_yellow,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOA,UD_red,GPIO_PIN_SET);
}

int main()
{
		initClock();
		sysInit();
		UART2_Config();
	
		//GPIOA 5 for red, 6 yellow, 7 green for vertical
		gpio_UDlight.Pin=GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	
		gpio_UDlight.Mode=GPIO_MODE_OUTPUT_PP;
		gpio_UDlight.Speed=GPIO_SPEED_FREQ_LOW;
		gpio_UDlight.Pull=GPIO_NOPULL;
		
		GPIO_Init(GPIOA,&gpio_UDlight);
	
		//GPIOB 13 for red, 14 yellow, 15 green for vertical
		gpio_LRlight.Pin=GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_2 |  GPIO_PIN_10 | GPIO_PIN_1 | GPIO_PIN_12 ;
	
		gpio_LRlight.Mode=GPIO_MODE_OUTPUT_PP;
		gpio_LRlight.Speed=GPIO_SPEED_FREQ_LOW;
		gpio_LRlight.Pull=GPIO_NOPULL;
		
		GPIO_Init(GPIOB,&gpio_LRlight);
		
		int16_t i=10;
		while(i>0)
		{
			traffic_LR_stop();
			traffic_UD_go();
			traffic_move_UD(Red_to_Green,GPIOA);
			
			traffic_UD_stop();
			traffic_LR_go();
			traffic_move_UD(Red_to_Green,GPIOB);
			
		} 
		/* GPIOA->MODER |= (1<<10);
		GPIOA->OSPEEDR=0;
		GPIOA->OTYPER=0;
		
		while(i>0)
		{
			i--;
			GPIO_WritePin(GPIOA,5,GPIO_PIN_SET);
			ms_delay(3000);
			GPIO_WritePin(GPIOA,5,GPIO_PIN_RESET);
			ms_delay(1000);
			UART_SendString(USART2,"HELLO\n");
		} */
	
}