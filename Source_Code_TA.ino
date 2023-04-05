#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


int sensorTA12 = A0;
int sensorValue;
int greenLED = 7;
int sensorPin = A1;
int A = A5;
const int relay = 9;

float nVPP;                 // Voltage measured across resistor
float nCurrThruResistorPP;  // Peak Current Measured Through Resistor
float nCurrThruResistorRMS; // RMS current through Resistor
float nCurrentThruWire;     // Actual RMS current in Wire
float Watt;
float Price; 

void setup() {
  Serial.begin(9600);
  pinMode(greenLED, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(sensorTA12, INPUT);
  lcd.init();
  lcd.backlight();
}
 
void loop() {
  sensorValue = analogRead(sensorPin);       // read analog input pin A5
  Serial.print("AirQuality Value: ");
  Serial.println(sensorValue, DEC);               // prints the value read
 
  if (sensorValue >= 113) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(relay, HIGH);
    Serial.println("Alert!!!");
    lcd.setCursor(0,0);
    lcd.print("Alert!!!");
    lcd.setCursor(0,1);
    lcd.print(sensorValue);
    
    nVPP = getVPP();
    nCurrThruResistorPP = (nVPP/310.0) * 1000.0; 
    nCurrThruResistorRMS = nCurrThruResistorPP * 0.707; 
    nCurrentThruWire = nCurrThruResistorRMS * 1000;
    Watt = (nCurrentThruWire/1000) * 220;
    Price = (Watt/1000) * 1352;
    
    Serial.print("Watt : ");
    Serial.print(Watt,3);
    Serial.println(" w");

    Serial.print("Price : Rp. ");
    Serial.print(Price,3);
    Serial.println(" /h");
    
    Serial.println();
    delay(6000); 
  }

  else {
    digitalWrite(greenLED, LOW);
    digitalWrite(relay, LOW);
    digitalWrite(A3, LOW);
    Serial.println("Normal");
    lcd.setCursor(0,0);
    lcd.print("Normal");
    lcd.setCursor(0,1);
    lcd.print(sensorValue);
    delay(1000);
  }
}

float getVPP()
{
  float result;
  int readValue;                                 //value read from the sensor
  int maxValue = 0;                              // store max value here
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000)            //sample for 1 Sec
   {
       readValue = analogRead(sensorTA12);
                                                    // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           maxValue = readValue;
       }
   }
                                                  // Convert the digital data to a voltage
   result = maxValue * (5.0/1023.0);
   
   return result;
 }
