package controller.objects;

public enum MeasDevice implements Device {
	RPI, SENSOR;

	@Override
	public Class<? extends Enum<? extends Parameter>> getParameter() {
		switch(this) {
			case RPI: return MeasRPI.class;
			case SENSOR: return MeasSensor.class;
		}
		return null;
	}

}
