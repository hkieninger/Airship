package gui;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import controller.Controller;
import controller.ControllerListener;
import controller.data.ConfDevice;
import controller.data.object.ConnectionData.UByte;
import controller.data.parameter.ConfSensor;
import controller.video.H264Connection;
import controller.video.JPGConnection;
import gui.Menu.MenuListener;
import gui.quarter.ControllQuarter;
import gui.quarter.SensorQuarter;
import gui.quarter.StatusQuarter;
import gui.quarter.VideoQuarter;

public class Frame extends JFrame implements WindowListener, ControllerListener {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	public static final String DEFAULT_IP = "172.17.72.204";//"192.168.0.27", "172.17.72.204", "192.168.4.1";
	public static final int FRONT_CAM_PORT = 0xCCCE;
	public static final int BOTTOM_CAM_PORT = 0xCCCD;
	
	private InetAddress ip;
	
	private Controller controller;
	private H264Connection bottomCamera;
	private JPGConnection frontCamera;

	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				Frame frame = new Frame();
				frame.setVisible(true);
			}
		});
	}
	
	public Frame() {
		super("Control Tower");
		//window settings
		setExtendedState(JFrame.MAXIMIZED_BOTH);
		setMinimumSize(new Dimension(480, 240));
		setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		addWindowListener(this);
		setLayout(new GridLayout(2, 2));
		
		try {
			ip = InetAddress.getByName(DEFAULT_IP);
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
		controller = new Controller();
		
		frontCamera = new JPGConnection();
		bottomCamera = new H264Connection();
		
		//controll panels
		add(new ControllQuarter(controller, frontCamera));
		//Status panels
		add(new StatusQuarter(controller));
		//Video panels
		add(new VideoQuarter(controller, frontCamera, bottomCamera));
		//Sensor panels
		add(new SensorQuarter(controller));
		
		controller.addListener(this);
		
		setJMenuBar(new Menu(new MenuListener() {
			
			@Override
			public void onConnect() {
				if(!controller.isConnected()) {
					try {
						ip = InetAddress.getByName(JOptionPane.showInputDialog("Enter the IP of the Airship: ", ip.getHostAddress()));
						Thread thread = new Thread(() -> {
							try {
								controller.connect(ip);
								frontCamera.connect(ip, FRONT_CAM_PORT);
								bottomCamera.connect(ip, BOTTOM_CAM_PORT);
								frontCamera.start();
								bottomCamera.start();
							} catch (IOException e) {
								e.printStackTrace();
								JOptionPane.showMessageDialog(null, "connect failed: " + e.getMessage(), e.getClass().getSimpleName(), JOptionPane.ERROR_MESSAGE);
								System.exit(1);
							}
						});
						thread.start();
					} catch (UnknownHostException e) {
						JOptionPane.showMessageDialog(null, "invalid IP: " + e.getMessage(), e.getClass().getSimpleName(), JOptionPane.ERROR_MESSAGE);
					}
				} else {
					JOptionPane.showMessageDialog(null, "Already connected.", "Info", JOptionPane.INFORMATION_MESSAGE);
				}
			}
			
			@Override
			public void onFront(boolean enabled) {
				if(enabled) {
					UByte enable = new UByte();
					enable.val = ConfSensor.ENABLE;
					controller.getConfPool().setValue(ConfDevice.SENSOR, ConfSensor.CAM_FRONT, enable);
				} else {
					UByte disable = new UByte();
					disable.val = ConfSensor.DISABLE;
					controller.getConfPool().setValue(ConfDevice.SENSOR, ConfSensor.CAM_FRONT, disable);
				}
			}
			
			@Override
			public void onBottom(boolean enabled) {
				if(enabled) {
					UByte enable = new UByte();
					enable.val = ConfSensor.ENABLE;
					controller.getConfPool().setValue(ConfDevice.SENSOR, ConfSensor.CAM_BOTTOM, enable);
				} else {
					UByte disable = new UByte();
					disable.val = ConfSensor.DISABLE;
					controller.getConfPool().setValue(ConfDevice.SENSOR, ConfSensor.CAM_BOTTOM, disable);
				}
			}
			
		}));
		
	}

	@Override
	public void windowOpened(WindowEvent arg0) {}

	@Override
	public void windowClosed(WindowEvent arg0) {
		//cleanup code here
		try {
			frontCamera.stopRunning();
			bottomCamera.stopRunning();
			controller.close();
		} catch (Exception e) {
			e.printStackTrace();
			System.exit(1);
		}
	}

	@Override
	public void windowClosing(WindowEvent arg0) {
		Object[] options = { "OK", "CANCEL" };
		int option = JOptionPane.showOptionDialog(this, "Do you really want to exit?", "bye bye", 
				JOptionPane.DEFAULT_OPTION, JOptionPane.WARNING_MESSAGE, null, options, options[0]);
		if(option == 0) {
			dispose();
		}
	}
	
	@Override
	public void windowActivated(WindowEvent arg0) {}
	@Override
	public void windowDeactivated(WindowEvent arg0) {}
	@Override
	public void windowDeiconified(WindowEvent arg0) {}
	@Override
	public void windowIconified(WindowEvent arg0) {}

	@Override
	public void onError(Exception e) {
		e.printStackTrace();
		restoreOptionDialog("The following error has occured: " + e.getMessage() + ".");
	}

	@Override
	public void onConnectionClosedByHost() {
		restoreOptionDialog("The connection has been closed by the host.");
	}

	@Override
	public void onConnectionLost() {
		JOptionPane.showMessageDialog(this, "The connection is lost.", "Connection", JOptionPane.WARNING_MESSAGE);
	}

	@Override
	public void onConnectionRestored() {
		//JOptionPane.showMessageDialog(this, "The connection is restored.", "Connection", JOptionPane.INFORMATION_MESSAGE);
	}
	
	private void restoreOptionDialog(String message) {
		Object[] options = { "Restore connection.", "Exit Programm" };
		int option = JOptionPane.showOptionDialog(this, message +" Which action do you want to perform?", 
				"Restart", JOptionPane.DEFAULT_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[0]);
		if(option == 0) {
			try {
				controller.connect(ip);
			} catch (IOException e) {
				e.printStackTrace();
				JOptionPane.showMessageDialog(this, "Restore failed with following error: " + e.getMessage(), e.getClass().getSimpleName(), JOptionPane.ERROR_MESSAGE);
				SwingUtilities.invokeLater(() -> dispose());
			}
		} else {
			SwingUtilities.invokeLater(() -> dispose());
		}
	}

}
