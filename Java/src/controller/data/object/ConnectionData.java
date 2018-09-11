package controller.data.object;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public interface ConnectionData {
	public void send(DataOutputStream out) throws IOException;
	public void receive(DataInputStream in) throws IOException;
	
	public static class UByte implements ConnectionData {
		
		public int val;

		@Override
		public void send(DataOutputStream out) throws IOException {
			out.writeByte(val);
		}

		@Override
		public void receive(DataInputStream in) throws IOException {
			val = in.readUnsignedByte();
		}
		
	}
	
	public static class Long implements ConnectionData {
		
		public long val;
		
		public Long() {}
		
		public Long(long val) { this.val = val;}

		@Override
		public void send(DataOutputStream out) throws IOException {
			out.writeLong(val);
		}

		@Override
		public void receive(DataInputStream in) throws IOException {
			val = in.readLong();
		}
		
	}
	
	public static class Float implements ConnectionData {
		
		public float val;

		@Override
		public void send(DataOutputStream out) throws IOException {
			out.writeFloat(val);
		}

		@Override
		public void receive(DataInputStream in) throws IOException {
			val = in.readFloat();
		}
		
	}
	
	public static class Double implements ConnectionData {
		
		public double val;

		@Override
		public void send(DataOutputStream out) throws IOException {
			out.writeDouble(val);
		}

		@Override
		public void receive(DataInputStream in) throws IOException {
			val = in.readDouble();
		}
		
	}
	
	public static class String implements ConnectionData {
		
		public java.lang.String val;

		@Override
		public void send(DataOutputStream out) throws IOException {
			out.writeUTF(val);
		}

		@Override
		public void receive(DataInputStream in) throws IOException {
			val = in.readUTF();
		}
		
	}
	
}
