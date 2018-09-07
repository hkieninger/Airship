package gui;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.AbstractAction;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.KeyStroke;

import controller.Pool;
import controller.data.ConfDevice;
import controller.data.object.ConnectionData;
import controller.data.object.UByteVector;
import controller.data.parameter.ConfSteering;
import gui.component.Slider2D;

public class SteeringPanel extends JPanel {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private static final int BUTTON_HEIGHT = 20;
	
	private JSlider sliderVelocity;
	private Slider2D sliderDirection;
	private JButton zeroButton, callibrateButton;

	public SteeringPanel(Pool<ConfDevice> pool) {
		//create the components
		sliderVelocity = new JSlider(JSlider.HORIZONTAL, 0, ConfSteering.MAX, 0);
		sliderVelocity.setFocusable(false);
		BufferedImage knob, background;
		try {
			knob = ImageIO.read(new File("res/zeppelin_knob.png"));
			background = ImageIO.read(new File("res/himmel_background.jpg"));
		} catch (IOException e) {
			e.printStackTrace();
			knob = null;
			background = null;
		}
		sliderDirection = new Slider2D(ConfSteering.MAX, ConfSteering.MAX, knob, background);
		sliderDirection.setFocusable(false);
		zeroButton = new JButton("zero");
		zeroButton.setFocusable(false);
		callibrateButton = new JButton("callibrate");
		callibrateButton.setFocusable(false);
		
		//set the listeners
		setFocusable(true);
		sliderVelocity.addChangeListener(e -> {
			if(!sliderVelocity.getValueIsAdjusting()) {
				ConnectionData.UByte data = new ConnectionData.UByte();
				data.val = getVelocity();
				pool.setValue(ConfDevice.STEERING, ConfSteering.VELOCITY, data);
			}
		});
		sliderDirection.addChangeListener(e -> {
			ConnectionData.UByte yaw = new ConnectionData.UByte();
			yaw.val = getYaw();
			pool.setValue(ConfDevice.STEERING, ConfSteering.YAW, yaw);
			ConnectionData.UByte pitch = new ConnectionData.UByte();
			pitch.val = getPitch();
			pool.setValue(ConfDevice.STEERING, ConfSteering.PITCH, pitch);
		});
		zeroButton.addActionListener(e -> {
			setYaw(0);
			setPitch(0);
		});
		callibrateButton.addActionListener(e -> {
			UByteVector data = new UByteVector(2);
			data.val[0] = getYaw();
			data.val[1] = getPitch();
			pool.setValue(ConfDevice.STEERING, ConfSteering.CALLIBRATION, data);
			setYaw(0);
			setPitch(0);
		});
		
		//set key bindings
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke('+'), "plus");
		getActionMap().put("plus", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setVelocity(getVelocity() + ConfSteering.MAX / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke('-'), "minus");
		getActionMap().put("minus", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setVelocity(getVelocity() - ConfSteering.MAX / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("DOWN"), "down");
		getActionMap().put("down", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setPitch(getPitch() - ConfSteering.MAX / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("UP"), "up");
		getActionMap().put("up", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setPitch(getPitch() + ConfSteering.MAX / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("LEFT"), "left");
		getActionMap().put("left", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setYaw(getYaw() - ConfSteering.MAX / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("RIGHT"), "right");
		getActionMap().put("right", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setYaw(getYaw() + ConfSteering.MAX / 10);
			}
		});
		
		//add the components to the layout
		setLayout(new BoxLayout(this, BoxLayout.PAGE_AXIS));
		JLabel label = new JLabel("velocity (+, -)");
		label.setAlignmentX(Component.CENTER_ALIGNMENT);
		add(label);
		add(sliderVelocity);
		add(Box.createRigidArea(new Dimension(0, 20)));
		label = new JLabel("direction (arrows)");
		label.setAlignmentX(Component.CENTER_ALIGNMENT);
		add(label);
		add(sliderDirection);
		JPanel panel = new JPanel();
		panel.setMaximumSize(new Dimension(Integer.MAX_VALUE, BUTTON_HEIGHT));
		panel.setLayout(new GridLayout(0, 2));
		panel.add(zeroButton);
		panel.add(callibrateButton);
		add(panel);
	}

	public void setVelocity(int velocity) {
		sliderVelocity.setValue(velocity);
	}

	public void setYaw(int yaw) {
		sliderDirection.setXSlider(yaw);
	}

	public void setPitch(int pitch) {
		sliderDirection.setYSlider(pitch);
	}

	public int getVelocity() {
		return sliderVelocity.getValue();
	}

	public int getYaw() {
		return sliderDirection.getXSlider();
	}

	public int getPitch() {
		return sliderDirection.getYSlider();
	}
	
}
