/*
 * ESP32 Heart Rate Monitor - Pin Configuration
 * 
 * Project Structure:
 * /src
 *   ├── main.cpp (main application logic)
 *   ├── config.h (pin definitions and constants)
 *   ├── sensors/
 *   │   ├── heartrate.cpp/.h (MAX30102 interface)
 *   │   ├── motion.cpp/.h (MPU-6050 interface)
 *   │   └── rtc.cpp/.h (DS3231 RTC interface)
 *   ├── display/
 *   │   └── oled.cpp/.h (SSD1306 OLED interface)
 *   ├── power/
 *   │   └── sleep.cpp/.h (power management)
 *   └── data/
 *       └── logger.cpp/.h (data logging and storage)
 */

#ifndef CONFIG_H
#define CONFIG_H

// ========== PIN DEFINITIONS ==========

// I2C Bus (shared by multiple sensors)
#define I2C_SDA_PIN         21
#define I2C_SCL_PIN         22
#define I2C_FREQ            400000  // 400kHz

// MAX30102 Heart Rate Sensor (I2C)
#define MAX30102_I2C_ADDR   0x57
#define MAX30102_INT_PIN    4       // Interrupt pin for data ready

// MPU-6050 IMU/Accelerometer (I2C) 
#define MPU6050_I2C_ADDR    0x68
#define MPU6050_INT_PIN     5       // Interrupt pin for motion detection

// SSD1306 OLED Display (I2C)
#define OLED_I2C_ADDR       0x3C
#define OLED_WIDTH          128
#define OLED_HEIGHT         64
#define OLED_RESET_PIN      -1      // Reset pin (-1 if sharing Arduino reset pin)

// DS3231 Real-Time Clock (I2C)
#define RTC_I2C_ADDR        0x68    // Note: Same as MPU6050, but different I2C addresses

// Status LEDs
#define LED_BUILTIN_PIN     2       // Built-in LED
#define STATUS_LED_PIN      13      // External status LED (optional)

// SD Card (SPI) - Optional for data logging
#define SD_CS_PIN           5
#define SD_MOSI_PIN         23
#define SD_MISO_PIN         19
#define SD_SCK_PIN          18

// ========== SYSTEM CONSTANTS ==========

// Wake-up detection parameters
#define MOTION_THRESHOLD        2.0     // g-force threshold for wake detection
#define MOTION_DURATION_MS      3000    // Sustained motion duration to confirm wake-up
#define SLEEP_MOTION_TIMEOUT    300000  // 5 minutes of no motion = sleep

// Heart rate measurement parameters
#define HR_MEASUREMENT_WINDOW   180000  // 3 minutes measurement window after wake-up
#define HR_SAMPLE_RATE          100     // Hz
#define HR_AVERAGE_SAMPLES      30      // Number of beats to average

// Time constraints
#define VALID_WAKE_HOUR_START   5       // 5 AM
#define VALID_WAKE_HOUR_END     10      // 10 AM

// Power management
#define DEEP_SLEEP_DURATION     300000000  // 5 minutes in microseconds
#define LIGHT_SLEEP_DURATION    60000000   // 1 minute in microseconds

// Data logging
#define MAX_DAILY_RECORDS       10
#define LOG_FILENAME           "/hr_data.json"

// ========== SYSTEM STATES ==========
enum SystemState {
    STATE_DEEP_SLEEP,
    STATE_LIGHT_SLEEP, 
    STATE_MOTION_DETECTED,
    STATE_MEASURING_HR,
    STATE_DISPLAYING_RESULT,
    STATE_LOGGING_DATA
};

enum WakeDetectionState {
    WAKE_ASLEEP,
    WAKE_MOTION_DETECTED,
    WAKE_CONFIRMED_AWAKE
};

#endif // CONFIG_H