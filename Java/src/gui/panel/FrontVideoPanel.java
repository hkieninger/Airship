package gui.panel;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;

import javax.imageio.ImageIO;

import controller.Pool;
import controller.data.MeasDevice;
import controller.data.object.Picture;
import controller.data.parameter.MeasSensor;
import controller.data.parameter.Parameter;

/**
 * 
 * @author hans
 *
 * Displays the JPG stream coming from the front camera
 */
public class FrontVideoPanel extends VideoPanel implements Pool.Listener<MeasDevice> {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	@Override
	public void onChanged(Pool<MeasDevice> pool, MeasDevice device, Enum<? extends Parameter> parameter) {
		if(device == MeasDevice.SENSOR && parameter == MeasSensor.CAM_FRONT) {
			Picture pic = (Picture) pool.getValue(device, parameter);
			ByteArrayInputStream stream = new ByteArrayInputStream(pic.getData(), 0, pic.getSize());
			try {
				BufferedImage image = ImageIO.read(stream);
				stream.close();
				setImage(image);
			} catch (IOException e) {
				e.printStackTrace();
			}
			
		}
	}

}
