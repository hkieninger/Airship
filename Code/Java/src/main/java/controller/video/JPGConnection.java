package controller.video;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.net.InetAddress;

import javax.imageio.ImageIO;

public class JPGConnection extends VideoConnection {

	private byte[] buffer;
	private DataInputStream input;
	
	public JPGConnection() {
		super();
		buffer = new byte[0];
		setName("JPG Connection");
	}
	
	@Override
	public void connect(InetAddress address, int port) throws IOException {
		super.connect(address, port);
		input = new DataInputStream(sock.getInputStream());
	}

	@Override
	protected BufferedImage loop() throws IOException {
		int bytes = input.readInt(); //in java int is int32_t actually it is a uint32_t (as long as the images aren't too big this isn't a problem)
		if(buffer.length < bytes)
			buffer = new byte[bytes];
		input.readFully(buffer, 0, bytes);
		ByteArrayInputStream stream = new ByteArrayInputStream(buffer, 0, bytes);
		BufferedImage image = ImageIO.read(stream);
		stream.close();
		return image;
	}


}
