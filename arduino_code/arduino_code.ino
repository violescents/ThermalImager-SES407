#include <Wire.h> // for I2C
#include <Adafruit_MLX90640.h> // thermal camera libary
#include <Servo.h> ///servo libary
Servo shutterServo; //shutterServo is the servo
const int closeSwitch = 9;
const int openSwitch = 7;
const int tima = 50;
const int servoStep = 5;//angle servo will move in each iteration

//create objects
Adafruit_MLX90640 mlx;
float frame[32 * 24];
String command = "";


void movein(){
shutterServo.writeMicroseconds(1450);
delay(50);
shutterServo.writeMicroseconds(1500);
}


void moveout(){
shutterServo.writeMicroseconds(1540);
delay(50);
shutterServo.writeMicroseconds(1500);
}

void blinkCommand(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(tima);
  digitalWrite(LED_BUILTIN, LOW);
  delay(tima);  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(tima);
  digitalWrite(LED_BUILTIN, LOW);
  delay(tima);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(tima);
  digitalWrite(LED_BUILTIN, LOW);
  delay(tima);  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(tima);
  digitalWrite(LED_BUILTIN, LOW);
  delay(tima);
}

void closeShutter(){
    while (digitalRead(closeSwitch) == LOW) {
 //     Serial.println("switch is open");
    movein();
      delay(750);
    }  
  if (digitalRead(closeSwitch) == HIGH ){
    blinkCommand();
  //  Serial.println("switch is closed");
  }
}

void openShutter(){
  while (digitalRead(openSwitch) == LOW ){
    //Serial.println("switch is open");
    moveout();
      delay(750);
  }
    if(digitalRead(openSwitch) == HIGH) {
    blinkCommand();
    //Serial.println("switch is closed");
    }  
}

//function that takes a single "picture"
void takeSnapshot() {
  //if no picture can be obtained, the failure message prints and then exit the function
  if (mlx.getFrame(frame) != 0) {
    //Serial.println("Failed to read frame");
    return;
  }

  for (int y = 0; y < 24; y++) {
    for (int x = 0; x < 32; x++) {
      Serial.print(frame[y * 32 + x], 1);
      if (x < 31) Serial.print(", ");
    }
    Serial.println();
  }


}

void setup() {



  //open serial communication
  Serial.begin(74880);
 //delay to let serial connect
  while (!Serial) {
    delay(100);
  }
pinMode(LED_BUILTIN, OUTPUT);
pinMode(closeSwitch,INPUT_PULLUP);
pinMode(openSwitch,INPUT_PULLUP);


shutterServo.attach(11); //set pin for servo control
//shutterServo.writeMicroseconds(1500);
blinkCommand();
//starts I2C and sets speed at 400kHz
  Wire.begin();
  Wire.setClock(400000);


  if (!mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("MLX90640 not found. Check wiring and reset");
    while (1) {
      //loop that just lasts forever
      delay(10);
    }
  }

  mlx.setMode(MLX90640_CHESS);
  mlx.setResolution(MLX90640_ADC_18BIT);
  mlx.setRefreshRate(MLX90640_2_HZ);

  Serial.println("Thermal Camera ready. going into main loop");
}

void loop() {
  //this section of code is what scans the serial monitor
  // a charcter is assigned the varible c. if it is NOT a enter stroke or new line, than it is added to the command variable
  //when c is assigned to the enter key we check if the command string is == "snap"
  //if it is, then we take a picture
  //if not, it returns an error message
  //either way the command string is set back to blank

    //handshake message receive + send


  while (Serial.available()) {
    //read serial monitor
    char c = Serial.read();
    // removes extra
    if (c == '\n' || c == '\r') {
      command.trim();
  //if the command is = "snap", then run the takesnapshot function. 
  //if not, print error message
      if (command.equals("snap")) {
        takeSnapshot();
      }
      else if(command.equals("ping")){
        Serial.println("pong");
      }
      else if(command.equals("close")){
        closeShutter();
      }
      else if(command.equals("open")){
        openShutter();
      }
      
       else if (command.length() > 0) {
        Serial.println("Unknown command: " + command);
      }

      command = "";
    } 

    else {
      command += c;
    }
  }
}