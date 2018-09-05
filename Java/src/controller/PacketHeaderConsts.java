package controller;

/**
 * 
 * @author hans
 *
 * Device and Param should be send as unsigned byte (0 - 255)
 *
 */

enum Device {
	RASPBERRY, BATTERY, ACTUATOR, STEERING, SENSOR
}

enum P_Raspberry {
	ECHO_REQUEST, ECHO_REPLY, MESSAGE
}

//out: -, in: int
enum P_Battery {
	PERCENTAGE
}

//out: int, in: -
enum P_Actuator {
	LEFT_MOTOR, RIGHT_MOTOR, LEFT_RUDDER, RIGHT_RUDDER, TOP_RUDDER
}

//out: int (1 or 2), in: -
enum P_Steering {
	VELOCITY, YAW, PITCH, CALLIBRATION
}

//out: boolean (enable / disable), in: values dependent on sensor
enum P_Sensor {
	ACCEL, GYRO, COMPASS, GPS, BARO, CAM_FRONT, CAM_BOTTOM, DIST_FRONT, DIST_BOTTOM, KALMAN
}