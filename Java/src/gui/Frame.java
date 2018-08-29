package gui;

import java.awt.GridLayout;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.IOException;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
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
		//window settings
		setExtendedState(JFrame.MAXIMIZED_BOTH); 
		setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		addWindowListener(this);
		setLayout(new GridLayout(2, 2));
		
		//controll panels
		JTabbedPane tabs = new JTabbedPane();
		tabs.setFocusable(false);
		ActuatorPanel actuatorPanel = new ActuatorPanel();
		tabs.addTab("actuator", actuatorPanel);
		SteeringPanel steeringPanel = new SteeringPanel();
		tabs.addTab("steering", steeringPanel);
		AutoPanel autoPanel = new AutoPanel();
		tabs.addTab("autopilot", autoPanel);
		add(tabs);
		
		//Sensor panels
		SensorPanel sensorPanel = new SensorPanel();
		add(sensorPanel);
		
		//Video panels
		VideoPanel bottomVideoPanel = new VideoPanel();
		add(bottomVideoPanel);
		
		VideoPanel frontVideoPanel = new VideoPanel();
		add(frontVideoPanel);
		
		//create the controller
		/*try {
			controller = new Controller(this, actuatorPanel, steeringPanel);
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
		try {
			controller.stop();
		} catch (IOException | InterruptedException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void windowClosing(WindowEvent arg0) {
		Object[] options = { "OK", "CANCEL" };
		int option = JOptionPane.showOptionDialog(this, "Do you really want to exit?", "", 
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

	private boolean optionPaneDisplayed = false;
	private Object lock = new Object();
	
	@Override
	public void onInformation(String information, Level level) {
		if(level == Level.CHATTERBOX) {
			System.out.println("Controller: " + information);
		} else {
			//do not block the current thread
			new Thread(() ->  {
				boolean allowed = false;
				synchronized(lock) {
					if(!optionPaneDisplayed) {
						optionPaneDisplayed = true;
						allowed = true;
					}
				}
				if(allowed) {
					JOptionPane.showMessageDialog(this, information, level.toString(), JOptionPane.PLAIN_MESSAGE);
					optionPaneDisplayed = false;
				} else {
					System.err.println("Controller: " + information);
				}
			}).start();
		}
	}

	@Override
	public void onError() {}

	@Override
	public void onConnectionLost(boolean closedByHost) {
		if(closedByHost) {
			try {
				controller.stop();
			} catch (IOException | InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public void onStarted() {}

	@Override
	public void onStopped() {}

}
