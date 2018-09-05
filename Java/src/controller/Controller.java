package controller;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.util.List;

public class Controller {
	
	
	//Socket for the communication with the airship
	private Socket socket;
	//Listener for asynchronous communication with local objects
	private ControllerListener listener;
	

	
	//the threads sending and receiving the data via the socket
	private SendThread sendThread;
	private ReceiveThread recvThread;
	
	/**
	 * creates a controller connected to the airship with the default ip on the default port
	 * @param listener
	 * @param statusView
	 * @param actuatorController
	 * @throws IOException
	 */
	public Controller(ControllerListener listener, StatusView statusView,ActuatorController actuatorController) throws IOException {
		this(listener, InetAddress.getByName(DEFAULT_IP), DEFAULT_PORT, statusView, actuatorController);
	}
	
	/**
	 * creates a controller connected to the airship
	 * @param listener
	 * @param host
	 * @param port
	 * @param statusView
	 * @param actuatorController
	 * @throws IOException
	 */
	public Controller(ControllerListener listener, InetAddress host, int port, StatusView statusView, ActuatorController actuatorController) throws IOException {
		this.listener = listener;
		this.statusView = statusView;
		this.actuatorController = actuatorController;
		socket = new Socket(host, port);
		sendThread = new SendThread(this);
		recvThread = new ReceiveThread(this);
		actuatorController.setListener(this);
	}
	
	/**
	 * starts the controller (send and receive thread)
	 */
	public void start() {
		recvThread.start();
		sendThread.start();
		statusView.information("Controller has started.", StatusView.WHITE);
		listener.onStarted();
	}
	
	/**
	 * triggers the threads to stop, but does not wait until they have stopped
	 */
	public void launchStop() {
		actuatorController.setListener(null);
		steeringController.setListener(null);
		sendThread.stopRunning();
		recvThread.stopRunning();
	}
	
	/**
	 * @return true if the controller has stopped
	 */
	public boolean hasStopped() {
		return socket.isClosed();
	}
	
	/*
	 * getter and methods for the receive and send threads to access data structures of the controller
	 */

	void terminateStop() throws IOException {
		socket.close();
		statusView.information("Controller has stopped.", StatusView.WHITE);
		listener.onStopped();
	}
	
	InputStream getInputStream() throws IOException {
		return socket.getInputStream();
	}
	
	OutputStream getOutputStream() throws IOException {
		return socket.getOutputStream();
	}
	
	SendThread getSendThread() {
		return sendThread;
	}
	
	ReceiveThread getReceiveThread() {
		return recvThread;
	}
	
	ControllerListener getListener() {
		return listener;
	}

}
