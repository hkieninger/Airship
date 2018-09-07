package controller.objects;

public enum ConfRPI implements Parameter {
	ECHO_REQUEST, INFO;

	@Override
	public Class<?> getDataType() {
		switch(this) {
			case ECHO_REQUEST: return Long.class;
			case INFO: return String.class;
		}
		throw new IllegalArgumentException();
	}	
}

