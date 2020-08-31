#include "max6675.h"

bool unit_F = false; //true = °F - false = °C

//Select Pins
int thermoDO = 7;
int thermoCS = 8;
int thermoCLK = 9;

MAX6675 thermocouple(thermoDO, thermoCS, thermoCLK);

double tempA;

double readThermoCouple() {
    double tempT;
    
    if (unit_F) {
        tempT = thermocouple.readFarenheit(); 
    }
    else {
        tempT = thermocouple.readCelsius();
    }      
    if (isnan(tempT)) {
        return 0;
    }
    else {
        return tempT;
    }
}

//Parsing Serial Commands
void handleSerialCommand() {   
    if (Serial.available() > 0) {
        String msg = Serial.readStringUntil('\n');
        if (msg.indexOf("CHAN;") == 0) { //Ignore this Setting
            Serial.print("#OK");
        }
        else if (msg.indexOf("UNITS;") == 0) {
            if (msg.substring(6,7) =="F"){ //Change to Farenheit
                unit_F = true;
                Serial.println("#OK Farenheit");
            }
            else if (msg.substring(6,7) =="C") { //Change to Celsius
                unit_F = false;
                Serial.println("#OK Celsius");
            }
        }
        else if (msg.indexOf("READ") == 0) { //Send Temps
           Command_READ();
       }
   }
}

//Send Data
void Command_READ() {
    tempA = readThermoCouple();
    Serial.print("0.00,");
    Serial.print(tempA);
    Serial.println(",0.00,0.00,0.00");
}


void setup() {
 Serial.begin(115200);
}


void loop() {
    handleSerialCommand();
}
