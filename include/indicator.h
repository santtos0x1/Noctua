#ifndef INDICATOR_H
#define INDICATOR_H

void setupIndicator(int ledPinout);
void showProcessing(int ledPinout);
void showSuccess(int ledPinout);
void showError(int ledPinout);
void showOn(int ledPinout);
void showOff(int ledPinout);

#endif // !INDICATOR_H