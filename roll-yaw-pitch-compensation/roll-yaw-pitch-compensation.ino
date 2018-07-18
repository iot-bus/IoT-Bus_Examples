//This finds a magnetic north bearing, correcting for board tilt and roll as measured by the accelerometer
//This doesn't account for dynamic acceleration - ie accelerations other than gravity will throw off the calculation
//Calculations based on AN4248
double[] lastAngles = { 0, 0, 0 };
void calculateCompassBearing(SpatialSpatialDataEventArgs e) {
  double[] gravity = {
    e.Acceleration[0],
    e.Acceleration[1],
    e.Acceleration[2]};

  double[] magField = {
    e.MagneticField[0],
    e.MagneticField[1],
    e.MagneticField[2]};

  //Roll Angle - about axis 0
  //  tan(roll angle) = gy/gz
  //  Use Atan2 so we have an output os (-180 - 180) degrees
  double rollAngle = Math.Atan2(gravity[1], gravity[2]);

  //Pitch Angle - about axis 1
  //  tan(pitch angle) = -gx / (gy * sin(roll angle) * gz * cos(roll angle))
  //  Pitch angle range is (-90 - 90) degrees
  double pitchAngle = Math.Atan(-gravity[0] / (gravity[1] * Math.Sin(rollAngle) + gravity[2] * Math.Cos(rollAngle)));

  //Yaw Angle - about axis 2
  //  tan(yaw angle) = (mz * sin(roll) – my * cos(roll)) /
  //                   (mx * cos(pitch) + my * sin(pitch) * sin(roll) + mz * sin(pitch) * cos(roll))
  //  Use Atan2 to get our range in (-180 - 180)
  //
  //  Yaw angle == 0 degrees when axis 0 is pointing at magnetic north
  double yawAngle = Math.Atan2(magField[2] * Math.Sin(rollAngle) - magField[1] * Math.Cos(rollAngle),
    magField[0] * Math.Cos(pitchAngle) + magField[1] * Math.Sin(pitchAngle) * Math.Sin(rollAngle) + magField[2] * Math.Sin(pitchAngle) * Math.Cos(rollAngle));

  double[] angles = { rollAngle, pitchAngle, yawAngle };

  //we low-pass filter the angle data so that it looks nicer on-screen
  try {
    //make sure the filter buffer doesn't have values passing the -180<->180 mark
    //Only for Roll and Yaw - Pitch will never have a sudden switch like that
    for (int i = 0; i < 3; i += 2) {
      if (Math.Abs(angles[i] - lastAngles[i]) > 3)
        foreach (double[] value in compassBearingFilter)
          if (angles[i] > lastAngles[i])
            value[i] += 360 * Math.PI / 180.0;
          else
            value[i] -= 360 * Math.PI / 180.0;
      }

    lastAngles = (double[])angles.Clone();

    compassBearingFilter.Add((double[])angles.Clone());
    if (compassBearingFilter.Count > compassBearingFilterSize)
      compassBearingFilter.RemoveAt(0);

    yawAngle = pitchAngle = rollAngle = 0;
    foreach (double[] stuff in compassBearingFilter) {
      rollAngle += stuff[0];
      pitchAngle += stuff[1];
      yawAngle += stuff[2];
    }
    yawAngle /= compassBearingFilter.Count;
    pitchAngle /= compassBearingFilter.Count;
    rollAngle /= compassBearingFilter.Count;

    //Convert radians to degrees for display
    compassBearing = yawAngle * (180.0 / Math.PI);

    bearingTxt.Text = compassBearing.ToString("F1") + "°";
    pitchAngleTxt.Text = (pitchAngle * (180.0 / Math.PI)).ToString("F1") + "°";
    rollAngleTxt.Text = (rollAngle * (180.0 / Math.PI)).ToString("F1") + "°";
  } catch { }
}
