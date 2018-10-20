package controller.video;

import java.awt.image.BufferedImage;
import java.io.EOFException;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.util.ArrayList;

public abstract class VideoConnection extends Thread {
	
	protected Socket sock;
	private boolean running;
	
	private ArrayList<Listener> listeners;
	
	public VideoConnection() {
		running = true;
		listeners = new ArrayList<>();
		setName("Video Connection");
	}
	
	public void connect(InetAddress address, int port) throws IOException {
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
	
	public void stopRunning() throws IOException {
		running = false;
		if(sock != null)
			sock.close();
	}
	
	@Override
	public void run() {
		if(sock == null) {
			System.err.println("VideoConnection not connected, but started.");
			return;
		}
		try {
			begin();
			while(running) {
				distributeFrame(loop());
			}
		} catch(EOFException | SocketException e) {
			//normal if eoef or socket close
			System.out.println("video connection terminating: " + e.getCause());
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
		System.out.println("socket has been closed");
	}
	
	public static interface Listener {
		void onFrame(BufferedImage image);
	}
	
}
