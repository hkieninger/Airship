package gui;

import javax.swing.BoxLayout;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import controller.SteeringController;
import gui.component.CircularSlider;

public class SteeringPanel extends JPanel implements ChangeListener, SteeringController {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private static final int SLIDER_MAX = 100;
	private static final int SLIDER_ZERO_MARGIN = 20; //in percent
	private static final int SLIDER_SPEED_MAX = SLIDER_MAX + SLIDER_MAX * SLIDER_ZERO_MARGIN / 200;
	
	private JSlider sliderSpeed, sliderRadius;
	private CircularSlider sliderAngle;
	
	private SteeringController.Listener listener;

	public SteeringPanel() {
		sliderSpeed = new JSlider(JSlider.VERTICAL, -SLIDER_SPEED_MAX, SLIDER_SPEED_MAX, 0);
		sliderRadius = new JSlider(JSlider.VERTICAL, 0, SLIDER_MAX, 0);
		sliderAngle = new CircularSlider(0, -Math.PI / 2);
		
		setLayout(new BoxLayout(this, BoxLayout.LINE_AXIS));
		
		sliderSpeed.addChangeListener(this);
		sliderRadius.addChangeListener(this);
		sliderAngle.addChangeListener(this);
		
		add(sliderSpeed);
		add(sliderAngle);
		add(sliderRadius);
	}

	@Override
	public void stateChanged(ChangeEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setListener(Listener l) {
		listener = l;
	}

	@Override
	public void setVelocity(double velocity) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setAngle(double angle) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setRadius(double radius) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public double getVelocity() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public double getAngle() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public double getRadius() {
		// TODO Auto-generated method stub
		return 0;
	}

	
	
}
