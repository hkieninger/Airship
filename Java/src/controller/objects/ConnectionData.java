package controller.objects;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public interface ConnectionData {
	public void send(DataOutputStream out) throws IOException;
	public void receive(DataInputStream in) throws IOException;
}
