package gui;

import java.awt.Dimension;
import java.awt.Font;
import java.util.Hashtable;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import controller.ActuatorController;

public class ActuatorPanel extends JPanel implements ActuatorController, ChangeListener {
	
	private static final int SLIDER_ZERO_MARGIN_MOTOR = 20; //in percent
	private static final int SLIDER_ZERO_MARGIN_RUDDER = 15; //in percent
	private static final int SLIDER_SPACING = 20;
	private static final int ACTUATOR_SPACING = 50;
	private static final int SLIDER_LABEL_SIZE = 11;
	
	private static final int MOTOR_MAX = ActuatorController.THRUST_MAX + ActuatorController.THRUST_MAX * SLIDER_ZERO_MARGIN_MOTOR / 200;
	private static final int MOTOR_ZERO = ActuatorController.THRUST_MAX * SLIDER_ZERO_MARGIN_MOTOR / 200;
	private static final int RUDDER_MAX = ActuatorController.ANGLE_MAX + ActuatorController.ANGLE_MAX * SLIDER_ZERO_MARGIN_RUDDER / 200;
	private static final int RUDDER_ZERO = ActuatorController.ANGLE_MAX * SLIDER_ZERO_MARGIN_RUDDER / 200;
	
	private ActuatorController.Listener listener;
	
	private JSlider sliderLeftMotor;
	private JSlider sliderRightMotor;
	
	private JSlider sliderLeftRudder;
	private JSlider sliderRightRudder;
	private JSlider sliderTopRudder;
	
	public ActuatorPanel() {
		//create the slider instances
		sliderLeftMotor = new JSlider(JSlider.HORIZONTAL, -MOTOR_MAX, MOTOR_MAX, 0);
		sliderRightMotor = new JSlider(JSlider.HORIZONTAL, -MOTOR_MAX, MOTOR_MAX, 0);
		
		sliderLeftRudder = new JSlider(JSlider.HORIZONTAL, -RUDDER_MAX, RUDDER_MAX, 0);
		sliderRightRudder = new JSlider(JSlider.HORIZONTAL, -RUDDER_MAX, RUDDER_MAX, 0);
		sliderTopRudder = new JSlider(JSlider.HORIZONTAL, -RUDDER_MAX, RUDDER_MAX, 0);
		
		//set the Layout of the Panel
		setLayout(new BoxLayout(this, BoxLayout.PAGE_AXIS));
		
		//create and add the labels for the sliders
		JSlider[] mSliders = new JSlider[] {sliderLeftMotor, sliderRightMotor};
		setSliderLabelsAndListener(mSliders, MOTOR_MAX, MOTOR_ZERO, "forward", "backward", "off");
		JSlider[] rSliders = new JSlider[] {sliderLeftRudder, sliderRightRudder, sliderTopRudder};
		setSliderLabelsAndListener(rSliders, RUDDER_MAX, RUDDER_ZERO, "90°", "-90°", "0°");
		
		//add the sliders with a label to the Panel
		JLabel lmLabel = new JLabel("Left Motor (+Q,-A) ", JLabel.LEFT);
		add(lmLabel);
		add(sliderLeftMotor);
		
		add(Box.createRigidArea(new Dimension(0, SLIDER_SPACING)));
		
		JLabel rmLabel = new JLabel("Right Motor (+W,-S)", JLabel.LEFT);
		add(rmLabel);
		add(sliderRightMotor);
		
		add(Box.createRigidArea(new Dimension(0, ACTUATOR_SPACING)));
		
		JLabel lrLabel = new JLabel("Left Rudder (+U, -J)", JLabel.LEFT);
		add(lrLabel);
		add(sliderLeftRudder);
		
		add(Box.createRigidArea(new Dimension(0, SLIDER_SPACING)));
		
		JLabel rrLabel = new JLabel("Right Rudder (+I, -K)", JLabel.LEFT);
		add(rrLabel);
		add(sliderRightRudder);
		
		add(Box.createRigidArea(new Dimension(0, SLIDER_SPACING)));
		
		JLabel trLabel = new JLabel("Top Rudder (+O, -L)", JLabel.LEFT);
		add(trLabel);
		add(sliderTopRudder);
	}
	
	private void setSliderLabelsAndListener(JSlider[] sliders, int max, int zero, String maxLabel, String minLabel, String zeroLabel) {
		JLabel label;
		Font font = new Font(Font.SANS_SERIF,Font.PLAIN, SLIDER_LABEL_SIZE);
		Hashtable<Integer, JLabel> table;
		
		table = new Hashtable<Integer, JLabel>();
		label = new JLabel(zeroLabel);
		label.setFont(font);
		table.put(Integer.valueOf(-zero), label);
		label = new JLabel(zeroLabel);
		label.setFont(font);
		table.put(Integer.valueOf(zero), label);
		label = new JLabel(minLabel);
		label.setFont(font);
		table.put(Integer.valueOf(-max), label);
		label = new JLabel(maxLabel);
		label.setFont(font);
		table.put(Integer.valueOf(max), label);
		for(JSlider s : sliders) {
			s.setLabelTable(table);
			s.setPaintLabels(true);
			s.setMajorTickSpacing(zero);
			s.setPaintTicks(true);
			s.addChangeListener(this);
			//s.setPaintTrack(false);
		}
	}
	
	@Override
	public void stateChanged(ChangeEvent e) {
		if(listener == null)
			return;
		if(e.getSource() == sliderLeftMotor && !sliderLeftMotor.getValueIsAdjusting()) {
			listener.onLeftMotorChanged(getMotorValue(sliderLeftMotor.getValue()));
		} else if(e.getSource() == sliderRightMotor && !sliderRightMotor.getValueIsAdjusting()) {
			listener.onRightMotorChanged(getMotorValue(sliderRightMotor.getValue()));
		} else if(e.getSource() == sliderLeftRudder && !sliderLeftRudder.getValueIsAdjusting()) {
			listener.onLeftRudderChanged(getRudderValue(sliderLeftRudder.getValue()));
		} else if(e.getSource() == sliderRightRudder && !sliderRightRudder.getValueIsAdjusting()) {
			listener.onRightRudderChanged(getRudderValue(sliderRightRudder.getValue()));
		} else if(e.getSource() == sliderTopRudder && !sliderTopRudder.getValueIsAdjusting()) {
			listener.onTopRudderChanged(getRudderValue(sliderTopRudder.getValue()));
		}
	}

	@Override
	public void setListener(ActuatorController.Listener l) {
		listener = l;
	}

	@Override
	public void setLeftMotor(int thrust) {
		if(thrust > 0)
			thrust += MOTOR_ZERO;
		else if(thrust < 0)
			thrust -= MOTOR_ZERO;
		sliderLeftMotor.setValue(thrust);
	}

	@Override
	public void setRightMotor(int thrust) {
		if(thrust > 0)
			thrust += MOTOR_ZERO;
		else if(thrust < 0)
			thrust -= MOTOR_ZERO;
		sliderRightMotor.setValue(thrust);
	}

	@Override
	public void setLeftRudder(int angle) {
		if(angle > 0)
			angle += RUDDER_ZERO;
		else if(angle < 0)
			angle -= RUDDER_ZERO;
		sliderLeftRudder.setValue(angle);
	}

	@Override
	public void setRightRudder(int angle) {
		if(angle > 0)
			angle += RUDDER_ZERO;
		else if(angle < 0)
			angle -= RUDDER_ZERO;
		sliderRightRudder.setValue(angle);
	}

	@Override
	public void setTopRudder(int angle) {
		if(angle > 0)
			angle += RUDDER_ZERO;
		else if(angle < 0)
			angle -= RUDDER_ZERO;
		sliderTopRudder.setValue(angle);
	}
	
	private int getMotorValue(int sliderValue) {
		if(sliderValue > MOTOR_ZERO) {
			sliderValue -= MOTOR_ZERO;
		} else if(sliderValue < -MOTOR_ZERO) {
			sliderValue += MOTOR_ZERO;
		} else {
			sliderValue = 0;
		}
		return sliderValue;
	}
	
	private int getRudderValue(int sliderValue) {
		if(sliderValue > RUDDER_ZERO) {
			sliderValue -= RUDDER_ZERO;
		} else if(sliderValue < -RUDDER_ZERO) {
			sliderValue += RUDDER_ZERO;
		} else {
			sliderValue = 0;
		}
		return sliderValue;
	}

	@Override
	public int getLeftMotor() {
		return getMotorValue(sliderLeftMotor.getValue());
	}

	@Override
	public int getRightMotor() {
		return getMotorValue(sliderRightMotor.getValue());
	}

	@Override
	public int getLeftRudder() {
		return getRudderValue(sliderLeftRudder.getValue());
	}

	@Override
	public int getRightRudder() {
		return getRudderValue(sliderRightRudder.getValue());
	}

	@Override
	public int getTopRudder() {
		return getRudderValue(sliderTopRudder.getValue());
	}

}
