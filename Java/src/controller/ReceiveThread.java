package controller;

import java.io.DataInputStream;
import java.io.EOFException;
import java.io.IOException;

import controller.data.MeasDevice;
import controller.data.object.ConnectionData;
import controller.data.parameter.MeasRPI;
import controller.data.parameter.Parameter;

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
				receiveMeas();
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
	
	public void receiveMeas() throws IOException, EOFException {
		//get to the next SYNC-mark in the stream
		int lastByte = input.readUnsignedByte();
		int b = input.readUnsignedByte();
		while((lastByte << 8 | b) != Connection.SYNC) {
			lastByte = b;
			b = input.readUnsignedByte();
		}
		//get the device and param of the next packet
		MeasDevice device;
		Enum<? extends Parameter> param;
		try {
			device = MeasDevice.values()[input.readUnsignedByte()];
			param = device.getParameters()[input.readUnsignedByte()];
		} catch(ArrayIndexOutOfBoundsException e) {
			throw new IOException("packet error: invalid header");
		}
		receiveValues(device, param);
	}
	
	private void receiveValues(MeasDevice device, Enum<? extends Parameter> param) throws IOException {
		Pool<MeasDevice> pool = connection.getReceivePool();
		//check if echo reply, then special
		if(device == MeasDevice.RPI && param == MeasRPI.ECHO_REPLY) {
			lastEchoReply = System.currentTimeMillis();
			echoTime = (int) (lastEchoReply - input.readLong());
			pool.setValue(device, param, new ConnectionData.Long(echoTime));
		} else {
			ConnectionData data = ((Parameter) param).getDataInstance();
			data.receive(input);
			pool.setValue(device, param, data);
		}
	}

}
