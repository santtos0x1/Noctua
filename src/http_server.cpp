// Local libs
#include "http_server.h"
#include "data_logger.h"
#include "config.h"

// Libs
#include <Arduino.h>
#include <WiFi.h>   
#include <SPI.h>
#include <SD.h>

// Instantiates the WiFi server on the port defined in config.h
WiFiServer server(WEB_SERVER_PORT);

// Network credentials for manual connection mode
const char* ssid = "";
const char* pass = "";
const bool manualWiFiConnection = false;

/**
   Scans for available networks and attempts to establish an HTTP server.
   Priority is given to open networks (No Auth) for field accessibility.
 */
bool startServer() 
{
    // Ensure the radio is in a clean state before starting the scan
    DEBUG_PRINTLN(CLR_YELLOW "Resetting WiFi connection..." CLR_RESET);
    WiFi.disconnect();
    delay(Time::LOW_DELAY);

    // Perform a synchronous scan of nearby Access Points
    DEBUG_PRINTLN(CLR_YELLOW "Scanning for available networks..." CLR_RESET);
    int networks = WiFi.scanNetworks();

    for (int i = 0; i < networks; i++)
    {
        // Check the security protocol of the detected network
        wifi_auth_mode_t encryptionType = WiFi.encryptionType(i);
        int32_t dbm = WiFi.RSSI(i);
        if(encryptionType == WIFI_AUTH_OPEN && dbm >= -65)
        {
            DEBUG_PRINTLN(CLR_GREEN "Open network identified!" CLR_RESET);
            DEBUG_PRINTF(CLR_CYN "Attempting connection to SSID: %s\n" CLR_RESET, WiFi.SSID(i).c_str());
            
            // Logic switch between hardcoded credentials and open connection
            if(!manualWiFiConnection)
            {
                DEBUG_PRINTLN("Mode: Automatic Association");
                WiFi.begin(WiFi.SSID(i).c_str());
            } else {
                DEBUG_PRINTLN("Mode: Manual Configuration");
                WiFi.begin(ssid, pass);
            }

            // Connection timeout loop to prevent infinite blocking
            int attempts = 0;
            wl_status_t connStatus = WiFi.status();

            while (connStatus != WL_CONNECTED && attempts < SERVER_ATTEMPTS_LIMIT) 
            {
                delay(Time::MID_DELAY);
                DEBUG_PRINT(CLR_YELLOW "." CLR_RESET);
                attempts++;
                connStatus = WiFi.status(); // Update status in each iteration
            }

            if(connStatus == WL_CONNECTED)
            {
                DEBUG_PRINTLN(CLR_GREEN "\nConnection established successfully!" CLR_RESET);
                break; // Exit scan loop once connected
            } else {
                DEBUG_PRINTLN(CLR_RED "\nFailed to associate with this network." CLR_RESET);
            }
        }
    }

    // Final check to initiate the HTTP service
    if(WiFi.status() == WL_CONNECTED)
    {
        DEBUG_PRINTLN(CLR_YELLOW "Initializing HTTP Listener..." CLR_RESET);
        server.begin();
        DEBUG_PRINTLN(CLR_GREEN "Server active!" CLR_RESET);
        DEBUG_PRINT(CLR_CYN "Local IP Address: " CLR_RESET);
        DEBUG_PRINTLN(WiFi.localIP());
        return true;
    } else {
        DEBUG_PRINTLN(CLR_RED "Server startup failed: No valid connection." CLR_RESET);
        return false;
    }
}

/**
   Streams a file from the SD card to the connected HTTP client.
   Uses a buffered approach to handle large CSV files efficiently.
 */
void handleDownload(WiFiClient& client, String path)
{
    DEBUG_PRINTF(CLR_YELLOW "Verifying path: %s..." CLR_RESET, path.c_str());
    if (SD.exists(path))
    {
        DEBUG_PRINTLN(CLR_GREEN "File found." CLR_RESET);
        File dataFile = SD.open(path);

        // Constructing standard HTTP headers for binary file transfer
        DEBUG_PRINTLN(CLR_YELLOW "Sending HTTP 200 Headers..." CLR_RESET);
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/octet-stream"); // Triggers browser download
        client.print("Content-Length: ");
        client.println(dataFile.size());
        client.println("Connection: close");
        client.println();

        // Data streaming loop using a memory-efficient buffer
        uint8_t buffer[HND_BUFFER_SIZE];
        DEBUG_PRINTLN(CLR_CYN "Streaming data payload..." CLR_RESET);
        while (dataFile.available())
        {
            int bytesRead = dataFile.read(buffer, sizeof(buffer));
            client.write(buffer, bytesRead);
        }
        
        dataFile.close();
        DEBUG_PRINTLN(CLR_GREEN "Transfer complete. File closed." CLR_RESET);
    } else {
        // Standard 404 response for non-existent log files
        DEBUG_PRINTLN(CLR_RED "File NOT found on SD card." CLR_RESET);
        client.println("HTTP/1.1 404 Not Found\r\n\r\nError: File not found on SD.");
    }
}

/**
   Generates a dynamic HTML index page listing all captured logs.
   Iterates through SD directories to build interactive download links.
 */
void sendIndexSD(WiFiClient& client)
{
    // Send basic HTML document headers
    DEBUG_PRINTLN(CLR_YELLOW "Generating Index Page..." CLR_RESET);
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=utf-8");
    client.println();

    client.println("<html><body>");
    
    // --- WiFi Logs Section ---
    client.println("<h1>WiFi Logs Inventory</h1><ul>");
    File WFRoot = SD.open("/wifi_log_data/");
    File WFFile = WFRoot.openNextFile();
    
    while (WFFile)
    {
        String fileName = WFFile.name();
        client.print("<li><a href=\"/wifi_log_data/");
        client.print(fileName);
        client.print("\">");
        client.print(fileName);
        client.println("</a></li>");
        WFFile = WFRoot.openNextFile();
    }
    WFRoot.close();
    client.println("</ul><hr>");

    // --- Bluetooth Logs Section ---
    client.println("<h1>Bluetooth Logs Inventory</h1><ul>");
    File BTRoot = SD.open("/bluetooth_log_data/");
    File BTFile = BTRoot.openNextFile();

    while (BTFile)
    {
        String fileName = BTFile.name();
        client.print("<li><a href=\"/bluetooth_log_data/");
        client.print(fileName);
        client.print("\">");
        client.print(fileName);
        client.println("</a></li>");
        BTFile = BTRoot.openNextFile();
    }
    BTRoot.close();
    
    client.println("</ul></body></html>");
    DEBUG_PRINTLN(CLR_GREEN "Index page sent successfully." CLR_RESET);
}    

/**
   Main server runtime loop. Listens for incoming TCP connections 
   and parses HTTP GET requests to serve files or the index.
 */
void serverRun()
{   
    // Check for an active incoming client connection
    WiFiClient client = server.available();
    
    if(client)
    {
        DEBUG_PRINTLN(CLR_GRN "New client connection established." CLR_RESET);
        
        // Simple HTTP GET request parsing
        String request = client.readStringUntil('\r');
        client.readStringUntil('\n');

        // Extract the requested path/filename from the HTTP header
        int addrStart = request.indexOf('/');
        int addrEnd = request.indexOf(' ', addrStart);
        String fileName = request.substring(addrStart, addrEnd);

        if(fileName == "/")
        {
            // Serve the root directory listing
            sendIndexSD(client);
        } else {
            // Serve specific file download
            handleDownload(client, fileName);
        }

        // Standard delay for connection stabilization before closing
        delay(Time::LOW_DELAY);
        client.stop();
        DEBUG_PRINTLN(CLR_YELLOW "Client session terminated." CLR_RESET);
    }
}