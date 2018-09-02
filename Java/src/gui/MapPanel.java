package gui;

import java.awt.BorderLayout;
import java.awt.event.ItemEvent;
import java.util.HashSet;
import java.util.Set;

import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.event.MouseInputListener;

import org.jxmapviewer.JXMapViewer;
import org.jxmapviewer.OSMTileFactoryInfo;
import org.jxmapviewer.VirtualEarthTileFactoryInfo;
import org.jxmapviewer.input.PanMouseInputListener;
import org.jxmapviewer.input.ZoomMouseWheelListenerCursor;
import org.jxmapviewer.viewer.DefaultTileFactory;
import org.jxmapviewer.viewer.DefaultWaypoint;
import org.jxmapviewer.viewer.GeoPosition;
import org.jxmapviewer.viewer.TileFactory;
import org.jxmapviewer.viewer.Waypoint;
import org.jxmapviewer.viewer.WaypointPainter;

import controller.view.MapView;

public class MapPanel extends JPanel implements MapView {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private JXMapViewer mapViewer;
	private TileFactory[] factories;
	private DefaultWaypoint waypoint;
	
	public MapPanel() {
		setLayout(new BorderLayout());
		//create the map factories
		factories = new TileFactory[3];
		//open street map
		factories[0] = new DefaultTileFactory(new OSMTileFactoryInfo());
		//proprietary Microsoft
		factories[1] = new DefaultTileFactory(new VirtualEarthTileFactoryInfo(VirtualEarthTileFactoryInfo.HYBRID));
		//porprietary Microsoft
		factories[2] = new DefaultTileFactory(new VirtualEarthTileFactoryInfo(VirtualEarthTileFactoryInfo.SATELLITE));
		
		//create the Mapviewer
		mapViewer = new JXMapViewer();
		mapViewer.setTileFactory(factories[0]);
		//default position is the Kantonsschule Baden
		GeoPosition position = new GeoPosition(47.470496, 8.313721);
		mapViewer.setAddressLocation(position);
		mapViewer.setZoom(2);
		
		//setup interaction with map (zoom and move)
        MouseInputListener mia = new PanMouseInputListener(mapViewer);
        mapViewer.addMouseListener(mia);
        mapViewer.addMouseMotionListener(mia);
        mapViewer.addMouseWheelListener(new ZoomMouseWheelListenerCursor(mapViewer));
        
		add(mapViewer, BorderLayout.CENTER);
		
        //selection of tile factory
        JComboBox<String> comboBox = new JComboBox<String>(new String[] {"Open Street Map", "Virtual Earth Hybrid", "Virtual Earth Satellite"});
        comboBox.addItemListener((ItemEvent e) -> {
        	mapViewer.setTileFactory(factories[comboBox.getSelectedIndex()]);
        });
        add(comboBox, BorderLayout.NORTH);
        
        //way point to display location of airship
        WaypointPainter<Waypoint> waypointPainter = new WaypointPainter<Waypoint>();
        Set<Waypoint> set = new HashSet<Waypoint>();
        waypoint = new DefaultWaypoint();
        set.add(waypoint);
        waypointPainter.setWaypoints(set);
        mapViewer.setOverlayPainter(waypointPainter);
	}

	@Override
	public void setPosition(int latitude, int longitude) {
		GeoPosition pos = new GeoPosition(latitude * 1e-7, longitude * 1e-7);
		waypoint.setPosition(pos);
		mapViewer.setAddressLocation(pos);
	}

}
