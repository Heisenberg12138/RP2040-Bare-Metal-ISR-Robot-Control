/* *
 * Student name : [ Yanran Zhao ]
 * Student number : [ 5902703 ]
 * Assignment1: Variable speed
 */

#include "hardware/pwm.h"
#include "hardware/gpio.h"


// Left motor
#define L_PWM 19   // GPIO19 -> PWM slice 1, channel B
#define L_DIR 18  

// Right motor
#define R_PWM 20   // GPIO20 -> PWM slice 2, channel A
#define R_DIR 21   


#define PWM_TOP        1000   
#define PWM_DIV        32     

// Speeds 
#define SPEED_CONST  550
#define SPEED_FAST   800
#define SPEED_KICK   900
#define SPEED_STOP     0

uint sliceL;
uint sliceR;


void setSpeed(uint duty)
{
  if (duty > PWM_TOP) duty = PWM_TOP;

  // LEFT motor: GPIO19 -> sliceL -> channel B (high 16 bits)
  pwm_hw->slice[sliceL].cc =
      (pwm_hw->slice[sliceL].cc & 0x0000FFFF) |
      (duty << 16);

  // RIGHT motor: GPIO20 -> sliceR -> channel A (low 16 bits)
  pwm_hw->slice[sliceR].cc =
      (pwm_hw->slice[sliceR].cc & 0xFFFF0000) |
      duty;
}

// Kick-start 
void kickStart()
{
  setSpeed(SPEED_KICK);
  delay(250);
}


void setup()
{
  // Direction pins
  pinMode(L_DIR, OUTPUT);
  pinMode(R_DIR, OUTPUT);
  digitalWrite(L_DIR, LOW);   
  digitalWrite(R_DIR, LOW);   

  // PWM pin functions
  gpio_set_function(L_PWM, GPIO_FUNC_PWM);
  gpio_set_function(R_PWM, GPIO_FUNC_PWM);

  // Get PWM slices
  sliceL = pwm_gpio_to_slice_num(L_PWM);  
  sliceR = pwm_gpio_to_slice_num(R_PWM);  

  // Configure LEFT slice 
  pwm_hw->slice[sliceL].top = PWM_TOP;
  pwm_hw->slice[sliceL].div = (PWM_DIV << 4);   
  pwm_hw->slice[sliceL].cc  = 0;
  pwm_hw->slice[sliceL].csr = PWM_CH0_CSR_EN_BITS;

  // Configure RIGHT slice
  pwm_hw->slice[sliceR].top = PWM_TOP;
  pwm_hw->slice[sliceR].div = (PWM_DIV << 4);
  pwm_hw->slice[sliceR].cc  = 0;
  pwm_hw->slice[sliceR].csr = PWM_CH0_CSR_EN_BITS;
}


void loop()
{
  // 1. Start + constant speed
  kickStart();
  setSpeed(SPEED_CONST);
  delay(3000);

  // 2. Speed up
  setSpeed(SPEED_FAST);
  delay(3000);

  // 3. Gradually slow down
  for (int s = SPEED_FAST; s >= SPEED_CONST; s -= 25)
  {
    setSpeed(s);
    delay(150);
  }

  // 4. Stop
  setSpeed(SPEED_STOP);
  delay(3000);
}

