package controller.data.object;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class FloatVector implements ConnectionData {
	
	public float[] val;
	
	public FloatVector(int dimensions) {
		val = new float[dimensions];
	}

	@Override
	public void send(DataOutputStream out) throws IOException {
		for(float f : val)
			out.writeFloat(f);
	}

	@Override
	public void receive(DataInputStream in) throws IOException {
		for(int i = 0; i < val.length; i++) {
			val[i] = in.readFloat();
		}
	}

}
