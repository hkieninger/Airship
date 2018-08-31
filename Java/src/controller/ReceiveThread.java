package controller;

import java.io.DataInputStream;
import java.io.EOFException;
import java.io.IOException;

public class ReceiveThread extends Thread {
	
	private boolean isRunning;
	private Controller controller;
	private DataInputStream input;
	
	private volatile long echoTime; //in ms, duration of last echo round trip
	private volatile long lastEchoReply; //in ms, timestamp when last echo reply arrived
	
	public ReceiveThread(Controller controller) throws IOException {
		this.controller = controller;
		isRunning = true;
		input = new DataInputStream(controller.getInputStream());
		lastEchoReply = Long.MAX_VALUE;
	}
	
	@Override
	public void run() {
		try {
			while(isRunning) {
				//get to the next SYNC-mark in the stream
				int lastByte = input.readUnsignedByte();
				int b = input.readUnsignedByte();
				while((lastByte << 8 | b) != Controller.SYNC) {
					lastByte = b;
					b = input.readUnsignedByte();
				}
				//get the device and param of the next packet
				byte device = input.readByte();
				byte param = input.readByte();
				//read and process the data of the packet
				switch(device) {
					case Controller.D_RASPBERRY: 
						receiveNetworkPaket(param); 
						break;
					default: 
						IOException e = new IOException("invalid device in packet header");
						controller.getStatusView().information("The following error has occured: " + e.getMessage(), 0xFF0000);
						controller.getListener().onError(e);
				}
			}
		} catch (EOFException e) {
			controller.launchStop();
			controller.getStatusView().information("The connection has been closed by the remote host.", 0xFF8000);
			controller.getStatusView().information("The controller will be stopped.", 0xFF8000);
			controller.getListener().onConnectionClosedByHost();
		} catch (IOException e) {
			e.printStackTrace();
			controller.launchStop();
			controller.getStatusView().information("The following error has occured: " + e.getMessage(), 0xFF0000);
			controller.getStatusView().information("The controller will be stopped.", 0xFF0000);
			controller.getListener().onError(e);
		}
		//wait for the other thread to terminate to call terminateStop()
		try {
			controller.getSendThread().join();
			controller.terminateStop();
		} catch (InterruptedException | IOException e) {
			e.printStackTrace();
		}
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
	
	private void receiveNetworkPaket(byte param) throws IOException {
		switch(param) {
			case Controller.P_ECHO_REQUEST: 
				controller.getSendThread().sendEchoReply(input.readLong());
				break;
			case Controller.P_ECHO_REPLY: 
				lastEchoReply = System.currentTimeMillis();
				echoTime = lastEchoReply - input.readLong();
				controller.getStatusView().setNetworkPing(echoTime);
				break;
			default: throw new IllegalArgumentException("unknown parameter for network device");
		}
	}

}
