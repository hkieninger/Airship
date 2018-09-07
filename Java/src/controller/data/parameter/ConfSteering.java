package controller.data.parameter;

import controller.data.object.ConnectionData;
import controller.data.object.UByteVector;

public enum ConfSteering implements Parameter {
	VELOCITY, YAW, PITCH, CALLIBRATION;
	
	public static final int MAX = 100;

	@Override
	public ConnectionData getDataInstance() {
		switch(this) {
			case VELOCITY: return new ConnectionData.UByte();
			case YAW: return new ConnectionData.UByte();
			case PITCH: return new ConnectionData.UByte();
			case CALLIBRATION: return new UByteVector(2);
		}
		throw new UnsupportedOperationException("Not implemented yet.");
	}


}
