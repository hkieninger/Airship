package controller;

import java.io.DataOutputStream;
import java.io.IOException;

import controller.CommandPool.Command;

public class SendThread extends Thread {
	
	static final int SEND_ECHO_DELAY = 1000; //in ms
	public static final long CONNECTION_LOST_TIME = SEND_ECHO_DELAY * 10;
	
	private boolean isRunning;
	private Connection connection;
	private DataOutputStream output;
	
	private long lastEchoRequest;
	
	public SendThread(Connection connection) throws IOException {
		this.connection = connection;
		isRunning = true;
		output = new DataOutputStream(connection.getSocket().getOutputStream());
		lastEchoRequest = 0;
	}

	@Override
	public void run() {
		try {
			while(isRunning) {
				try {
					//sleep a while
					long delay = SEND_ECHO_DELAY - (System.currentTimeMillis() - lastEchoRequest);
					if(delay > 0)
						Thread.sleep(delay);
					//send a roundtrip
					sendEchoRequest();
					//check if the connection is broken
					if(System.currentTimeMillis() - connection.getRecvThead().getLastEchoReply() > CONNECTION_LOST_TIME)
						throw new Connection.LostException();
					//set the time of the last echo request
					lastEchoRequest = System.currentTimeMillis();
				} catch(InterruptedException e) {
					sendCommands();
				}
			}
		} catch(IOException | Connection.LostException e) {
			connection.getRecvThead().setStopCause(e);
		} finally {
			//set the isRunning variable to false
			stopRunning();
			//stop the receive thread
			connection.getRecvThead().stopRunning();
			// close the socket: causes the receive thread to stop read call, because inputStream throws an IOException in the receive thread
			try {
				connection.getSocket().close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	public void stopRunning() {
		isRunning = false;
	}
	
	/*
	 * Helper methods
	 */
	
	private void sendCommands() throws IOException {
		CommandPool pool = connection.getCommandPool();
		boolean[] changed = pool.getChanged();
		
		if(changed[Command.DATA.ordinal()]) {
			sendDataChanges(pool);
		}
		
		if(changed[Command.ACTUATOR.ordinal()]) {
			sendActuatorChanges(pool);
		}
		
		if(changed[Command.STEERING.ordinal()]) {
			sendSteeringChanges(pool);
		}
		
		if(changed[Command.AUTO.ordinal()]) {
			sendAutoChanges(pool);
		}
	}
	
	private void sendAutoChanges(CommandPool pool) {
		// TODO Auto-generated method stub
		
	}

	private void sendSteeringChanges(CommandPool pool) {
		// TODO Auto-generated method stub
		
	}

	private void sendActuatorChanges(CommandPool pool) {
		// TODO Auto-generated method stub
		
	}

	private void sendDataChanges(CommandPool pool) {
		boolean[] changed = pool.getDataChanged();
		for(int i = 0; i < changed.length; i++) {
			if(changed[i])
				
		}
	}

	void sendEchoRequest() throws IOException {
		sendLongValue(Device.RASPBERRY, P_Raspberry.ECHO_REQUEST, System.currentTimeMillis());
	}
	
	void sendEchoReply(long timestamp) throws IOException {
		sendLongValue(Device.RASPBERRY, P_Raspberry.ECHO_REPLY, timestamp);
	}
	
	private void sendHeader(Device device, Enum<?> param) throws IOException {
		synchronized(output) {
			output.writeShort(Connection.SYNC);
			output.writeByte(device.ordinal());
			output.writeByte(param.ordinal());
		}
	}
	
	/*
	 * the long value is sent in big endian order
	 */
	private void sendLongValue(Device device, Enum<?> param, long value) throws IOException {
		synchronized(output) {
			sendHeader(device, param);
			output.writeLong(value);
		}
	}
	
	private void sendBooleanValue(Device device, Enum<?> param, boolean value) throws IOException {
		synchronized(output) {
			sendHeader(device, param);
			output.writeBoolean(value);
		}
	}
	
	/*
	 * the int value is sent in big endian order
	 */
	private void sendIntValue(Device device, Enum<?> param, int value) throws IOException {
		synchronized(output) {
			sendHeader(device, param);
			output.writeInt(value);
		}
	}
	
}
