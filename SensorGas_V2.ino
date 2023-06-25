//Incude the macros
//Internet config
#define AUTH "***"
#define SSID_ "***"
#define PASS "***"

//Pin numbers
#define MQ2 32
#define MQ5 33
#define MQ7 34
#define MQ135 35
#define BUZZ1 2
#define BUZZ2 4
#define LED2 22
#define LED5 23
#define LED7 18
#define LED135 19
#define LEDON 13

//Include Libraries to connect to Wifi and Blynk server
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial

//Wifi login variables
char auth[] = AUTH;
char ssid[] = SSID_;
char pass[] = PASS;

//Define Variables used in loop
float mq2_value;
float mq5_value;
float mq7_value;
float mq135_value;
int led_status = 0;
int alert_button = 0;


void setup() {

  //For Debugging
  Serial.begin(9600);

  Serial.println("Nimbus - Sensor V1");

  //Set input pins
  pinMode (MQ2, INPUT);
  pinMode (MQ5, INPUT);
  pinMode (MQ7, INPUT);
  pinMode (MQ135, INPUT);

  //Set Output pins
  pinMode (BUZZ1, OUTPUT);
  pinMode (BUZZ2, OUTPUT);
  pinMode (LEDON, OUTPUT);
  pinMode (LED2, OUTPUT);
  pinMode (LED5, OUTPUT);
  pinMode (LED7, OUTPUT);
  pinMode (LED135, OUTPUT);

  //Blynk Setup
  Blynk.begin(auth, ssid, pass);

  Serial.println("Initializing Sensors");
  delay (20000);
  Serial.println("Sensors ready!");
}

void loop () {

  if (led_status) {

    digitalWrite(LEDON, HIGH);

    mq2_value = analogRead(MQ2);
    mq5_value = analogRead(MQ5);
    mq7_value = analogRead(MQ7);
    mq135_value = analogRead(MQ135);

    Serial.println(mq2_value);
    Serial.println(mq5_value);
    Serial.println(mq7_value);
    Serial.println(mq135_value);

    Blynk.virtualWrite(V2 , mq2_value);
    Blynk.virtualWrite(V3 , mq5_value);
    Blynk.virtualWrite(V4 , mq7_value);
    Blynk.virtualWrite(V5 , mq135_value);

  }
  else {

    digitalWrite(LEDON, LOW);
    Serial.println ("Device is off");

  }


   alerts (led_status, alert_button, mq2_value, mq5_value, mq7_value, mq135_value);
   leds_on(led_status, mq2_value, mq5_value, mq7_value, mq135_value);
 
  Blynk.run();

  delay (2000);

}


//Assign a virtual pin value to a variable
BLYNK_WRITE(V0) {
  led_status = param.asInt();
}

BLYNK_WRITE(V1) {
  alert_button = param.asInt();
}

//Compare the reading data with the establish references
int check_references(float sensor_value, int reference_value) {

  if (sensor_value > reference_value) return 1;
  else return 0;
}

//Turn the buzzer piezzo ON 
void buzzer (int buzz_type) {

  if (buzz_type > 0 && buzz_type < 5) {

  digitalWrite (BUZZ1, HIGH);
  digitalWrite (BUZZ2, HIGH);

  delay(50);

  digitalWrite (BUZZ1, LOW);
  digitalWrite (BUZZ2, LOW);
 
  delay(200);
  }

  if (buzz_type == 5) {

  digitalWrite (BUZZ1, HIGH);
  digitalWrite (BUZZ2, HIGH);
  
  delay(50);

  digitalWrite (BUZZ1, LOW);
  digitalWrite (BUZZ2, LOW);
 
  delay(150);

  }

  if (buzz_type == 0) {

  digitalWrite (BUZZ1, HIGH);
  digitalWrite (BUZZ2, HIGH);

  delay(50);

  digitalWrite (BUZZ1, LOW);
  digitalWrite (BUZZ2, LOW);
 
  delay(50);

  }

}

//Emits the alerts
void alerts(int led_status, int alert_button, float mq2_value, float mq5_value, float mq7_value, float mq135_value) {

  int alert2 = 0, alert5 = 0, alert7 = 0, alert135 = 0, all_alerts = 0;

  if (led_status){
  
  alert2 = check_references (mq2_value, 2000);
  alert5 = check_references (mq5_value, 2000);
  alert7 = check_references(mq7_value, 2000);
  alert135 = check_references(mq135_value, 2000);

  all_alerts = alert2 + alert5 + alert7 + alert135;
  
  if (alert_button) {
  buzzer(0);

  }

  if (alert2 && all_alerts == 1 && alert_button == 0) buzzer(1);
  if (alert5 && all_alerts == 1 && alert_button == 0) buzzer(2);
  if (alert7 && all_alerts == 1 && alert_button == 0) buzzer(3);
  if (alert135 && all_alerts == 1 && alert_button == 0) buzzer(4);
  if (all_alerts > 1 && alert_button == 0) buzzer(5);
}
}
//Turn the Sensor Leds ON/OFF
void leds_on (int led_stauts, float mq2_value, float mq5_value, float mq7_value, float mq135_value){

   int led2 = 0, led5 = 0, led7 = 0, led135 = 0;

  if(led_status){

  led2 = check_references (mq2_value, 2000);
  led5 = check_references (mq5_value, 2000);
  led7 = check_references(mq7_value, 2000);
  led135 = check_references(mq135_value, 2000);

  if (led2) digitalWrite(LED2, HIGH);
  else digitalWrite(LED2, LOW);
  
  if (led5) digitalWrite(LED5, HIGH);
  else digitalWrite(LED5, LOW);

  if (led7) digitalWrite(LED7, HIGH);
  else digitalWrite(LED7, LOW);

  if (led135) digitalWrite(LED135, HIGH);
  else digitalWrite(LED135, LOW);

  }
  else{
    digitalWrite(LED2, LOW);
    digitalWrite(LED5, LOW);
    digitalWrite(LED7,LOW);
    digitalWrite(LED135, LOW);
  }
}
