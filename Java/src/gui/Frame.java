package gui;

import java.awt.GridLayout;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.SwingUtilities;

import controller.Controller;
import controller.ControllerListener;
import controller.data.parameter.MeasSensor;

public class Frame extends JFrame implements WindowListener, ControllerListener {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	public static final String DEFAULT_IP = "172.17.72.204";//"192.168.4.1";
	
	private Controller controller;
	private StatusPanel statusPanel;

	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				Frame frame = null;
				try {
					frame = new Frame();
				} catch (IOException e) {
					e.printStackTrace();
					System.exit(1);
				}
				frame.setVisible(true);
			}
		});
	}
	
	public Frame() throws UnknownHostException, IOException {
		super("Control Tower");
		//window settings
		setExtendedState(JFrame.MAXIMIZED_BOTH);
		setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		addWindowListener(this);
		setLayout(new GridLayout(2, 2));
		
		controller = new Controller(InetAddress.getByName(DEFAULT_IP));
		
		//controll panels
		JTabbedPane tabs = new JTabbedPane();
		tabs.setFocusable(false);
		ActuatorPanel actuatorPanel = new ActuatorPanel(controller.getConfPool());
		JScrollPane scrollPane = new JScrollPane(actuatorPanel);
		tabs.addTab("actuator", scrollPane);
		SteeringPanel steeringPanel = new SteeringPanel(controller.getConfPool());
		tabs.addTab("steering", steeringPanel);
		AutoPanel autoPanel = new AutoPanel();
		tabs.addTab("autopilot", autoPanel);
		add(tabs);
		
		//Status panels
		StatusPanel statusPanel = new StatusPanel();
		controller.getMeasPool().addListener(statusPanel);
		add(statusPanel);
		statusPanel.setInformation("Connection established.", 0x00FF00);
		
		//Video panels
		tabs = new JTabbedPane();
		tabs.setFocusable(false);
		VideoPanel bottomVideoPanel = new VideoPanel(MeasSensor.CAM_BOTTOM);
		controller.getMeasPool().addListener(bottomVideoPanel);
		tabs.addTab("camera bottom", bottomVideoPanel);
		VideoPanel frontVideoPanel = new VideoPanel(MeasSensor.CAM_FRONT);
		controller.getMeasPool().addListener(frontVideoPanel);
		tabs.addTab("camera front", frontVideoPanel);
		tabs.addChangeListener((event) -> {
			System.out.println("Changed");
		});
		add(tabs);
		
		//Sensor panels
		tabs = new JTabbedPane();
		SensorPanel sensorPanel = new SensorPanel();
		controller.getMeasPool().addListener(sensorPanel);
		tabs.addTab("sensor data", sensorPanel);
		MapPanel mapPanel = new MapPanel();
		controller.getMeasPool().addListener(mapPanel);
		tabs.addTab("map", mapPanel);
		add(tabs);
	}

	@Override
	public void windowOpened(WindowEvent arg0) {}

	@Override
	public void windowClosed(WindowEvent arg0) {
		//cleanup code here
		controller.close();
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
		restoreOptionDialog("The following error has occured: " + e.getMessage());
	}

	@Override
	public void onConnectionClosedByHost() {
		restoreOptionDialog("The connection has been closed by the host.");
	}

	@Override
	public void onConnectionLost() {
		JOptionPane.showMessageDialog(this, "The connection is lost.", "Connection", JOptionPane.INFORMATION_MESSAGE);
	}

	@Override
	public void onConnectionRestored() {
		JOptionPane.showMessageDialog(this, "The connection is restored.", "Connection", JOptionPane.INFORMATION_MESSAGE);
	}
	
	private void restoreOptionDialog(String message) {
		Object[] options = { "Restart Controller", "Exit Programm" };
		int option = JOptionPane.showOptionDialog(this, message +" Which action do you want to perform?", 
				"Restart", JOptionPane.DEFAULT_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[0]);
		if(option == 0) {
			try {
				controller.restore();
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
