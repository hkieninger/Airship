package gui;

import java.awt.Color;
import java.awt.Component;

import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.SwingUtilities;

import controller.pool.SensorView;
import gui.component.StaticTableModel;

public class SensorPanel extends JPanel implements SensorView {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private JTable vectorTable, gpsTable, restTable;
	private JTable positionTable, orientationTable;

	public SensorPanel() {
		setLayout(new BoxLayout(this, BoxLayout.PAGE_AXIS));
		
		//display raw data
		JPanel panel = new JPanel();
		panel.setBackground(Color.CYAN);
		panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
		
		JLabel label = new JLabel("Raw Data");
		label.setAlignmentX(Component.CENTER_ALIGNMENT);
		panel.add(label);
		
		String[] columns = new String[] {"", "x", "y", "z"};
		Object[][] data = new Object[][] {
			{"Compass [Gs]", 0, 0, 0},
			{"Accelerometer [m/s²]", 0, 0, 0},
			{"Gyroscope [°/s]", 0, 0, 0}
		};
		vectorTable = new JTable(new StaticTableModel(data, columns));
		addTable(vectorTable, panel);
		
		columns = new String[] {"", "Latitude", "Longitude", "Altitude [m]", "Precision [m]", "Velocity [m/s]", "Azimuth [°]"};
		data = new Object[][] {
			{"GPS", "0 °N", "0 °E", 0, 0, 0, 0}
		};
		gpsTable = new JTable(new StaticTableModel(data, columns));
		addTable(gpsTable, panel);
		
		columns = new String[] {"Barometer [Pa]", "Distance front [cm]", "Distance bottom [cm]"};
		data = new Object[][] {
			{0, 0, 0}
		};
		restTable = new JTable(new StaticTableModel(data, columns));
		addTable(restTable, panel);
		
		add(panel);
		
		//display filtered data
		panel = new JPanel();
		panel.setBackground(Color.MAGENTA);
		panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
		
		label = new JLabel("Filtered Data");
		label.setAlignmentX(Component.CENTER_ALIGNMENT);
		panel.add(label);
		
		columns = new String[] {"", "Latitude", "Longitude", "Altitude"};
		data = new Object[][] {
			{"Position", 0, 0, 0},
			{"Precision [m]", 0, 0, 0},
			{"Velocity [m/s]", 0, 0, 0}
		};
		positionTable = new JTable(new StaticTableModel(data, columns));
		addTable(positionTable, panel);
		
		columns = new String[] {"", "Yaw", "Pitch", "Roll"};
		data = new Object[][] {
			{"Orientation [°]", 0, 0, 0},
			{"Precision [°]", 0, 0, 0},
			{"Rotation [°/s]", 0, 0, 0}
		};
		orientationTable = new JTable(new StaticTableModel(data, columns));
		addTable(orientationTable, panel);
		
		add(panel);
	}
		
	private void addTable(JTable table, JPanel panel) {
		table.getTableHeader().setReorderingAllowed(false);
		table.setFocusable(false);
		JScrollPane scrollPane = new JScrollPane(table);
		panel.add(scrollPane);
	}

	@Override
	public void setCompass(float x, float y, float z) {
		SwingUtilities.invokeLater(() -> {
			vectorTable.setValueAt(x, 0, 1);
			vectorTable.setValueAt(y, 0, 2);
			vectorTable.setValueAt(z, 0, 3);
		});
	}

	@Override
	public void setAccelerometer(float x, float y, float z) {
		SwingUtilities.invokeLater(() -> {
			vectorTable.setValueAt(x, 1, 1);
			vectorTable.setValueAt(y, 1, 2);
			vectorTable.setValueAt(z, 1, 3);
		});
	}

	@Override
	public void setGyroscope(float x, float y, float z) {
		SwingUtilities.invokeLater(() -> {
			vectorTable.setValueAt(x, 2, 1);
			vectorTable.setValueAt(y, 2, 2);
			vectorTable.setValueAt(z, 2, 3);
		});
	}

	@Override
	public void setGPS(int latitude, int longitude, int altitude, int precision, int velocity, int azimuth) {
		SwingUtilities.invokeLater(() -> {
			if(latitude > 0) {
				gpsTable.setValueAt(latitude * 1e-7 + " °N", 0, 1);
			} else {
				gpsTable.setValueAt(-latitude * 1e-7 + " °S", 0, 1);
			}
			if(longitude > 0) {
				gpsTable.setValueAt(longitude * 1e-7 + " °E", 0, 2);
			} else {
				gpsTable.setValueAt(-longitude * 1e-7 + " °W", 0, 2);
			}
			gpsTable.setValueAt(altitude / 1000.0, 0, 3);
			gpsTable.setValueAt(precision / 1000.0, 0, 4);
			gpsTable.setValueAt(velocity / 100.0, 0, 5);
			gpsTable.setValueAt(azimuth * 1e-5, 0, 6);
		});
	}

	@Override
	public void setBarometer(double pressure) {
		SwingUtilities.invokeLater(() -> {
			restTable.setValueAt(pressure, 0, 0);
		});
	}

	@Override
	public void setDistanceFront(float cm) {
		SwingUtilities.invokeLater(() -> {
			restTable.setValueAt(cm, 0, 1);
		});
	}

	@Override
	public void setDistanceBottom(float cm) {
		SwingUtilities.invokeLater(() -> {
			restTable.setValueAt(cm, 0, 2);
		});
	}

}
