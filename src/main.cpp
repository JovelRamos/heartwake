#include <Arduino.h>        // Core Arduino functions
#include <Wire.h>           // I2C communication protocol
#include "config.h"         // Your custom configuration file

// Sensor-specific libraries
#include <MAX30105.h>       // Heart rate sensor
#include <Adafruit_MPU6050.h>  // Motion sensor
#include <Adafruit_Sensor.h>   // Base sensor library
#include <Adafruit_SSD1306.h>  // OLED display
#include <RTClib.h>         // Real-time clock
#include <ArduinoJson.h>    // JSON data formatting

// Global objects
MAX30105 heartRateSensor;
Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET_PIN);
RTC_DS3231 rtc;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("=== ESP32 Heart Rate Monitor Starting ===");
    
    // Initialize all hardware components
    initializeHardware();
    
    // Set initial state based on current time
    // if (isValidWakeTime()) {
    //     currentState = STATE_LIGHT_SLEEP;
    //     Serial.println("System ready - Light sleep mode");
    // } else {
    //     currentState = STATE_DEEP_SLEEP;
    //     Serial.println("System ready - Deep sleep mode");
    // }
}

void initializeHardware() {
    // Initialize I2C
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(I2C_FREQ);
    
    // Initialize built-in LED
    pinMode(LED_BUILTIN_PIN, OUTPUT);
    digitalWrite(LED_BUILTIN_PIN, LOW);
    
    // Initialize interrupt pins
    pinMode(MAX30102_INT_PIN, INPUT_PULLUP);
    pinMode(MPU6050_INT_PIN, INPUT_PULLUP);
    
    // Initialize MAX30102 Heart Rate Sensor
    if (!heartRateSensor.begin()) {
        Serial.println("ERROR: MAX30102 not found");
        while(1); // Halt execution
    }
    
    // Configure MAX30102 for heart rate detection
    heartRateSensor.setup(); // Configure sensor with default settings
    heartRateSensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to indicate sensor is running
    heartRateSensor.setPulseAmplitudeGreen(0);   // Turn off Green LED
    
    Serial.println("✓ MAX30102 Heart Rate Sensor initialized");
    
    // Initialize MPU6050
    if (!mpu.begin()) {
        Serial.println("ERROR: MPU6050 not found");
        while(1);
    }
    
    // Configure MPU6050 for motion detection
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    
    Serial.println("✓ MPU6050 Motion Sensor initialized");
    
    // Initialize OLED Display
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
        Serial.println("ERROR: SSD1306 OLED allocation failed");
        while(1);
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("HR Monitor Ready");
    display.display();
    
    Serial.println("✓ OLED Display initialized");
    
    // Initialize RTC
    if (!rtc.begin()) {
        Serial.println("ERROR: RTC not found");
        while(1);
    }
    
    if (rtc.lostPower()) {
        Serial.println("RTC lost power, setting time to compile time");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    
    Serial.println("✓ RTC initialized");
    Serial.println("=== Hardware initialization complete ===");
}

void loop() {
  // put your main code here, to run repeatedly:
}
