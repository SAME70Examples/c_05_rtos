#include "same70.h"                     // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

void led_init(void);
#define LED_ON 1
#define LED_OFF 0
void led_setState(int led_state);
void button_init(void);
int button_isPressed(void);

int main(){
	SystemCoreClockUpdate();
	osKernelInitialize();
	led_init();
	button_init();
	osKernelStart();
	while(1){
		led_setState(LED_ON);
		osDelay(500);
		led_setState(LED_OFF);
		osDelay(500);
	}
}

#define PMC_PCR_CMD_READ (0<<12) 
#define PMC_PCR_CMD_WRITE (1<<12)
#define PMC_PCR_PID_PIOA (10)
#define PMC_PCR_PID_PIOB (11)
#define PMC_PCR_PID_PIOC (12)

/**
	* Initialize led on PC8
	*
**/
void led_init(void){
	PMC->PMC_PCR = (PMC_PCR_EN)|(PMC_PCR_CMD_WRITE)|(PMC_PCR_PID_PIOC);//Enable PIOC Peripheral
	PIOC->PIO_PER = PIO_PER_P8;//Enable direct control of PC8
	PIOC->PIO_MDER = PIO_MDER_P8;//Open drain driver
	PIOC->PIO_PUDR = PIO_PUDR_P8;//Disable pull ups
	PIOC->PIO_PPDDR = PIO_PPDDR_P8;//Disable pull downs
	PIOC->PIO_OER = PIO_OER_P8;//Configure as output
}

void led_setState(int led_state){
	if(led_state == LED_ON){
		PIOC->PIO_CODR = PIO_CODR_P8;//Output low: led on
	}else{
		PIOC->PIO_SODR = PIO_SODR_P8;//Ouput floating: led off
	}
}

/**
 * Configure PA11 as input with pull-ups
 */
void button_init(void){
	PMC->PMC_PCR = (PMC_PCR_EN)|(PMC_PCR_CMD_WRITE)|(PMC_PCR_PID_PIOA);//Enable PIOA Peripheral
	PIOA->PIO_PER = PIO_PER_P11;//Enable direct control of PA11
	PIOA->PIO_PUER = PIO_PUER_P11;//Enable pull ups
	PIOA->PIO_PPDDR = PIO_PPDDR_P11;//Disable pull downs
	PIOA->PIO_ODR = PIO_ODR_P11;//Configure as input
}

int button_isPressed(void){
	return ((PIOA->PIO_PDSR) & (PIO_PDSR_P11)) ? 0 : 1;//Return pin status
}
