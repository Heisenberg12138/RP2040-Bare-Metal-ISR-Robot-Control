#define LS A1   // GPIO27
#define RS A0   // GPIO26
#define L_PWM   19
#define L_DIR   18
#define R_PWM   20
#define R_DIR   21
#define FRONT_TRIG 7
#define FRONT_ECHO 6

//Line Thresholds 
#define TH_L     60
#define TH_R     70


//Line follower speed
#define FWD_SPEED           120
#define FWD_SMALL_SPEED     105
#define TURN_SLIGHT         115
#define TURN_MODERATE       140
#define TURN_SHARP          160
#define TURN_INVERSE        60
#define TURN_MODERATE_INVERSE 55
#define TURN_SMALL_INVERSE  45
#define TURN_DELAY          5
#define TURN_AVOID  180


//Avoidance speed
#define CLEAR_TH        32 // cm
#define SCAN_STEP_TIME  80   // ms
#define SIDE_FORWARD_TIME   3300
#define RETURN_TURN_TIME    1100
#define RETURN_FORWARD_TIME 2000



bool possibleAvoidance = true;
bool findLine = false;
char last = 'F';          // 'L' 'R' 'F'
char currentDir = 'F';   
int  sameDirCount = 0;    

int speedL = 0;
int speedR = 0;


long getDistance() {
  digitalWrite(FRONT_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(FRONT_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(FRONT_TRIG, LOW);

  long duration = pulseIn(FRONT_ECHO, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void stopRobot() {
  analogWrite(L_PWM, 0);
  analogWrite(R_PWM, 0);
}

void turnRightAvoid() {
  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);
  analogWrite(L_PWM, 220);  
  analogWrite(R_PWM,0);
}

void turnLeftAvoid() {
  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);
  analogWrite(L_PWM, 0);
  analogWrite(R_PWM, TURN_AVOID);
}

void moveForwardAvoid() {
  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);
  analogWrite(L_PWM, FWD_SPEED);
  analogWrite(R_PWM, FWD_SPEED);
}

//obstacle avoidance
void avoidObstacle() 
{
  stopRobot();
  delay(300);

  // ===== Phase 1: Scan  =====
  int safeCount = 0;
  long dist = getDistance();

  while (dist < CLEAR_TH || safeCount < 3) {
    if (dist >= CLEAR_TH) safeCount++;

    turnRightAvoid();
    delay(SCAN_STEP_TIME);
    stopRobot();
    delay(60);

    dist = getDistance();
  }

  // ===== Phase 2: Execute =====
  moveForwardAvoid();
  delay(SIDE_FORWARD_TIME);
  stopRobot();
  delay(200);

  turnLeftAvoid();
  delay(RETURN_TURN_TIME);
  stopRobot();
  delay(200);

  // ===== Phase 3: SECOND scan  =====
  safeCount = 0;
  dist = getDistance();

    while (dist < CLEAR_TH + 12||safeCount<1) 
    {
      if (dist >= CLEAR_TH + 12) safeCount++;

      turnRightAvoid();
      delay(SCAN_STEP_TIME);
      stopRobot();
      delay(60);

      dist = getDistance();
    }
  // Finish: return to caller
  findLine=true;
  possibleAvoidance = false;
}


//Line follower actions
void forward() {
  last = 'F';
  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);

  speedL = FWD_SPEED;
  speedR = FWD_SPEED;

  analogWrite(L_PWM, speedL);
  analogWrite(R_PWM, speedR);
}
void turnBigRight()
{
    digitalWrite(L_DIR, LOW);
    digitalWrite(R_DIR, HIGH);

    speedL = 200;
    speedR = 90;

    analogWrite(L_PWM, speedL);
    analogWrite(R_PWM, speedR);
    delay(30);
}
void turnLeft() {
  last = 'L';

  digitalWrite(L_DIR, HIGH);
  digitalWrite(R_DIR, LOW);

  if (sameDirCount <= 1) 
  {
    speedL = TURN_SMALL_INVERSE;
    speedR = TURN_SLIGHT;
  }
  else if (sameDirCount == 2) 
  {
    speedL = TURN_MODERATE_INVERSE;
    speedR = TURN_MODERATE;
  }
  else 
  {
    speedL = TURN_INVERSE;
    speedR = TURN_SHARP;
  }

  analogWrite(L_PWM, speedL);
  analogWrite(R_PWM, speedR);
  delay(TURN_DELAY);
  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);
  analogWrite(L_PWM, FWD_SMALL_SPEED );
  analogWrite(R_PWM, FWD_SMALL_SPEED );
}

void turnRight() {
  last = 'R';

  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, HIGH);

  if (sameDirCount <= 1) 
  {
    speedL = TURN_SLIGHT;
    speedR = TURN_SMALL_INVERSE;
  }
  else if (sameDirCount == 2) 
  {
    speedL = TURN_MODERATE;
    speedR = TURN_MODERATE_INVERSE;
  }
  else 
  {
    speedL = TURN_SHARP;
    speedR = TURN_INVERSE;
  }

  analogWrite(L_PWM, speedL);
  analogWrite(R_PWM, speedR);
  delay(TURN_DELAY);
  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);
  analogWrite(L_PWM, FWD_SMALL_SPEED );
  analogWrite(R_PWM, FWD_SMALL_SPEED );
}


void setup() 
{
  pinMode(L_DIR, OUTPUT);
  pinMode(R_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PWM, OUTPUT);

  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);

  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);

  stopRobot();
}

void loop() 
{

  // Obstacle check FIRST (avoidance takes over)
  long frontDist = getDistance();
  if ((frontDist > 0 && frontDist < CLEAR_TH)&&possibleAvoidance) 
  {
    avoidObstacle();
    return;
  }

  int rawL = analogRead(LS);
  int rawR = analogRead(RS);

  bool L = rawL > TH_L;
  bool R = rawR > TH_R;

  //when refind line,stop a while
  if(findLine&&(L||R))
  {
    stopRobot();
    delay(220);
    last='R';
    turnBigRight();
    findLine=false;
    stopRobot();
    delay(20);
  }
  else if (L && !R) 
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
