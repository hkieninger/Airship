package controller.data.parameter;

import controller.data.object.ConnectionData;

public enum ConfSensor implements Parameter {
	BATTERY, ACCEL, GYRO, COMPASS, GPS, BARO, CAM_FRONT, CAM_BOTTOM, DIST_FRONT, DIST_BOTTOM, KALMAN;

	@Override
	public ConnectionData getDataInstance() {
		return new ConnectionData.UByte();
	}

}