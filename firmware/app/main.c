#include "platform.h"


#define GPIO_PIN_13         13U
#define GLED_PIN            GPIO_PIN_13
#define LED_GPIO_PORT       GPIOD


#define APP_HEADER_IMAGE_BASE_ADDR      0x08004000UL
#define APP_HEADER_IMAGE_SIZE           16 * 1024UL
#define APP_IMAGE_BASE_ADDR             0x08008000UL
#define APP_IMAGE_SIZE                  16 * 1024UL
#define APP_RAM_BASE_ADDR               0x20000800UL
#define APP_RAM_SIZE                    64 * 1024UL


void SystemClock_Config(void);
uint8_t app_image_verify(void);
void wait(void);

typedef void (*app_reset_handler_t)(void);


typedef struct
{
    uint32_t ota_flag;
    uint32_t magic_num;
    uint32_t crc;
    uint32_t size;
    uint32_t version;
}app_image_header_t;



#define userConfigASSERT(reg, mask, val)\
        { do{\
            if((reg & mask) != val)\
            {\
                __disable_irq();\
                for(;;);\
            }\
        }while(0); }

    


int main(void)
{
    SystemClock_Config();

    uint8_t ret = app_image_verify();
    switch(ret)
    {
        case 0:
            break;
        case 1:
            // invalid stack pointer
            for(;;);
        case 2:
            // invalid reset handler
            for(;;);
        case 3:
            // invalid magic number
            for(;;);
        case 4:
            // invalid size
            for(;;);
        case 5:
            // invalid crc
            for(;;);
        case 6:
            // invalid ota flag
            for(;;);
        case 7:
            // invalid version
            for(;;);
        default:
            for(;;);
    }

    // relocate vector table
    SCB->VTOR = APP_IMAGE_BASE_ADDR;

    // let prior memory accesses complete and flush instruction pipeline using dsb and isb
    __asm volatile ("dsb 0xF" ::: "memory");
    __asm volatile ("isb 0xF" ::: "memory");

    // change sp of application
    __asm volatile ("msr msp, %0" : : "r" (*((volatile uint32_t *)APP_IMAGE_BASE_ADDR)) :);

    // jump to reset handler of application
    uint32_t app_reset_handler = *((volatile uint32_t *)(APP_IMAGE_BASE_ADDR + 4UL));
    app_reset_handler_t reset_handler = (app_reset_handler_t)(app_reset_handler);
    reset_handler();    // jump to application reset handler


    //enable gpiod clk
    RCC->AHB1ENR |= (1U<<3);

    //this reduces pwr consumption of schmitt trigger
    LED_GPIO_PORT->MODER |= 0xffffffffU;               //make all pins analog mode
    
    LED_GPIO_PORT->MODER &= (~(3U<<(GLED_PIN * 2)));   //reset pin 13 dir mode state (input)
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


/**
 * @brief  verify application image
 * @retval 0 if valid, else error code
 */
uint8_t app_image_verify(void)
{
    // verify magic number, crc, size, sp and reset handler of application image
    app_image_header_t *app_header = (app_image_header_t *)APP_HEADER_IMAGE_BASE_ADDR;

    uint32_t app_sp = *((volatile uint32_t *)APP_IMAGE_BASE_ADDR);
    uint32_t app_reset_handler = *((volatile uint32_t *)(APP_IMAGE_BASE_ADDR + 4UL));

    if((app_sp < APP_RAM_BASE_ADDR) || (app_sp > (APP_RAM_BASE_ADDR + APP_RAM_SIZE)))
    {
        return 1;   // invalid stack pointer
    }
    
    // check if reset handler is in valid range and thumb bit is set (LSB = 1)
    if((app_reset_handler < APP_IMAGE_BASE_ADDR) || (app_reset_handler >= (APP_IMAGE_BASE_ADDR + APP_IMAGE_SIZE)) || ((app_reset_handler & 0x1UL) == 0UL))
    {
        return 2;   // invalid reset handler
    }

    if(app_header->magic_num != 0xDEADBEEFUL)
    {
        return 3;   // invalid magic number
    }

    if(app_header->size > APP_IMAGE_SIZE)
    {
        // return 4;    // invalid size
    }

    if(app_header->crc != 0xFFFFFFFFUL)
    {
        // return 5;    // invalid crc
    }

    if(app_header->ota_flag != 0xFFFFFFFFUL)
    {
        // return 6;    // invalid ota flag
    }

    if(app_header->version != 0xFFFFFFFFUL)
    {
        // return 7;    // invalid version
    }

    return 0;   // valid application image
}


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




