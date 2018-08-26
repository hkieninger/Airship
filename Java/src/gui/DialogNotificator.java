package gui;

import javax.swing.JOptionPane;

import controller.Notificator;

public class DialogNotificator implements Notificator {

	@Override
	public void notificate(String notification, Level level) {
		switch(level) {
			case CHATTERBOX: System.out.println(notification); break;
			default: 
				JOptionPane.showMessageDialog(null, notification, level.toString(), JOptionPane.PLAIN_MESSAGE);
		}
	}

}
