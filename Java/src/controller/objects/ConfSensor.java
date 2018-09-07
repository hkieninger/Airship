package controller.objects;

public enum ConfSensor implements Parameter {
	BATTERY, ACCEL, GYRO, COMPASS, GPS, BARO, CAM_FRONT, CAM_BOTTOM, DIST_FRONT, DIST_BOTTOM, KALMAN;

	@Override
	public Class<?> getDataType() {
		// TODO Auto-generated method stub
		return null;
	}
}