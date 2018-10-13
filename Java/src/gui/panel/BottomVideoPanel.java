package gui.panel;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

import org.bytedeco.javacv.FFmpegFrameGrabber;
import org.bytedeco.javacv.Frame;
import org.bytedeco.javacv.FrameGrabber.Exception;
import org.bytedeco.javacv.Java2DFrameConverter;

import controller.Pool;
import controller.data.MeasDevice;
import controller.data.object.Picture;
import controller.data.parameter.MeasSensor;
import controller.data.parameter.Parameter;

/**
 * 
 * @author hans
 *
 * Displays the h264 stream coming from the raspi-camera
 *
 */
public class BottomVideoPanel extends VideoPanel implements Pool.Listener<MeasDevice> {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private FFmpegFrameGrabber grabber;
	private Java2DFrameConverter converter;
	private PipedInputStream input;
	private PipedOutputStream output;
	private boolean hasStarted;
	
	public BottomVideoPanel() throws IOException {
		input = new PipedInputStream(Picture.MAX_SIZE * 3);
		output = new PipedOutputStream(input);
		converter = new Java2DFrameConverter();
		grabber = new FFmpegFrameGrabber(input);
		grabber.setFormat("h264");
		hasStarted = false;
	}

	@Override
	public void onChanged(Pool<MeasDevice> pool, MeasDevice device, Enum<? extends Parameter> parameter) {
		if(device == MeasDevice.SENSOR && parameter == MeasSensor.CAM_BOTTOM) {
			Picture pic = (Picture) pool.getValue(device, parameter);
			try {
				System.out.println("received image, size: " + pic.getSize());
				output.write(pic.getData(), 0, pic.getSize()); //inefficient data gets copied from socket to buffer to pipe
				//output.flush();
				Thread thread = new Thread(() -> {
					synchronized(grabber) {
						System.out.println("grabber thread");
						//if following code is blocking the thread, put it in a new thread
						try {
							if(!hasStarted) {
								hasStarted = true;
								System.out.println("start");
								grabber.start();
							}
							System.out.println("grab");
							Frame frame = grabber.grab();
							System.out.println("convert");
							BufferedImage image = converter.convert(frame);
							System.out.println("set image");
							setImage(image);
						} catch (Exception e) {
							e.printStackTrace();
						}
					}
				});
				thread.start();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	void close() throws IOException {
		grabber.stop();
		input.close();
		output.close();
	}
	
}
