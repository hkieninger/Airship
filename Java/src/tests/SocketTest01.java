package tests;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class SocketTest01 {
	
	final static int PORT = 0xCCCC;
	final static String IP = "192.168.4.1";

	public static void main(String[] args) throws UnknownHostException, IOException {
		Socket socket = new Socket(InetAddress.getByName(IP), PORT);
		InputStream in = socket.getInputStream();
		OutputStream out = socket.getOutputStream();
		int c = 1;
		while(c != 0) {
			c = in.read();
			if(c == -1) {
				System.out.println("socket has been closed by peer");
				socket.close();
				return;
			}
			System.out.print((char) c);
		}
		String message = "Hello Server. I am a Java Client.";
		for(int i = 0; i < message.length(); i++) {
			out.write(message.charAt(i));
		}
		socket.close();
	}

}
