package controller;

import java.util.ArrayList;
import java.util.List;

import controller.data.Device;
import controller.data.object.ConnectionData;
import controller.data.parameter.Parameter;


/*
 * add structure that registers changes instead of changed array
 */

public class Pool<D extends Enum<?> & Device> {
	
	private boolean[][] changed;
	private ConnectionData[][] values;
	private List<Listener<D>> listeners;
	
	public Pool(Class<D> deviceClass) {
		listeners = new ArrayList<>();
		D[] devices = deviceClass.getEnumConstants();
		changed = new boolean[devices.length][];
		values = new ConnectionData[devices.length][];
		for(int i = 0; i < devices.length; i++) {
			int parameters = devices[i].getParameters().length;
			changed[i] = new boolean[parameters];
			values[i] = new ConnectionData[parameters];
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
	}
	
	synchronized public boolean hasChanged(D device, Enum<? extends Parameter> parameter) {
		return changed[device.ordinal()][parameter.ordinal()];
	}
	
	synchronized public void resetChanged(D device, Enum<? extends Parameter> parameter) {
		changed[device.ordinal()][parameter.ordinal()] = false;
	}
	
	synchronized public ConnectionData getValue(D device, Enum<? extends Parameter> parameter) {
		return values[device.ordinal()][parameter.ordinal()];
	}
	
	synchronized public void setValue(D device, Enum<? extends Parameter> parameter, ConnectionData value) {
		changed[device.ordinal()][parameter.ordinal()] = true;
		values[device.ordinal()][parameter.ordinal()] = value;
		notifyListeners(device, parameter);
	}
	
	public interface Listener<D extends Enum<?> & Device> {
		void onChanged(Pool<D> pool, D device, Enum<? extends Parameter> parameter);
	}

}
