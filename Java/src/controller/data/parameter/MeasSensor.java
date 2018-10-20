package controller.data.parameter;

import controller.data.object.ConnectionData;
import controller.data.object.FloatVector;
import controller.data.object.GPSData;

public enum MeasSensor implements Parameter {
	BATTERY, ACCEL, GYRO, COMPASS, GPS, BARO, DIST_FRONT, DIST_BOTTOM, KALMAN;

	@Override
	public ConnectionData getDataInstance() {
		switch(this) {
			case BATTERY: return new ConnectionData.Byte();
			case ACCEL: return new FloatVector(3);
			case GYRO: return new FloatVector(3);
			case COMPASS: return new FloatVector(3);
			case GPS: return GPSData.getDefaultInstance(); //avoid frequent allocation
			case BARO: return new ConnectionData.Double();
			case DIST_FRONT: return new ConnectionData.Short();
			case DIST_BOTTOM: return new ConnectionData.Short();
			case KALMAN: throw new UnsupportedOperationException("Not implemented yet.");
		}
		throw new UnsupportedOperationException("Not implemented yet.");
	}
	
}