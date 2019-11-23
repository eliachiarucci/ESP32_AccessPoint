void eeprom() {
  EEPROM.begin(512);

  check_ssid();
  check_passwd();

  EEPROM.commit();

}

void check_ssid() {

  char add = 0;
  char data[100]; //Max 100 Bytes
  int len = 0;
  unsigned char k;
  k = EEPROM.read(add);
  while (k != '\0' && len < 500) //Read until null character
  {
    k = EEPROM.read(add + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  Serial.println(data);
  ssid = data;

}

void check_passwd() {

  char add = 100;
  char data[100]; //Max 100 Bytes
  int len = 0;
  unsigned char k;
  k = EEPROM.read(add);
  while (k != '\0' && len < 500) //Read until null character
  {
    k = EEPROM.read(add + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  Serial.println(data);
  password = data;

}
