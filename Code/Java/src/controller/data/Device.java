package controller.data;

import controller.data.parameter.Parameter;

public interface Device {
	public Enum<? extends Parameter>[] getParameters();
}
