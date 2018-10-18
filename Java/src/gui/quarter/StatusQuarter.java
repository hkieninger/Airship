package gui.quarter;

import controller.Controller;
import gui.panel.StatusPanel;

public class StatusQuarter extends StatusPanel {
	
	private static final long serialVersionUID = 1L;
	
	public StatusQuarter(Controller controller) {
		super();
		controller.addListener(this);
		controller.getMeasPool().addListener(this);
		setInformation("Not connected. Connect via the menu.", 0xFFFFFF);
	}

}
