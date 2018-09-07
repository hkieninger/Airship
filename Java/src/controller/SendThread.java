package controller;

import java.io.DataOutputStream;
import java.io.IOException;

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
	
	private ConfDevice device;
	private Enum<? extends Parameter> param;
	
	public SendThread(Connection connection) throws IOException {
		this.connection = connection;
		connection.getSendPool().addListener(this);
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
	
	/*
	 * Helper methods
	 */
	
	synchronized private void sendConf() throws IOException {
		if(device == null || param == null) {
			//loop over all configuration parameters
			for(ConfDevice dev : ConfDevice.values()) {
				for(Enum<? extends Parameter> param : dev.getParameters()) {
					//check if parameter has changed
					if(connection.getSendPool().hasChanged(dev, param))
						sendSingleConf(dev, param);
				}
			}
		} else {
			sendSingleConf(device, param);
			device = null;
			param = null;
		}
	}
	
	private void sendSingleConf(ConfDevice dev, Enum<? extends Parameter> param) throws IOException {
		Pool<ConfDevice> pool = connection.getSendPool();
		ConnectionData data = pool.getValue(dev, param);
		sendData(dev, param, data);
		//reset changed flag
		pool.resetChanged(dev, param);
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
		if(device == null && parameter == null) {
			this.device = device;
			this.param = parameter;
		} else {
			this.device = null;
			this.param = null;
		}
		interrupt();
	}
	
}
