package controller;

public interface VideoView {
	
	public final static int RGB = 0;
	//public final static int JPEG = 1;
	
	public void setImage(byte[] data, int format);
	
}
