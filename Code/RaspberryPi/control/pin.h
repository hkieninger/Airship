#ifndef PIN_H
#define PIN_H 1

/*
 * GPIO pins
 */
#define LEFT_MOTOR_ESC 23
#define LEFT_MOTOR_RELAIS 17
#define RIGHT_MOTOR_ESC 22
#define RIGHT_MOTOR_RELAIS 27

#define LEFT_RUDDER_SERVO 18
#define RIGHT_RUDDER_SERVO 25
#define TOP_RUDDER_SERVO 24


#define FRONT_HCSR04_TRIG 16
#define FRONT_HCSR04_ECHO 26
#define BOTTOM_HCSR04_TRIG 20
#define BOTTOM_HCSR04_ECHO 21

/*
 * other resources
 */

//raspi cam
#define CSI_CAMERA "/dev/video0"
#define CSI_WIDTH 640
#define CSI_HEIGHT 480
#define CSI_PORT 0xCCCD

//usb cam
#define USB_CAMERA "/dev/video1"
#define USB_WIDTH 320
#define USB_HEIGHT 240
#define USB_PORT 0xCCCE

#define ADS_INPUT_PIN 3

#endif /* PIN_H */