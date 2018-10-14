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
	
	private BottomVideoPanel bottomVideoPanel;
	private FrontVideoPanel frontVideoPanel;
	
	public VideoQuarter(Controller controller) throws IOException {
		this.setFocusable(false);
		Thread thread;
		bottomVideoPanel = new BottomVideoPanel(controller.getHost());
		thread = new Thread(bottomVideoPanel);
		thread.start();
		this.addTab("camera bottom", bottomVideoPanel);
		frontVideoPanel = new FrontVideoPanel(controller.getHost());
		thread = new Thread(frontVideoPanel);
		thread.start();
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
	
	public void close() throws IOException {
		bottomVideoPanel.stopRunning();
		frontVideoPanel.stopRunning();
	}

}
