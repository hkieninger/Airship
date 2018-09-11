package controller;

import java.io.IOException;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.List;

import controller.data.ConfDevice;
import controller.data.MeasDevice;

public class Controller implements Connection.Listener {

	private static final long CLOSE_TIMEOUT = 1 * 1000;
	
	public static final int PORT = 0xCCCC;
	
	private boolean stop;
	
	private InetAddress host;
	private Connection connection;
	private List<ControllerListener> listeners; //listener for asynchronous communication with local objects
	
	private Pool<ConfDevice> confPool;
	private Pool<MeasDevice> measPool;
	
	public Controller(InetAddress host) throws IOException {
		this.listeners = new ArrayList<>();
		this.host = host;
		this.confPool = new Pool<>(ConfDevice.class);
		this.measPool = new Pool<>(MeasDevice.class);
		restore();
	}
	
	public Pool<ConfDevice> getConfPool() {
		return confPool;
	}

	public Pool<MeasDevice> getMeasPool() {
		return measPool;
	}
	
	public void addListener(ControllerListener l) {
		listeners.add(l);
	}
	
	public void removeListener(ControllerListener l) {
		listeners.remove(l);
	}
	
	public void restore() throws IOException {
		connection = new Connection(host, PORT, measPool, confPool);
		connection.addListener(this);
		connection.start();
	}
	
	public void close() {
		synchronized(host) {
			stop = true;
			connection.stop();
			try {
				host.wait(CLOSE_TIMEOUT);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public void onStopped(Exception cause) {
		synchronized(host) {
			if(stop) {
				host.notify();
				return;
			}
		}
		
		try {
			if(cause != null)
				throw cause;
		} catch(Connection.LostException e) {
			try {
				for(ControllerListener l : listeners)
					l.onConnectionLost();
				restore();
				for(ControllerListener l : listeners)
					l.onConnectionRestored();
			} catch (IOException ioe) {
				ioe.printStackTrace();
			}
		} catch(Connection.ClosedByHostException e) {
			for(ControllerListener l : listeners)
				l.onConnectionClosedByHost();
		} catch (Exception e) {
			for(ControllerListener l : listeners)
				l.onError(e);
		}
	}

}
