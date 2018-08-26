package tests;

import java.awt.KeyEventDispatcher;
import java.awt.KeyboardFocusManager;
import java.awt.event.KeyEvent;
import java.io.IOException;

import javax.swing.JFrame;

import controller.ActuatorController;
import gui.ActuatorPanel;

public class ActuatorPanelTest01 {

	public static void main(String[] args) throws IOException {
		//create the panel, which contains the gui
		ActuatorPanel actuatorPanel = new ActuatorPanel();
		actuatorPanel.setListener(new ActuatorController.Listener() {
			
			@Override
			public void onLeftMotorChanged(int thrust) {
				System.out.println("left motor thrust is " + thrust);
			}

			@Override
			public void onRightMotorChanged(int thrust) {
				System.out.println("right motor thrust is " + thrust);
			}

			@Override
			public void onLeftRudderChanged(int angle) {
				System.out.println("left rudder angle is " + angle);
			}

			@Override
			public void onRightRudderChanged(int angle) {
				System.out.println("right rudder angle is " + angle);
			}

			@Override
			public void onTopRudderChanged(int angle) {
				System.out.println("top rudder angle is " + angle);
			}
			
		});
		
		//process the keyboard shortcuts
		KeyboardFocusManager.getCurrentKeyboardFocusManager().addKeyEventDispatcher(new KeyEventDispatcher() {
			
			private static final int KEY_CHANGE_PERCENT_MOTOR = 10;
			private static final int KEY_CHANGE_PERCENT_RUDDER = 5;
			
			private static final int KEY_CHANGE_MOTOR = KEY_CHANGE_PERCENT_MOTOR * ActuatorController.THRUST_MAX / 100;
			private static final int KEY_CHANGE_RUDDER = KEY_CHANGE_PERCENT_RUDDER * ActuatorController.ANGLE_MAX / 100;
			
			@Override
			public boolean dispatchKeyEvent(KeyEvent e) {
				if(e.getID() == KeyEvent.KEY_PRESSED) {
					if(e.getKeyCode() == KeyEvent.VK_Q)
						actuatorPanel.setLeftMotor(actuatorPanel.getLeftMotor() + KEY_CHANGE_MOTOR);
					else if(e.getKeyCode() == KeyEvent.VK_A)
						actuatorPanel.setLeftMotor(actuatorPanel.getLeftMotor() - KEY_CHANGE_MOTOR);
					else if(e.getKeyCode() == KeyEvent.VK_W)
						actuatorPanel.setRightMotor(actuatorPanel.getRightMotor() + KEY_CHANGE_MOTOR);
					else if(e.getKeyCode() == KeyEvent.VK_S)
						actuatorPanel.setRightMotor(actuatorPanel.getRightMotor() - KEY_CHANGE_MOTOR);
					else if(e.getKeyCode() == KeyEvent.VK_U)
						actuatorPanel.setLeftRudder(actuatorPanel.getLeftRudder() + KEY_CHANGE_RUDDER);
					else if(e.getKeyCode() == KeyEvent.VK_J)
						actuatorPanel.setLeftRudder(actuatorPanel.getLeftRudder() - KEY_CHANGE_RUDDER);
					else if(e.getKeyCode() == KeyEvent.VK_I)
						actuatorPanel.setRightRudder(actuatorPanel.getRightRudder() + KEY_CHANGE_RUDDER);
					else if(e.getKeyCode() == KeyEvent.VK_K)
						actuatorPanel.setRightRudder(actuatorPanel.getRightRudder() - KEY_CHANGE_RUDDER);
					else if(e.getKeyCode() == KeyEvent.VK_O)
						actuatorPanel.setTopRudder(actuatorPanel.getTopRudder() + KEY_CHANGE_RUDDER);
					else if(e.getKeyCode() == KeyEvent.VK_L)
						actuatorPanel.setTopRudder(actuatorPanel.getTopRudder() - KEY_CHANGE_RUDDER);
				}
				return false;
			}
			
		});
		
		//create the controller instance, which communicates with the zeppelin
		//Controller controller = new Controller(actuatorPanel);
		
		//display the actuator panel
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.add(actuatorPanel);
		frame.setSize(600, 450);
		frame.setLocation(800-300, 450-225);
		frame.setVisible(true);
		
		//normally should use invokeLater or invokeAndWait
	}

}
