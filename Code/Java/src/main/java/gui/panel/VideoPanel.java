package gui.panel;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;

import javax.swing.JComponent;
import javax.swing.SwingUtilities;

import controller.video.*;

public class VideoPanel extends JComponent {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	protected BufferedImage image;
	
	public VideoPanel(VideoConnection connection) {
		connection.addListener((i) -> setImage(i));
	}
	
	public void setImage(BufferedImage image) {
		SwingUtilities.invokeLater(() -> {
			this.image = image;
			repaint();
		});
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
			g2.setColor(Color.BLACK);
			g2.fillRect(0, 0, width, height);
			//draw image
			float imageRatio = 1.0f * image.getWidth() / image.getHeight();
			float panelRatio = 1.0f * width / height;
			if(panelRatio > imageRatio) {
				int scaledWidth = (int) (height * imageRatio);
				Image scaledImage = image.getScaledInstance(scaledWidth, height, Image.SCALE_FAST);
				g2.drawImage(scaledImage, (width - scaledWidth) / 2, 0, null);
			} else {
				int scaledHeight = (int) (width / imageRatio);
				Image scaledImage = image.getScaledInstance(width, scaledHeight, Image.SCALE_FAST);
				g2.drawImage(scaledImage, 0, (height - scaledHeight) / 2, null);
			}
		}
	}

}
