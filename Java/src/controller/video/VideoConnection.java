package controller.video;

import java.awt.image.BufferedImage;
import java.io.EOFException;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.util.ArrayList;

public abstract class VideoConnection extends Thread {
	
	protected Socket sock;
	private boolean running;
	
	private ArrayList<Listener> listeners;
	
	public VideoConnection(InetAddress address, int port) throws IOException {
		running = true;
		listeners = new ArrayList<>();
		sock = new Socket(address, port);
	}
	
	public void addListener(Listener l) {
		listeners.add(l);
	}
	
	public void removeListener(Listener l) {
		listeners.remove(l);
	}
	
	private void distributeFrame(BufferedImage image) {
		for(Listener l : listeners) {
			l.onFrame(image);
		}
	}
	
	public void stopRunning() {
		running = false;
	}
	
	@Override
	public void run() {
		try {
			begin();
			while(running) {
				distributeFrame(loop());
			}
		} catch(EOFException e) {
			//normal
		} catch(IOException e) {
			e.printStackTrace();
		} finally {
			try {
				end();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	protected void begin() throws IOException {}
	
	protected abstract BufferedImage loop() throws IOException;
	
	protected void end() throws IOException {
		sock.close();
	}
	
	public static interface Listener {
		void onFrame(BufferedImage image);
	}
	
}
