package controller;

import java.io.DataInputStream;
import java.io.EOFException;
import java.io.IOException;

import controller.DataPool.Data;

public class ReceiveThread extends Thread {
	
	private boolean isRunning;
	private Connection connection;
	private DataInputStream input;
	
	private Exception stopCause;
	
	private volatile int echoTime; //in ms, duration of last echo round trip
	private volatile long lastEchoReply; //in ms, timestamp when last echo reply arrived
	
	public ReceiveThread(Connection connection) throws IOException {
		this.connection = connection;
		isRunning = true;
		input = new DataInputStream(connection.getSocket().getInputStream());
		lastEchoReply = Long.MAX_VALUE;
	}
	
	@Override
	public void run() {
		try {
			while(isRunning)
				receiveData();
		} catch(EOFException e) {
			setStopCause(new Connection.ClosedByHostException());
		} catch(IOException | UnsupportedOperationException e) {
			setStopCause(e);
		} finally {
			//set the isRunning variable
			stopRunning();
			//stop the send thread
			connection.getSendThread().stopRunning();
			connection.getSendThread().interrupt();
			//wait for the send thread to stop
			try {
				connection.getSendThread().join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			//notify the listeners
			connection.notifyListeners(stopCause);
		}
	}
	
	synchronized public void setStopCause(Exception cause) {
		if(stopCause == null)
			stopCause = cause;
	}

	public void stopRunning() {
		isRunning = false;
	}
	
	public long getEchoTime() {
		return echoTime;
	}
	
	public long getLastEchoReply() {
		return lastEchoReply;
	}
	
	public void receiveData() throws IOException, EOFException {
		//get to the next SYNC-mark in the stream
		int lastByte = input.readUnsignedByte();
		int b = input.readUnsignedByte();
		while((lastByte << 8 | b) != Connection.SYNC) {
			lastByte = b;
			b = input.readUnsignedByte();
		}
		//get the device and param of the next packet
		Device device;
		try {
			device = Device.values()[input.readUnsignedByte()];
		} catch(ArrayIndexOutOfBoundsException e) {
			throw new IOException("packet error: invalid device");
		}
		int param = input.readUnsignedByte();
		//read and process the data of the packet
		try {
			switch(device) {
				case RASPBERRY: handleRaspberry(P_Raspberry.values()[param]); break;
				case BATTERY: handleBattery(P_Battery.values()[param]); break;
				case ACTUATOR: handleActuator(P_Actuator.values()[param]); break;
				case STEERING: handleSteering(P_Steering.values()[param]); break;
				case SENSOR: handleSensor(P_Sensor.values()[param]); break;
				default: throwUnsupportedOperationException(device);
			}
		} catch (ArrayIndexOutOfBoundsException e) {
			throw new IOException("packet error: device " + device.getClass().getSimpleName() + ": invalid parameter");
		}
	}
	
	private void handleSensor(P_Sensor param) {
		throwUnsupportedOperationException(param);
	}

	private void handleSteering(P_Steering param) {
		throwUnsupportedOperationException(param);
	}

	private void handleBattery(P_Battery param) {
		throwUnsupportedOperationException(param);
	}

	private void handleActuator(P_Actuator param) {
		throwUnsupportedOperationException(param);
	}

	private void handleRaspberry(P_Raspberry param) throws IOException {
		switch(param) {
			case ECHO_REPLY: 
				lastEchoReply = System.currentTimeMillis();
				echoTime = (int) (lastEchoReply - input.readLong());
				connection.getDataPool().setPing(echoTime);
				connection.getDataPool().notifyListeners(Data.NETWORK);
				break;
			case ECHO_REQUEST: 
				connection.getSendThread().sendEchoReply(input.readLong());
				break;
			default: 
				throwUnsupportedOperationException(param);
		}
	}
	
	private void throwUnsupportedOperationException(Enum<?> opperation) {
		throw new UnsupportedOperationException("handling of \"" + opperation.getClass().getSimpleName() + "\" is not implemented yet.");
	}

}
