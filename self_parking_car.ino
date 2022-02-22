
#include <AFMotor.h>
//motor oins define
AF_DCMotor Left_On_Motor(4);
AF_DCMotor Right_On_Motor(3);
AF_DCMotor Left_Back_Motor(1);
AF_DCMotor Right_Back_Motor(2);
#include <Ultrasonic.h>
//ultrasonic pins define
Ultrasonic ultrasonic_back(40, 41), ultrasonic_left_back(38, 39), ultrasonic_left_on(36, 37), ultrasonic_on(34, 35);

#define Left 0 //left direction command
#define Right 1 //right direction command 
#define Next 2 //Next direction command
#define Back 3 //back direction command
#define minimum_limit 15 //Width of the car (cm)
#define minimum_limit1 28 //Length of the car(cm)

byte park_status = 0;
int signalpin = 21;
volatile int val;

int counter = 0;
int current_status = 0;
int previous_status = 0;

void count(int countdir)
{
  for (int i = 0 ; i <= countdir; i + 1)
  {
    val = digitalRead(signalpin);
    if (val == LOW) {

      current_status = 0;
    }
    else {

      current_status = 1;
    }

    if (current_status != previous_status)
    {
      if (current_status == 1)
      {
        counter = counter + 1;
        Serial.println(counter);
        i = i + 1;
      }
      else
      {
        i = i ;
      }

      previous_status = current_status;

    }
    if (i == countdir)
    {

      Left_On_Motor.run(RELEASE);
      Right_On_Motor.run(RELEASE);
      Left_Back_Motor.run(RELEASE);
      Right_Back_Motor.run(RELEASE);

    }

  }

}

void motor_pinSetup()
{

  Left_On_Motor.run(RELEASE);
  Right_On_Motor.run(RELEASE);
  Left_Back_Motor.run(RELEASE);
  Right_Back_Motor.run(RELEASE);
}

//Car Movement
void Robot_Movement(byte motor, byte spd)
{
  if (motor == Next)
  {
    Left_On_Motor.setSpeed(spd);
    Right_On_Motor.setSpeed(spd);
    Left_Back_Motor.setSpeed(spd);
    Right_Back_Motor.setSpeed(spd);
    Left_On_Motor.run(FORWARD);
    Right_On_Motor.run(FORWARD);
    Left_Back_Motor.run(FORWARD);
    Right_Back_Motor.run(FORWARD);

  }
  if (motor == Back)
  {
    Left_On_Motor.setSpeed(spd);
    Right_On_Motor.setSpeed(spd);
    Left_Back_Motor.setSpeed(spd);
    Right_Back_Motor.setSpeed(spd);
    Left_On_Motor.run(BACKWARD);
    Right_On_Motor.run(BACKWARD);
    Left_Back_Motor.run(BACKWARD);
    Right_Back_Motor.run(BACKWARD);

  }
  if (motor == Left)
  {
    Left_On_Motor.setSpeed(spd);
    Right_On_Motor.setSpeed(spd);
    Left_Back_Motor.setSpeed(spd);
    Right_Back_Motor.setSpeed(spd);
    Left_On_Motor.run(BACKWARD);
    Right_On_Motor.run(FORWARD);
    Left_Back_Motor.run(BACKWARD);
    Right_Back_Motor.run(FORWARD);

  }

  if (motor == Right)
  {
    Left_On_Motor.setSpeed(spd);
    Right_On_Motor.setSpeed(spd);
    Left_Back_Motor.setSpeed(spd);
    Right_Back_Motor.setSpeed(spd);
    Left_On_Motor.run(FORWARD);
    Right_On_Motor.run(BACKWARD);
    Left_Back_Motor.run(FORWARD);
    Right_Back_Motor.run(BACKWARD);

  }

}

void Robot_Hard()
{
  Left_On_Motor.run(RELEASE);
  Right_On_Motor.run(RELEASE);
  Left_Back_Motor.run(RELEASE);
  Right_Back_Motor.run(RELEASE);
}

// control parking steps
bool Park_Place_Control()
{

  long on_Sensor = ultrasonic_on.Ranging(CM);
  long right_Sensor = ultrasonic_left_on.Ranging(CM);
  long right_back_Sensor = ultrasonic_left_back.Ranging(CM);

  if ( (right_Sensor <= minimum_limit) && (right_back_Sensor <= minimum_limit) && (park_status == 0))
  {
    Robot_Movement(Next, 100);
    park_status = 1; 
    Serial.println(park_status);
  }

  if ((right_Sensor > minimum_limit) && (right_Sensor < minimum_limit1) && (right_back_Sensor > minimum_limit) && (right_back_Sensor < minimum_limit1) && (park_status == 1))
  {
    Robot_Movement(Next, 100);
    park_status = 2; 
    Serial.println(park_status);
  }

  if ((right_Sensor >= minimum_limit1) && (right_back_Sensor >= minimum_limit1) && (park_status == 1))
  {
    /* Vertical Parking Method*/
    Robot_Hard() ;
    delay(500);
    park_status = 10; 
    Serial.println(park_status);
  }

  if ((right_Sensor <= minimum_limit) && (right_back_Sensor <= minimum_limit) && (park_status == 2))
  {
    /* Parallel Parking Method */
    park_status = 3; 
    Serial.println(park_status);
  }

  return park_status;
}

void Park_find()
{
  Park_Place_Control();
  if (park_status == 3 )
  {
    Robot_Hard(); 
    Serial.println(park_status);
    delay(400);
    park_status = 4;
  }
  if (park_status == 4 )
  {

    Robot_Movement(Back, 120);
    count(18);
    Robot_Hard(); 
    Serial.println(park_status);
    delay(500);
    Robot_Movement(Right, 150);
    count(9);
    Robot_Hard();
    delay(500);
    park_status = 5;
  }
  if (park_status == 5)
  {

    Robot_Movement(Back, 120);
    long back_Sensor = ultrasonic_back.Ranging(CM); 
    Serial.println(back_Sensor);

    if (back_Sensor > 0 && back_Sensor <= 13)
    {
      Robot_Hard();
      delay(400);
      park_status = 6;
    }
    return back_Sensor;
  }

  if (park_status == 6)
  {
    Robot_Movement(Left, 150);
    long right_Sensor = ultrasonic_left_on.Ranging(CM); 
    Serial.println(right_Sensor);
    long right_back_Sensor = ultrasonic_left_back.Ranging(CM); 
    Serial.println(right_back_Sensor);

    if (right_Sensor == right_back_Sensor)
    {
      Robot_Hard();
      park_status = 7;
    }

    return right_Sensor, right_back_Sensor;
  }
  if (park_status == 7)
  {
    long on_Sensor = ultrasonic_on.Ranging(CM);

    if (on_Sensor <= 6)
    {
      Robot_Hard();
      park_status = 8;
    }
    else
    {
      Robot_Movement(Next, 100);
    }
    return on_Sensor;
  }
  if (park_status == 10)
  {

    Robot_Movement(Left, 180);
    count(14);
    Robot_Hard();
    delay(500);
    park_status = 7;

  }

}

void setup()
{
    // put your setup code here, to run once:

  Serial.begin(9600);
  attachInterrupt(5, count, CHANGE);
  pinMode (signalpin, INPUT) ;

  motor_pinSetup();
}

void loop()
{
    // put your main code here, to run repeatedly:
  Park_find();
}
