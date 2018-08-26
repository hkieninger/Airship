package controller;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;

import javax.swing.JOptionPane;

import controller.Notificator.Level;

public class Controller implements ActuatorController.Listener, SteeringController.Listener {
	
	public static final String DEFAULT_IP = "192.168.4.1";
	public static final int DEFAULT_PORT = 0xCCCC;
	
	public static final int SYNC = 0xABCD; //unsigned short, DataOutputStream sends it big endian
	
	/*
	 * Device IDs
	 */
	public static final byte D_RASPBERRY = 0;
	
	//Actuators
	public static final byte D_LEFT_MOTOR = 2;
	public static final byte D_RIGHT_MOTOR = 3;
	
	public static final byte D_LEFT_RUDDER = 4;
	public static final byte D_RIGHT_RUDDER = 5;
	public static final byte D_TOP_RUDDER = 6;
	
	//Software Actuators
	public static final byte D_SOFTWARE_CONTROLL = 13;
	
	//Sensors
	public static final byte D_BATTERY = 1;
	public static final byte D_NEO6M = 7;
	public static final byte D_HCSRO4_FRONT = 8;
	public static final byte D_HCSRO4_DOWN = 9;
	public static final byte D_BMP280 = 10;
	public static final byte D_QMC5883L = 11;
	public static final byte D_MPU6050 = 12;
	
	/*
	 * Parameter IDs
	 * if the most significant bit is set, its not setting the parameter but requesting it
	 */
	public static final byte P_ECHO_REQUEST = 0; //D_RASPBERRY
	public static final byte P_ECHO_REPLY = 1; //D_RASPBERRY
	public static final byte P_POWER_OFF = 2; //D_RASPBERRY
	public static final byte P_REBOOT = 3; //D_RASPBERRY
	
	//Actuators
	public static final byte P_THRUST = 0; //D_*_MOTOR
	public static final byte P_ANGLE = 0; //D_*_RUDDER
	
	//Software Actuators
	public static final byte P_VELOCITY = 0;
	public static final byte P_YAW = 1;
	public static final byte P_PITCH = 2;
	public static final byte P_CALLIBRATION = 3;
	
	//Sensors
	public static final byte P_PERCENT = 0; //D_BATTERY
	
	private Socket socket;
	private ActuatorController actuatorController;
	private SteeringController steeringController;
	private Notificator notificator;
	private SendThread sendThread;
	private ReceiveThread recvThread;
	
	public Controller(Notificator notificator, ActuatorController actuatorController, SteeringController steeringController) throws IOException {
		this(notificator, InetAddress.getByName(DEFAULT_IP), DEFAULT_PORT, actuatorController, steeringController);
	}
	
	public Controller(Notificator notificator, InetAddress host, int port, ActuatorController actuatorController, SteeringController steeringController) throws IOException {
		this.actuatorController = actuatorController;
		this.steeringController = steeringController;
		socket = new Socket(host, port);
		sendThread = new SendThread(this);
		recvThread = new ReceiveThread(this);
		this.actuatorController.setListener(this);
		this.steeringController.setListener(this);
	}
	
	public void start() {
		recvThread.start();
		sendThread.start();
		notificator.notificate("controller started", Level.CHATTERBOX);
	}
	
	public void stop() throws IOException, InterruptedException {
		recvThread.stopRunning();
		sendThread.stopRunning();
		recvThread.join();
		sendThread.join();
		socket.close();
		notificator.notificate("controller stopped", Level.CHATTERBOX);
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
	
	void onSendError(Exception e) {
		e.printStackTrace();
		notificator.notificate(e.getMessage(), Level.ERROR);
	}
	
	void onReceiveError(Exception e) {
		e.printStackTrace();
		notificator.notificate(e.getMessage(), Level.ERROR);
	}
	
	/*
	 * called when host closes the socket or the connection gets lost because signal is too weak
	 */
	void onConnectionLost(boolean closedByHost) {
		if(closedByHost) {
			notificator.notificate("Connection has been closed by host.", Level.IMPORTANT);
		} else {
			notificator.notificate("Connection has been lost.", Level.IMPORTANT);
		}
	}
	
	/*
	 * Callbacks of the Controller interfaces
	 */
	
	@Override
	public void onLeftMotorChanged(int thrust) {
		sendThread.actuatorChanged(D_LEFT_MOTOR, thrust);
	}

	@Override
	public void onRightMotorChanged(int thrust) {
		sendThread.actuatorChanged(D_RIGHT_MOTOR, thrust);
	}

	@Override
	public void onLeftRudderChanged(int angle) {
		sendThread.actuatorChanged(D_LEFT_RUDDER, angle);
	}

	@Override
	public void onRightRudderChanged(int angle) {
		sendThread.actuatorChanged(D_RIGHT_RUDDER, angle);
	}

	@Override
	public void onTopRudderChanged(int angle) {
		sendThread.actuatorChanged(D_TOP_RUDDER, angle);
	}

	@Override
	public void onVelocityChanged(int velocity) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onYawChanged(int yaw) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onPitchChanged(int pitch) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onCallibrated(int yaw, int pitch) {
		// TODO Auto-generated method stub
		
	}

}
