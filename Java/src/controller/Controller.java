package controller;

import java.io.IOException;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.List;

import controller.data.ConfDevice;
import controller.data.MeasDevice;

public class Controller implements Connection.Listener {

	private static final long CLOSE_TIMEOUT = 3 * 1000;
	
	public static final int PORT = 0xCCCC;
	
	private boolean stopping;
	
	private InetAddress host;
	private Connection connection;
	private List<ControllerListener> listeners; //listener for asynchronous communication with local objects
	
	private Pool<ConfDevice> confPool;
	private Pool<MeasDevice> measPool;
	
	public Controller(InetAddress host) throws IOException {
		stopping = false;
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
		for(ControllerListener l : listeners)
			l.onConnectionRestored();
	}
	
	public void close() throws Exception {
		synchronized(host) {
			stopping = true;
			connection.stop();
			try {
				host.wait(CLOSE_TIMEOUT);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			if(stopping == true)
				throw new Exception("close hangs.");
		}
	}

	@Override
	public void onStopped(Exception cause) {
		synchronized(host) {
			if(stopping) {
				host.notify();
				stopping = false;
				return;
			}
		}
		
		try {
			if(cause != null)
				throw cause;
		} catch(Connection.LostException e) {
			for(ControllerListener l : listeners)
				l.onConnectionLost();
			try {
				restore();
			} catch (IOException ioe) {
				for(ControllerListener l : listeners)
					l.onError(ioe);
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
