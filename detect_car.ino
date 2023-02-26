#define trig 13
#define echo 12
int thresholddistance = 50;   //adjust distance (50 = 50 cm.)

long duration;
float forwarddistance;
float distancefront;
float distanceArr[4];
float distance_const = 10.0;

#include <Servo.h>
#define pinServo 3

//Initialization Setting up Servo Angles
int servoArray[4] = {0,50,130,180};
Servo myServo;

//Initialization Motors
int motor_left[2] = {7, 6};
int motor_right[2] = {4, 5};

void setup() {
  Serial.begin(9600);
  myServo.attach(pinServo);
  //Set up Motors
  pinMode(motor_left[0], OUTPUT);
  pinMode(motor_right[0], OUTPUT);
  pinMode(motor_left[1], OUTPUT);
  pinMode(motor_right[1], OUTPUT);
  // Setup Sensors Ultrasonic
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

// Following Module is for break
void to_break(){
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], LOW);
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], LOW);
}

// Following Module is for the robot to go foword
void drive_forward(){
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], HIGH);
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], HIGH);
}

// Following Module is for the robot turn left
void turn_left(){
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], HIGH);
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], LOW);
  delay(400);   // Delay 400 ms.
}

// Following Module is for the robot turn right
void turn_right(){
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], LOW);
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], HIGH);
  delay(400);   // Delay 400 ms.
}

// Following Module is to make backward
void backward(){
  digitalWrite(motor_left[0], HIGH);
  digitalWrite(motor_left[1], LOW);
  digitalWrite(motor_right[0], HIGH);
  digitalWrite(motor_right[1], LOW);
  delay(600);   // Delay 600 ms.  
}
// ---------------------------- Sight Forward
float distance(int angle){
  myServo.write(angle);
  delay(400);
  digitalWrite(trig, LOW);
  delayMicroseconds(20);
  digitalWrite(trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  forwarddistance = duration / 58; // to centimeters
  return forwarddistance;
}

// ---------------------------- Sensor Array Module
void scan_around(){
  for (int i = 0;i < 4; i++){
    myServo.write(servoArray[i]); //0 50 130 180
    delay(100);
    digitalWrite(trig, LOW);
    delayMicroseconds(20);
    digitalWrite(trig, HIGH);
    delayMicroseconds(20);
    digitalWrite(trig, LOW);
    duration = pulseIn(echo, HIGH);
    distanceArr[i] = duration / 58; // to centimeters recieve distance
    Serial.print("distanceArr[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(distanceArr[i]);
    delay(100);
  }  
}

void loop() 
{
  myServo.write(90);
  distancefront = distance(90); 
  Serial.println(distancefront);
  // Serial.println(thresholddistance);
  if ((distancefront > thresholddistance) && (distancefront < 1000))
  {
    drive_forward();
    delay(400);
    Serial.println("1. drive_forward");
  }
  else
  {
    to_break();
    scan_around();
    if ((distanceArr[0] > 1000) + (distanceArr[1] > 1000) && (distanceArr[2] > 1000) + (distanceArr[3] > 1000))
    {
      return;
      Serial.println("Restarted the Loop");
      delay(100);
    }
    else if (((distanceArr[0]) + (distanceArr[1]) + (distanceArr[2]) + (distanceArr[3])) < 80 )
    {
      backward();  
      delay(800);
      drive_forward();
      delay(400);
      Serial.println("2. backward and then Forward");      
    }
    else if ((((distanceArr[0]) + (distanceArr[1]) + (distanceArr[2]) + (distanceArr[3])) < 80) && ((distanceArr[0]) > (distanceArr[3])))
    {
      backward();
      delay(400);
      turn_right();
      to_break();
      delay(400);
      Serial.println("3. Backward and then turn right");
    }
    else if ((((distanceArr[0]) + (distanceArr[1]) + (distanceArr[2]) + (distanceArr[3])) < 80) && ((distanceArr[0]) < (distanceArr[3])))
    {
      backward();
      delay(400);
      turn_left();
      to_break();
      delay(400);
      Serial.println("4. Backward and then turn left");
    }
    else if (((distanceArr[2]) < distance_const) && ((distanceArr[1]) < distance_const))
    {
      backward();
      turn_left(); //or turn_right();
      delay(400);
      to_break();
      delay(400);
      Serial.println("5. Backward and then turn left");
    }
    else if(((distanceArr[0]) + (distanceArr[1])) > ((distanceArr[2]) + (distanceArr[3])))
    {
      turn_right();
      drive_forward();
      delay(600);
      Serial.println("6. turn right");      
    }    
    else if(((distanceArr[0]) + (distanceArr[1])) < ((distanceArr[2]) + (distanceArr[3])))
    {
      turn_left();
      drive_forward();
      delay(600);
      Serial.println("7. turn left");
    }
    else
    {
      backward();
      delay(400);
      to_break();
      turn_right();   //or turn_left();
      drive_forward();
      Serial.println("8. backward");      
    }    
  }
}
