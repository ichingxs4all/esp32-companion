setupPreferences(){
  preferences.begin("companion_prefs", true);
  unsigned int _companion_ip = preferences.getUInt("companion_ip", );
  _ip = _companion_ip;
  //IPAddress _ip;
  //uint32_t ip_addr = (uint32_t) _ip;

   // Close the Preferences
  preferences.end();
}