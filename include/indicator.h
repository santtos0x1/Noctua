#ifndef INDICATOR_H
#define INDICATOR_H

void setupIndicator(int ledPinout);
void showProcessing(int ledPinout);
void showSuccess(int ledPinout);
void showError(int ledPinout);
void showOn(int ledPinout);
void showOff(int ledPinout);
void idleState(int ledPinout1, int ledPinout2, int ledPinout3);
void allLeds(int state);

#endif // !INDICATOR_H