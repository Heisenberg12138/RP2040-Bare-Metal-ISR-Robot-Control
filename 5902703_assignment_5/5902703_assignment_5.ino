/*
 * Student name : [ Yanran Zhao ]
 * Student number : [ 5902703 ]
 *
 * Assignment 5: fixed distance
 */
#define LS A1   // GPIO27
#define RS A0   // GPIO26
#define L_PWM   19
#define L_DIR   18
#define R_PWM   20
#define R_DIR   21
//Encoder (LEFT ONLY) 
#define ENCODER_L_PIN  3

//Threshold 
#define TH_L     60
#define TH_R     70

//Speed
#define FWD_SPEED             120
#define FWD_SMALL_SPEED       110
#define TURN_SLIGHT           120
#define TURN_MODERATE         140
#define TURN_MODERATE_INVERSE  60
#define TURN_SMALL_INVERSE     40
#define TURN_DELAY              1

//test
#define test_distance 180.0
//Distance calibration 
#define CM_PER_TICK   0.52
#define TICKS_PER_CM (1.0 / 0.52)   

char last = 'F';
char currentDir = 'F';
int  sameDirCount = 0;
int speedL = 0;
int speedR = 0;



volatile unsigned long leftTicks = 0;
unsigned long targetTicks = 0;

volatile unsigned long lastEncoderTime = 0;

void updateLeftEncoder() {
  unsigned long now = micros();
 
  if (now - lastEncoderTime > 300) {
    leftTicks++;
    lastEncoderTime = now;
  }
}



void stopRobot() {
  analogWrite(L_PWM, 0);
  analogWrite(R_PWM, 0);
}

void forward() {
  last = 'F';
  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);

  speedL = FWD_SPEED;
  speedR = FWD_SPEED;

  analogWrite(L_PWM, speedL);
  analogWrite(R_PWM, speedR);
}

void turnLeft() {
  last = 'L';

  digitalWrite(L_DIR, HIGH);
  digitalWrite(R_DIR, LOW);

  if (sameDirCount <= 1) {
    speedL = TURN_SMALL_INVERSE;
    speedR = TURN_SLIGHT;
  } else {
    speedL = TURN_MODERATE_INVERSE;
    speedR = TURN_MODERATE;
  }

  analogWrite(L_PWM, speedL);
  analogWrite(R_PWM, speedR);
  delay(TURN_DELAY);

  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);
  analogWrite(L_PWM, FWD_SMALL_SPEED);
  analogWrite(R_PWM, FWD_SMALL_SPEED);
}

void turnRight() {
  last = 'R';

  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, HIGH);

  if (sameDirCount <= 1) {
    speedL = TURN_SLIGHT;
    speedR = TURN_SMALL_INVERSE;
  } else {
    speedL = TURN_MODERATE;
    speedR = TURN_MODERATE_INVERSE;
  }

  analogWrite(L_PWM, speedL);
  analogWrite(R_PWM, speedR);
  delay(TURN_DELAY);

  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);
  analogWrite(L_PWM, FWD_SMALL_SPEED);
  analogWrite(R_PWM, FWD_SMALL_SPEED);
}


void setup() 
{
  pinMode(L_DIR, OUTPUT);
  pinMode(R_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PWM, OUTPUT);

  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);
  stopRobot();

  // Encoder
  pinMode(ENCODER_L_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_L_PIN),
                  updateLeftEncoder,
                  RISING);

  // ===== Target distance =====
  targetTicks = test_distance  * TICKS_PER_CM;
}

/* ================= LOOP ================= */
void loop() 
{
  if (leftTicks >= targetTicks) 
  {
    stopRobot();
    while (1);
  }

  int rawL = analogRead(LS);
  int rawR = analogRead(RS);

  bool L = rawL > TH_L;
  bool R = rawR > TH_R;

  if (L && !R) 
  {
    if (currentDir == 'L') sameDirCount++;
    else 
    {
      currentDir = 'L';
      sameDirCount = 1;
    }
    turnLeft();
  }
  else if (!L && R) 
  {
    if (currentDir == 'R') sameDirCount++;
    else 
    {
      currentDir = 'R';
      sameDirCount = 1;
    }
    turnRight();
  }
  else 
  {
    currentDir = 'F';
    sameDirCount = 0;
    forward();
  }
}