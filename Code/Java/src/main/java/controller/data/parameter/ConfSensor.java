package controller.data.parameter;

import controller.data.object.ConnectionData;

public enum ConfSensor implements Parameter {
	BATTERY, ACCEL, GYRO, COMPASS, GPS, BARO, CAM_FRONT, CAM_BOTTOM, DIST_FRONT, DIST_BOTTOM, KALMAN;

	public static final int ENABLE = 1;
	public static final int DISABLE = 0;
	
	@Override
	public ConnectionData getDataInstance() {
		return new ConnectionData.UByte(); //interpret it as bitfield with flags
	}

}