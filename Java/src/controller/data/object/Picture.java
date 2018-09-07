package controller.data.object;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class Picture implements ConnectionData {
	
	private static final int MAX_SIZE =  3 * 2592 * 1944; //from max resolution of rpi cam
	
	private static Picture bottom = new Picture();
	private static Picture front = new Picture();
	
	public static Picture getBottomPicture() {
		return bottom;
	}
	
	public static Picture getFrontPicture() {
		return front;
	}
	
	/**
	 * @return the size
	 */
	public int getSize() {
		return size;
	}

	/**
	 * @return the data
	 */
	public byte[] getData() {
		return data;
	}

	private int size;
	private byte[] data;
	
	public Picture() {
		data = new byte[MAX_SIZE];
	}

	@Override
	public void send(DataOutputStream out) throws IOException {
		throw new UnsupportedOperationException("Not implemented yet");
	}

	@Override
	public void receive(DataInputStream in) throws IOException {
		size = in.readInt();
		in.readFully(data, 0, size);
	}

}
