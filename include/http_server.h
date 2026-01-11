#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <Arduino.h>

void sendIndexSD();
void handleDownload();
bool startServer();
void serverCFG(); 

#endif // !HTTP_SERVER_H