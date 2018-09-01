package gui;

import javax.swing.BoxLayout;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.SwingUtilities;

import controller.view.SensorView;
import gui.component.StaticTableModel;

public class SensorPanel extends JPanel implements SensorView {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private JTable vectorTable, gpsTable, restTable;

	public SensorPanel() {
		setLayout(new BoxLayout(this, BoxLayout.PAGE_AXIS));
		
		String[] columns = new String[] {"Sensor", "x", "y", "z"};
		Object[][] data = new Object[][] {
			{"Compass [Gs]", 0, 0, 0},
			{"Accelerometer [m/s²]", 0, 0, 0},
			{"Gyroscope [°/s]", 0, 0, 0}
		};
		vectorTable = new JTable(new StaticTableModel(data, columns));
		vectorTable.getTableHeader().setReorderingAllowed(false);
		vectorTable.setFocusable(false);
		add(new JScrollPane(vectorTable));
		
		columns = new String[] {"Device", "Latitude", "Longitude", "Altitude [m]", "Precision [m]", "Velocity [m/s]", "Azimuth [°]"};
		data = new Object[][] {
			{"GPS", "0 °N", "0 °E", 0, 0, 0, 0}
		};
		gpsTable = new JTable(new StaticTableModel(data, columns));
		gpsTable.getTableHeader().setReorderingAllowed(false);
		gpsTable.setFocusable(false);
		add(new JScrollPane(gpsTable));
		
		columns = new String[] {"Barometer [Pa]", "Distance front [cm]", "Distance bottom [cm]"};
		data = new Object[][] {
			{0, 0, 0}
		};
		restTable = new JTable(new StaticTableModel(data, columns));
		restTable.getTableHeader().setReorderingAllowed(false);
		restTable.setFocusable(false);
		add(new JScrollPane(restTable));
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
				gpsTable.setValueAt(latitude * 10e-7 + " °N", 0, 1);
			} else {
				gpsTable.setValueAt(-latitude * 10e-7 + " °S", 0, 1);
			}
			if(longitude > 0) {
				gpsTable.setValueAt(longitude * 10e-7 + " °E", 0, 2);
			} else {
				gpsTable.setValueAt(-longitude * 10e-7 + " °W", 0, 2);
			}
			gpsTable.setValueAt(altitude / 1000.0, 0, 3);
			gpsTable.setValueAt(precision / 1000.0, 0, 4);
			gpsTable.setValueAt(velocity / 100.0, 0, 5);
			gpsTable.setValueAt(azimuth * 10e-5, 0, 6);
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
