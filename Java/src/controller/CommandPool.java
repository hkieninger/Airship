package controller;

import java.util.ArrayList;
import java.util.List;

import controller.DataPool.Data;

public class CommandPool {
	
	public static final int MOTOR_THRUST_MAX = 100;
	public static final int RUDDER_ANGLE_MAX = 90;
	
	private boolean[] changed;
	
	private boolean[] actuatorChanged;
	private int[] actuatorValues;
	
	private boolean[] steeringChanged;
	private int[] steeringValues;
	
	private boolean[] sensorChanged;
	private boolean[] sensorEnabled;
	
	private List<Listener> listeners;
	
	public CommandPool() {
		listeners = new ArrayList<>();
		changed = new boolean[Command.values().length];
		
		actuatorChanged = new boolean[5];
		actuatorValues = new int[actuatorChanged.length];
		
		steeringChanged = new boolean[5];
		steeringValues = new int[steeringChanged.length];
		
		sensorChanged = new boolean[P_Sensor.values().length];
		sensorEnabled = new boolean[sensorChanged.length];
	}
	
	public void addListener(Listener l) {
		listeners.add(l);
	}
	
	public void removeListener(Listener l) {
		listeners.remove(l);
	}
	
	public void notifyListeners() {
		for(Listener l : listeners) {
			l.onCommandChanged(this);
		}
		for(int i = 0; i < actuatorChanged.length; i++) {
			actuatorChanged[i] = false;
		}
		for(int i = 0; i < steeringChanged.length; i++) {
			steeringChanged[i] = false;
		}
		for(int i = 0; i < sensorChanged.length; i++) {
			sensorChanged[i] = false;
		}
		for(int i = 0; i < changed.length; i++) {
			changed[i] = false;
		}
	}
	
	public void setActuator(Actuator actuator, int value) {
		actuatorValues[actuator.ordinal()] = value;
		actuatorChanged[actuator.ordinal()] = true;
		changed[Command.ACTUATOR.ordinal()] = true;
	}
	
	public void setSteering(Steering steering, int value) {
		steeringValues[steering.ordinal()] = value;
		steeringChanged[steering.ordinal()] = true;
		changed[Command.STEERING.ordinal()] = true;
	}
	
	public void enableData(P_Sensor data, boolean enabled) {
		sensorEnabled[data.ordinal()] = enabled;
		sensorChanged[data.ordinal()] = true;
		changed[Command.SENSOR.ordinal()] = true;
	}
	
	/**
	 * @return the changed
	 */
	boolean[] getChanged() {
		return changed;
	}

	/**
	 * @return the actuatorChanged
	 */
	boolean[] getActuatorChanged() {
		return actuatorChanged;
	}

	/**
	 * @return the actuatorValues
	 */
	int[] getActuatorValues() {
		return actuatorValues;
	}

	/**
	 * @return the steeringChanged
	 */
	boolean[] getSteeringChanged() {
		return steeringChanged;
	}

	/**
	 * @return the steeringValues
	 */
	int[] getSteeringValues() {
		return steeringValues;
	}

	/**
	 * @return the dataChanged
	 */
	boolean[] getDataChanged() {
		return sensorChanged;
	}

	/**
	 * @return the dataEnabled
	 */
	boolean[] getDataEnabled() {
		return sensorEnabled;
	}
	
	interface Listener {
		
		void onCommandChanged(CommandPool pool);
		
	}
	
	public enum Steering {
		VELOCITY, YAW, PITCH, CALLIBRATE_YAW, CALLIBRATE_PITCH
	}
	
	public enum Actuator {
		LEFT_MOTOR, RIGHT_MOTOR, LEFT_RUDDER, RIGHT_RUDDER, TOP_RUDDER
	}
	
	public enum Command {
		SENSOR, ACTUATOR, STEERING, AUTO
	}

}
