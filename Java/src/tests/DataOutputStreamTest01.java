package tests;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class DataOutputStreamTest01 {
	
	final static int PORT = 0xCCCC;
	final static String IP = "localhost"; //"192.168.4.1";

	public static void main(String[] args) throws UnknownHostException, IOException {
		Socket socket = new Socket(InetAddress.getByName(IP), PORT);
		DataOutputStream out = new DataOutputStream(socket.getOutputStream());
		short s = (short) 0xABCD;
		out.writeShort(s);
		socket.close();
	}

}