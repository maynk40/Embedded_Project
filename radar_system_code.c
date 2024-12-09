
#include "stm32f412zx.h"   // Device header 

static uint32_t Val1 = 0; 
static uint32_t Val2 = 0; 
static uint32_t pulse = 0; 
static uint8_t First_captured = 0; 
static int distance; 

void DELAY (int dd) 
{ 
 for (;dd>0;dd--) 
 { 
  for(int i=0;i<3000;i++); 
 } 
} 

void PERIPHERAL_SETUP(void) 
{  
 //LED AND BUZZER SETUP 
 RCC->AHB1ENR |=0x1; // Enables Clock of PORT A 
 GPIOA->MODER |= 0x1; //BUZZER, RED LED as OUTPUT 
  
 //GREEN LED FROM START 
 RCC->AHB1ENR |=0x8; // Enables Clock of PORT D 
 GPIOD->MODER |=0x1; // making PD0 as ouput 
 GPIOD->ODR=0x1;//TURNING ON GREEN LED FROM START 
}

void MOTOR_SETUP(void) 
{ 
 // Enable GPIOB for generating pulse 
    RCC->AHB1ENR |= 0x2; // Enable GPIOB 
    GPIOB->MODER |= 0x800; // Set PB5 to AF2 mode 
    GPIOB->AFR[0] |= 0x200000; // AF2 for TIM3_CH2 

    // Generating PWM pulse in TIM4_CH2 
    RCC->APB1ENR |= 0x2; // Enable TIM3 
    TIM3->CCMR1 |= 0x6000; // PWM mode 1 for CH2 
    TIM3->CCER |= 0x10; // Enable compare mode for CH2 
    TIM3->SR = 0; 

    // Setup TIMER4 CH2 
    TIM3->PSC = 320 - 1; 
    TIM3->ARR = 1000 - 1; // 50Hz 
    TIM3->CR1 = 1; // Enable TIM3 
} 

void ULTRASONIC_SETUP(void) 
{ 
 GPIOB->MODER |= 0x8000; //AF mode for PB7  
 GPIOB->AFR[0] |= 0x20000000; //AF2 for TIM4  
 RCC->APB1ENR |= 0x4; //ENABLE TIM4 
 TIM4->CCMR1 |= 0x6000;//FOR CH2, PWM MODE 1 
 TIM4->CCER |= 0x10; //ENABLE COMPARE 
 TIM4->SR = 0; 
 TIM4->PSC = 160 - 1;//0.1MHz 
 TIM4->ARR = 10001 - 1;  
 TIM4->CCR2 = 1; //GENERATE PWM PULSE WITH 10us HIGH  
 TIM4->CR1 = 1; 
 GPIOB->MODER |=0x80; //AF MODE FOR PB3  
 GPIOB->AFR[0] |= 0x1000; //AF1 for TIM2_CH2 WITH PB3 
 RCC->APB1ENR |= 0x1; //ENABLE TIM2 
 TIM2->CCMR1 |= 0x100; //INPUT CAPTURE MODE IC1 => TI1 
 TIM2->CCER |= 0xB0; //CCE1 input enabled and 00 for measuring rising and falling edge 
 TIM2->DIER |= 0x4; 
 TIM2->SR |= 0x0; 
 TIM2->PSC = 16 - 1; 
 TIM2->ARR = 65536 - 1; // 15.25 Hz 
 TIM2->CR1 |= 0x1; //TURN ON TIMER 
 NVIC_EnableIRQ(TIM2_IRQn); 
}

void ENEMY_DETECTED(int X) 
{ 
 switch(X) 
 { 
  case 1: GPIOA->ODR |= 0x1; //BUZZER and RED LED TOGGLE ON 
      break; 
  case 0: GPIOA->ODR = 0x00000000; // Set all bits of GPIOA ODR to 0 
      break; 
 } 
}

//----------------------------MAIN LOGIC-------------------------------// 
int main(void) 
{   
  PERIPHERAL_SETUP(); 
  MOTOR_SETUP(); 
  ULTRASONIC_SETUP(); 
   
  while(1) 
  {  
    // Increasing duty cycle from 0 to 60 
   for (int duty = 20; duty <= 120; duty++)  
   { 
        TIM3->CCR2 = duty; 
    DELAY(55); // Adjust delay time as needed for servo response 
   } 

   // Decreasing duty cycle from 60 back to 0 
   for (int duty = 120; duty >= 20; duty--)  
   { 
        TIM3->CCR2 = duty; 
    DELAY(55); // Adjust delay time as needed for servo response 
   } 
  } 
}

void TIM2_IRQHandler(void)  
{ 
  TIM2->SR &= ~TIM_SR_CC2IF;     // Clear the interrupt flag 
  if (First_captured%2==0) 
   { 
    Val1 = TIM2->CCR2; 
    First_captured++; 
   } 
  else if (First_captured%2==1) 
   { 
    Val2 = TIM2->CCR2; 
    if (Val2 > Val1) 
     { 
      pulse = Val2 - Val1; 
     } 
    else 
     { 
      pulse = (TIM2->ARR - Val1) + Val2 + 1; 
     } 
    distance = pulse * (0.034/2) ; 
    if (distance <= 25)  
     { 
      ENEMY_DETECTED(1); 
      DELAY(7); 
      ENEMY_DETECTED(0); 
     }  
    First_captured++; 
  } 
} 
