package gui.quarter;

import javax.swing.JTabbedPane;

import controller.Controller;
import controller.video.VideoConnection;
import gui.panel.VideoPanel;

public class VideoQuarter extends JTabbedPane {

	private static final long serialVersionUID = 1L;
	
	public VideoQuarter(Controller controller, VideoConnection frontCamera, VideoConnection bottomCamera){
		this.setFocusable(false);
		VideoPanel bottomVideoPanel = new VideoPanel(bottomCamera);
		this.addTab("camera bottom", bottomVideoPanel);
		VideoPanel frontVideoPanel = new VideoPanel(frontCamera);
		this.addTab("camera front", frontVideoPanel);
		/*this.addChangeListener((event) -> {
			UByte enable = new UByte();
			enable.val = ConfSensor.ENABLE;
			UByte disable = new UByte();
			disable.val = ConfSensor.DISABLE;
			if(getSelectedComponent() == bottomVideoPanel) {
				controller.getConfPool().setValue(ConfDevice.SENSOR, ConfSensor.CAM_FRONT, disable);
				controller.getConfPool().setValue(ConfDevice.SENSOR, ConfSensor.CAM_BOTTOM, enable);
			} else {
				controller.getConfPool().setValue(ConfDevice.SENSOR, ConfSensor.CAM_BOTTOM, disable);
				controller.getConfPool().setValue(ConfDevice.SENSOR, ConfSensor.CAM_FRONT, enable);
			}
		});*/
	}

}
