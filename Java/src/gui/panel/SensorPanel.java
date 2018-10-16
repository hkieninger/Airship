package gui.panel;

import java.awt.Color;
import java.awt.Component;

import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.SwingUtilities;

import controller.Pool;
import controller.data.MeasDevice;
import controller.data.object.ConnectionData;
import controller.data.object.FloatVector;
import controller.data.object.GPSData;
import controller.data.parameter.MeasSensor;
import controller.data.parameter.Parameter;
import gui.component.StaticTableModel;

public class SensorPanel extends JPanel implements Pool.Listener<MeasDevice> {

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
		
		columns = new String[] {"", "Latitude", "Longitude", "Altitude [m]", "Precision [m]", "Velocity [m/s]", "Azimuth [°]", "Satelites"};
		data = new Object[][] {
			{"GPS", "0 °N", "0 °E", 0, 0, 0, 0, 0}
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

	public void setCompass(float x, float y, float z) {
		SwingUtilities.invokeLater(() -> {
			vectorTable.setValueAt(x, 0, 1);
			vectorTable.setValueAt(y, 0, 2);
			vectorTable.setValueAt(z, 0, 3);
		});
	}

	public void setAccelerometer(float x, float y, float z) {
		SwingUtilities.invokeLater(() -> {
			vectorTable.setValueAt(x, 1, 1);
			vectorTable.setValueAt(y, 1, 2);
			vectorTable.setValueAt(z, 1, 3);
		});
	}

	public void setGyroscope(float x, float y, float z) {
		SwingUtilities.invokeLater(() -> {
			vectorTable.setValueAt(x, 2, 1);
			vectorTable.setValueAt(y, 2, 2);
			vectorTable.setValueAt(z, 2, 3);
		});
	}

	public void setGPS(double latitude, double longitude, double altitude, double precision, double velocity, double azimuth, int satelites) {
		SwingUtilities.invokeLater(() -> {
			if(latitude > 0) {
				gpsTable.setValueAt(latitude + " °N", 0, 1);
			} else {
				gpsTable.setValueAt(-latitude + " °S", 0, 1);
			}
			if(longitude > 0) {
				gpsTable.setValueAt(longitude + " °E", 0, 2);
			} else {
				gpsTable.setValueAt(-longitude + " °W", 0, 2);
			}
			gpsTable.setValueAt(altitude, 0, 3);
			gpsTable.setValueAt(precision, 0, 4);
			gpsTable.setValueAt(velocity, 0, 5);
			gpsTable.setValueAt(azimuth, 0, 6);
			gpsTable.setValueAt(satelites, 0, 7);
		});
	}

	public void setBarometer(double pressure) {
		SwingUtilities.invokeLater(() -> {
			restTable.setValueAt(pressure, 0, 0);
		});
	}

	public void setDistanceFront(short mm) {
		SwingUtilities.invokeLater(() -> {
			if(mm == -1)
				restTable.setValueAt("out of range", 0, 1);
			else
				restTable.setValueAt(mm/10.0f, 0, 1);
		});
	}

	public void setDistanceBottom(short mm) {
		SwingUtilities.invokeLater(() -> {
			if(mm == -1)
				restTable.setValueAt("out of range", 0, 2);
			else
				restTable.setValueAt(mm/10.0f, 0, 2);
		});
	}

	@Override
	public void onChanged(Pool<MeasDevice> pool, MeasDevice device, Enum<? extends Parameter> parameter) {
		if(device == MeasDevice.SENSOR) {
			if(parameter == MeasSensor.ACCEL) {
				FloatVector vec = (FloatVector) pool.getValue(device, parameter);
				setAccelerometer(vec.val[0], vec.val[1], vec.val[2]);
			} else if(parameter == MeasSensor.GYRO) {
				FloatVector vec = (FloatVector) pool.getValue(device, parameter);
				setGyroscope(vec.val[0], vec.val[1], vec.val[2]);
			} else if(parameter == MeasSensor.COMPASS) {
				FloatVector vec = (FloatVector) pool.getValue(device, parameter);
				setCompass(vec.val[0], vec.val[1], vec.val[2]);
			} else if(parameter == MeasSensor.GPS) {
				GPSData data = (GPSData) pool.getValue(device, parameter);
				double[] pos = data.getGpsPosition();
				double[] prec = data.getGpsPosPrecision();
				setGPS(pos[0], pos[1], pos[2], prec[0], data.getVelocityOverGround(), data.getGpsHeading(), data.getSattelites());
			} else if(parameter == MeasSensor.BARO) {
				setBarometer(((ConnectionData.Double) pool.getValue(device, parameter)).val);
			} else if(parameter == MeasSensor.DIST_FRONT) {
				setDistanceFront(((ConnectionData.Short) pool.getValue(device, parameter)).val);
			} else if(parameter == MeasSensor.DIST_BOTTOM) {
				setDistanceBottom(((ConnectionData.Short) pool.getValue(device, parameter)).val);
			} else
				return;
		}
	}

}
