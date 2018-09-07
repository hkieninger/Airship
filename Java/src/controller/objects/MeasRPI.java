package controller.objects;

public enum MeasRPI implements Parameter {
	ECHO_REPLY, INFO;

	@Override
	public Class<?> getDataType() {
		switch(this) {
			case ECHO_REPLY: return Long.class;
			case INFO: return String.class;
		}
		throw new IllegalArgumentException();
	}	
}

