package controller.objects;

/**
 * 
 * @author hans
 *
 * class to represent a location for the autopilot
 * a list of locations represent a route
 *
 */
public class Location {
	int latitude, longitude, altitude;
	
	public Location(int latitude, int longitude, int altitude) {
		this.latitude = latitude;
		this.longitude = longitude;
		this.altitude = altitude;
	}
}

