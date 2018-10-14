package gui.panel;

import java.io.IOException;
import java.net.InetAddress;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

public class Stream03 {
	
	public static void main(String[] args) {
		SwingUtilities.invokeLater(() -> {
			try {
				JFrame frame = new JFrame();
				frame.setSize(640, 480);
				FrontVideoPanel panel = new FrontVideoPanel(InetAddress.getByName("192.168.0.17"));
				frame.setContentPane(panel);
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.setVisible(true);
				Thread thread = new Thread(panel);
				thread.start();
			} catch (IOException e) {
				e.printStackTrace();
			}
		});
	}

}