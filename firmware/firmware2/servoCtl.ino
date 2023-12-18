struct Servomotor {
  Servo servomotor;
  uint8_t position;
  uint8_t inPosition;
  unsigned long time;
};

Servomotor Servomotors[3];

