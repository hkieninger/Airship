package gui.quarter;

import java.io.IOException;

import javax.swing.JTabbedPane;

import controller.Controller;
import controller.data.ConfDevice;
import controller.data.object.ConnectionData.UByte;
import controller.data.parameter.ConfSensor;
import gui.panel.BottomVideoPanel;
import gui.panel.FrontVideoPanel;

public class VideoQuarter extends JTabbedPane {

	private static final long serialVersionUID = 1L;
	
	public VideoQuarter(Controller controller) throws IOException {
		this.setFocusable(false);
		BottomVideoPanel bottomVideoPanel = new BottomVideoPanel();
		controller.getMeasPool().addListener(bottomVideoPanel);
		this.addTab("camera bottom", bottomVideoPanel);
		FrontVideoPanel frontVideoPanel = new FrontVideoPanel();
		controller.getMeasPool().addListener(frontVideoPanel);
		this.addTab("camera front", frontVideoPanel);
		this.addChangeListener((event) -> {
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
		});
	}

}
