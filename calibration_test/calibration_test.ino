#define RSERIES 10000 // series resistor 
#define THERMISTOR A2
#define R0 10000 //thermosistor nominal 10k
#define T0 295.65 // nominal room temp -->22.5
#define BCOEFF 3950 // beta coeff of thermosistor
#define SAMPLES 5 //how many samples to take so avg + smoother (more accurate) reading

int samples[SAMPLES]

void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL);
}

void loop() {
  unit8_t i;
  float avg;
  float tempConv; // in K
  float temp

  //averaging out temp reads
  for (i =0; i < SAMPLES; i++){
    samples[i] = analogRead(THERMISTOR);
    delay(10);
  }
  avg /= SAMPLES;

  Serial.print("avg analog read: "); Serial.println(avg);

  //value conversion to resistance
  avg = (1023/avg) - 1;
  avg = (RSERIES / avg);
  Serial.print("thermosistor resistance: "); Serial.println(avg); //-> R_avg = avg thermosistor resistance 
  delay(1000);

  // temp conversion --> (1/T) = (1/T0) + (1/BCOEFF)*ln(R/R0)

  tempConv = (avg / R0);
  tempConv = log(tempConv);
  tempConv /= BCOEFF;
  tempConv += (1.0 / T0);
  tempConv = 1/tempConv;
  temp = tempConv - 273/15; // temp in C

  Serial.print("temp (C)"); Serial.println(temp)
  delay (1000);
}
