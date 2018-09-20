package controller.data.object;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public interface ConnectionData {
	public void send(DataOutputStream out) throws IOException;
	public void receive(DataInputStream in) throws IOException;
	
	public static class Byte implements ConnectionData {
		
		public int val;
		
		@Override
		public void send(DataOutputStream out) throws IOException {
			out.writeShort(1);
			out.writeByte(val);
		}

		@Override
		public void receive(DataInputStream in) throws IOException {
			in.readUnsignedShort();
			val = in.readByte();
		}
	}
	
	public static class UByte implements ConnectionData {
		
		public int val;

		@Override
		public void send(DataOutputStream out) throws IOException {
			out.writeShort(1);
			out.writeByte(val);
		}

		@Override
		public void receive(DataInputStream in) throws IOException {
			in.readUnsignedShort();
			val = in.readUnsignedByte();
		}
		
	}
	
	public static class Long implements ConnectionData {
		
		public long val;
		
		public Long() {}
		
		public Long(long val) { this.val = val;}

		@Override
		public void send(DataOutputStream out) throws IOException {
			out.writeShort(8);
			out.writeLong(val);
		}

		@Override
		public void receive(DataInputStream in) throws IOException {
			in.readUnsignedShort();
			val = in.readLong();
		}
		
	}
	
	public static class Float implements ConnectionData {
		
		public float val;

		@Override
		public void send(DataOutputStream out) throws IOException {
			out.writeShort(4);
			out.writeFloat(val);
		}

		@Override
		public void receive(DataInputStream in) throws IOException {
			in.readUnsignedShort();
			val = in.readFloat();
		}
		
	}
	
	public static class Double implements ConnectionData {
		
		public double val;

		@Override
		public void send(DataOutputStream out) throws IOException {
			out.writeShort(8);
			out.writeDouble(val);
		}

		@Override
		public void receive(DataInputStream in) throws IOException {
			in.readUnsignedShort();
			val = in.readDouble();
		}
		
	}
	
	public static class String implements ConnectionData {
		
		public java.lang.String val = "";

		@Override
		public void send(DataOutputStream out) throws IOException {
			char[] chars = val.toCharArray();
			out.writeShort(chars.length);
			for(char c : chars) {
				out.writeByte(c);
			}
		}

		@Override
		public void receive(DataInputStream in) throws IOException {
			char[] chars = new char[in.readUnsignedShort()];
			for(int i = 0; i < chars.length; i++) {
				chars[i] = (char) in.readByte();
			}
			val = new java.lang.String(chars);
		}
		
	}
	
}
