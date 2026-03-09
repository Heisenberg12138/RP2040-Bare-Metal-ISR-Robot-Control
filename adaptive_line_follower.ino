#define LS A1   // GPIO27
#define RS A0   // GPIO26

#define L_PWM   19
#define L_DIR   18
#define R_PWM   20
#define R_DIR   21

// white line
#define TH_L     53
#define TH_R     60


/*********** Speed ***********/
#define FWD_SPEED           110
#define FWD_SMALL_SPEED     105
#define TURN_SLIGHT         110
#define TURN_MODERATE       130
#define TURN_SHARP          160
#define TURN_INVERSE        80
#define TURN_MODERATE_INVERSE 60
#define TURN_SMALL_INVERSE  45
#define TURN_DELAY          5


//state
char last = 'F';         
char currentDir = 'F';    
int  sameDirCount = 0;    

int speedL = 0;
int speedR = 0;


void setup() {

  pinMode(L_DIR, OUTPUT);
  pinMode(R_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PWM, OUTPUT);

  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, LOW);

  analogWrite(L_PWM, 0);
  analogWrite(R_PWM, 0);
}

void loop() 
{
  int rawL = analogRead(LS);
  int rawR = analogRead(RS);

  bool L = rawL > TH_L;
  bool R = rawR > TH_R;

  /* ======= Direction decision ======= */
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
    if (sameDirCount==1) 
    {
    speedL = TURN_SMALL_INVERSE;
    speedR = TURN_SLIGHT;
    
    }
    else if (sameDirCount==2) 
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

    if(sameDirCount<5)
    {
      digitalWrite(L_DIR, LOW);
      digitalWrite(R_DIR, LOW);
      analogWrite(L_PWM, FWD_SMALL_SPEED );
      analogWrite(R_PWM, FWD_SMALL_SPEED );
    }
 

}

void turnRight() 
{
  last = 'R';

  digitalWrite(L_DIR, LOW);
  digitalWrite(R_DIR, HIGH);
  if (sameDirCount ==1) 
  {
    speedL = TURN_SLIGHT;
    speedR = TURN_SMALL_INVERSE;
  }
    else if (sameDirCount ==2) 
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

    if(sameDirCount<10)
    {
      digitalWrite(L_DIR, LOW);
      digitalWrite(R_DIR, LOW);
      speedL = FWD_SMALL_SPEED;
      speedR = FWD_SMALL_SPEED;
      analogWrite(L_PWM, FWD_SMALL_SPEED );
      analogWrite(R_PWM, FWD_SMALL_SPEED );
    }
    
}


