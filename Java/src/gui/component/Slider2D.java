package gui.component;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.awt.image.BufferedImage;

import javax.swing.JComponent;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class Slider2D extends JComponent {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private static final int LIGHT_GRAY_LINES = 11;
	private static final int KNOB_WIDTH = 80;
	
	private int maxX, maxY;
	private int xSlider, ySlider;
	private int width, height;
	private Image knob;
	private BufferedImage background;
	private Image scaledBackground;
	private Color color = Color.RED;
	
	public Slider2D(int maxX, int maxY, Color color) {
		this(maxX, maxY);
		this.color = color;
	}
	
	public Slider2D(int maxX, int maxY, BufferedImage knob, BufferedImage background) {
		this(maxX, maxY);
		if(knob != null) {
			this.knob = knob.getScaledInstance(KNOB_WIDTH, knob.getHeight() * KNOB_WIDTH / knob.getWidth(), Image.SCALE_DEFAULT);
		}
		this.background = background;
	}
	
	public Slider2D(int maxX, int maxY) {
		this.maxX = maxX;
		this.maxY = maxY;
		
		addMouseListener(new MouseAdapter() {
			public void mousePressed(MouseEvent e) {
				setXWithoutChangeEvent((e.getX() - width / 2) * 2 * maxX / width);
				setYWithoutChangeEvent((height / 2 - e.getY()) * 2 * maxY / height);
			}
			public void mouseReleased(MouseEvent e) {
				fireStateChanged();
			}
		});
		addMouseMotionListener(new MouseMotionAdapter() {
			public void mouseDragged(MouseEvent e) {
				setXWithoutChangeEvent((e.getX() - width / 2) * 2 * maxX / width);
				setYWithoutChangeEvent((height / 2 - e.getY()) * 2 * maxY / height);
			}
		});
	}
	
	public int getXSlider() {
		return xSlider;
	}
	
	public int getYSlider() {
		return ySlider;
	}
	
	public void setXSlider(int xSlider) {
		setXWithoutChangeEvent(xSlider);
		fireStateChanged();
	}
	
	private void setXWithoutChangeEvent(int xSlider) {
		if(xSlider > maxX)
			xSlider = maxX;
		else if(xSlider < -maxX)
			xSlider = -maxX;
		firePropertyChange("xSlider", this.xSlider, xSlider);
		this.xSlider = xSlider;
		repaint();
	}
	
	public void setYSlider(int ySlider) {
		setYWithoutChangeEvent(ySlider);
		fireStateChanged();
	}
	
	private void setYWithoutChangeEvent(int ySlider) {
		if(ySlider > maxY)
			ySlider = maxY;
		else if(ySlider < -maxY)
			ySlider = -maxY;
		firePropertyChange("ySlider", this.ySlider, ySlider);
		this.ySlider = ySlider;
		repaint();
	}
	
	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D) g;
		int width = getWidth();
		int height = getHeight();
		int xSlider = this.xSlider * width / 2 / maxX;
		int ySlider = this.ySlider * height / 2 / maxY;
		//draw background
		if(background == null ) {
			g2.setColor(Color.WHITE);
			g2.fillRect(0, 0, width, height);
		} else {
			if(width != this.width || height != this.height)
				scaledBackground = background.getScaledInstance(width, height, Image.SCALE_FAST);
			g2.drawImage(scaledBackground, 0, 0, null);
		}
		//draw axis and squares
		g2.setColor(Color.LIGHT_GRAY);
		for(int i = 1; i <= LIGHT_GRAY_LINES; i++) {
			int x = width * i / (LIGHT_GRAY_LINES + 1);
			int y = height * i / (LIGHT_GRAY_LINES + 1);
			g2.drawLine(x, 0, x, height);
			g2.drawLine(0, y, width, y);
		}
		g2.setColor(Color.BLACK);
		g2.setStroke(new BasicStroke(2));
		g2.drawLine(0, height / 2, width, height / 2);
		g2.drawLine(width / 2, 0, width / 2, height);
		//draw the knob
		int x = xSlider + width / 2 - KNOB_WIDTH / 2;
		g2.setColor(color);
		if(knob == null)
			g2.fillOval(x, height / 2 - ySlider - KNOB_WIDTH / 2, KNOB_WIDTH, KNOB_WIDTH);
		else {
			int y = height / 2 - ySlider - knob.getHeight(null) / 2;
			if(!g2.drawImage(knob, x, y, null))
				g2.fillOval(x, height / 2 - ySlider - KNOB_WIDTH / 2, KNOB_WIDTH, KNOB_WIDTH);
		}
		this.width = width;
		this.height = height;
	}
	
	public void addChangeListener(ChangeListener l) {
		listenerList.add(ChangeListener.class, l);
	}
	
	private void fireStateChanged() {
		Object[] list = listenerList.getListenerList();
		for(int i = 0; i < list.length; i++) {
			if(list[i] == ChangeListener.class) {
				ChangeListener l = (ChangeListener) list[i + 1];
				l.stateChanged(new ChangeEvent(this));
			}
		}
	}
	
}
