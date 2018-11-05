package gui.component;

import javax.swing.table.AbstractTableModel;

public class StaticTableModel extends AbstractTableModel {
	
	private static final long serialVersionUID = 1L;
	
	private Object[][] data;
	private Object[] columns;

	public StaticTableModel(final Object[][] data, final Object[] columns) {
		this.data = data;
		this.columns = columns;
	}
	
	@Override
	public boolean isCellEditable(int row, int column){  
		return false;
	}
	
	@Override
	public int getColumnCount() {
		return columns.length;
	}
	
	@Override
	public int getRowCount() {
		return data.length;
	}
	
	@Override
	public Object getValueAt(int row, int col) {
		return data[row][col];
	}
	
	@Override
	public String getColumnName(int col) {
		return columns[col].toString();
	}
	
	@Override
	public void setValueAt(Object value, int row, int col) {
		data[row][col] = value;
		fireTableCellUpdated(row, col);
	}

}
