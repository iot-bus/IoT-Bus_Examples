/*
  MAG3110 Compass example  
*/
  
#include <SparkFun_MAG3110.h> // Magnetometer
#include <LSM6.h> // Accelerometer / Gyro
#include <SPI.h>
#include <Wire.h>
#include <TFT_eSPI.h> // TFT

#include "compass.h"

float declination = 13.449; //13 deg 25 secs in Danville, CA

MAG3110 mag = MAG3110(); //Instantiate MAG3110

LSM6 imu; //instance of LSM6

TFT_eSPI tft = TFT_eSPI();  // Bodmer TFT library

Compass compass(&tft); // instance of compass

void setup()
{
  Serial.begin(115200);  // start serial for output

  mag.initialize(); //Initialize the MAG3110
  
  if (!imu.init()) // initialize the LSM6DS3
  {
    Serial.println("Failed to detect and initialize IMU!");
  }
  else{
    imu.enableDefault();
  }

  // initialize tft
  tft.init();
  tft.setRotation(1); // 3 shows heading  
  tft.fillScreen(TFT_BLACK);

  // Turn on backlight
  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH); 

}

void loop() {

  int x, y, z;

  imu.read(); // update imu

  if(!mag.isCalibrated()) //If we're not calibrated
  {
    if(!mag.isCalibrating()) //And we're not currently calibrating
    {
      //Serial.println("Entering calibration mode");
      mag.enterCalMode(); //This sets the output data rate to the highest possible and puts the mag sensor in active mode
    }
    else
    {
      //Must call every loop while calibrating to collect calibration data
      //This will automatically exit calibration
      //You can terminate calibration early by calling mag.exitCalMode();
      mag.calibrate();
      //Serial.println("Calibrating..."); 
    }
  }
  else
  {
    //Serial.println("Calibrated!");
  }

  //mag.readMag(&x, &y, &z);
  
  //float heading = compensate(x, y, z, imu.g.x, imu.g.y, imu.g.z);
  //if (heading < 0)
    //heading += 360;

//  Serial.println("--------");
//  Serial.print("X: ");
//  Serial.print(x);
//  Serial.print(", Y: ");
//  Serial.print(y);
//  Serial.print(", Z: ");
//  Serial.println(z);

  compass.drawRose(mag.isCalibrated());  
  if (mag.calibrated){
//    Serial.println("--------");
//    Serial.print("Heading: ");
//    //double heading = mag.readHeading();
//    Serial.println(heading);
//    Serial.println("--------");
    double heading = mag.readHeadingRadians();  // not using compensation yet
    compass.drawHeading(heading); // adjust for screen orientation
  }
  delay(1);

}



/* global variables */
float Psi, The, Phi; /* yaw, pitch, roll angles in deg */
//float Vx, Vy, Vz; /* hard iron calibration coefficients */
//float Bfx, Bfy, Bfz; /* calibrated mag data in uT after tilt correction */

/* NED tilt-compensated e-Compass function */
float compensate(float Bx, float By, float Bz, float Gx, float Gy, float Gz)
{
    float sinAngle, cosAngle; /* sine and cosine */
    /* subtract off the hard iron interference computed using equation 9 */
//    Bx -= Vx;
//    By -= Vy;
//    Bz -= Vz;
    /* calculate roll angle Phi (-180deg, 180deg) and sin, cos */
    Phi = atan2(Gy, Gz) * RadToDeg; /* Equation 2 */
    sinAngle = sin(Phi * DegToRad); /* sin(Phi) */
    cosAngle = cos(Phi * DegToRad); /* cos(Phi) */
    /* de-rotate by roll angle Phi */
    float Bfy = By * cosAngle - Bz * sinAngle; /* Equation 5 y component */
    Bz = By * sinAngle + Bz * cosAngle; /* Bz=(By-Vy).sin(Phi)+(Bz-Vz).cos(Phi) */
    Gz = Gy * sinAngle + Gz * cosAngle; /* Gz=Gy.sin(Phi)+Gz.cos(Phi) */
    /* calculate pitch angle Theta (-90deg, 90deg) and sin, cos*/
    The = atan(-Gx / Gz) * RadToDeg; /* Equation 3 */
    sinAngle = sin(The * DegToRad); /* sin(Theta) */
    cosAngle = cos(The * DegToRad); /* cos(Theta) */
    /* de-rotate by pitch angle Theta */
    float Bfx = Bx * cosAngle + Bz * sinAngle; /* Equation 5 x component */
    float Bfz = -Bx * sinAngle + Bz * cosAngle; /* Equation 5 z component */
    /* calculate yaw = ecompass angle psi (-180deg, 180deg) */
    Psi = atan2(-Bfy, Bfx) * RadToDeg; /* Equation 7 */
    return Psi;
}

