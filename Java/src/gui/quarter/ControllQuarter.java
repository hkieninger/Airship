package gui.quarter;

import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;

import controller.Controller;
import gui.panel.ActuatorPanel;
import gui.panel.AutoPanel;
import gui.panel.SteeringPanel;

public class ControllQuarter extends JTabbedPane {
	
	private static final long serialVersionUID = 1L;

	public ControllQuarter(Controller controller) {
		this.setFocusable(false);
		ActuatorPanel actuatorPanel = new ActuatorPanel(controller.getConfPool());
		JScrollPane scrollPane = new JScrollPane(actuatorPanel);
		this.addTab("actuator", scrollPane);
		SteeringPanel steeringPanel = new SteeringPanel(controller.getConfPool());
		this.addTab("steering", steeringPanel);
		AutoPanel autoPanel = new AutoPanel();
		this.addTab("autopilot", autoPanel);
	}

}
