package controller.video;

import java.awt.image.BufferedImage;
import java.io.EOFException;
import java.io.IOException;
import java.net.InetAddress;

import org.bytedeco.javacv.FFmpegFrameGrabber;
import org.bytedeco.javacv.Frame;
import org.bytedeco.javacv.Java2DFrameConverter;

public class H264Connection extends VideoConnection {
	
	private FFmpegFrameGrabber grabber;
	private Java2DFrameConverter converter;

	public H264Connection() {
		super();
		converter = new Java2DFrameConverter();
		setName("H264 Connection");
	}
	
	@Override
	public void connect(InetAddress address, int port) throws IOException {
		super.connect(address, port);
		grabber = new FFmpegFrameGrabber(sock.getInputStream());
		grabber.setFormat("h264");
	}
	
	@Override
	protected void begin() throws IOException {
		super.begin();
		grabber.start();
	}

	@Override
	protected BufferedImage loop() throws IOException {
		Frame frame = grabber.grab();
		if(frame == null)
			throw new EOFException();
		return converter.convert(frame);
	}
	
	@Override
	protected void end() throws IOException {
		grabber.stop();
		super.end();
	}

}
