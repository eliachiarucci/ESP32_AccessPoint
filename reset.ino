void eeprom_reset() {
  EEPROM.begin(512);
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);

  }
  EEPROM.commit();
  WiFi.disconnect();
  ESP.restart();
}
