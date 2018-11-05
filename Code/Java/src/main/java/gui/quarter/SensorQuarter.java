package gui.quarter;

import javax.swing.JTabbedPane;

import controller.Controller;
import gui.panel.MapPanel;
import gui.panel.SensorPanel;

public class SensorQuarter extends JTabbedPane {

	private static final long serialVersionUID = 1L;
	
	public SensorQuarter(Controller controller) {
		setFocusable(false);
		SensorPanel sensorPanel = new SensorPanel();
		controller.getMeasPool().addListener(sensorPanel);
		this.addTab("sensor data", sensorPanel);
		MapPanel mapPanel = new MapPanel();
		controller.getMeasPool().addListener(mapPanel);
		this.addTab("map", mapPanel);
	}

}
