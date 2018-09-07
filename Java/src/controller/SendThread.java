package controller;

import java.io.DataOutputStream;
import java.io.IOException;

import controller.objects.ConfDevice;
import controller.objects.ConfRPI;
import controller.objects.ConnectionData;
import controller.objects.Parameter;

public class SendThread extends Thread implements Pool.Listener<ConfDevice>{
	
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
				for(Enum<? extends Parameter> param : dev.getParameter().getEnumConstants()) {
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
		//check if parameter has changed
		if(pool.hasChanged(dev, param)) {
			pool.resetChanged(dev, param);
			Class<?> dataType = ((Parameter) param).getDataType();
			//check if primitive datatype, if case send with local method, add further primitve datatypes if needed
			if(dataType == Boolean.class) {
				boolean value = pool.getValue(dev, param, Boolean.class);
				sendBooleanValue(dev, param, value);
			} else if(dataType == Byte.class) {
				byte value = pool.getValue(dev, param, Byte.class);
				sendByteValue(dev, param, value);
			} else if(dataType == Long.class) {
				long value = pool.getValue(dev, param, Long.class);
				sendLongValue(dev, param, value);
			} else if(dataType.isAssignableFrom(ConnectionData.class)) {
				//if complex datatype, send with method from interface
				ConnectionData cObj = pool.getValue(dev, param, ConnectionData.class);
				synchronized(output) {
					sendHeader(dev, param);
					cObj.send(output);
				}
			} else {
				//should never be reached
				throw new IllegalArgumentException();
			}
		}
	}

	private void sendEchoRequest() throws IOException {
		sendLongValue(ConfDevice.RPI, ConfRPI.ECHO_REQUEST, System.currentTimeMillis());
	}
	
	private void sendHeader(ConfDevice device, Enum<? extends Parameter> param) throws IOException {
		synchronized(output) {
			output.writeShort(Connection.SYNC);
			output.writeByte(device.ordinal());
			output.writeByte(param.ordinal());
		}
	}
	
	private void sendBooleanValue(ConfDevice device, Enum<? extends Parameter> param, boolean value) throws IOException {
		synchronized(output) {
			sendHeader(device, param);
			output.writeByte(value ? 1 : 0);
		}
	}
	
	private void sendByteValue(ConfDevice device, Enum<? extends Parameter> param, byte value) throws IOException {
		synchronized(output) {
			sendHeader(device, param);
			output.writeByte(value);
		}
	}
	
	/*
	 * the long value is sent in big endian order
	 */
	private void sendLongValue(ConfDevice device, Enum<? extends Parameter> param, long value) throws IOException {
		synchronized(output) {
			sendHeader(device, param);
			output.writeLong(value);
		}
	}

	@Override
	synchronized public void onChanged(Pool<ConfDevice> pool, ConfDevice device, Enum<? extends Parameter> parameter) {
		this.device = device;
		this.param = parameter;
		interrupt();
	}
	
}
