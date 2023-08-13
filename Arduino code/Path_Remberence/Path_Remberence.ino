#include <EEPROM.h>
#include <AFMotor.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <Servo.h>

AF_DCMotor motor1 (1, MOTOR12_1KHZ);  //create motor #1 using M1 output on Motor Driver Shield, set to 1Khz PWM frequency
AF_DCMotor motor2 (2, MOTOR12_1KHZ);  //create motor #2 using M1 output on Motor Driver Shield, set to 1Khz PWM frequency
AF_DCMotor motor3 (3, MOTOR34_1KHZ);  //create motor #3 using M1 output on Motor Driver Shield, set to 1Khz PWM frequency
AF_DCMotor motor4 (4, MOTOR34_1KHZ);  //create motor #4 using M1 output on Motor Driver Shield, set to 1Khz PWM frequency

#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 100

NewPing sonar(TRIG_PIN,ECHO_PIN,MAX_DISTANCE);
SoftwareSerial BT(0,1);

Servo myServo;
bool goesRover = false;
bool updateMemory =false;
int distanceF = 100;
int distanceR = 0;
int distanceL = 0;
int i = 200;            //sets Motor speed
int j = 300;            //sets turn angle
int k = 0;
int range;
int addr = 0;
int addr_Start = 1;
int addr_End = 0;
String myVoice = "forward";
char cmd;

void setup()                           // put your setup code here, to run once:
{
  BT.begin(9600);
  myServo.attach(10);                 // the Motor Shield Servo1 pin corresponds to D2 of arduino Uno Board
  myServo.write(90);                  // Servo motor position itself to 90 degrees

  distanceF = readPing();
  delay(2000);
  cmd = '9';
  range = MAX_DISTANCE-20;
  delay(100);
  k = EEPROM.read(200);
}

void Stop()
{ 
    motor1.run(RELEASE); 
    motor4.run(RELEASE); 
    motor2.run(RELEASE); 
    motor3.run(RELEASE); 
}

int readPing()
{
    int cm = sonar.ping_cm();
    delay(200);
    if(cm == 0)
    {
      cm = range;
    }
    return cm;
}

void goBackward()
{ 
    motor1.run(BACKWARD); 
    motor4.run(BACKWARD); 
    motor2.run(BACKWARD); 
    motor3.run(BACKWARD); 

    motor1.setSpeed(i+3);
    motor4.setSpeed(i-27);
    motor2.setSpeed(i+3);
    motor3.setSpeed(i-27);
    delay(400);
    Stop();
}


void goForward()
{
    motor1.run(FORWARD); 
    motor4.run(FORWARD); 
    motor2.run(FORWARD); 
    motor3.run(FORWARD); 

    motor1.setSpeed(i);
    motor4.setSpeed(i-25);
    motor2.setSpeed(i);
    motor3.setSpeed(i-25);
    delay(1000);
    Stop();
}

void goSmart()
{
  while((goesRover) && (cmd == 's'))
  {
    motor1.run(FORWARD); 
    motor4.run(FORWARD); 
    motor2.run(FORWARD); 
    motor3.run(FORWARD); 

    motor1.setSpeed(i);
    motor4.setSpeed(i-26);
    motor2.setSpeed(i);
    motor3.setSpeed(i-26);
    delay(100);
    distanceF = readPing();
    delay(100);

   if(distanceF < range-50)
    {
      Stop();
      goesRover=false;
      goBackward();
      takeTurn();
      break;
    }
   }
}

void turnRight()
{ 
    motor4.run(BACKWARD); 
    motor4.setSpeed(255);
    motor1.run(FORWARD); 
    motor1.setSpeed(255);
    motor3.run(BACKWARD); 
    motor3.setSpeed(255);
    motor2.run(FORWARD); 
    motor2.setSpeed(255);
    delay(j);
    Stop();
}

void turnLeft()
{ 
    motor1.run(BACKWARD); 
    motor1.setSpeed(255);
    motor4.run(FORWARD); 
    motor4.setSpeed(255);
    motor2.run(BACKWARD); 
    motor2.setSpeed(255);
    motor3.run(FORWARD); 
    motor3.setSpeed(255);
    delay(j);
    Stop();
}

void takeTurn()
{
  j = 100;
            distanceR = lookRight();
            delay(200);
            distanceL = lookLeft();
            delay(200);
            
           if(distanceR >= distanceL) 
              {
                while(distanceF < range)
                {
                turnRight();
                distanceF = readPing();

                if(distanceF = range)
                {
                  goesRover = true;
                  goSmart();
                  break;
                  }
                }
              }
           else if(distanceL >= distanceR)
              {
                while(distanceF < range)
                {
                turnLeft();
                distanceF = readPing();

                if(distanceF = range)
                {
                  goesRover = true;
                  goSmart();
                  break;
                  }
                }
              }          
}
              
int lookRight()
{
    myServo.write(0);
    int distanceR = readPing();
    delay(400);
    myServo.write(90);
    return distanceR;  
}

int lookLeft()
{
    myServo.write(180);
    int distanceL = readPing();
    delay(400);
    myServo.write(90);
    return distanceL;  
}

void memory_IN()
{
     EEPROM.update(addr,cmd);
      
     EEPROM.write(addr_Start,addr);
     
     addr = addr + 1;
  }

void goPath()
{
  for(addr=addr_Start; addr <= addr_End; addr += 1)
  {
  cmd = EEPROM.read(addr);
   if(cmd == 'f')
   {
    goForward();
    delay(50);
    }
   else if(cmd == 'b')
   {
    goBackward();
    delay(50);
    }
   else if(cmd == 'r')
   {
    j = 170+k;
    Stop();
    turnRight();
    }
   else if(cmd == 'l')
   {
    j = 170+k;
    Stop();
    turnLeft();
    }
   else if(cmd == 'u')
   {
    j = 335+k;
    Stop();
    turnRight();
    }
   else if(cmd == 'v')
   {
    j = 335+k;
    Stop();
    turnLeft();
    }
  }
}

void returnPath()
{
  j = 640+k;
    turnRight();  
    Stop();
  j = 100;
  
  for(addr=addr_End; addr >= addr_Start; addr -= 1)
  {
  cmd = EEPROM.read(addr);
   if(cmd == 'f')
   {
    goForward();
    delay(50);
    }
   else if(cmd == 'b')
   {
    goBackward();
    delay(50);
    }
   else if(cmd == 'l')
   {
    j = 170+k;
    Stop();
    turnRight();
    }
   else if(cmd == 'r')
   {
    j = 170+k;
    Stop();
    turnLeft();
    }
   else if(cmd == 'v')
   {
    j = 335+k;
    Stop();
    turnRight();
    }
   else if(cmd == 'u')
   {
    j = 335+k;
    Stop();
    turnLeft();
    }
  }
}

void memory_OUT()
{
  goPath();
  delay(10000);
  returnPath();
}

void loop()                          // put your main code here, to run repeatedly:
{
while(BT.available())  
  {
  cmd = BT.read();

   if(cmd == '1')               //to Start memory IN to Trace Path1
    {
       BT.println("Trace Path for Room-1");
       updateMemory = true;
       addr = 1;
       addr_Start = 0; 
       i = 180;
     }
     
   else if(cmd == '2')               //to Start memory IN to Trace Path2
    {
       BT.println("Trace Path for Room-2");
       updateMemory = true;
       addr = 51;
       addr_Start = 50; 
       i = 180;
     }
     
   else if(cmd == '3')               //to Start memory IN to Trace Path3
    {
       BT.println("Trace Path for Room-3");
       updateMemory = true;
       addr = 101;
       addr_Start = 100; 
       i = 180;
     }
        
   else if(cmd == '4')          //to Start memory OUT to Retrace Path1 i.e., Room1
    {
     BT.println("Go Room-1");
     updateMemory = false;
     i = 175;
     addr_End=EEPROM.read(0);
     addr_Start = 1;
     memory_OUT();
    }

   else if(cmd == '5')          //to Start memory OUT to Retrace Path2 i.e., Room2
    {
     BT.println("Go Room-2");
     updateMemory = false;
     i = 175;
     addr_End = EEPROM.read(50);
     addr_Start = 51;
     memory_OUT();
    }
    
   else if(cmd == '6')          //to Start memory OUT to Retrace Path3 i.e., Room3
    {
     BT.println("Go Room-3");
     updateMemory = false;
     i = 175;
     addr_End = EEPROM.read(100);
     addr_Start = 101;
     memory_OUT();
    }

  else if(cmd == '7')
    {
      k += 1;
      BT.println(k);
      EEPROM.write(200,k); 
      }
      
  else if(cmd == '8')
    { 
      k -= 1;
      BT.println(k);
      EEPROM.write(200,k); 
      }

   else if(cmd == 'z')          //to End memory IN or Speed change
    {
     BT.println("End of TracePath");
     updateMemory = false; 
     }    
    
   else if(cmd == 's')           //move in Rover action i.e., goSmart
    {
     BT.println("Go Smart");
     goesRover = true; 
     j = 100;         
     goSmart();   
    }
   
   else if(cmd == 'f')          //move in Forward direction    
    {
     BT.println("Go Forward");
     goForward();
     if(updateMemory)
        {
        memory_IN();
        } 
    }   
    
   else if(cmd == 'b')          //move in Backward direction    
    {
    BT.println("Go Backward");
    goBackward(); 
    if(updateMemory)
        {
        memory_IN();
        }
     }   
    
   else if(cmd == 'r')         //Turns in Right direction
    {
    BT.println("Turn Right 45D");
    j = 170;
    turnRight();
    if(updateMemory)
        {
        memory_IN();
        }
    } 
    
   else if(cmd == 'l')         //Turns in Left direction
    {
    BT.println("Turn Left 45D");
    j = 170;
    turnLeft();
    if(updateMemory)
        {
        memory_IN();
        }
    } 
    
   else if(cmd == 'u')         //take 90 degree Right turn
    {
    BT.println("Turn Right 90D");
    j = 340;
    turnRight(); 
    if(updateMemory)
        {
        memory_IN();
        }
     } 

   else if(cmd == 'v')         //take 90 degree Left turn
    {
    BT.println("Turn Left 90D");
    j = 340;
    turnLeft(); 
    if(updateMemory)
        {
        memory_IN();
        }
    } 
    
   else if(cmd == 'm')         //Sets motor Speed High
    {
//     BT.println(cmd);
     i = 250; 
     BT.println("speed set ");
     BT.print(i);
    }
    
   else if(cmd == 'n')         //Sets motor Speed Normal
    {
//     BT.println(cmd);
     i = 180; 
     BT.println("speed set ");
     BT.print(i);    }
 }
}  