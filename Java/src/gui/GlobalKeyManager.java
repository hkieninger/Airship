package gui;

import java.awt.event.KeyEvent;

public class GlobalKeyManager {
	
	public GlobalKeyManager() {
		
	}
	
	public interface Listener {
		public void onKeyEvent(KeyEvent e);
	}
	
}
