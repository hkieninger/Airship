package tests;

import javax.swing.JFrame;

import controller.SteeringController;
import gui.SteeringPanel;

public class SteeringPanelTest01 {

	public static void main(String[] args) {
		SteeringPanel steeringPanel = new SteeringPanel();
		steeringPanel.setListener(new SteeringController.Listener() {
			
			@Override
			public void onYawChanged(int yaw) {
				System.out.println("yaw changed: " + yaw);
			}
			
			@Override
			public void onVelocityChanged(int velocity) {
				System.out.println("velocity changed: " + velocity);
			}
			
			@Override
			public void onPitchChanged(int pitch) {
				System.out.println("pitch changed: " + pitch);
			}
			
			@Override
			public void onCallibrated(int yaw, int pitch) {
				System.out.println("callibrated: (yaw/picht)=(" + yaw + "/" + pitch + ")");
			}
			
		});
		
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
