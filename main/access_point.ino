char INDEX_HTML[] =
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta content=\"text/html; charset=ISO-8859-1\""
  " http-equiv=\"content-type\">"
  "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
  "<title>Remote-Key Control</title>"
  "<style>"
  "body { background-color: #aaffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }"
  "</style>"
  "</head>"
  "<body>"
  "<div style='display:flex; flex-direction: column; justify-content: center; position: absolute; top:50%; left:50%; transform: translate(-50%,-50%)'>"
  "<h2>Remote-Key Control</h2>"
  "<FORM action=\"/\" method=\"post\">"
  "<div style='display:flex; flex-direction: column; height: 100px; justify-content: space-around'>"
  "<div style='display:flex; flex-direction: row'>"
  "<label style='flex:1'>SSID:</label>"
  "<input style='flex:1' maxlength=\"30\" name=\"ssid\"><br>"
  "</div>"
  "<div style='display:flex; flex-direction: row'>"
  "<label style='flex:1'>Password:</label>"
  "<input style='flex:1' maxlength=\"30\" name=\"Password\"><br>"
  "</div>"
  "<INPUT type=\"submit\" value=\"Save Configuration\">"
  "</div>"
  "</FORM>"
  
  "<div style='display:flex; flex-direction: column; margin-top: 30px'>"
  "<div style='display:flex; flex-direction:row'>"
  "<div style='flex:1'>"
  "SSID"
  "</div>"
  "<div style='flex:1; text-align:right'>"
  "db"
  "</div>"
  "</div>"
  ;

  char END_HTML[] =
  
  "</div>"
  "</div>"
  "</body>"
  "</html>"
  ;


void access_point() {
  Serial.println();
  Serial.print("Configuring access point...");
  Serial.print("Setting soft-AP configuration ... ");
  IPAddress local_IP(192,168,100,1);
  IPAddress gateway(192,168,100,1);
  IPAddress subnet(255,255,255,0);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  Serial.print("Setting soft-AP ... ");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Serial.println(ap_ssid);
  Serial.println(ap_password);

  //Configuring the web server
  server.on("/", handle_ap_Root);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void handleSubmit() { //display values and write to memmory

  server.send(200, "text/html", "");
  //calling function that writes data to memory
  write_to_memory(String(server.arg("ssid")), String(server.arg("Password")));
}
//Write data to memory
/*
   We prepare the data strings by adding the 'end of line' symbol. I decided to use ";".
   Then we pass it off to the write_EEPROM function to actually write it to memmory.
*/
void write_to_memory(String new_ssid, String new_password) {
  EEPROM.begin(512); //Starting and setting size of the EEPROM
  
  int _size = new_ssid.length();
  for (int i = 0; i < _size; i++)
  {
    EEPROM.write(0 + i, new_ssid[i]);
  }
  EEPROM.write(0 + _size, '\0'); //Add termination null character for String Data

  EEPROM.commit();
  
  _size = new_password.length();
  for (int i = 0; i < _size; i++)
  {
    EEPROM.write(100 + i, new_password[i]);
  }
  EEPROM.write(100 + _size, '\0'); //Add termination null character for String Data
  EEPROM.commit();
  
  WiFi.disconnect();
  ESP.restart();
}

void handle_ap_Root() {
  if (server.hasArg("ssid") && server.hasArg("Password")) { //If all form fields contain data call handelSubmit()
    handleSubmit();
  }
  else { //Redisplay the form
    scan_wifi_networks();
  }
}

void scan_wifi_networks() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  String networks;
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      networks += 
      "<div style='display:flex; flex-direction:row; margin-top: 10px'>"
      "<div style='flex:1'>"
      + WiFi.SSID(i) + 
      "</div>"
      "<div style='flex:1; text-align:right'>"
      + WiFi.RSSI(i) +
      "</div>"
      "</div>"
      ;
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
    
  }
  Serial.println("");
  //Serial.println(networks);
  //strcat(INDEX_HTML, END_HTML);
  char FINAL_HTML[2048];
  strcpy(FINAL_HTML, INDEX_HTML);
  strcat(FINAL_HTML, networks.c_str());
  strcat(FINAL_HTML, END_HTML);
  Serial.println(FINAL_HTML);
  server.send(200, "text/html", FINAL_HTML);
  // Wait a bit before scanning again
  }
 