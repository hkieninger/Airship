package gui.panel;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;

import javax.swing.JComponent;
import javax.swing.SwingUtilities;

public class VideoPanel extends JComponent {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	protected BufferedImage image;
	private Image scaledImage;
	private int width, height;
	
	public void setImage(BufferedImage image) {
		SwingUtilities.invokeLater(() -> {
			width = 0;
			height = 0;
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
			if(width != this.width || height != this.height)
				scaledImage = image.getScaledInstance(width, height, Image.SCALE_FAST);
			g2.drawImage(scaledImage, 0, 0, null);
			this.width = width;
			this.height = height;
		}
	}

}
