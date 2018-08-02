package controller;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;

public class Controller extends Thread implements ActuatorController.Listener {
	
	public static final String DEFAULT_IP = "192.168.4.1";
	public static final int DEFAULT_PORT = 0xCCCC;
	
	//Device IDs
	public static final int D_LEFT_MOTOR = 1;
	public static final int D_RIGHT_MOTOR = 2;
	
	public static final int D_LEFT_RUDDER = 3;
	public static final int D_RIGHT_RUDDER = 4;
	public static final int D_TOP_RUDDER = 5;
	
	//Parameter IDs
	public static final int P_THRUST = 1;
	public static final int P_ANGLE = 2;
	
	
	private Socket socket;
	private DataInputStream input;
	private DataOutputStream output;
	private ActuatorController actuatorController;
	
	private boolean[] flags; //controll settings have changed?
	
	public Controller(ActuatorController actuatorController) throws IOException {
		this(InetAddress.getByName(DEFAULT_IP), DEFAULT_PORT, actuatorController);
	}
	
	public Controller(InetAddress host, int port, ActuatorController actuatorController) throws IOException {
		this.actuatorController = actuatorController;
		socket = new Socket(host, port);
		input = new DataInputStream(socket.getInputStream());
		output = new DataOutputStream(socket.getOutputStream());
		actuatorController.setListener(this);
	}
	
	@Override
	public void run() {
		//send/read packets to check whether the zeppelin is still alive (reachable)
	}

	@Override
	public void onLeftMotorChanged(int thrust) {
		
	}

	@Override
	public void onRightMotorChanged(int thrust) {
		
	}

	@Override
	public void onLeftRudderChanged(int angle) {
		
	}

	@Override
	public void onRightRudderChanged(int angle) {
		
	}

	@Override
	public void onTopRudderChanged(int angle) {
		
	}

}
