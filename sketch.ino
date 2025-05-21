#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//const char* WIFI_SSID = "NoInternet";
//const char* WIFI_PASSWORD = "NoNetwork546";
#define DHTPIN 2            // Digital pin connected to the DHT22 sensor
#define DHTTYPE DHT22      // DHT22 sensor type
#define LDR_PIN A0          // Analog pin connected to the LDR sensor
#define SOIL_MOISTURE_PIN A1// Analog pin connected to the soil moisture sensor
#define coolingSwitch 11       // Digital pin connected to the relay controlling the fan
#define LightingSwitch 9       // Digital pin connected to the relay controlling the lighting system
#define wateringSwitch 7       // Digital pin connected to the relay controlling the watering system

#define LCD_ADDRESS 0x27    // I2C address of the LCD
#define LCD_COLUMNS 20      // Number of columns of the LCD
#define LCD_ROWS 4          // Number of rows of the LCD

DHT dht(DHTPIN, DHTTYPE);   // Initialize the DHT22 sensor
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS); // Initialize the LCD

int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

const float GAMMA = 0.7;
const float RL10 = 50;
//const int WIFI_CONNECT_TIMEOUT = 10000; // Timeout for WiFi connection in milliseconds

// WiFi credentials
//const char* WIFI_SSID = "NoInternet";
//const char* WIFI_PASSWORD = "NoNetwork546";

// ThinkSpeak channel information
//const char* THINKSPEAK_API_KEY = "Z8Q8Y4BPAA5VEJRM";
//const char* THINKSPEAK_CHANNEL_ID = "2273478";

// Thresholds for the different parameters
//const int TEMPERATURE_MIN = 22;
const int TEMPERATURE_MAX = 24;
const int LDR_THRESHOLD = 500;
const int SOIL_MOISTURE_THRESHOLD = 500;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  dht.begin();
  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Initialize the relays
  pinMode(coolingSwitch, OUTPUT);
  pinMode(LightingSwitch, OUTPUT);
  pinMode (wateringSwitch, OUTPUT);
  lcd.print("Welcome");
  delay(1000);
  lcd.clear();
  // Connect to WiFi
  //WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //Serial.print("Connecting to WiFi");
  // unsigned long wifi_connect_start_time = millis();
  //while (WiFi.status() != WL_CONNECTED && millis() - wifi_connect_start_time < WIFI_CONNECT_TIMEOUT) {
  //  delay(500);
  //  Serial.print(".");
  //}
  //Serial.println();
  //if (WiFi.status() == WL_CONNECTED) {
  // lcd.println("WiFi connected");
  //} else {
  // lcd.println("WiFi connection failed");
  // }
}

void loop() {
  // Read temperature
  float temperature = dht.readTemperature();
  hum = dht.readHumidity();
  lcd.clear();
  //delay(100);
  lcd.print("TEMP = ");
  lcd.print(temperature);
  lcd.print("*C");
  hum = dht.readHumidity();
  lcd.setCursor(0, 1);
  lcd.print("HUMIDITY = ");
  lcd.print(hum);
  lcd.print("%");



  // Read sunlight
  //int ldr_value = analogRead(LDR_PIN);
  //bool is_dark = ldr_value < LDR_THRESHOLD;
  int analogValue = analogRead(A0);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  lcd.setCursor(0, 2);
  lcd.print("LIGHT= ");
  lcd.print(lux);
  lcd.print(" LUX");




  // Read soil moisture
  int soil_moisture = analogRead(SOIL_MOISTURE_PIN);
  //bool is_dry = soil_moisture < SOIL_MOISTURE_THRESHOLD;
  lcd.setCursor(0, 3);
  lcd.print("SOIL MOISTURE = ");
  lcd.print(soil_moisture);
  lcd.print("%");
  delay(1000);


  // Control temperature
  if (temperature > TEMPERATURE_MAX)
  {
    digitalWrite(coolingSwitch, HIGH);  // Fan on
    lcd.clear();
    lcd.print("Temp =");
    lcd.print(temperature);
    lcd.print("*C");
    lcd.print(" FAN ON");
    //delay (1000);
  }
  else if (temperature < TEMPERATURE_MAX)
  {
    digitalWrite(coolingSwitch, LOW);  // Fan Off
    lcd.clear();
    lcd.print("TEMP=");
    lcd.print(temperature);
    lcd.print("*C");
    lcd.print(" FAN OFF");
    //delay(1000);
  }

  //Control lighting
  if (lux < 1000)
  {
    digitalWrite(LightingSwitch, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("LIGHT=");
    lcd.print(lux);
    lcd.print("LIGHT ON");
    //delay(1000);
  }
  else if (lux > 1000)
  {
    digitalWrite(LightingSwitch, LOW);
    lcd.setCursor(0, 1);
    lcd.print("LIGHT=");
    lcd.print(lux);
    lcd.print("LIGHTOF");
    //delay(1000);
  }


  //Control irrigation
  if (soil_moisture < 500)
  {
    digitalWrite(wateringSwitch, HIGH);
    lcd.setCursor(0, 2);
    lcd.print("Soil=");
    lcd.print(soil_moisture);
    lcd.print("%");
    lcd.print(" Watering");
    delay(1000);
  }
  else if (soil_moisture > 500)
  {
    digitalWrite(wateringSwitch, LOW);
    lcd.setCursor(0, 2);
    lcd.print("SOIL=");
    lcd.print(soil_moisture);
    lcd.print("%");
    lcd.print(" NotWaterin");
    delay(1000);
  }
  delay (500);
}
