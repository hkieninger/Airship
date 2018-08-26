package controller;

public interface Notificator {
	
	enum Level {
		CHATTERBOX, NORMAL, IMPORTANT, ERROR, LETHAL
	}
	
	public void notificate(String notification, Level level);
	
}
