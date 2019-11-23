
//Creating the input form
const char INDEX_HTML[] =
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta content=\"text/html; charset=ISO-8859-1\""
  " http-equiv=\"content-type\">"
  "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
  "<title>ESP8266 Web Form Demo</title>"
  "<style>"
  "\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
  "</style>"
  "</head>"
  "<body>"
  "<div style='display:flex; flex-direction: column; justify-content: center; position: absolute; top:50%; left:50%; transform: translate(-50%,-50%)'>"
  "<h1>ESP8266 Web Form Demo</h1>"
  "<FORM action=\"/\" method=\"post\">"
  "<P>"
  "<label>ssid:&nbsp;</label>"
  "<input maxlength=\"30\" name=\"ssid\"><br>"
  "<label>Password:&nbsp;</label><input maxlength=\"30\" name=\"Password\"><br>"
  "<INPUT type=\"submit\" value=\"Send\"> <INPUT type=\"reset\">"
  "</P>"
  "</FORM>"
  "</div>"
  "</body>"
  "</html>";




void access_point() {
  Serial.println();
  Serial.print("Configuring access point...");
  Serial.print("Setting soft-AP configuration ... ");
  //WiFi.softAPConfig(ap_local_IP, ap_gateway, ap_subnet);
  Serial.print("Setting soft-AP ... ");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Serial.println(ap_ssid);
  Serial.println(ap_password);


  /**IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);**/
  //Configuring the web server
  server.on("/", handle_ap_Root);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void handleSubmit() { //dispaly values and write to memmory
  String response = "<p>The ssid is ";
  response += server.arg("ssid");
  response += "<br>";
  response += "And the password is ";
  response += server.arg("Password");
  response += "<br>";
  response += "And the IP Address is ";
  response += server.arg("IP");
  response += "</P><BR>";
  response += "<H2><a href=\"/\">go home</a></H2><br>";

  server.send(200, "text/html", response);
  //calling function that writes data to memory
  write_to_memory(String(server.arg("ssid")), String(server.arg("Password")));
}
//Write data to memory
/**
   We prepping the data strings by adding the end of line symbol I decided to use ";".
   Then we pass it off to the write_EEPROM function to actually write it to memmory
*/
void write_to_memory(String new_ssid, String new_password) {
  EEPROM.begin(512);//Starting and setting size of the EEPROM
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
  else {//Redisplay the form
    server.send(200, "text/html", INDEX_HTML);
  }
}
