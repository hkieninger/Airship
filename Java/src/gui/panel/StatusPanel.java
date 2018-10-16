package gui.panel;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridLayout;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyledDocument;

import controller.ControllerListener;
import controller.Pool;
import controller.SendThread;
import controller.data.MeasDevice;
import controller.data.object.ConnectionData;
import controller.data.parameter.MeasRPI;
import controller.data.parameter.MeasSensor;
import controller.data.parameter.Parameter;

public class StatusPanel extends JPanel implements Pool.Listener<MeasDevice>, ControllerListener {
	
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

	public void setInformation(String info, int color) {
		SwingUtilities.invokeLater(() -> {	
			String date = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());
			appendLine(date + "$ " + info, new Color(color));
		});
	}

	@Override
	public void onChanged(Pool<MeasDevice> pool, MeasDevice device, Enum<? extends Parameter> parameter) {
		if(device == MeasDevice.RPI) {
			if(parameter == MeasRPI.ECHO_REPLY) {
				setNetworkPing(((ConnectionData.Long) pool.getValue(device, parameter)).val);
			} else if(parameter == MeasRPI.INFO) {
				setInformation("Information from remote host: " + 
						((ConnectionData.String) pool.getValue(device, parameter)).val, 0xFFFFFF);
			}
		} else if(device == MeasDevice.SENSOR && parameter == MeasSensor.BATTERY) {
			setBatteryPercentage(((ConnectionData.Byte) pool.getValue(device, parameter)).val);
		}
	}

	@Override
	public void onError(Exception e) {
		setInformation("The following error has occured: " + e.getMessage() + ".", 0xFF0000);
	}

	@Override
	public void onConnectionClosedByHost() {
		setInformation("The connection has been closed by the remote host.", 0xFF8000);
	}

	@Override
	public void onConnectionLost() {
		setInformation("The connection has been lost. Restoring ...", 0xFFFF00);
	}

	@Override
	public void onConnectionRestored() {
		setInformation("The connection has been (re)established.", 0x00FF00);
	}

}
