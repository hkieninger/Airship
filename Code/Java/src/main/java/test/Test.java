package test;

import net.java.games.input.Component;
import net.java.games.input.Controller;
import net.java.games.input.ControllerEnvironment;

public class Test {

	public static void main(String[] args) throws InterruptedException {
		Controller[] ca = ControllerEnvironment.getDefaultEnvironment().getControllers();

        for(int i =0;i<ca.length;i++){

            /* Get the name of the controller */
            System.out.println(ca[i].getName());
            
            System.out.println("Type: "+ca[i].getType().toString());

            /* Get this controllers components (buttons and axis) */
            Component[] components = ca[i].getComponents();
            System.out.println("Component Count: "+components.length);
            for(int j=0;j<components.length;j++){
                
                /* Get the components name */
                System.out.println("Component "+j+": "+components[j].getName());
                System.out.println("    Identifier: "+ components[j].getIdentifier().getName());
                System.out.print("    ComponentType: ");
                if (components[j].isRelative()) {
                    System.out.print("Relative");
                } else {
                    System.out.print("Absolute");
                }
                if (components[j].isAnalog()) {
                    System.out.print(" Analog");
                } else {
                    System.out.print(" Digital");
                }
            }
        }
        
        Controller ps = ca[0];
        while(true) {
        	ps.poll();
        	Component[] components = ps.getComponents();
        	for(int i = 16; i <= 16; i++) {
        		System.out.println("component " + components[i].getIdentifier() + ": " + components[i].getPollData());
        		Thread.sleep(10);
        	}
        }
	}
}
