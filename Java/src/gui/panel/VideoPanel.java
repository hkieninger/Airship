package gui.panel;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JComponent;
import javax.swing.SwingUtilities;

import controller.Pool;
import controller.data.MeasDevice;
import controller.data.object.Picture;
import controller.data.parameter.MeasSensor;
import controller.data.parameter.Parameter;

public class VideoPanel extends JComponent implements Pool.Listener<MeasDevice> {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private BufferedImage image;
	private Image scaledImage;
	private int width, height;
	
	private MeasSensor camera;
	
	public VideoPanel(MeasSensor camera) {
		if(camera != MeasSensor.CAM_BOTTOM && camera != MeasSensor.CAM_FRONT)
			throw new IllegalArgumentException();
		this.camera = camera;
		if(camera == MeasSensor.CAM_BOTTOM) {
			try {
				image = ImageIO.read(new File("res/vlc.jpg"));
			} catch (IOException e) {
				//e.printStackTrace();
				image = null;
			}
		}
	}
	
	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D) g;
		if(image == null) {
			BufferedImage canvas = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_RGB);
			for(int w = 0; w < canvas.getWidth(); w++) {
				for(int h = 0; h < canvas.getHeight(); h++) {
					canvas.setRGB(w, h, (int) (Math.random() * 0xFFFFFF));
				}
			}
			g2.drawImage(canvas, 0, 0, null);
		} else {
			int width = getWidth();
			int height = getHeight();
			//draw background
			if(width != this.width || height != this.height)
				scaledImage = image.getScaledInstance(width, height, Image.SCALE_FAST);
			g2.drawImage(scaledImage, 0, 0, null);
			this.width = width;
			this.height = height;
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
			repaint();
		});
	}

	@Override
	public void onChanged(Pool<MeasDevice> pool, MeasDevice device, Enum<? extends Parameter> parameter) {
		if(device == MeasDevice.SENSOR && parameter == camera) {
			Picture pic = (Picture) pool.getValue(device, parameter);
			width = 0;
			height = 0;
			setImage(pic.getData(), pic.getSize());
		}
	}
	
	
	
}
