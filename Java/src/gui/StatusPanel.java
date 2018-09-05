package gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridLayout;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyledDocument;

import controller.SendThread;
import controller.pool.StatusView;

public class StatusPanel extends JPanel implements StatusView {
	
	private static final int GREEN_PING = 50;
	private static final long RED_PING = SendThread.CONNECTION_LOST_TIME;
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private JTextPane textPane;
	private JLabel networkLabel, batteryLabel;
	
	public StatusPanel() {
		setLayout(new BorderLayout());
		textPane = new JTextPane();
		textPane.setBackground(Color.BLACK);
		textPane.setEditable(false);
		textPane.setFocusable(false);
		JScrollPane scrollPane = new JScrollPane(textPane);
		add(scrollPane, BorderLayout.CENTER);
		
		//add the status labels for the battery and the ping
		JPanel panel = new JPanel();
		panel.setLayout(new GridLayout(1, 2));
		
		networkLabel = new JLabel();
		networkLabel.setOpaque(true);
		networkLabel.setHorizontalAlignment(JLabel.CENTER);
		setNetworkPing(Integer.MAX_VALUE);
		panel.add(networkLabel);
		
		batteryLabel = new JLabel();
		batteryLabel.setOpaque(true);
		batteryLabel.setHorizontalAlignment(JLabel.CENTER);
		setBatteryPercentage(0);
		panel.add(batteryLabel);
		add(panel, BorderLayout.NORTH);
	}
	
	public void appendLine(String text, Color color) {
		StyledDocument doc = textPane.getStyledDocument();
		
        Style style = textPane.addStyle("style", null);
        StyleConstants.setForeground(style, color);

        try { 
        	doc.insertString(doc.getLength(), text + "\n",style); 
        } catch (BadLocationException e){}
	}

	@Override
	public void setBatteryPercentage(final int percentage) {
		SwingUtilities.invokeLater(() -> {
			int green, red;
			if(percentage > 50) {
				green = 255;
				red = 255 * (100 - percentage) / 50;
			} else {
				red = 255;
				green = 255 * percentage / 50;
			}
			batteryLabel.setBackground(new Color(red, green, 0));
			batteryLabel.setText("Battery percentage is: " + percentage + " %.");
		});
	}

	@Override
	public void setNetworkPing(final long ping) {
		SwingUtilities.invokeLater(() -> {
			int green, red;
			if(ping < GREEN_PING) {
				green = 255;
				red = 0;
			} else if(ping > RED_PING) {
				green = 0;
				red = 255;
			} else {
				int max = (int) (RED_PING - GREEN_PING);
				int gradient = (int) (ping - GREEN_PING);
				if(gradient < max / 2) {
					green = 255;
					red = (int) (255 * gradient / max * 2);
				} else {
					red = 255;
					green = (int) (255 * (max - gradient) / max * 2);
				}
			}
			networkLabel.setBackground(new Color(red, green, 0));
			networkLabel.setText("Ping is: " + ping + " ms.");
		});
	}

	@Override
	//may need to make it thread safe or run it in a gui thread
	public void information(String info, int color) {
		SwingUtilities.invokeLater(() -> appendLine(info, new Color(color)));
	}

}
