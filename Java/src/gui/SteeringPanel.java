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

import controller.pool.SteeringController;
import gui.component.Slider2D;

public class SteeringPanel extends JPanel implements SteeringController {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private static final int BUTTON_HEIGHT = 20;
	
	private JSlider sliderVelocity;
	private Slider2D sliderDirection;
	private JButton zeroButton, callibrateButton;
	
	private SteeringController.Listener listener;

	public SteeringPanel() {
		//create the components
		sliderVelocity = new JSlider(JSlider.HORIZONTAL, 0, SteeringController.MAX_VELOCITY, 0);
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
		sliderDirection = new Slider2D(SteeringController.MAX_YAW, SteeringController.MAX_PITCH, knob, background);
		sliderDirection.setFocusable(false);
		zeroButton = new JButton("zero");
		zeroButton.setFocusable(false);
		callibrateButton = new JButton("callibrate");
		callibrateButton.setFocusable(false);
		
		//set the listeners
		setFocusable(true);
		sliderVelocity.addChangeListener(e -> {
			if(listener != null && !sliderVelocity.getValueIsAdjusting())
				listener.onVelocityChanged(getVelocity());
		});
		sliderDirection.addChangeListener(e -> {
			if(listener != null) {
				listener.onYawChanged(getYaw());
				listener.onPitchChanged(getPitch());
			}
		});
		zeroButton.addActionListener(e -> {
			setYaw(0);
			setPitch(0);
		});
		callibrateButton.addActionListener(e -> {
			if(listener != null) {
				setYaw(0);
				setPitch(0);
				listener.onCallibrated(getYaw(), getPitch());
			}
		});
		
		//set key bindings
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke('+'), "plus");
		getActionMap().put("plus", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setVelocity(getVelocity() + SteeringController.MAX_VELOCITY / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke('-'), "minus");
		getActionMap().put("minus", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setVelocity(getVelocity() - SteeringController.MAX_VELOCITY / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("DOWN"), "down");
		getActionMap().put("down", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setPitch(getPitch() - SteeringController.MAX_PITCH / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("UP"), "up");
		getActionMap().put("up", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setPitch(getPitch() + SteeringController.MAX_PITCH / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("LEFT"), "left");
		getActionMap().put("left", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setYaw(getYaw() - SteeringController.MAX_YAW / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("RIGHT"), "right");
		getActionMap().put("right", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setYaw(getYaw() + SteeringController.MAX_YAW / 10);
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

	@Override
	public void setListener(Listener l) {
		listener = l;
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

	@Override
	public int getVelocity() {
		return sliderVelocity.getValue();
	}

	@Override
	public int getYaw() {
		return sliderDirection.getXSlider();
	}

	@Override
	public int getPitch() {
		return sliderDirection.getYSlider();
	}
	
}
