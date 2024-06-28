/**
 * set_get_rpm example of ZLAC8015D library
 * This example will set RPM to each wheel,
 * then read the feedback RPM immediately.
 * 
 * by Rasheed Kittinanthapanya
 * 
 */

#include <ModbusMaster.h>
#include <ZLAC8015D.h>

////////////////////////////////
/// ZLAC8015D & ModbusMaster ///
////////////////////////////////
#define MAX485_DE 32
#define MAX485_RE_NEG  33
#define MODBUS_RX_PIN 12
#define MODBUS_TX_PIN 13

uint8_t res;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

ModbusMaster node;
ZLAC8015D driver;

/// Test ///
bool drive_fast = true;
unsigned long last_drive_fast_stamp;
int16_t rpmFB[2];

void setup() {

  // Just to print out
  Serial.begin(115200);

  ////////////////////////////////
  /// ZLAC8015D & ModbusMaster ///
  ////////////////////////////////

  // you need to setup one serial port
  Serial1.begin(115200, SERIAL_8E1, MODBUS_RX_PIN, MODBUS_TX_PIN);

  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  delay(1000);

  // Init the ModbusMaster with selected serial port
  node.begin(1, Serial1);

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  // Pass ModbusMaster object to the library lastly.
  driver.set_modbus(&node);

  /// Disable motor to make sure it starts from 0 speed ///
  bool done = false;
  while (! done) {
    res = driver.disable_motor();
    Serial.print("disable motor ");
    Serial.println(res);
    if (res == 0) {
      done = true;
    }
  }

  /// Set velocity control ///
  res = driver.set_mode(3);
  Serial.print("set mode ");
  Serial.println(res);

  /// Enable motor, torque is applied ///
  res = driver.enable_motor();
  Serial.print("enable motor ");
  Serial.println(res);

  /// Set accel/decel ///
  res = driver.set_accel_time(200, 200);
  Serial.print("set accel ");
  Serial.println(res);

  res = driver.set_decel_time(200, 200);
  Serial.print("set decel ");
  Serial.println(res);

  last_drive_fast_stamp = millis();
  
}

void loop() {

  /// Set RPM by switching between two speed set (100,100) and (30,-20)
  /// each speed will run at 5 seconds, then switch to another over and over
  if (drive_fast){
    res = driver.set_rpm(100, 100);
  } else {
    res = driver.set_rpm(30, -20);
  }

  if ((millis() - last_drive_fast_stamp) > 5000){
    drive_fast = ! drive_fast;
    last_drive_fast_stamp = millis();
  }


  /// Get RPM ///
  res = driver.get_rpm(rpmFB);

  Serial.print("res: ");
  Serial.print(res);
  Serial.print(" rpmL_fb: ");
  Serial.print(rpmFB[0]);
  Serial.print(" rpmR_fb: ");
  Serial.println(rpmFB[1]);
  

}
