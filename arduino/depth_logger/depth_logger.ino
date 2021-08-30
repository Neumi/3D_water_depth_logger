#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>


TinyGPSPlus gps;

SoftwareSerial ssGPS(3, A1);
//SoftwareSerial ssDepth(2, A0);

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int warnLightPin = 1;
const int chipSelect = 10;

boolean warning = false;

float waterDepth = 0.0;

boolean SDfailure = false;
String filename = "";

unsigned int gpsTime = 0;
unsigned int depthTime = 0;

void setup()
{
  //Serial.begin(9600);
  ssGPS.begin(9600);
  Serial.begin(4800);

  pinMode(warnLightPin, OUTPUT);
  digitalWrite(warnLightPin, HIGH);

  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0, 0);
  lcd.print("Depth Logger"); // print a simple message
  delay(1000);
  lcd.clear();

  // Serial.print("Initializing SD card...");
  lcd.print("init SD");
  if (!SD.begin(chipSelect)) {
    // Serial.println("Card failed, or not present");
    lcd.clear();
    lcd.print("SD INIT FAIL!");
    lcd.setCursor(0, 1);
    lcd.print("please reset");
    while (1);
  }
  delay(100);
  // Serial.println("card initialized.");
  lcd.clear();
  lcd.print("SD init DONE");
  digitalWrite(warnLightPin, LOW);
}

void loop()
{
  smartGPSDelay(1000);

  if (Serial.available()) {
    String incomingString = Serial.readStringUntil('\n');
    if (incomingString.substring(0, 6) == "$SDDPT") {
      waterDepth = getValue(incomingString, ',', 1).toFloat();
      depthTime = millis();
    }
  }


  if (gps.location.age() < 1100 && gps.location.isValid() && gps.date.isValid() && gps.time.isValid() && !SDfailure) {
    char date[32];
    char time[32];
    char filenameDate[64];
    gpsTime = millis();
    sprintf(filenameDate, "%02d%02d%02d", gps.date.day(), gps.time.hour(), gps.time.minute());
    sprintf(date, "%02d/%02d/%02d", gps.date.month(), gps.date.day(), gps.date.year());
    sprintf(time, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());

    if (filename == "") {
      filename = String(filenameDate) + ".log";
    }

    if ( abs(gpsTime - depthTime) < 2000) {
      warning = false;
      logData(date, time, waterDepth, gps.location.lat(), gps.location.lng(), gps.speed.kmph(), gps.satellites.value());
      displayData(date, time, waterDepth, gps.location.lat(), gps.location.lng(), gps.speed.kmph());

    }
    else {
      lcd.clear();
      lcd.print("ERROR 1");
      lcd.setCursor(0, 1);
      lcd.print("TIME DIFF:" + String(abs(gpsTime - depthTime)));
      warning = true;
    }



  } else {
    lcd.clear();
    lcd.print("ERROR 2");
    lcd.setCursor(0, 1);
    lcd.print("GPS age: " + gps.location.age());
    //lcd.print("SD FAIL: " + String(SDfailure));
    warning = true;
  }

  if (warning) {
    digitalWrite(warnLightPin, HIGH);
  }
  else {
    digitalWrite(warnLightPin, LOW);
  }

}

void displayData(String date, String time, float depth, double latitude, double longitude, float speed) {
  lcd.clear();
  lcd.print("Logging..." + String(depth) + "m");
  lcd.setCursor(0, 1);
  lcd.print(String(speed) + "KMh " + time);

}

void logData(String date, String time, float depth, double latitude, double longitude, float speed, uint32_t satCount) {
  String dataString = "";

  dataString += String(date);
  dataString += ",";
  dataString += String(time);
  dataString += ",";
  dataString += String(depth, 2);
  dataString += ",";
  dataString += String(latitude, 6);
  dataString += ",";
  dataString += String(longitude, 6);
  dataString += ",";
  dataString += String(speed, 2);
  dataString += ",";
  dataString += String(satCount);

  writeData(dataString);
  dataString = "";
}

void writeData(String data) {
  File dataFile = SD.open(filename, FILE_WRITE);

  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
    warning = false;
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SD WRITE FAIL!");
    lcd.setCursor(0, 1);
    lcd.print("please reset");
    warning = true;
    SDfailure = true;
  }
}

static void smartGPSDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ssGPS.available())
      gps.encode(ssGPS.read());
  } while (millis() - start < ms);
}



String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
