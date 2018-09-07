package controller;

import java.util.ArrayList;
import java.util.List;

import controller.objects.Device;
import controller.objects.Parameter;

public class Pool<D extends Enum<?> & Device> {
	
	private boolean[][] changed;
	private Object[][] values;
	private List<Listener<D>> listeners;
	
	public Pool(Class<D> deviceClass) {
		listeners = new ArrayList<>();
		D[] devices = deviceClass.getEnumConstants();
		changed = new boolean[devices.length][];
		values = new Object[devices.length][];
		for(int i = 0; i < devices.length; i++) {
			int parameters = devices[i].getParameter().getEnumConstants().length;
			changed[i] = new boolean[parameters];
			values[i] = new Object[parameters];
		}
	}
	
	synchronized public void addListener(Listener<D> l) {
		listeners.add(l);
	}
	
	synchronized public void removeListener(Listener<D> l) {
		listeners.remove(l);
	}
	
	synchronized void notifyListeners(D device, Enum<? extends Parameter> parameter) {
		for(Listener<D> l : listeners) {
			l.onChanged(this, device, parameter);
		}
		for(int i = 0; i < changed.length; i++) {
			for(int j = 0; j < changed[i].length; j++) {
				changed[i][j] = false;
			}
		}
	}
	
	synchronized public boolean hasChanged(D device, Enum<? extends Parameter> parameter) {
		return changed[device.ordinal()][parameter.ordinal()];
	}
	
	synchronized public void resetChanged(D device, Enum<? extends Parameter> parameter) {
		changed[device.ordinal()][parameter.ordinal()] = false;
	}
	
	@SuppressWarnings("unchecked")
	synchronized public <T> T getValue(D device, Enum<? extends Parameter> parameter, Class<T> dataType) {
		if(((Parameter) parameter).getDataType() == dataType)
			return (T) values[device.ordinal()][parameter.ordinal()];
		throw new IllegalArgumentException();
	}
	
	synchronized public <T> void setValue(D device, Enum<? extends Parameter> parameter, T value) {
		if(((Parameter) parameter).getDataType() == value.getClass()) {
			changed[device.ordinal()][parameter.ordinal()] = true;
			values[device.ordinal()][parameter.ordinal()] = value;
		} else
			throw new IllegalArgumentException();
	}
	
	public interface Listener<D extends Enum<?> & Device> {
		void onChanged(Pool<D> pool, D device, Enum<? extends Parameter> parameter);
	}

}
