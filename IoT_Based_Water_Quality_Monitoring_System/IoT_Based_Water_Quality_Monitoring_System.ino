#define BLYNK_TEMPLATE_ID "TMPL3SAMivBEx"
#define BLYNK_TEMPLATE_NAME "IoT Water"
#define BLYNK_AUTH_TOKEN "_dljcVHPDKRigu5dbRh6mSmRcByLHLfD"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "Bhalu";
char pass[] = "idk010101";

#define TDS_PIN A0

BlynkTimer timer;
float tdsValue = 0;
bool tdsEnabled = true;  
bool alertSent = false; 


BLYNK_WRITE(V1) {
  int value = param.asInt();
  tdsEnabled = (value == 1);
  if (!tdsEnabled) {
    alertSent = false;
    Blynk.virtualWrite(V0, 0); 
  }
}

void readTDS() {
  if (tdsEnabled) {
    int analogValue = analogRead(TDS_PIN);
    float voltage = analogValue * (3.3 / 1023.0);
    tdsValue = (133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage) * 0.5;

    Serial.print("TDS Value: ");
    Serial.println(tdsValue);

    Blynk.virtualWrite(V0, tdsValue);

  
    if (tdsValue > 600 && !alertSent) {
      Blynk.logEvent("tds_alert", "⚠ Alert: TDS is above 600!");
      alertSent = true;
    }
 
    else if (tdsValue <= 600) {
      alertSent = false;
    }
  } else {
    Serial.println("TDS Reading Disabled");
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("ESP8266 Started");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, readTDS);  
}

void loop() {
  Blynk.run();
  timer.run();
}




