package controller.data.object;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * 
 * @author hans
 *
 * class to represent a location for the autopilot
 * a list of locations represent a route
 *
 */
public class Route implements ConnectionData {
	
	private List<Location> route;
	
	public Route() {
		route = new ArrayList<>();
	}
	
	public void addLocation(Location l) {
		route.add(l);
	}
	
	public void removeLocation(Location l) {
		route.remove(l);
	}

	@Override
	public void send(DataOutputStream out) throws IOException {
		for(Location l : route) {
			out.writeInt(l.latitude);
			out.writeInt(l.longitude);
			out.writeInt(l.altitude);
		}
	}

	@Override
	public void receive(DataInputStream in) {
		//not needed yet
		throw new UnsupportedOperationException("Not implemented yet.");
	}
	
	public static class Location {
		public int latitude, longitude, altitude;
		
		public Location(int latitude, int longitude, int altitude) {
			this.latitude = latitude;
			this.longitude = longitude;
			this.altitude = altitude;
		}
		
	}
	
}

