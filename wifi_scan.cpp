#include "wifi_scan.h"
#include "config.h"
#include "WiFi.h"

#define CONN_TIMEOUT_MS (10 * 1000) 

QueueHandle_t wifiQueue;

void wifiInit()
{
  wifiQueue = xQueueCreate(10, sizeof(wifiData));
}

void wifiSniffer()
{
  int numNetworksFound = WiFi.scanNetworks();
  for(int i = 0; i < numNetworksFound; i++)
  {
    wifiData data;

    strncpy(d.ssid, WiFi.SSID(i).c_str(), 32);
    data.rssi = WiFi.RSSI(i);
    data.bssid = WiFi.BSSIDstr(i);
    data.encryptationType = WiFi.encryptionType(i);
    data.channel = WiFi.channel(i);

    if(d.encryptationType == WIFI_AUTH_OPEN)
    {
      initTimer = milis();
      Wifi.begin(data.ssid);

      Serial.print("Connecting");
      while(WiFi.status()!= Wl_CONNECTED){
        if (milis() - initTimer >= CONN_TIMEOUT_MS))
        {
          Serial.println("Connection timeout...");
          break;
        }
              
        delay(500);
        Serial.print(".");
      }

      Serial.println("Sucessfully connected on " + data.ssid);
      Serial.println("Getting data from" + data.ssid);
      
      data.hostname = WiFi.hostname();
      data.dnsIP = WiFi.dnsIP(0);
      data.subNetMask = WiFi.subnetMask();
      data.localIP = WiFi.localIP();
      
      Serial.println("Done!");
    }
    xQueueSend(wifiQueue, &data, pdMS_TO_TICKS(10));
  }
}
