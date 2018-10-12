package gui;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.IOException;
import java.net.InetAddress;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import controller.Controller;
import controller.ControllerListener;
import gui.quarter.ControllQuarter;
import gui.quarter.SensorQuarter;
import gui.quarter.StatusQuarter;
import gui.quarter.VideoQuarter;

public class Frame extends JFrame implements WindowListener, ControllerListener {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	public static final String DEFAULT_IP = "192.168.0.24";//"192.168.0.27", "172.17.72.204", "192.168.4.1";
	
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
		super("Control Tower");
		//window settings
		setExtendedState(JFrame.MAXIMIZED_BOTH);
		setMinimumSize(new Dimension(480, 240));
		setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		addWindowListener(this);
		setLayout(new GridLayout(2, 2));
		
		String ip = JOptionPane.showInputDialog("IP of the airship: ", DEFAULT_IP);
		try {
			controller = new Controller(InetAddress.getByName(ip));
		} catch (IOException e) {
			e.printStackTrace();
			JOptionPane.showMessageDialog(this, "Connection couldn't be established: " + e.getMessage(), e.getClass().getSimpleName(), JOptionPane.ERROR_MESSAGE);
			System.exit(1);
		}
		
		//controll panels
		add(new ControllQuarter(controller));
		//Status panels
		add(new StatusQuarter(controller));
		//Video panels
		try {
			add(new VideoQuarter(controller));
		} catch (IOException e) {
			e.printStackTrace();
		}
		//Sensor panels
		add(new SensorQuarter(controller));
		
		controller.addListener(this);
	}

	@Override
	public void windowOpened(WindowEvent arg0) {}

	@Override
	public void windowClosed(WindowEvent arg0) {
		//cleanup code here
		try {
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
		JOptionPane.showMessageDialog(this, "The connection is restored.", "Connection", JOptionPane.INFORMATION_MESSAGE);
	}
	
	private void restoreOptionDialog(String message) {
		Object[] options = { "Restore connection.", "Exit Programm" };
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
