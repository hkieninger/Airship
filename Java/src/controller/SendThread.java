package controller;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

import controller.data.ConfDevice;
import controller.data.object.ConnectionData;
import controller.data.parameter.ConfRPI;
import controller.data.parameter.Parameter;

public class SendThread extends Thread implements Pool.Listener<ConfDevice> {
	
	static final int SEND_ECHO_DELAY = 1000; //in ms
	public static final long CONNECTION_LOST_TIME = SEND_ECHO_DELAY * 10;
	
	private boolean isRunning;
	private Connection connection;
	private DataOutputStream output;
	
	private long lastEchoRequest;
	
	private Set<Change> changes;
	
	public SendThread(Connection connection) throws IOException {
		setName("Send Thread");
		this.connection = connection;
		changes = new HashSet<>();
		isRunning = true;
		output = new DataOutputStream(connection.getSocket().getOutputStream());
		lastEchoRequest = 0;
		connection.getSendPool().addListener(this);
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
					if(isRunning)
						sendConf();
				}
			}
		} catch(IOException | Connection.LostException e) {
			connection.getRecvThead().setStopCause(e);
		} finally {
			connection.getSendPool().removeListener(this);
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
	
	public boolean isRunning() {
		return isRunning;
	}
	
	/*
	 * Helper methods
	 */
	
	synchronized private void sendConf() throws IOException {
		for(Change change : changes) {
			sendSingleConf(change.device, change.param);
		}
		changes.clear();
	}
	
	private void sendSingleConf(ConfDevice dev, Enum<? extends Parameter> param) throws IOException {
		Pool<ConfDevice> pool = connection.getSendPool();
		ConnectionData data = pool.getValue(dev, param);
		sendData(dev, param, data);
	}

	private void sendEchoRequest() throws IOException {
		ConnectionData.Long cl = new ConnectionData.Long(System.currentTimeMillis());
		sendData(ConfDevice.RPI, ConfRPI.ECHO_REQUEST, cl);
	}
	
	private void sendData(ConfDevice device, Enum<? extends Parameter> param, ConnectionData data) throws IOException {
		synchronized(output) {
			output.writeShort(Connection.SYNC);
			output.writeByte(device.ordinal());
			output.writeByte(param.ordinal());
			data.send(output);
		}
	}

	@Override
	synchronized public void onChanged(Pool<ConfDevice> pool, ConfDevice device, Enum<? extends Parameter> parameter) {
		changes.add(new Change(device, parameter));
		interrupt();
	}
	
	private static class Change {
		
		ConfDevice device;
		Enum<? extends Parameter> param;
		
		public Change(ConfDevice device, Enum<? extends Parameter> param) {
			this.device = device;
			this.param = param;
		}
		
		@Override
		public boolean equals(Object obj) {
			return device.hashCode() == obj.hashCode();
		}
		
		@Override
		public int hashCode() {
			return device.ordinal() << 8 | param.ordinal();
		}
	}
	
}
