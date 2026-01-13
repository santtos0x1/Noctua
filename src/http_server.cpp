// Local libs
#include "http_server.h"
#include "config.h"

// Libs
#include <Arduino.h>
#include <WiFi.h>   
#include <SPI.h>
#include <SD.h>

WiFiServer server(WEB_SERVER_PORT);

const char* ssid = "";
const char* pass = "";
const bool manualWiFiConnection = false;

bool startServer() 
{
    DEBUG_PRINTLN("Remove before WiFi connection...");
    WiFi.disconnect();
    delay(Time::LOW_DELAY);

    // Starts the scan
    DEBUG_PRINTLN("Starting WiFi connection...");
    int networks = WiFi.scanNetworks();

    for (int i = 0; i < networks; i++)
    {
        DEBUG_PRINTLN("Getting the encryption type...");
        wifi_auth_mode_t encryptionType = WiFi.encryptionType(i);
        if(encryptionType == WIFI_AUTH_OPEN)
        {
            DEBUG_PRINTLN("Found an open network!");
            DEBUG_PRINTF("Trying to connect to %s\n", WiFi.SSID(i));
            
            DEBUG_PRINTLN("Verifying the connection mode...");
            if(!manualWiFiConnection)
            {
                DEBUG_PRINTLN("Mode: Automatic mode");
                WiFi.begin(WiFi.SSID(i).c_str());
            } else {
                DEBUG_PRINTLN("Mode: Manual mode");
                WiFi.begin(ssid, pass);
            }

            int attempts = 0;
            DEBUG_PRINTLN("Getting WiFi status");
            wl_status_t connStatus = WiFi.status();

            while (connStatus != WL_CONNECTED && attempts < SERVER_ATTEMPTS_LIMIT) 
            {
                delay(Time::MID_DELAY);
                DEBUG_PRINT(".");
                attempts++;
            }

            if(connStatus == WL_CONNECTED)
            {
                DEBUG_PRINTLN("Connection estabilished!");
                break;
            } else {
                DEBUG_PRINTLN("Cannot connect to this network!");
            }
        }
    }

    wl_status_t status = WiFi.status();
    if(status == WL_CONNECTED)
    {
        DEBUG_PRINTLN("Starting server...");
        server.begin();
        DEBUG_PRINTLN("HTTP Server started!");
        DEBUG_PRINT("IP: ");
        DEBUG_PRINTLN(WiFi.localIP());
        return true;
    } else {
        DEBUG_PRINTLN("Cannot open server!");
        return false;
    }
}

void handleDownload(WiFiClient& client, String path)
{
    DEBUG_PRINTF("Verifying if %s exists...", path);
    if (SD.exists(path))
    {
        DEBUG_PRINTLN("Done!");
        DEBUG_PRINTLN("Opening path...");
        File dataFile = SD.open(path);
        // HEAD
        DEBUG_PRINTLN("Writing the server page...");
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/octet-stream");
        client.print("Content-Length: ");
        client.println(dataFile.size());
        client.println("Connection: close");
        client.println();

        uint8_t buffer[HND_BUFFER_SIZE];
        DEBUG_PRINTLN("Seding data to the server...");
        while (dataFile.available())
        {
            int bytesRead = dataFile.read(buffer, sizeof(buffer));
            client.write(buffer, bytesRead);
        }
        DEBUG_PRINTLN("Closing path...");
        dataFile.close();
    } else {
        client.println("HTTP/1.1 404 Not Found\r\n\r\nFile not found.");
    }
}

void sendIndexSD(WiFiClient& client)
{
    // HEAD
    DEBUG_PRINTLN("Writing the server page...");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=utf-8");
    client.println();

    // WiFi files section
    client.println("<h1>WiFi logs folder - SD files</h1><br>");
    client.println("<ul>");
    DEBUG_PRINTLN("Opening WiFi log path...");
    File WFRoot = SD.open("/wifi_log_data/");
    DEBUG_PRINTLN("Defining the next file to open...")
    File WFFile = WFRoot.openNextFile();
    
    while (WFFile)
    {
        String WDLogName = WFFile.name();
        client.print("<li><a href=\"/wifi_log_data/");
        client.print(WDLogName);
        client.print("\">");
        client.print(WDLogName);
        client.println("</a></li>");
        
        WFFile = WFRoot.openNextFile();
    }
    client.println("</ul>");

    DEBUG_PRINTLN("Closing path...");
    WFRoot.close();
    
    // Bluetooth files section
    client.println("<h1>Bluetooth logs folder - SD files</h1><br>");
    client.println("<ul>");

    DEBUG_PRINTLN("Opening Bluetooth log path...");
    File BTRoot = SD.open("/bluetooth_log_data/");
    DEBUG_PRINTLN("Defining the next file to open...")
    File BTFile = BTRoot.openNextFile();

    while (BTFile)
    {
        String BTLogName = BTFile.name();
        client.print("<li><a href=\"/bluetooth_log_data/");
        client.print(BTLogName);
        client.print("\">");
        client.print(BTLogName);
        client.println("</a></li>");
        
        BTFile = BTRoot.openNextFile();
    }
    client.println("</ul>");
    
    DEBUG_PRINTLN("Closing path...");
    BTRoot.close();
}    

void serverCFG()
{   
    DEBUG_PRINTLN("Defining the client...");
    WiFiClient client = server.available();
    
    if(client)
    {
        DEBUG_PRINTLN("Done!");
        DEBUG_PRINTLN("Reading the request...");
        String request = client.readStringUntil('\r');
        client.readStringUntil('\n');

        int addrStart = request.indexOf('/');
        int addrEnd = request.indexOf(' ', addrStart);
        String fileName = request.substring(addrStart, addrEnd);

        if(fileName == "/")
        {
            DEBUG_PRINTLN("Sending the home page...");
            sendIndexSD(client);
        } else {
            DEBUG_PRINTLN("Sending download page...");
            handleDownload(client, fileName);
        }

        delay(Time::LOW_DELAY);
        DEBUG_PRINTLN("Closing client...");
        client.stop();
        DEBUG_PRINTLN("Done!");
    }
}
