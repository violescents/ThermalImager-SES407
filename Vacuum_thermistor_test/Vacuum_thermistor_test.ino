

// Define thermistor pins
const int therm1 = A2;             // Analog value for thermistor 1
const int therm2 = A1;             // Analog value for thermistor 2



//Constants//

const int tenK = 10000;       // Constant value for 10k ohm resistor
const int therm_nom = 10000;  // thermistor resistance at nominal temperature
const int B = 3950;           // Beta parameter
const int T0 = 22.4;            // nominal temperature

float reading1;                // define variable for analong reading for thermistor 1
float reading2;                // define variable for analong reading for thermistor 2


float Resistance1;             // define variable for resistance
float Resistance2;             // define variable for resistance


float steinhart1;              // Variable for stienhart equation results
float steinhart2;              // Variable for stienhart equation results

float celsius;
int DT = 1000;  // delay time [ms]


void setup() {
  Serial.begin(9600);
}

void loop() {

// Reading both thermistors
 reading1 = analogRead(therm1);   // analog value from thermistor A0
 reading2 = analogRead(therm2);   // analog value from thermistor A1




  // Convert analog reading to resistance
  Resistance1 = (tenK * reading1) / (1023 - reading1);  // Equation to convert analog value into resistance
  Resistance2 = (tenK * reading2) / (1023 - reading2);  // Equation to convert analog value into resistance

  //Steinhart equation for thermistor 1
  steinhart1 = Resistance1 / therm_nom;      //(R/Ro)
  steinhart1 = log(steinhart1);              //ln(R/Ro)
  steinhart1 /= B;                          // 1/B * ln(R/Ro) divide by biot coefficent. Assign new value to variable steinhart
  steinhart1 += 1.0 / (T0 + 273.15);        // +(1/To)
  steinhart1 = 1.0 / steinhart1;             // invert
  steinhart1 -= 273.15;                     // convert to celsius

  //Steinhart equation for thermistor 2
  steinhart2 = Resistance2 / therm_nom;      //(R/Ro)
  steinhart2 = log(steinhart2);              //ln(R/Ro)
  steinhart2 /= B;                          // 1/B * ln(R/Ro) divide by biot coefficent. Assign new value to variable steinhart
  steinhart2 += 1.0 / (T0 + 273.15);        // +(1/To)
  steinhart2 = 1.0 / steinhart2;             // invert
  steinhart2 -= 273.15;                     // convert to celsius


  // Print Results
  Serial.print("Thermistor 1 Temp: ");
  Serial.print(steinhart1);
  Serial.print(" [C] ");

  Serial.print("Thermistor 2 Temp: ");
  Serial.print(steinhart2);
  Serial.println(" [C] ");

  delay(DT);
}