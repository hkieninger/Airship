package tests;

import javax.swing.JFrame;

import gui.SteeringPanel;

public class SteeringPanelTest01 {

	public static void main(String[] args) {
		SteeringPanel steeringPanel = new SteeringPanel();
		
		//display the actuator Panel
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.add(steeringPanel);
		frame.setSize(600, 450);
		frame.setLocation(800-300, 450-225);
		frame.setVisible(true);
		
		//normally should use invokeLater or invokeAndWait
	}

}
