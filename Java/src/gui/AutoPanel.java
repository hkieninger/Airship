package gui;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.event.MouseInputListener;

import org.jxmapviewer.JXMapViewer;
import org.jxmapviewer.OSMTileFactoryInfo;
import org.jxmapviewer.input.PanMouseInputListener;
import org.jxmapviewer.input.ZoomMouseWheelListenerCursor;
import org.jxmapviewer.viewer.DefaultTileFactory;
import org.jxmapviewer.viewer.GeoPosition;

import controller.view.AutoController;
import gui.component.StaticTableModel;

/**
 * 
 * @author hans
 *
 * only shows the rough traits how to class should look like
 *
 */
public class AutoPanel extends JPanel implements AutoController {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private AutoController.Listener listener;
	
	private List<Location> route;
	private JTable table;
	private JXMapViewer mapViewer;
	
	public AutoPanel() {
		setLayout(new GridLayout(1, 2));
		
		//add the map
		mapViewer = new JXMapViewer();
		mapViewer.setTileFactory(new DefaultTileFactory(new OSMTileFactoryInfo()));
		//default position is the Kantonsschule Baden
		GeoPosition position = new GeoPosition(47.470496, 8.313721);
		mapViewer.setAddressLocation(position);
		mapViewer.setZoom(2);
		
		//setup interaction with map (zoom and move)
        MouseInputListener mia = new PanMouseInputListener(mapViewer);
        mapViewer.addMouseListener(mia);
        mapViewer.addMouseMotionListener(mia);
        mapViewer.addMouseWheelListener(new ZoomMouseWheelListenerCursor(mapViewer));
		add(mapViewer);
		
		
		//add the table
		JPanel panel = new JPanel(new BorderLayout());
		String[] columns = new String[] {"Nr.", "Latitude [°]", "Longitude [°]", "Altitude [m]", "Remove"};
		Object[][] data = new Object[][] {
			{0, 47.42434424, 8.84432432, 430, "JButton"}
		};
		table = new JTable(new StaticTableModel(data, columns));
		panel.add(new JScrollPane(table), BorderLayout.CENTER);
		JButton sendButton = new JButton("Send");
		panel.add(sendButton, BorderLayout.SOUTH);
		add(panel);
		
	}

	@Override
	public void setListener(Listener l) {
		listener = l;
	}

	@Override
	public List<Location> getRoute() {
		return null;
	}

}
