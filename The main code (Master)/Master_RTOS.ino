#include <Arduino_FreeRTOS.h>
 #include <Servo.h>
//radio
 #include <TEA5767.h>
#include <Wire.h>

//////////////////////////////////////////////////////////CAR 
//Ulra Sonic Pins (3 ultra sonic sensors)
//pwm pins
//Back sensor
#define trigPin1 13
#define echoPin1 12
//Right sensor
#define trigPin2 11 
#define echoPin2 10 
//Front sensor
#define trigPin3 7 
#define echoPin3 6 
// Motors Speed 
#define S 130

int sound = 45; 

// Motor A

#define enA 9
#define in1  8
#define in2  2

// Motor B

#define enB 5
#define in3  3
#define in4 4
//parking space
int space=0;

//Move Functions Prototype

void Stop();
void MoveForward();
void MoveLeft();
void MoveBack();
void MoveRight();
//ultra Sonic Function prototype
int UFun(int triger,int echo);
//functions prototypes
void Park();
int Check();


//////////////////////////////////////////////////////////7SEG & FUEL
const byte a = 37 ;
const byte b = 39;
const byte c = 38;
const byte d = 36;
const byte e = 34;
const byte f = 35;
const byte g = 33;

//////////////////////////////////////////////////////////RAINDROP&SERVO
 //digital input
 const int  rainDrop = 50;
 //output
int servoPin = 44;
Servo Servo1; 



//////////////////////////////////////////////////////////RADIO
//Constants:
TEA5767 Radio; //Pinout SLC and SDA - Arduino uno pins A5 and A4


//Variables:
int firstTime = 1;
double old_frequency;
double frequency;
int search_mode = 0;
int search_direction;
unsigned long last_pressed;
unsigned char buf[5];
int stereo;
int signal_level;
double current_freq;
unsigned long current_millis = millis();
int inByte;
int flag=0;
int counter = 0;
String freq = "";




//////////////////////////////////////////////////////////RTOS
void Task1( void *pvParameters );
void Task2( void *pvParameters );
void Task3( void *pvParameters );
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;



void setup() {
   Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);
////////////////////////////////////////////////////////////CAR SETUP
//ultra sonic pins

  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);
  pinMode(trigPin3,OUTPUT);
  pinMode(echoPin3,INPUT);
  
// Set all the motor control pins to outputs

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);


    pinMode(sound, OUTPUT); 


//////////////////////////////////////////////////////////7SEG & FUEL SETUP
//pinMode(a, OUTPUT);
//  pinMode(b, OUTPUT);
//  pinMode(c, OUTPUT);
//  pinMode(d, OUTPUT);
//  pinMode(e, OUTPUT);
//  pinMode(f, OUTPUT);
//  pinMode(g, OUTPUT);
////////////////////////////////////////////////////////////RAINDROP&SERVO SETUP  
//Servo1.attach(servoPin);
//pinMode(rainDrop,INPUT);  

////////////////////////////////////////////////////////////RADIO SETUP 
// pinMode(52, OUTPUT);
//  digitalWrite(52,HIGH);
//  Radio.init();
//  Radio.set_frequency(95.4); 
xTaskCreate(
    Task1
    ,  "task1"   
    ,  500  
    ,  NULL
    ,  10
    ,  &TaskHandle_1 ); 

  vTaskStartScheduler();  
}

void loop() {
  // put your main code here, to run repeatedly:

}

void Task1(void *pvParameters)  {
  pinMode(8, OUTPUT);
  while(1)
  {
     if (/*Check()==1&& space>75*/1){//space is measured by trial and error to check the right value ,it's varied by the speed
    /*space=0;
    Stop();
    delay(1000);
    Park();*/
    
      xTaskCreate(
    Task2
    ,  "task2"
    ,  2000
    ,  NULL
    ,  10  
    ,  NULL );

       xTaskCreate(
    Task3
    ,  "task3"
    ,  1024
    ,  NULL
    ,  10 
    ,  NULL );

    
     vTaskDelete(TaskHandle_1);
    }
  else {
    if(UFun(trigPin1,echoPin1)>5&&UFun(trigPin3,echoPin3)>15)
    {
      MoveForward();
    }
    else{
      Stop();
      }
    }
  }
}

void Task2(void *pvParameters)  {
  //////////////////////////////////////////////////////////7SEG & FUEL SETUP
pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  //////////////////////////////////////////////////////////RAINDROP&SERVO SETUP  
Servo1.attach(servoPin);
pinMode(rainDrop,INPUT);  
  while(1){
     Seg_Fuel();
     
     raindrop_servo();
     
    Serial.print("here3");
      vTaskDelay( 200 / portTICK_PERIOD_MS );
    }
  }
void Task3(void *pvParameters)  {
  //////////////////////////////////////////////////////////RADIO SETUP 
 pinMode(52, OUTPUT);
  digitalWrite(52,HIGH);
  Radio.init();
  Radio.set_frequency(95.4); 
  while(1){
    radio();
     vTaskDelay( 200 / portTICK_PERIOD_MS );
    }
  }
void Park(){//Parking algorithm
  MoveBack();
  delay(500);
  noTone(sound);
  Stop();
  delay(3000);
  MoveLeft();
  delay(300);
  Stop();
   delay(500);
  MoveLeft();
  delay(300);
  Stop();
   delay(500);
  MoveLeft();
  delay(300);
  Stop();
   delay(500);
   MoveLeft();
  delay(350);
  Stop();
   delay(500);
   
  while(UFun(trigPin1,echoPin1)>8){
    buzzerON();
   delay(2000);
 
   
  MoveBack();
  
   delay(150);
   noTone(sound);
  Stop();
  delay(500);
  }
  noTone(sound);
  Stop();
  delay(2000);
   MoveRight();
  delay(250);
  Stop();
  delay(500);
   MoveRight();
  delay(300);
  Stop();
  delay(500);
  MoveRight();
  delay(250);
  Stop();
  delay(500);
   
 // while(UFun(trigPin3,echoPin3)>8){
  MoveForward();
  delay(200);
 // Stop();
 // delay(500);
  //}
  //MoveRight();
  //delay(1600);
  Stop();
 
  }
int Check(){//Chicking the sutable parking space for the car width
  int Flag =0;
  while(UFun(trigPin2,echoPin2)>10){
    MoveForward();
    space++;
    Flag =1;
    }
  return Flag;
  }
int UFun(int triger , int echo){//ultrasonic reading 
   long duration ;
   int distance;
        digitalWrite(triger,LOW);
        delayMicroseconds(2);
        digitalWrite(triger, HIGH);
        delayMicroseconds(10);
        digitalWrite(triger,LOW);
    
        duration = pulseIn(echo,HIGH);
        distance = duration/2 /29.1 ; // 331.5 m/s ===> 0 C.....331.5 +0.6*temp....343.5*100/1000000=.03435cm/us
  return distance; 
  
  }
void MoveBack(){
  
   
   
   
      
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
      
        // Set speed to 200 out of possible range 0~255
      
        analogWrite(enA, S+80);
      
        // Turn on motor B
      
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
      
        // Set speed to 200 out of possible range 0~255
      
        analogWrite(enB, S+80);

       
 
  
  
  }
void MoveLeft(){

  // Turn on motor A

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  // Set speed to 200 out of possible range 0~255

  analogWrite(enA, S+80);

  // Turn on motor B

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  // Set speed to 200 out of possible range 0~255

  analogWrite(enB, S+80);

  

  }
void MoveRight(){
      
    // Turn on motor A

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Set speed to 200 out of possible range 0~255

  analogWrite(enA, S+80);

  // Turn on motor B

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  // Set speed to 200 out of possible range 0~255

  analogWrite(enB, S+80);


     }
void Stop(){
  // Now turn off motors
 
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
  }
void MoveForward() {
    // Turn on motor A

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Set speed to 200 out of possible range 0~255

  analogWrite(enA, S+20);

  // Turn on motor B

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  // Set speed to 200 out of possible range 0~255

  analogWrite(enB, S+20);


} 
void radio(){
  Wire.requestFrom(8, 1);    // request 6 bytes from slave device #8

  String f = "";
  float f1 ;
 while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
          // print the character
          Serial.println(c);
          if(c == '0'){
            f =f+"0";
          }else if(c == '1'){
            f =f+"1";
          }else if(c == '2'){
            f =f+"2";
          }else if(c == '3'){
            f =f+"3";
          }else if(c == '4'){
            f =f+"4";
          }else if(c == '5'){
            f =f+"5";
          }else if(c == '6'){
            f =f+"6";
          }else if(c == '7'){
            f =f+"7";
          }else if(c == '8'){
            f =f+"8";
          }else if(c == '9'){
            f =f+"9";
          }
          else if(c == '+'){
            inByte = '+';
            flag=0;
          }
          else if(c == '-'){
            inByte = '-';
            flag=0;
          }else if(c == '*'){
            inByte = '*';
            flag=0;
          }
          else if(c == '/'){
            inByte = '/';
            flag=0;
          }else if(c=='0'){
            flag=1;
          }
          Serial.print(c);
  }
  f1= f.toFloat()/100;
  
   Serial.print(current_freq);
//Serial.println(inByte);
  //Serial.println("");
  if (Serial.available()>0) {
    inByte = Serial.read();
    if (inByte == '+' || inByte == '-'){  //accept only + and - from keyboard
      flag=0;
    }
  }


  if (Radio.read_status(buf) == 1) {
     current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
     stereo = Radio.stereo(buf);
     signal_level = Radio.signal_level(buf);
     //By using flag variable the message will be printed only one time. 
     if(flag == 0){
      Serial.print("Current freq: ");
      Serial.print(current_freq);
      Serial.print("MHz Signal: ");
      //Strereo or mono ?
      if (stereo){
        Serial.print("STEREO "); 
      } 
    else{
      Serial.print("MONO ");
    } 
      Serial.print(signal_level);
      Serial.println("/15");
      flag=1;
     }
  }
   
  //When button pressed, search for new station
  if (search_mode == 1) {
      if (Radio.process_search (buf, search_direction) == 1) {
          search_mode = 0;
      }
  }
  //If forward button is pressed, go up to next station
  if (inByte == '+') {
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
    inByte='0';
  }
  //If backward button is pressed, go down to next station
  if (inByte == '-') {
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_DOWN;
    Radio.search_down(buf);
    inByte='0';
  } 
  if (inByte == '/') {
    digitalWrite(52,HIGH);
     Radio.init();
  Radio.set_frequency(95.2);
  } 
  if (inByte == '*') {
    digitalWrite(52,LOW);
  } 
  
  if (inByte == '/') {
    digitalWrite(52,HIGH);
     Radio.init();
  Radio.set_frequency(95.2);
  } 
  if (inByte == '*') {
    digitalWrite(52,LOW);
  } 
  
  
  delay(500);    

}
void buzzerON(){
  if(UFun(trigPin1,echoPin1)>25 ){
    tone(sound, 1000, 1000);
   }else
  if(UFun(trigPin1,echoPin1)<25 && UFun(trigPin1,echoPin1)>20){
    tone(sound, 1000, 1000);
    delay(500);
    noTone(sound);
    delay(500);
    tone(sound, 1000, 1000);
    delay(500);
    noTone(sound);
    delay(500);
   }else
   if(UFun(trigPin1,echoPin1)<20 && UFun(trigPin1,echoPin1)>15){
    tone(sound, 1000, 1000);
    delay(300);
    noTone(sound);
    delay(300);
    tone(sound, 1000, 1000);
    delay(300);
    noTone(sound);
    delay(300);
    tone(sound, 1000, 1000);
    delay(300);
    noTone(sound);
    delay(300);
   }else 
   if(UFun(trigPin1,echoPin1)<15 && UFun(trigPin1,echoPin1)>10){

    tone(sound, 1000, 1000);
    delay(100);
    noTone(sound);
    delay(100);
    tone(sound, 1000, 1000);
    delay(100);
    noTone(sound);
    delay(100);
    tone(sound, 1000, 1000);
    delay(100);
    noTone(sound);
    delay(100);
    tone(sound, 1000, 1000);
    delay(100);
    noTone(sound);
    delay(100);
   }else if(UFun(trigPin1,echoPin1)<10 ){
   tone(sound, 1000, 1000);
    delay(50);
    noTone(sound);
    delay(50);
    tone(sound, 1000, 1000);
    delay(50);
    noTone(sound);
    delay(50);
    tone(sound, 1000, 1000);
    delay(50);
    noTone(sound);
    delay(50);
    tone(sound, 1000, 1000);
   delay(50);
    noTone(sound);
    delay(50);
   }
}

void Seg_Fuel(){
  
    int value = analogRead(A0);
    Serial.println(value);
  
  if (value > 550) {
   
    showNumber(2);
    delay(100);
    }
  else if ((value > 300) && (value <= 550)) {
    showNumber(1);
    delay(100);
    }
  else{
    showNumber(0);
  delay(100);
  }
  
}
void raindrop_servo(){
  
    if(digitalRead(rainDrop)==0)
{
Servo1.write(0); 
   delay(1000); 
    
   Servo1.write(180); 
   delay(2000);
  
}
  }

void showNumber(int state){
  if(state ==0){
    digitalWrite(a,LOW);
     digitalWrite(b,LOW);
     digitalWrite(c,LOW);
     digitalWrite(d,LOW);
     digitalWrite(e,LOW);
     digitalWrite(f,LOW);
     digitalWrite(g,HIGH);
  }else if(state ==1){
    digitalWrite(a,HIGH);
     digitalWrite(b,LOW);
     digitalWrite(c,LOW);
     digitalWrite(d,HIGH);
     digitalWrite(e,HIGH);
     digitalWrite(f,HIGH);
     digitalWrite(g,HIGH);
    
  }else if(state ==2){
    digitalWrite(a,LOW);
     digitalWrite(b,LOW);
     digitalWrite(c,HIGH);
     digitalWrite(d,LOW);
     digitalWrite(e,LOW);
     digitalWrite(f,HIGH);
     digitalWrite(g,LOW);
    
  }
}
