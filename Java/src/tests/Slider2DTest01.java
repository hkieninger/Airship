package tests;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import gui.component.Slider2D;

public class Slider2DTest01 {

	public static void main(String[] args) throws IOException {
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		BufferedImage img = ImageIO.read(new File("zeppelin_icon.png"));
		Slider2D slider = new Slider2D(100, 100, img);
		slider.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent arg0) {
				System.out.println("x: " + slider.getXSlider() + " / y: " + slider.getYSlider());
			}
		});
		//frame.setLayout(null);
		//slider.setPreferredSize(new Dimension(400, 400));
		//slider.setBounds(100, 100, 200, 200);
		frame.add(slider);
		frame.setSize(400, 400);
		frame.setVisible(true);
	}

}
