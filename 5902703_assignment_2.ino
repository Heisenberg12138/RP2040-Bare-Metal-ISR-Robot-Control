/*
 * Student name : [ Yanran Zhao ]
 * Student number : [ 5902703 ]
 *
 * Assignment 2: Obstacle detection 
 */

// Motor 
#define A1A 19   
#define A1B 18   

#define B1A 20   
#define B2A 21   

//Ultrasonic 
#define TRIG_PIN 7
#define ECHO_PIN 6

#define STOP_DISTANCE 15   // cm

// Ultrasonic state
volatile unsigned long echoStart = 0;
volatile unsigned long echoEnd   = 0;
volatile bool echoMeasuring = false;
volatile bool echoReady     = false;

// ISR
void echoISR() {
  if (digitalRead(ECHO_PIN) == HIGH) {
    // Rising edge
    echoStart = micros();
    echoMeasuring = true;
  } 
  else if (echoMeasuring) {
    // Falling edge
    echoEnd = micros();
    echoMeasuring = false;
    echoReady = true;
  }
}

void triggerUltrasonic() {
  // reset state BEFORE trigger
  echoReady = false;
  echoMeasuring = false;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
}

long getDistanceCM() {
  if (!echoReady) return -1;

  noInterrupts();
  unsigned long duration = echoEnd - echoStart;
  echoReady = false;
  interrupts();

  return duration * 0.034 / 2;
}

void setup() {
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(B1A, OUTPUT);
  pinMode(B2A, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(A1B, LOW);
  digitalWrite(B2A, LOW);

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoISR, CHANGE);
}

// Main loop 
void loop() 
{
  static unsigned long lastTriggerTime = 0;
  static long lastDistance = -1;

  unsigned long now = millis();

  if (now - lastTriggerTime >= 60) {
    triggerUltrasonic();
    lastTriggerTime = now;
  }

  long distance = getDistanceCM();
  if (distance > 0) {
    lastDistance = distance;
  }

  // Motor control 
  if (lastDistance > 0 && lastDistance < STOP_DISTANCE) {
    analogWrite(A1A, 0);
    analogWrite(B1A, 0);
  } else {
    analogWrite(A1A, 100);
    analogWrite(B1A, 100);
  }
}
