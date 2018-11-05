package gui.panel;

import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.Hashtable;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.AbstractAction;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

import controller.Pool;
import controller.data.ConfDevice;
import controller.data.object.ByteVector;
import controller.data.object.ConnectionData;
import controller.data.parameter.ConfSteering;
import controller.video.VideoConnection;
import gui.component.Slider2D;
import net.java.games.input.Component;
import net.java.games.input.Component.Identifier;
import net.java.games.input.Controller;
import net.java.games.input.ControllerEnvironment;
import net.java.games.input.ControllerEvent;
import net.java.games.input.ControllerListener;

public class SteeringPanel extends JPanel implements WindowListener {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private static final int BUTTON_HEIGHT = 20;
	
	private static final int SLIDER_LABEL_SIZE = 11;
	private static final int SLIDER_ZERO_MARGIN_VELOCITY = 20;
	private static final int VELOCITY_MAX = ConfSteering.MAX + ConfSteering.MAX * SLIDER_ZERO_MARGIN_VELOCITY / 200;
	private static final int VELOCITY_ZERO = ConfSteering.MAX * SLIDER_ZERO_MARGIN_VELOCITY / 200;
	
	private static final long TIMER_PERIOD_REDUCE = 300; //ms
	private static final int TIMER_STEP = (int) (TIMER_PERIOD_REDUCE * ConfSteering.MAX / 1000 / 10);
	
	private static final long TIMER_PERIOD_GAMEPAD = 100; //ms
	
	private JSlider sliderVelocity;
	private Slider2D sliderDirection;
	private JButton zeroButton, callibrateButton;
	
	private Controller gamepad;
	private Timer timer;

	public SteeringPanel(Pool<ConfDevice> pool, VideoConnection videoBackground) {
		//create the components
		
		sliderVelocity = new JSlider(JSlider.HORIZONTAL, -VELOCITY_MAX, VELOCITY_MAX, 0);
		setSliderLabels(sliderVelocity, VELOCITY_MAX, VELOCITY_ZERO, "foward", "backward", "off");
		sliderVelocity.setFocusable(false);
		sliderDirection = new Slider2D(ConfSteering.MAX, ConfSteering.MAX, videoBackground);
		sliderDirection.setFocusable(false);
		zeroButton = new JButton("zero");
		zeroButton.setFocusable(false);
		callibrateButton = new JButton("callibrate");
		callibrateButton.setFocusable(false);
		
		//set the listeners
		setFocusable(true);
		sliderVelocity.addChangeListener(e -> {
			if(!sliderVelocity.getValueIsAdjusting()) {
				ConnectionData.Byte data = new ConnectionData.Byte();
				data.val = getVelocity();
				pool.setValue(ConfDevice.STEERING, ConfSteering.VELOCITY, data);
			}
		});
		sliderDirection.addChangeListener(e -> {
			ByteVector data = new ByteVector(2);
			data.val[0] = getYaw();
			data.val[1] = getPitch();
			pool.setValue(ConfDevice.STEERING, ConfSteering.DIRECTION, data);
		});
		zeroButton.addActionListener(e -> {
			setYaw(0);
			setPitch(0);
		});
		callibrateButton.addActionListener(e -> {
			ByteVector data = new ByteVector(2);
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
				setVelocity(sliderVelocity.getValue() + ConfSteering.MAX / 10);
			}
		});
		getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke('-'), "minus");
		getActionMap().put("minus", new AbstractAction() {
			private static final long serialVersionUID = 1L;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				setVelocity(sliderVelocity.getValue() - ConfSteering.MAX / 10);
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
		label.setAlignmentX(JLabel.CENTER_ALIGNMENT);
		add(label);
		add(sliderVelocity);
		add(Box.createRigidArea(new Dimension(0, 20)));
		label = new JLabel("direction (arrows)");
		label.setAlignmentX(JLabel.CENTER_ALIGNMENT);
		add(label);
		add(sliderDirection);
		JPanel panel = new JPanel();
		panel.setMaximumSize(new Dimension(Integer.MAX_VALUE, BUTTON_HEIGHT));
		panel.setLayout(new GridLayout(0, 2));
		panel.add(zeroButton);
		panel.add(callibrateButton);
		add(panel);
		
		//add a timer that the value is reduced slowly
		timer = new Timer(true);
		//check if a ps controller is available
		gamepad = getGamepad();
		System.out.println("Gamepad found: " + gamepad);
		if(gamepad == null) {
			timer.scheduleAtFixedRate(new TimerTask() {
				
				@Override
				public void run() {
					SwingUtilities.invokeLater(new Runnable() {
						
						@Override
						public void run() {
							//velocity
							int value = sliderVelocity.getValue();
							if(value != 0) {
								int newVal = (int) (value - Math.signum(value) * TIMER_STEP);
								if(Math.signum(newVal) != Math.signum(value))
									newVal = 0;
								sliderVelocity.setValue(newVal);
							}
							//x direction
							value = sliderDirection.getXSlider();
							if(value != 0) {
								int newVal = (int) (value - Math.signum(value) * TIMER_STEP);
								if(Math.signum(newVal) != Math.signum(value))
									newVal = 0;
								sliderDirection.setXSlider(newVal);
							}
							//y direction
							value = sliderDirection.getYSlider();
							if(value != 0) {
								int newVal = (int) (value - Math.signum(value) * TIMER_STEP);
								if(Math.signum(newVal) != Math.signum(value))
									newVal = 0;
								sliderDirection.setYSlider(newVal);
							}
						}
					});
				}
				
			}, 0, TIMER_PERIOD_REDUCE); //run
		} else {
			timer.scheduleAtFixedRate(new TimerTask() {
				
				@Override
				public void run() {
					System.out.println("gamepad poll");
					gamepad = getGamepad();
					if(gamepad.poll()) {
						System.out.println("true");
						Component x = gamepad.getComponent(Identifier.Axis.RX);
						Component y = gamepad.getComponent(Identifier.Axis.RY);
						Component vel = gamepad.getComponent(Identifier.Axis.Y);
						float v = vel.getPollData();
						setVelocity((int) (v * -ConfSteering.MAX - Math.signum(v) * VELOCITY_ZERO));
						setYaw((int) (x.getPollData() * ConfSteering.MAX));
						setPitch((int) (y.getPollData() * -ConfSteering.MAX));
					}
					
					
				}
			}, 0, TIMER_PERIOD_GAMEPAD);
		}
		
		
	}
	
	private Controller getGamepad() {
		Controller[] ca = ControllerEnvironment.getDefaultEnvironment().getControllers();
		for(Controller c : ca) {
			if(c.getComponent(Identifier.Axis.RX) != null && 
				c.getComponent(Identifier.Axis.RY) != null &&
				c.getComponent(Identifier.Axis.Y) != null) {
				return c;
			}
		}
		return null;
	}
	
	private void setSliderLabels(JSlider slider, int max, int zero, String maxLabel, String minLabel, String zeroLabel) {
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
		slider.setLabelTable(table);
		slider.setPaintLabels(true);
		slider.setMajorTickSpacing(zero);
		slider.setPaintTicks(true);
	}
	
	public void setVelocity(int vel) {
		sliderVelocity.setValue(vel);
	}

	public void setYaw(int yaw) {
		sliderDirection.setXSlider(yaw);
	}

	public void setPitch(int pitch) {
		sliderDirection.setYSlider(pitch);
	}
	
	private int considerZeroMarginVelocity(int sliderValue) {
		if(sliderValue > VELOCITY_ZERO) {
			sliderValue -= VELOCITY_ZERO;
		} else if(sliderValue < -VELOCITY_ZERO) {
			sliderValue += VELOCITY_ZERO;
		} else {
			sliderValue = 0;
		}
		return sliderValue;
	}

	public int getVelocity() {
		return considerZeroMarginVelocity(sliderVelocity.getValue());
	}

	public int getYaw() {
		return sliderDirection.getXSlider();
	}

	public int getPitch() {
		return sliderDirection.getYSlider();
	}



	@Override
	public void windowOpened(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void windowClosing(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void windowClosed(WindowEvent e) {
		timer.cancel();
	}



	@Override
	public void windowIconified(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void windowDeiconified(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void windowActivated(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void windowDeactivated(WindowEvent e) {
		// TODO Auto-generated method stub
		
	}
	
}
