package controller.view;

public interface VideoView {
	
	public enum Format {
		RGB, JPG, PNG
	}
	
	public void setImage(byte[] data, Format format);
	
}
