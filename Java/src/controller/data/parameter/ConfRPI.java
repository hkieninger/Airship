package controller.data.parameter;

import controller.data.object.ConnectionData;

public enum ConfRPI implements Parameter {
	ECHO_REQUEST, INFO;
	
	@Override
	public ConnectionData getDataInstance() {
		switch(this) {
			case ECHO_REQUEST: return new ConnectionData.Long();
			case INFO: return new ConnectionData.String();
		}
		throw new UnsupportedOperationException("Not implemented yet.");
	}	
}

