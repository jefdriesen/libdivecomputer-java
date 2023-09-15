package org.libdivecomputer;

public class Parser
{
	public long handle;
	private native long New(long device, byte[] data);
	private native long New2(long context, long descriptor, byte[] data);
	private native void Foreach(long handle, Callback callback);
	private native void GetDatetime(long handle, Datetime datetime);

	public class Datetime {
		public static final int None = 0x80000000;
		public int year;
		public int month;
		public int day;
		public int hour;
		public int minute;
		public int second;
		public int timezone;
	}

	public interface Callback {
		void Sample(int type);
	}

	public Parser(Device device, byte[] data)
	{
		this.handle = New(device.handle, data);
	}

	public Parser(Context context, Descriptor descriptor, byte[] data)
	{
		this.handle = New2(context.handle, descriptor.handle, data);
	}

	public Datetime GetDatetime()
	{
		Datetime datetime = new Datetime();
		GetDatetime(handle, datetime);
		return datetime;
	}

	public void Foreach(Callback callback)
	{
		Foreach(handle, callback);
	}

	static {
		System.loadLibrary("divecomputer-java");
	}
}
