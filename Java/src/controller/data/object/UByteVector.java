package controller.data.object;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class UByteVector implements ConnectionData {
	
	public int[] val;
	
	public UByteVector(int dimensions) {
		val = new int[dimensions];
	}

	@Override
	public void send(DataOutputStream out) throws IOException {
		for(int i : val)
			out.writeByte(i);
	}

	@Override
	public void receive(DataInputStream in) throws IOException {
		for(int i = 0; i < val.length; i++) {
			val[i] = in.readUnsignedByte();
		}
	}

}
