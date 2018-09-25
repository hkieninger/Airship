package controller.data.parameter;

import controller.data.object.ConnectionData;
import controller.data.object.ByteVector;

public enum ConfSteering implements Parameter {
	VELOCITY, DIRECTION, CALLIBRATION;
	
	public static final int MAX = 100;

	@Override
	public ConnectionData getDataInstance() {
		switch(this) {
			case VELOCITY: return new ConnectionData.Byte();
			case DIRECTION: return new ByteVector(2);
			case CALLIBRATION: return new ByteVector(2);
		}
		throw new UnsupportedOperationException("Not implemented yet.");
	}


}
