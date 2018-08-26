package tests;

import javax.swing.JFrame;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import gui.component.CircularSlider;

public class CircularSliderTest01 {

	public static void main(String[] args) {
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		CircularSlider slider = new CircularSlider(0, -Math.PI/2);
		slider.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent arg0) {
				System.out.println(slider.getValue());
			}
		});
		frame.setLayout(null);
		//slider.setPreferredSize(new Dimension(400, 400));
		slider.setBounds(100, 100, 200, 200);
		frame.add(slider);
		frame.setSize(400, 400);
		frame.setVisible(true);
	}

}
