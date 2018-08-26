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
