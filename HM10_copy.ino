
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

const unsigned long BAUD_RATE = 9600;
//unsigned long lastCommunicationTime = 0;
//unsigned long connectionTimeout = 200;  // 5 seconds timeout

//--------------------------------------------------------------------------------------------------
// PINS BLUETOOTH
const int BLUETOOTH_TX = 8;
const int BLUETOOTH_RX = 7;
//--------------------------------------------------------------------------------------------------
//PINS MOTEUR SÉLECTIONNEUR
int pinA = 9;
int motorpin1 = 2;
int motorpin2 = 3;
//--------------------------------------------------------------------------------------------------
//PINS MOTEURS DES ROUES
int pinMA = 6 ;
int motorpinMA1 = 5 ;
int motorpinMA2 = 4 ;

int pinMB = 10 ;
int motorpinMB1 = 11;
int motorpinMB2 = 12;
//--------------------------------------------------------------------------------------------------
SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue constructor.

void setup() {
  Serial.begin(9600);
  
  pinMode(pinA, OUTPUT);
  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT);

  pinMode(pinMA,OUTPUT);
  pinMode(motorpinMA1,OUTPUT);
  pinMode(motorpinMA2,OUTPUT);

  pinMode(pinMB,OUTPUT);
  pinMode(motorpinMB1,OUTPUT);
  pinMode(motorpinMB2,OUTPUT);

  bluetooth.begin(BAUD_RATE);
  delay(10);

  bluetooth.write("AT+NAMEArduinoBlueEQ34"); // RENOMMER NOTRE MODULE BLUETOOTH

//  lastCommunicationTime = millis();  // Initialiser le temps de la dernière communication

}

/*
bool isConnected() {
  return (millis() - lastCommunicationTime) > connectionTimeout;
}

*/
void loop() {
/*
  if (bluetooth.available()) {
    lastCommunicationTime = millis();
    // Vérifie la connection bluetooth
  }
  if (phone.isConnected()) {
    // Si la connection est perdue, éteint tous les moteurs
    digitalWrite(motorpin1, LOW); 
    digitalWrite(motorpin2, LOW);
    digitalWrite(motorpinMA1, LOW); 
    digitalWrite(motorpinMA2, LOW);
    digitalWrite(motorpinMB1, LOW); 
    digitalWrite(motorpinMB2, LOW);
    return; 
  }
*/
//--------------------------------------------------------------------------------------------------
// ALLER CHERCHER LES VALEURS RECU DU MODULE 

    int buttonValue = phone.getButton();
    int throttleValue = phone.getThrottle();
    int steeringValue = phone.getSteering();
    int sliderValue = phone.getSliderVal();

    //Serial.print("Button value: ");
    //Serial.println(buttonValue);
    Serial.print("Throttle value: ");
    Serial.println(throttleValue);
    Serial.print("Steering value: ");
    Serial.println(steeringValue);
    //Serial.print("Slider value: ");
    //Serial.println(sliderValue);
    //delay(250);
//--------------------------------------------------------------------------------------------------
    if (buttonValue == 10) { // Boutton CW pour du contrôle fin en sens horaire
          Serial.println("Button 10 pressed");
          
          analogWrite(pinA,150);
          digitalWrite(motorpin1, LOW);
          digitalWrite(motorpin2,HIGH);
          delay(15);
//--------------------------------------------------------------------------------------------------  
    } else if (buttonValue == 20) { // Button CCW pour du contrôle fin en sens anti-horaire
          Serial.println("Button 20 pressed");
          
          analogWrite(pinA,150);
          digitalWrite(motorpin1, HIGH); //CC
          digitalWrite(motorpin2, LOW);
          delay(15);
//--------------------------------------------------------------------------------------------------  
    } else if (buttonValue == 30) { // Button pour sélectionner un Haricot Rouge
          Serial.println("Button 30 pressed");
   
          analogWrite(pinA,75); //ALLER
          digitalWrite(motorpin1, LOW); 
          digitalWrite(motorpin2, HIGH);
          delay(330);
          
          analogWrite(pinA,70); //RETOUR
          digitalWrite(motorpin1, HIGH); 
          digitalWrite(motorpin2, LOW);
          delay(325);

          digitalWrite(motorpin1, LOW); //CC
          digitalWrite(motorpin2, HIGH);
          delay(10);
//--------------------------------------------------------------------------------------------------
    } else if (buttonValue == 40) { //Button pour sélectionner un Haricot Blanc
          Serial.println("Button 40 pressed");

          analogWrite(pinA,75); //ALLER
          digitalWrite(motorpin1, HIGH); 
          digitalWrite(motorpin2, LOW);
          delay(360);
          
          analogWrite(pinA,70); //RETOUR
          digitalWrite(motorpin1, LOW); 
          digitalWrite(motorpin2, HIGH);
          delay(240);

          digitalWrite(motorpin1, HIGH); //CC
          digitalWrite(motorpin2, LOW);
          delay(10);
          
    }
//--------------------------------------------------------------------------------------------------
//AVANCER
  if (throttleValue > 52) { 
    float multiplier = 120/47;
    float pwmValueCW = multiplier*throttleValue - 22.76596 ;
        
    analogWrite(pinMA,0.7*pwmValueCW);
    digitalWrite(motorpinMA1, HIGH);
    digitalWrite(motorpinMA2, LOW);

    analogWrite(pinMB,pwmValueCW);
    digitalWrite(motorpinMB1, HIGH);
    digitalWrite(motorpinMB2, LOW);

    if (steeringValue > 60 && steeringValue < 80) { //TOURNER À DROITE
      float multiplier2 = 0.45;
      analogWrite(pinMB,multiplier2 * pwmValueCW);
      digitalWrite(motorpinMB1, HIGH); 
      digitalWrite(motorpinMB2, LOW);

      analogWrite(pinMA,pwmValueCW);
      digitalWrite(motorpinMA1, HIGH); 
      digitalWrite(motorpinMA2, LOW);
 
    } else if (steeringValue >= 80) { // TOURNER À DROITE
      float multiplier2 = 1;
      analogWrite(pinMB,multiplier2*pwmValueCW);
      digitalWrite(motorpinMB1, LOW); 
      digitalWrite(motorpinMB2, HIGH);

      analogWrite(pinMA,pwmValueCW);
      digitalWrite(motorpinMA1, HIGH); 
      digitalWrite(motorpinMA2, LOW);
    }
    else if (steeringValue < 40 && steeringValue > 20 ) { // TOURNER À GAUCHE
      float multiplier2 = 0.6;
      analogWrite(pinMB,pwmValueCW);
      digitalWrite(motorpinMB1, HIGH); 
      digitalWrite(motorpinMB2, LOW);

      analogWrite(pinMA,multiplier2*pwmValueCW);
      digitalWrite(motorpinMA1, HIGH); 
      digitalWrite(motorpinMA2, LOW);
      
    } else if (steeringValue <= 20) { // TOURNER À GAUCHE
      float multiplier2 = 1;
      analogWrite(pinMB,pwmValueCW);
      digitalWrite(motorpinMB1, HIGH); 
      digitalWrite(motorpinMB2, LOW);

      analogWrite(pinMA,multiplier2 * pwmValueCW);
      digitalWrite(motorpinMA1, LOW); 
      digitalWrite(motorpinMA2, HIGH);
    }
    else {
      analogWrite(pinMA,pwmValueCW);
      digitalWrite(motorpinMA1, HIGH); 
      digitalWrite(motorpinMA2, LOW);

      analogWrite(pinMB,pwmValueCW);
      digitalWrite(motorpinMB1, HIGH); 
      digitalWrite(motorpinMB2, LOW);
    }
//--------------------------------------------------------------------------------------------------
// RECULER
  } else if (throttleValue < 30) { 
    // Motor control code when throttleValue is under 45
    // For CounterClockWise motor
    float multiplier = -105/40 ;//can be change
    int pwmValueCCW = multiplier*throttleValue + 255;

    //analogWrite(pinA,pwmValueCCW);
    //digitalWrite(motorpin1, LOW);
    //digitalWrite(motorpin2, HIGH);
    if (steeringValue > 60 && steeringValue < 90) { // TOURNER À DROITE
      float multiplier2 = (steeringValue/45) - 1;
      analogWrite(pinMA,multiplier2 * pwmValueCCW);
      digitalWrite(motorpinMB1, LOW); 
      digitalWrite(motorpinMA2, HIGH);

      analogWrite(pinMB,pwmValueCCW);
      digitalWrite(motorpinMB1, LOW); 
      digitalWrite(motorpinMB2, HIGH);

    } else if (steeringValue >= 90) { // TOURNER À DROITE
      float multiplier2 = 0;
      analogWrite(pinMB,multiplier2*pwmValueCCW);
      digitalWrite(motorpinMB1, LOW); 
      digitalWrite(motorpinMB2, HIGH);

      analogWrite(pinMA,pwmValueCCW);
      digitalWrite(motorpinMA1, LOW); 
      digitalWrite(motorpinMA2, HIGH);
    }
    else if (steeringValue < 40 && steeringValue > 10 ) { // TOURNER À GAUCHER
      float multiplier2 = steeringValue/30 - (1/3);
      analogWrite(pinMA,pwmValueCCW);
      digitalWrite(motorpinMA1, LOW); 
      digitalWrite(motorpinMA2, HIGH);

      analogWrite(pinMB,multiplier2*pwmValueCCW);
      digitalWrite(motorpinMB1, LOW); 
      digitalWrite(motorpinMB2, HIGH); 
    } else if (steeringValue <= 10) { // TOURNER À GAUCHE
      float multiplier2 = 0;
      analogWrite(pinMB,pwmValueCCW);
      digitalWrite(motorpinMB1, LOW); 
      digitalWrite(motorpinMB2, HIGH);

      analogWrite(pinMA,multiplier2*pwmValueCCW);
      digitalWrite(motorpinMA1, LOW); 
      digitalWrite(motorpinMA2, HIGH);
    }
    else {
      analogWrite(pinMA,pwmValueCCW);
      digitalWrite(motorpinMA1, LOW); 
      digitalWrite(motorpinMA2, HIGH);

      analogWrite(pinMB,pwmValueCCW);
      digitalWrite(motorpinMB1, LOW); 
      digitalWrite(motorpinMB2, HIGH);
    }
//--------------------------------------------------------------------------------------------------
  } else { // ARRETE LE ROBOT
    analogWrite(pinA,0);
    digitalWrite(motorpin1, LOW);
    digitalWrite(motorpin2, LOW);

    analogWrite(pinMA,0);
    digitalWrite(motorpinMA1, LOW);
    digitalWrite(motorpinMA2, LOW);

    analogWrite(pinMB,0);
    digitalWrite(motorpinMB1, LOW);
    digitalWrite(motorpinMB2, LOW);
  }
}
