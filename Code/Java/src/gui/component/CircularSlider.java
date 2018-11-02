package gui.component;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;

import javax.swing.JComponent;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

/**
 * A slider with a circular form, the value ranges between 0 and 2 pi
 */
public class CircularSlider extends JComponent {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private static final int STROKE_WIDTH = 2;
	private static final int KNOB_RADIUS = 15;
	
	private double value, offset;
	private int radius, centerX, centerY;
	private Color circleColor, knobColor;
	
	/**
	 * Creates a circular slider
	 * @param initalValue of the slider
	 * @param offset defines the zero point
	 */
	public CircularSlider(double initalValue, double offset) {
		this.offset = offset;
		value = normalizeAngle(initalValue + offset);
		setForeground(Color.BLACK);
		circleColor = Color.BLACK;
		knobColor = Color.GREEN;
		addMouseListener(new MouseAdapter( ) {
			public void mousePressed(MouseEvent e) {
				spin(e);
			}
			public void mouseReleased(MouseEvent e) {
			    fireStateChanged();
			}
	    });
	    addMouseMotionListener(new MouseMotionAdapter( ) {
	    	public void mouseDragged(MouseEvent e) {
	    		spin(e);
	    	}
	    });
	}
	
	public void setCircleColor(Color c) {
		circleColor = c;
	}
	
	public void setKnobColor(Color c) {
		knobColor = c;
	}
	
	@Override
	public void paintComponent(Graphics g) {
		//update some attributes
		int width = getSize().width, height = getSize().height;
		centerX = width / 2;
		centerY = height / 2;
		radius = (width < height) ? width : height;
		radius /= 2;
		radius -=  KNOB_RADIUS + 1;
		//draw the slider path
		Graphics2D g2d = (Graphics2D) g;
		g2d.setStroke(new BasicStroke(STROKE_WIDTH));
		g2d.setColor(circleColor);
		g2d.drawOval(centerX - radius, centerY - radius, 2 * radius, 2 * radius);
		//draw the slider circle
		int knobX = centerX + (int) (radius * Math.cos(value)), knobY = centerY - (int) (radius * Math.sin(value));
		g2d.setColor(knobColor);
		g2d.fillOval(knobX - KNOB_RADIUS, knobY - KNOB_RADIUS, 2 * KNOB_RADIUS, 2 * KNOB_RADIUS);
	}
	
	/**
	 * retuns the value of the slider in radian
	 * @return a number between 0 and 2 pi
	 */
	public double getValue() {
		return normalizeAngle(value - offset);
	}
	
	/**
	 * sets the value of the slider at updates the gui
	 * @param value any real number, the value is in radian
	 */
	public void setValue(double value) {
		setValueWithoutFiring(value);
		fireStateChanged();
	}
	
	private void setValueWithoutFiring(double value) {
		value = normalizeAngle(value + offset);
		firePropertyChange("value", this.value, value );
	    repaint();
		this.value = value;
	}
	
	/**
	 * change events are send when value is set from outside or after the user has released the mouse after changing the value
	 * @param l
	 */
	public void addChangeListener(ChangeListener l) {
		listenerList.add(ChangeListener.class, l);
	}
	
	private double normalizeAngle(double angle) {
		angle %= 2 * Math.PI;
		if(angle < 0)
			angle += 2 * Math.PI;
		return angle;
	}
	
	private void spin(MouseEvent e) {
		int x = e.getX() - centerX, y = centerY - e.getY();
		double angle;
		if(x == 0) {
			if(y > 0) {
				angle = Math.PI / 2;
			} else {
				angle = -Math.PI / 2;
			}
		} else {
			angle = Math.atan(1.0 * y / x);
			if(x < 0) {
				angle += Math.PI;
			}
		}
		setValueWithoutFiring(angle - offset);
	}

	private void fireStateChanged() {
		Object[] listeners = listenerList.getListenerList();
		for(int i = 0; i < listeners.length; i++) {
			if(listeners[i] == ChangeListener.class) {
				ChangeListener l = (ChangeListener) listeners[i + 1];
				l.stateChanged(new ChangeEvent(this));
			}
		}
	}
	
}
