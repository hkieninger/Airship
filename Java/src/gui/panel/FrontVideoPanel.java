package gui.panel;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.EOFException;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;

import javax.imageio.ImageIO;

import controller.Controller;

/**
 * 
 * @author hans
 *
 * Displays the JPG stream coming from the front camera
 */
public class FrontVideoPanel extends VideoPanel implements Runnable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private static final int PORT = 0xCCCB;
	
	private Socket sock;
	private boolean running;
	
	public FrontVideoPanel(InetAddress host) throws IOException {
		sock = new Socket(host, PORT);
		running = true;
	}
	
	public void stopRunning() {
		running = false;
	}

	@Override
	public void run() {
		try {
			byte[] buffer = new byte[0];
			DataInputStream input = new DataInputStream(sock.getInputStream());
			while(running) {
				int bytes = input.readInt(); //in java int is int32_t actually it is a uint32_t (as long as the images aren't too big this isn't a problem)
				if(buffer.length < bytes)
					buffer = new byte[bytes];
				input.readFully(buffer, 0, bytes);
				ByteArrayInputStream stream = new ByteArrayInputStream(buffer, 0, bytes);
				BufferedImage image = ImageIO.read(stream);
				stream.close();
				setImage(image);
			}
		} catch(EOFException e) {
			//normal
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				sock.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

}
