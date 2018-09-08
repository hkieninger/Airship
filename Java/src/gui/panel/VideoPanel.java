package gui.panel;

import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.SwingUtilities;

import controller.*;
import controller.data.*;
import controller.data.object.Picture;
import controller.data.parameter.MeasSensor;
import controller.data.parameter.Parameter;

public class VideoPanel extends JLabel implements Pool.Listener<MeasDevice> {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private BufferedImage image;
	private MeasSensor camera;
	
	public VideoPanel(MeasSensor camera) {
		if(camera != MeasSensor.CAM_BOTTOM && camera != MeasSensor.CAM_FRONT)
			throw new IllegalArgumentException();
		this.camera = camera;
	}
	
	@Override
	public void paintComponent(Graphics g) {
		if(image == null) {
			BufferedImage canvas = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_RGB);
			for(int w = 0; w < canvas.getWidth(); w++) {
				for(int h = 0; h < canvas.getHeight(); h++) {
					canvas.setRGB(w, h, (int) (Math.random() * 0xFFFFFF));
				}
			}
			g.drawImage(canvas, 0, 0, null);
		} else {
			super.paintComponent(g);
		}
	}

	public void setImage(byte[] data, int length) {
		SwingUtilities.invokeLater(() -> {
			ByteArrayInputStream stream = new ByteArrayInputStream(data, 0, length);
			try {
				image = ImageIO.read(stream);
				stream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			setIcon(new ImageIcon(image));
		});
	}

	@Override
	public void onChanged(Pool<MeasDevice> pool, MeasDevice device, Enum<? extends Parameter> parameter) {
		if(device == MeasDevice.SENSOR && parameter == camera) {
			Picture pic = (Picture) pool.getValue(device, parameter);
			setImage(pic.getData(), pic.getSize());
		}
	}
	
	
	
}
