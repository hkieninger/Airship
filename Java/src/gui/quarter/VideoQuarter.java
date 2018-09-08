package gui.quarter;

import javax.swing.JTabbedPane;

import controller.Controller;
import controller.data.parameter.MeasSensor;
import gui.panel.VideoPanel;

public class VideoQuarter extends JTabbedPane {

	private static final long serialVersionUID = 1L;
	
	public VideoQuarter(Controller controller) {
		this.setFocusable(false);
		VideoPanel bottomVideoPanel = new VideoPanel(MeasSensor.CAM_BOTTOM);
		controller.getMeasPool().addListener(bottomVideoPanel);
		this.addTab("camera bottom", bottomVideoPanel);
		VideoPanel frontVideoPanel = new VideoPanel(MeasSensor.CAM_FRONT);
		controller.getMeasPool().addListener(frontVideoPanel);
		this.addTab("camera front", frontVideoPanel);
		this.addChangeListener((event) -> {
			
		});
	}

}
