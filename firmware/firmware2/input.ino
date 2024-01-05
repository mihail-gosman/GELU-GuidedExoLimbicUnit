void getInput() {
  if (Serial.available() > 0) {
    char buffer;
    unsigned uint8_t index = 0;
    do {
      buffer = Serial.read();
      input[index] = buffer;
      index += 1;
    } while(buffer != '\n');
  }
}