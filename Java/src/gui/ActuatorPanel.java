package gui;

import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.util.Hashtable;

import javax.swing.AbstractAction;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.KeyStroke;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import controller.data.parameter.ConfActuator;
import controller.Pool;
import controller.data.ConfDevice;
import controller.data.object.ConnectionData;

public class ActuatorPanel extends JPanel implements ChangeListener {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private static final int SLIDER_ZERO_MARGIN_MOTOR = 20; //in percent
	private static final int SLIDER_ZERO_MARGIN_RUDDER = 15; //in percent
	private static final int SLIDER_SPACING = 20;
	private static final int ACTUATOR_SPACING = 50;
	private static final int SLIDER_LABEL_SIZE = 11;
	
	private static final int MOTOR_MAX = ConfActuator.MOTOR_THRUST_MAX + ConfActuator.MOTOR_THRUST_MAX * SLIDER_ZERO_MARGIN_MOTOR / 200;
	private static final int MOTOR_ZERO = ConfActuator.MOTOR_THRUST_MAX * SLIDER_ZERO_MARGIN_MOTOR / 200;
	private static final int RUDDER_MAX = ConfActuator.RUDDER_ANGLE_MAX + ConfActuator.RUDDER_ANGLE_MAX * SLIDER_ZERO_MARGIN_RUDDER / 200;
	private static final int RUDDER_ZERO = ConfActuator.RUDDER_ANGLE_MAX * SLIDER_ZERO_MARGIN_RUDDER / 200;
	
	private Pool<ConfDevice> pool;
	
	private JSlider sliderLeftMotor;
	private JSlider sliderRightMotor;
	
	private JSlider sliderLeftRudder;
	private JSlider sliderRightRudder;
	private JSlider sliderTopRudder;
	
	public ActuatorPanel(Pool<ConfDevice> pool) {
		this.pool = pool;
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
		
		//set keyboard shortcuts
		setFocusable(true);
		setSliderKeyboardShortcut(sliderLeftMotor, 'q', 'a', ConfActuator.MOTOR_THRUST_MAX);
		setSliderKeyboardShortcut(sliderRightMotor, 'w', 's', ConfActuator.MOTOR_THRUST_MAX);
		setSliderKeyboardShortcut(sliderTopRudder, 'o', 'l', ConfActuator.RUDDER_ANGLE_MAX);
		setSliderKeyboardShortcut(sliderLeftRudder, 'u', 'j', ConfActuator.RUDDER_ANGLE_MAX);
		setSliderKeyboardShortcut(sliderRightRudder, 'i', 'k', ConfActuator.RUDDER_ANGLE_MAX);
	}
	
	private void setSliderKeyboardShortcut(JSlider slider, char plus, char minus, int max) {
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(plus), "+ " + plus);
		getActionMap().put("+ " + plus, new AbstractAction() {
			private static final long serialVersionUID = 1L;

			@Override
			public void actionPerformed(ActionEvent arg0) {
				slider.setValue(slider.getValue() + max / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(minus), "- " + minus);
		getActionMap().put("- " + minus, new AbstractAction() {
			private static final long serialVersionUID = 1L;

			@Override
			public void actionPerformed(ActionEvent arg0) {
				slider.setValue(slider.getValue() - max / 10);
			}
		});
		slider.setFocusable(false);
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
		ConnectionData.UByte data = new ConnectionData.UByte();
		if(e.getSource() == sliderLeftMotor && !sliderLeftMotor.getValueIsAdjusting()) {
			data.val = getLeftRudder();
			pool.setValue(ConfDevice.ACTUATOR, ConfActuator.LEFT_MOTOR, data);
		} else if(e.getSource() == sliderRightMotor && !sliderRightMotor.getValueIsAdjusting()) {
			data.val = getRightMotor();
			pool.setValue(ConfDevice.ACTUATOR, ConfActuator.RIGHT_MOTOR, data);
		} else if(e.getSource() == sliderLeftRudder && !sliderLeftRudder.getValueIsAdjusting()) {
			data.val = getLeftRudder();
			pool.setValue(ConfDevice.ACTUATOR, ConfActuator.LEFT_RUDDER, data);
		} else if(e.getSource() == sliderRightRudder && !sliderRightRudder.getValueIsAdjusting()) {
			data.val = getRightRudder();
			pool.setValue(ConfDevice.ACTUATOR, ConfActuator.RIGHT_RUDDER, data);;
		} else if(e.getSource() == sliderTopRudder && !sliderTopRudder.getValueIsAdjusting()) {
			data.val = getTopRudder();
			pool.setValue(ConfDevice.ACTUATOR, ConfActuator.TOP_RUDDER, data);
		}
		
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

	public int getLeftMotor() {
		return getMotorValue(sliderLeftMotor.getValue());
	}

	public int getRightMotor() {
		return getMotorValue(sliderRightMotor.getValue());
	}

	public int getLeftRudder() {
		return getRudderValue(sliderLeftRudder.getValue());
	}

	public int getRightRudder() {
		return getRudderValue(sliderRightRudder.getValue());
	}

	public int getTopRudder() {
		return getRudderValue(sliderTopRudder.getValue());
	}

}
