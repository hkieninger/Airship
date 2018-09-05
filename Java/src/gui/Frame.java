package gui;

import java.awt.GridLayout;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.IOException;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.SwingUtilities;

import controller.Controller;
import controller.ControllerListener;

public class Frame extends JFrame implements WindowListener, ControllerListener {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private Controller controller;
	
	private StatusPanel statusPanel;
	private VideoPanel bottomVideoPanel;
	private VideoPanel frontVideoPanel;
	private SensorPanel sensorPanel;
	
	private ActuatorPanel actuatorPanel;
	private SteeringPanel steeringPanel;
	private AutoPanel autoPanel;

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
		setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		addWindowListener(this);
		setLayout(new GridLayout(2, 2));
		
		//controll panels
		JTabbedPane tabs = new JTabbedPane();
		tabs.setFocusable(false);
		actuatorPanel = new ActuatorPanel();
		JScrollPane scrollPane = new JScrollPane(actuatorPanel);
		tabs.addTab("actuator", scrollPane);
		steeringPanel = new SteeringPanel();
		tabs.addTab("steering", steeringPanel);
		autoPanel = new AutoPanel();
		tabs.addTab("autopilot", autoPanel);
		add(tabs);
		
		//Status panels
		statusPanel = new StatusPanel();
		add(statusPanel);
		
		//Video panels
		tabs = new JTabbedPane();
		tabs.setFocusable(false);
		bottomVideoPanel = new VideoPanel();
		tabs.addTab("camera bottom", bottomVideoPanel);
		frontVideoPanel = new VideoPanel();
		tabs.addTab("camera front", frontVideoPanel);
		tabs.addChangeListener((event) -> {
			System.out.println("Changed");
		});
		add(tabs);
		
		//Sensor panels
		tabs = new JTabbedPane();
		sensorPanel = new SensorPanel();
		tabs.addTab("sensor data", sensorPanel);
		MapPanel mapPanel = new MapPanel();
		tabs.addTab("map", mapPanel);
		add(tabs);
		
		//create the controller
		/*try {
			controller = new Controller(this, statusPanel, actuatorPanel);
			controller.start();
		} catch (IOException e) {
			e.printStackTrace();
			JOptionPane.showMessageDialog(null, e.getMessage(), e.getClass().getSimpleName(), JOptionPane.ERROR_MESSAGE);
			System.exit(1);
		}*/
	}

	@Override
	public void windowOpened(WindowEvent arg0) {}

	@Override
	public void windowClosed(WindowEvent arg0) {
		//cleanup code here
		controller.launchStop();
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
		restartController("The following error has occured: " + e.getMessage());
	}

	@Override
	public void onStarted() {
		System.out.println("Controller has started.");
	}

	@Override
	public void onStopped() {
		System.out.println("Controller has stopped.");
	}

	@Override
	public void onConnectionClosedByHost() {
		restartController("The connection has been closed by the host.");
	}

	@Override
	public void onConnectionLost() {
		JOptionPane.showMessageDialog(this, "The connection is lost.", "Connection", JOptionPane.INFORMATION_MESSAGE);
	}

	@Override
	public void onConnectionRestored() {
		JOptionPane.showMessageDialog(this, "The connection is restored.", "Connection", JOptionPane.INFORMATION_MESSAGE);
	}
	
	private void restartController(String message) {
		Object[] options = { "Restart Controller", "Exit Programm" };
		int option = JOptionPane.showOptionDialog(this, message +" Which action do you want to perform?", 
				"Restart", JOptionPane.DEFAULT_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[0]);
		if(option == 0) {
			try {
				controller = new Controller(this, statusPanel, actuatorPanel);
			} catch (IOException e) {
				e.printStackTrace();
				JOptionPane.showMessageDialog(this, "Restart failed with following error: " + e.getMessage(), e.getClass().getSimpleName(), JOptionPane.ERROR_MESSAGE);
				SwingUtilities.invokeLater(() -> dispose());
			}
		} else {
			SwingUtilities.invokeLater(() -> dispose());
		}
	}

}
