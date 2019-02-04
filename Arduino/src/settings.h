const char* ssid     = "YOUR_SSID";
const char* password = "YOUR_WLAN_PASSWORD";
/* IP SETTINGS*/
IPAddress local_IP = IPAddress(192, 168, 190, 37);
IPAddress gateway = IPAddress(192, 168, 190, 1);
IPAddress subnet = IPAddress(255, 255, 255, 0);
IPAddress primaryDNS = IPAddress(8, 8, 8, 8); //optional
IPAddress secondaryDNS = IPAddress(8, 8, 4, 4); //optional
 
const char* mqtt_server = "192.168.190.70";
const int   mqtt_port = 1884;

const char* mqtt_user = "YOUR_MQTT_USER"; 
const char* mqtt_pwd = "YOUR_MQTT_PASSWORD";
