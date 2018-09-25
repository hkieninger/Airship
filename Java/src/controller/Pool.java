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
	
	private ConnectionData[][] values;
	private List<Listener<D>> listeners;
	
	public Pool(Class<D> deviceClass) {
		listeners = new ArrayList<>();
		D[] devices = deviceClass.getEnumConstants();
		values = new ConnectionData[devices.length][];
		for(int i = 0; i < devices.length; i++) {
			int parameters = devices[i].getParameters().length;
			values[i] = new ConnectionData[parameters];
		}
	}
	
	public void addListener(Listener<D> l) {
		listeners.add(l);
	}
	
	public void removeListener(Listener<D> l) {
		listeners.remove(l);
	}
	
	void notifyListeners(D device, Enum<? extends Parameter> parameter) {
		for(Listener<D> l : listeners) {
			l.onChanged(this, device, parameter);
		}
	}
	
	public ConnectionData getValue(D device, Enum<? extends Parameter> parameter) {
		synchronized(parameter) { //added for the case that setting a value is not atomic, better safe than sorry
			return values[device.ordinal()][parameter.ordinal()];
		}
	}
	
	public void setValue(D device, Enum<? extends Parameter> parameter, ConnectionData value) {
		synchronized(parameter) {
			values[device.ordinal()][parameter.ordinal()] = value;
		}
		notifyListeners(device, parameter);
	}
	
	public interface Listener<D extends Enum<?> & Device> {
		void onChanged(Pool<D> pool, D device, Enum<? extends Parameter> parameter);
	}

}
