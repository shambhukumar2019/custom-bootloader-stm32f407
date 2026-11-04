#include "platform.h"


#define GPIO_PIN_12         12U
#define GLED_PIN            GPIO_PIN_12

#define LED_GPIO_PORT       GPIOD


void SystemClock_Config(void);



#define userConfigASSERT(reg, mask, val)\
        { do{\
            if((reg & mask) != val)\
            {\
                __disable_irq();\
                for(;;);\
            }\
        }while(0); }



// dummy delay
void wait(void)
{
    volatile uint16_t i = 0;
    volatile uint16_t j = 0;

    for(;j < 100U; ++j)
    {
        for(i = 0;i < 65535U; ++i);
    }
}
    


int main(void)
{
    SystemClock_Config();
    
    //enable gpiod clk
    RCC->AHB1ENR |= (1U<<3);

    //this reduces pwr consumption of schmitt trigger
    LED_GPIO_PORT->MODER |= 0xffffffffU;               //make all pins analog mode
    
    LED_GPIO_PORT->MODER &= (~(3U<<(GLED_PIN * 2)));   //reset pin 12 dir mode state (input)
    LED_GPIO_PORT->MODER |= (1U<<(GLED_PIN * 2));      //output dir mode

    //default push-pull output mode
    //default low speed
    //default pupd off

    for(;;)
    {
        //toggle green led pin output
        LED_GPIO_PORT->ODR ^= (1U<<GLED_PIN);
        wait();
    }

    return 0;
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  //HSI = 16MHz default, PLL input clock frequency
  RCC->PLLCFGR &= (~(1U<<22));	//HSI as PLL source clock
  userConfigASSERT(RCC->PLLCFGR, (1U<<22), (0U<<22));

  //VCO input frequency = PLL input clock frequency / M , 2MHz
  RCC->PLLCFGR &= (~(0x3FU<<0));	//mask PLLM bits
  RCC->PLLCFGR |= (0x8U<<0);		//M = 8
  userConfigASSERT(RCC->PLLCFGR, (0x3FU<<0), (0x8U<<0));

  //VCO output frequency [336MHz] = VCO input frequency * N ,336MHz
  RCC->PLLCFGR &= (~(0x1FFU<<6));	//mask PLLN bits
  RCC->PLLCFGR |= (0xA8U<<6);		//N = 168
  userConfigASSERT(RCC->PLLCFGR, (0x1FFU<<6), (0xA8U<<6));

  //PLL output frequency [168MHz] = VCO output frequency / P , 168MHz SYSCLK
  RCC->PLLCFGR &= (~(0x3U<<16));	//mask PLLP bits
  RCC->PLLCFGR |= (0x0U<<16);		//P = 2
  userConfigASSERT(RCC->PLLCFGR, (0x3U<<16), (0x0U<<16));

  //USB, RNG, SDIO, OTG FS frequency = VCO output frequency / Q , 48MHz
  RCC->PLLCFGR &= (~(0xFU<<24));	//mask PLLQ bits
  RCC->PLLCFGR |= (0x7U<<24);		//Q = 7
  userConfigASSERT(RCC->PLLCFGR, (0xFU<<24), (0x7U<<24));

  //AHB clk out = 168MHz, HCLK
  RCC->CFGR &= (~(0xFU<<4));		//mask AHB prescaler bits
  RCC->CFGR |= (0x0U<<4);			//AHB prescaler = null
  userConfigASSERT(RCC->CFGR, (0xFU<<4), (0x0U<<4));

  //APB1 clk out = 42MHz, APB1 clk = AHB clk / 4
  RCC->CFGR &= (~(0x7U<<10));		//mask APB1 prescaler bits
  RCC->CFGR |= (0x5U<<10);			//APB1 prescaler = 4
  userConfigASSERT(RCC->CFGR, (0x7U<<10), (0x5U<<10));

  //APB2 clk out = 84MHz, APB2 clk = AHB clk / 2
  RCC->CFGR &= (~(0x7U<<13));		//mask APB2 prescaler bits
  RCC->CFGR |= (0x4U<<13);			//APB2 prescaler = 2
  userConfigASSERT(RCC->CFGR, (0x7U<<13), (0x4U<<13));

  //configure PLL as system clock
  RCC->CFGR &= (~(0x3U<<0));
  RCC->CFGR |= (0x2U<<0);
  userConfigASSERT(RCC->CFGR, (0x3U<<0), (0x2U<<0));
//  while((RCC->CFGR & (0x3U<<2)) != (0x2U<<2));	//wait for pll select as sys clk

  RCC->CR |= (1U<<0);	//hsi on
  while((RCC->CR & (0x1U<<1)) != (0x1U<<1));	//wait for hsi stable

  RCC->CR |= (1U<<24);	//main PLL on
  while((RCC->CR & (0x1U<<25)) != (0x1U<<25));	//wait for main PLL stable

  SystemCoreClockUpdate();

}



