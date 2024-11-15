Preferences preferences;

#define READ_WRITE false
#define READ_ONLY true

void save_ssid(String ssid) {
  preferences.begin("wifi", READ_WRITE);  // "my-app" is the namespace, 'false' for read/write mode
  preferences.putString("ssid", ssid);
  preferences.end();
}

void save_pwd(String pwd) {
  preferences.begin("wifi", READ_WRITE);  // "my-app" is the namespace, 'false' for read/write mode
  preferences.putString("pwd", pwd);
  preferences.end();
}

void storage_reset() {
  preferences.begin("wifi", READ_WRITE);  // "my-app" is the namespace, 'false' for read/write mode
  preferences.clear();
  preferences.end();
}


String get_saved_ssid() {
  preferences.begin("wifi", READ_ONLY);  // "my-app" is the namespace, 'false' for read/write mode
  String ssid = preferences.getString("ssid");
  preferences.end();
  return ssid;
}

String get_saved_pwd() {
  preferences.begin("wifi", READ_ONLY);  // "my-app" is the namespace, 'false' for read/write mode
  String pwd = preferences.getString("pwd");
  preferences.end();
  return pwd;
}
