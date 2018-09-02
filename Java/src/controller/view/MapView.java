package controller.view;

public interface MapView {
	
	/**
	 * sets the latitude and longitude of the zeppelin
	 * @param latitude ° scaled with 1e-7
	 * @param longitude ° scaled with 1e-7
	 */
	void setPosition(int latitude, int longitude);

}
