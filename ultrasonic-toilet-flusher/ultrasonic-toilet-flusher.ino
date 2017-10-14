#include <SR04.h>
#define TRIG_PIN 12
#define ECHO_PIN 11
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);


//---------------------- Set this value to the distance from ultrasonic sensor to the end of your toilet
int toilet_distance = 85; // unit: centimeter
//--------------------------------------------

#include <Servo.h>
Servo myservo;
String userInput;
int deg;
long distance;
int counter = 0;
int miss = 0;
const int missMax = 5;  // How many miss count before flush
const int flushMin = 10; // Minimum count to flush
const servo_stop_point = 45 //default should be 90 but it's up to your servo
const servo_low = 15
const servo_high = 80

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  myservo.attach(9);
  //---------------------- Set this value to your continuous servo position (default 90)
  myservo.write(servo_stop_point);
  //--------------------------------------------
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  distance = sr04.Distance();
  //Serial.print("Distance ");
  //Serial.println(distance);

  if (distance > toilet_distance && counter == 0){                      // Do nothing go out of if statement
  }else if (distance < toilet_distance){                               // Object detected start counting
    counter ++;
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    miss = 0;
  }else if ( distance > toilet_distance && counter >= 1 && miss < missMax){   // Detected Object missing mark miss
    miss ++;
  }else if ( distance > toilet_distance && ( counter < flushMin) &&  miss == missMax){
    counter = 0;
    miss = 0;
    //Serial.println("clear");
  }else if ( distance > toilet_distance && miss >= missMax && counter >= flushMin){  // Object Stay for over 10 secs but now missing flush
    if (counter >= flushMin){                             // Half Flush
      // Action : Roll Left 4.9 seconds stop for 0.5 second and roll right for 4.95 second to get to original position
      // test and modify the value that fit you Note: due to slower roll right speed on my servo I need a slightly longer run time
      myservo.write(servo_high);
      delay(4900);
      myservo.write(servo_stop_point);
      delay(500);
      myservo.write(servo_low);
      delay(4950);
      myservo.write(servo_stop_point);
    }else if (counter >= 60){                            // Full Flush
      myservo.write(servo_high);
      delay(4900);
      myservo.write(servo_stop_point);
      delay(1000);
      myservo.write(servo_low);
      delay(4950);
      myservo.write(servo_stop_point);
    }
    counter = 0;
    miss = 0;
  }
  //Serial.println("counter: " + String(counter));
  //Serial.println("miss: " + String(miss));
  delay(1000);
  /*
  if(Serial.available() > 0){
    userInput = Serial.readString();
    deg = userInput.toInt();
    Serial.print("Userinput ");
    Serial.println(deg);
    motor_status = 1;
  }
  if(deg >= 0 && deg <= 175 && motor_status == 1){
    myservo.write(deg);
    //myservo.write(0);
    motor_status = 0;
  }
  delay(1000);
  */
}
