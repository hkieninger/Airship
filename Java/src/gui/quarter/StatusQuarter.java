package gui.quarter;

import controller.Controller;
import gui.panel.StatusPanel;

public class StatusQuarter extends StatusPanel {
	
	private static final long serialVersionUID = 1L;
	
	public StatusQuarter(Controller controller) {
		super();
		controller.addListener(this);
		controller.getMeasPool().addListener(this);
		setInformation("Connection established.", 0x00FF00);
	}

}
