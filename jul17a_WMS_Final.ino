//Project on Weather Monitering System

  #define BLYNK_TEMPLATE_ID "TMPLfPcWcEXJ"
  #define BLYNK_DEVICE_NAME "WMSP1"
  #define BLYNK_AUTH_TOKEN "xrblWk-gIOLc-3x5p3iTiDY3f8Sjcn3B"
  
  #define BLYNK_PRINT Serial 
  
  #include<DHT.h>
  #define Dpin 23
  #define Ana 34
  #define An 33
  #define Dtype DHT11
  
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <BlynkSimpleEsp32.h>

  char auth[] = BLYNK_AUTH_TOKEN;
  char ssid[] = "dlink";
  char pass[] = "";
  
  DHT dht(Dpin, Dtype);
  BlynkTimer timer;
  
  int ADC_data = 0, An_data = 0;
  float Vout = 0, Vrout = 0;
  float t, h,f;
  
void DHT11sensor()
{
    h = dht.readHumidity();
    t = dht.readTemperature();
    f = dht.readTemperature(true);

    if(isnan(t)||isnan(h)||isnan(f))
    {
      //isnan->is not a num
      Serial.println("Check the Connection of DHT11.");
      return;
    }
    Serial.print(F("Humidity = "));
    Serial.print(h);
    Serial.println("% ");
  
    Serial.print(F("Temperature = "));;
    Serial.print(t);
    Serial.println(F("°c "));
    Serial.print(F("Temperature = "));
    Serial.print(f);
    Serial.println(F("°F "));
    Serial.println("");

    Blynk.virtualWrite(V0, h);
    Blynk.virtualWrite(V1, t);
}
void LDRsensor()
{
    ADC_data = analogRead(Ana);
    Serial.print("LDR Analog Data = ");
    Serial.println(ADC_data);
    Vout = ((3.3*ADC_data)/4096);
    Serial.print("Voltage out = ");
    Serial.println(Vout);
    delay(100);

      if (ADC_data>= 3000)
      { 
         Serial.println("NIGHT");
         WidgetLED LED(V3);
         LED.on();
      }
      else if (ADC_data < 3000)
      {
        Serial.println("DAY");
        WidgetLED LED(V3);
        LED.off();
          if(ADC_data > 110)
          {
            Serial.println("CLOUDY");
          }
          else if(ADC_data < 50)
          {
            Serial.println("SUNNY");
          }
      }   
}
  
void RainSensor()
{
    An_data = analogRead(An);
    Serial.print("Rain Sensor Analog Data = ");
    Serial.println(An_data);
 
    Vrout = ((3.3*An_data)/4096);
    Serial.print("Voltage out = ");
    Serial.println(Vrout);

    if (An_data >= 2500)
    { 
      Serial.println("NOT RAINING");
    }
    else if (An_data > 1000 && An_data < 2000)
    {
      Serial.println("MODERATE RAINING");
    }
    else if (An_data < 1000)
    {
      Serial.println("Heavy RAINING");
    }

     An_data = map(An_data, 4095, 0, 0, 100);
     Blynk.virtualWrite(V2, An_data);
     
}
  
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(auth,ssid, pass);
  timer.setInterval(1000L, DHT11sensor);
  timer.setInterval(1000L, LDRsensor);
  timer.setInterval(1000L, RainSensor);

  delay(3000);
  
}
  
void loop() {
   Blynk.run();
   timer.run();
    // put your main code here, to run repeatedly:
    
}
