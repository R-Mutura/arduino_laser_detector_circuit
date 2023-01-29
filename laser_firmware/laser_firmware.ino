#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <Servo.h>

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.

#define Laser_LED 7
#define servo 6
#define DC_motor 5
#define IR_receiver 3 

#define servo_angle_pos 80
#define servo_home_pos 0

int angle = 0;  //keep the angle of the servo

Servo myservo;

volatile int interrupt_counts = 0;
volatile long timer = 0; //used to keep track of time between interrupts

int startprocess = 1;

void ISR_IR();

void setup() {
  // Initiate the LCD:
  lcd.init();
  lcd.backlight();
  
  pinMode(Laser_LED, OUTPUT);
  pinMode(servo, OUTPUT);
  pinMode(DC_motor, OUTPUT);
  pinMode(IR_receiver, INPUT);

  myservo.attach(servo); 
//setup interrupt pin on IR_RECEIVER
  attachInterrupt(digitalPinToInterrupt(IR_receiver), ISR_IR, FALLING);
   lcd.setCursor(2, 0); // Set the cursor on the third column and first row.
  lcd.print("ARDUINO LASER SENSOR!");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Print 'Hello World!' on the first line of the LCD:
  // Print the string "Hello World!"
  
  interrupt_counts = interrupt_counts/3;
  if(interrupt_counts != 0){ //check that the data is not equal to zero
     //send to serial monitor....
     Serial.print(interrupt_counts );
     Serial.println();
    }
  
  
  
  //check if the period between the interrupt is grater than 2minutes
  if(millis()-timer > (120000))
    {
      startprocess = 1; //enable the process to start
    }
  //if the startprocess is set then we do the required process
  if(startprocess){
      //1. start DC Motor
      digitalWrite(DC_motor, HIGH);
      //2. move servo motor to position
      myservo.write(servo_angle_pos);
      //wait 10 sec --
      delay(10000);
      //retract servo motor
      myservo.write(servo_home_pos);
      //stop dc motor
      digitalWrite(DC_motor, LOW);
      //leave loop
      startprocess = 0;//RESET THE START PROCESS.
    }
    //DO OTHER STAFF IN THE LOOP
}

void ISR_IR()
{
  //process the interrupt
    interrupt_counts++;
    timer = millis();
}
