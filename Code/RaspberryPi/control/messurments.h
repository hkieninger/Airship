#ifndef MESSURMENTS_H
#define MESSURMENTS_H 1

struct Messurments {
  uint8_t voltagePercentage;
  float accel[3];
  float gyro[3];
  float mag[3];
  int16_t distFront;
  int16_t distBotton;
  double pres;



};

#endif /* MESSURMENTS_H */
