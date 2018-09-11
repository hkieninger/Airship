#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>

#define TRIG 28
#define ECHO 29
void setup() {
        wiringPiSetup();
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);

        //TRIG pin must start LOW
        digitalWrite(TRIG, LOW);
        delay(30);
}

int getCM() {
        //Send trig pulse
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG, LOW);
        //Wait for echo start
        while(digitalRead(ECHO) == LOW);
        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHO) == HIGH);
        long travelTime = micros() - startTime;
        //Get distance in cm
        int distance = travelTime / 58;

        return distance;
}

int main(void) {
        setup();
        long whilestarttime = micros();
        while(1){
          if(whilestarttime <= micros() - 1000000){
                printf("Distance: %dcm\n", getCM());
                whilestarttime = micros();
          }

        }

        return 0;
}
