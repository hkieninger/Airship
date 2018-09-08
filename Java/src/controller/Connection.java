package controller;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.ArrayList;
import java.util.List;

import controller.data.ConfDevice;
import controller.data.MeasDevice;

public class Connection {
	
	private static final int CONNECT_TIMEOUT = 3000;
	
	public static final int SYNC = 0xABCD; //unsigned short, DataOutputStream sends it big endian
	
	private Socket socket;
	private SendThread sendThread;
	private ReceiveThread recvThread;
	
	private Pool<MeasDevice> receivePool;
	private Pool<ConfDevice> sendPool;
	
	private List<Listener> listeners;
	
	public Connection(InetAddress host, int port, Pool<MeasDevice> receivePool, Pool<ConfDevice> sendPool) throws IOException {
		this.sendPool = sendPool;
		this.receivePool = receivePool;
		listeners = new ArrayList<>();
		while(socket == null) {
			socket = new Socket();
			try {
				socket.connect(new InetSocketAddress(host, port), CONNECT_TIMEOUT);
			} catch (SocketTimeoutException e) {
				socket = null;
				//System.err.println("Socket: connect to " + host.getHostName() + " at port " + port + "timed out. Retrying ..."  );
			}
		}
		sendThread = new SendThread(this);
		recvThread = new ReceiveThread(this);
	}
	
	/*
	 * methods to be started from the outside
	 */
	
	public void start() {
		sendThread.start();
		recvThread.start();
	}
	
	public void stop() {
		sendThread.stopRunning();
		sendThread.interrupt();
		recvThread.stopRunning();
	}
	
	public void addListener(Listener l) {
		listeners.add(l);
	}
	
	public void removeListener(Listener l) {
		listeners.remove(l);
	}
	
	/*
	 * methods with access modifier package for the SendThread and ReceiveThread
	 */
	
	void notifyListeners(Exception stopCause) {
		for(Listener l : listeners) {
			l.onStopped(stopCause);
		}
	}
	
	Socket getSocket() {
		return socket;
	}
	
	SendThread getSendThread() {
		return sendThread;
	}
	
	ReceiveThread getRecvThead() {
		return recvThread;
	}
	
	Pool<MeasDevice> getReceivePool() {
		return receivePool;
	}
	
	Pool<ConfDevice> getSendPool() {
		return sendPool;
	}
	
	/*
	 * interfaces and classes linked to the Connection class
	 */
	
	public static interface Listener {
		void onStopped(Exception cause);
	}
	
	public static class ClosedByHostException extends Exception {private static final long serialVersionUID = 1L;}
	public static class LostException extends Exception {private static final long serialVersionUID = 1L;}

}
