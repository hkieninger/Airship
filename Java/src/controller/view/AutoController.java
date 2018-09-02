package controller.view;

import java.util.List;

public interface AutoController {
	
	void setListener(Listener l);
	
	List<Location> getRoute();
	
	public interface Listener {
		void onRouteChanged(List<Location> route);
	}
	
	public class Location {
		int latitude, longitude, altitude;
		
		public Location(int latitude, int longitude, int altitude) {
			this.latitude = latitude;
			this.longitude = longitude;
			this.altitude = altitude;
		}
	}
	
}
