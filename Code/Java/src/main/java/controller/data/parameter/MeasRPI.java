package controller.data.parameter;

import controller.data.object.ConnectionData;

public enum MeasRPI implements Parameter {
	ECHO_REPLY, INFO;

	@Override
	public ConnectionData getDataInstance() {
		switch(this) {
			case ECHO_REPLY: return new ConnectionData.Long();
			case INFO: return new ConnectionData.String();
		}
		throw new UnsupportedOperationException("Not implemented yet.");
	}


}

