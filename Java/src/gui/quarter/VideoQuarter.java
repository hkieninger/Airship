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
	}

}
