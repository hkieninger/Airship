package gui.panel;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.Socket;

import org.bytedeco.javacv.FFmpegFrameGrabber;
import org.bytedeco.javacv.Frame;
import org.bytedeco.javacv.Java2DFrameConverter;

import controller.Controller;

/**
 * 
 * @author hans
 *
 * Displays the h264 stream coming from the raspi-camera
 *
 */
public class BottomVideoPanel extends VideoPanel implements Runnable {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private static final int PORT = 0xCCCD;
	
	private FFmpegFrameGrabber grabber;
	private Java2DFrameConverter converter;
	private Socket sock;
	
	private boolean running;
	
	public BottomVideoPanel(Controller controller) throws IOException {
		running = true;
		sock = new Socket(controller.getHost(), PORT);
		converter = new Java2DFrameConverter();
		grabber = new FFmpegFrameGrabber(sock.getInputStream());
		grabber.setFormat("h264");
	}
	
	public void stopRunning() {
		running = false;
	}

	@Override
	public void run() {
		try {
			grabber.start();
			Frame frame = grabber.grab();
			do {
				BufferedImage image = converter.convert(frame);
				setImage(image);
				frame = grabber.grab();
			} while(frame != null && running);
			grabber.stop();
		} catch(IOException e) {
			e.printStackTrace();
		}
		try {
			sock.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
}
