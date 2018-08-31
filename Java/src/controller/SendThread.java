package controller;

import java.io.DataOutputStream;
import java.io.IOException;

public class SendThread extends Thread {
	
	static final int SEND_ECHO_DELAY = 1000; //in ms
	public static final long CONNECTION_LOST_TIME = SEND_ECHO_DELAY * 10;
	private static final int THREAD_DELAY = 50; //in ms
	
	private boolean isRunning;
	private boolean weakConnection;
	private Controller controller;
	private DataOutputStream output;
	
	private boolean[] deviceChanged; //flags which say if a setting has changed and need to be sent
	private int[] actuatorValues; //settings of the actuators
	
	private long lastMillis;
	
	public SendThread(Controller controller) throws IOException {
		this.controller = controller;
		deviceChanged = new boolean[Controller.D_TOP_RUDDER + 1];
		for(int i = 0; i < deviceChanged.length; i++) //all default values will be sent at the beginning
			deviceChanged[i] = true;
		actuatorValues = new int[Controller.D_TOP_RUDDER - Controller.D_LEFT_MOTOR + 1]; //default values are all 0s
		isRunning = true;
		output = new DataOutputStream(controller.getOutputStream());
	}

	@Override
	public void run() {
		try {
			while(isRunning) {
				//sleep some ms
				try {
					Thread.sleep(THREAD_DELAY);
				} catch (InterruptedException e) {}
				//send the changes in configuration
				sendChanges();
				//ping the server periodically to check the network quality
				if(System.currentTimeMillis() - lastMillis > SEND_ECHO_DELAY) {
						sendEchoRequest();
						lastMillis = System.currentTimeMillis();
				}
				//check if the pings came back
				if(System.currentTimeMillis() - controller.getReceiveThread().getLastEchoReply() > CONNECTION_LOST_TIME) {
					if(!weakConnection) {
						controller.getStatusView().information(
								"The connection has been lost (ping > " + CONNECTION_LOST_TIME / 1000 + " s).", 0xFF8000);
						controller.getListener().onConnectionLost();
						weakConnection = true;
					}
				} else {
					if(weakConnection) {
						controller.getStatusView().information(
								"The connection has been restored (ping < " + CONNECTION_LOST_TIME / 1000 + " s).", 0x00FF80);
						controller.getListener().onConnectionRestored();
						weakConnection = false;
					}
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
			controller.launchStop();
			controller.getStatusView().information("The following error has occured: " + e.getMessage(), 0xFF0000);
			controller.getStatusView().information("The controller will be stopped.", 0xFF0000);
			controller.getListener().onError(e);
		}
	}
	
	public void stopRunning() {
		isRunning = false;
	}
	
	public boolean weakConnection() {
		return weakConnection;
	}
	
	synchronized public void actuatorChanged(byte device, int value) {
		deviceChanged[device] = true;
		actuatorValues[device - Controller.D_LEFT_MOTOR] = value;
	}
	
	/*
	 * Helper methods
	 */
	
	private void sendChanges() throws IOException {
		for(byte device = 0; device < deviceChanged.length; device++) {
			boolean changed = false;
			int value = 0;
			
			synchronized (this) {
				changed = deviceChanged[device];
				deviceChanged[device] = false; //reset flag
				if(device >= Controller.D_LEFT_MOTOR && device <= Controller.D_TOP_RUDDER)
					value = actuatorValues[device - Controller.D_LEFT_MOTOR];
			}
			
			if(changed) {
				if(device >= Controller.D_LEFT_MOTOR && device <= Controller.D_TOP_RUDDER)
					sendActuatorConfig(device, value);
			}
		}
	}
	
	private void sendActuatorConfig(byte device, int value) throws IOException {
		sendIntValue(device, (byte) 0, value);
	}
	
	void sendEchoRequest() throws IOException {
		sendLongValue(Controller.D_RASPBERRY, Controller.P_ECHO_REQUEST, System.currentTimeMillis());
	}
	
	void sendEchoReply(long timestamp) throws IOException {
		sendLongValue(Controller.D_RASPBERRY, Controller.P_ECHO_REPLY, timestamp);
	}
	
	/*
	 * the long value is sent in big endian order
	 */
	private void sendLongValue(byte device, byte param, long value) throws IOException {
		synchronized(output) {
			output.writeShort(Controller.SYNC);
			output.writeByte(device);
			output.writeByte(param);
			output.writeLong(value);
		}
	}
	
	/*
	 * the int value is sent in big endian order
	 */
	private void sendIntValue(byte device, byte param, int value) throws IOException {
		synchronized(output) {
			output.writeShort(Controller.SYNC);
			output.writeByte(device);
			output.writeByte(param);
			output.writeInt(value);
		}
	}
	
}
