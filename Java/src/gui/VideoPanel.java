package gui;

import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.SwingUtilities;

import controller.view.VideoView;

public class VideoPanel extends JLabel implements VideoView {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private BufferedImage image;

	public VideoPanel() {
		
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

	@Override
	public void setImage(byte[] data, Format format) {
		SwingUtilities.invokeLater(() -> {
			ByteArrayInputStream stream = new ByteArrayInputStream(data);
			try {
				image = ImageIO.read(stream);
			} catch (IOException e) {
				e.printStackTrace();
			}
			setIcon(new ImageIcon(image));
		});
	}
	
	
	
}
