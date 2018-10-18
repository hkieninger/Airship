package gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

public class Menu extends JMenuBar implements ItemListener, ActionListener {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private JMenuItem connect;
	private JCheckBoxMenuItem bottom, front;
	
	private MenuListener listener;
	
	public Menu(MenuListener listener) {
		this.listener = listener;
		JMenu menuC = new JMenu("connection");
		add(menuC);
		
		connect = new JMenuItem("connect");
		connect.addActionListener(this);
		menuC.add(connect);
		
		JMenu menuV = new JMenu("sensors");
		add(menuV);
		
		bottom = new JCheckBoxMenuItem("bottom camera");
		bottom.addItemListener(this);
		menuV.add(bottom);
		front = new JCheckBoxMenuItem("bottom camera");
		front.addItemListener(this);
		menuV.add(front);
		
	}

	@Override
	public void itemStateChanged(ItemEvent e) {
		if(e.getSource() == bottom) {
			listener.onBottom(bottom.isSelected());
		} else if(e.getSource() == front) {
			listener.onFront(front.isSelected());
		}
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getSource() == connect) {
			listener.onConnect();
		}
	}
	
	public static interface MenuListener {
		void onConnect();
		void onBottom(boolean enabled);
		void onFront(boolean enabled);
	}

}
