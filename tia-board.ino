#include <ADS1119.h>
#include <SPI.h>
#include <Arduino.h>


ADS1119 ads1 = ADS1119();

struct ADS1119Configuration configuration;

// Setup communication
int CS_pot_gain = 4;
int CS_pot_offset = 5;

// 
int lightLevel = 0;
int lightMeasurements = 4;

int startGain = 190; 
int levelOffset = 3; 

void setup() {  
  Serial.begin(9600);
  pinMode (CS_pot_gain, OUTPUT);
  pinMode (CS_pot_offset, OUTPUT);
  SPI.begin();

  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV16); //1MHz clock 

  digitalPotWrite(levelOffset, "offset");
  digitalPotWrite(startGain, "gain");
  delay(100);
 
  configuration.mux = ADS1119MuxConfiguration::positiveAIN0negativeAIN1;
  configuration.gain = ADS1119Configuration::Gain::one;
  configuration.dataRate  = ADS1119Configuration::DataRate::sps90;
  configuration.conversionMode = ADS1119Configuration::ConversionMode::continuous;
  configuration.voltageReference = ADS1119Configuration::VoltageReferenceSource::external;
  configuration.externalReferenceVoltage = 5.0;

  ads1.begin();
  ads1.reset();

}


void loop() {


  lightLevel = 0;
  
  for(int i = 0; i<lightMeasurements; i++){
    lightLevel = lightLevel + (1000*ads1.readVoltage(configuration));
  }
  lightLevel = lightLevel/lightMeasurements;

  Serial.println(lightLevel);
  
  delay(30);  
  
}


int digitalPotWrite(int value, String mode)
{
  if (mode == "gain"){
    digitalWrite(CS_pot_gain, LOW);
    SPI.transfer(B00010001);
    SPI.transfer(value);
    digitalWrite(CS_pot_gain , HIGH);

  } else if (mode == "offset"){
    digitalWrite(CS_pot_offset, LOW);
    SPI.transfer(B00010001);
    SPI.transfer(value);
    digitalWrite(CS_pot_offset , HIGH);
  }
}
