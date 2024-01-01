package org.libdivecomputer;

public class Device
{
	public long handle;
	private native long Open(long context, long descriptor, long iostream);
	private native void Foreach(long handle, Callback callback);
	private native void SetFingerprint(long handle, byte[] fingerprint);
	private native void SetEvents(long handle, Events events);

	public Device(Context context, Descriptor descriptor, IOStream iostream)
	{
		this.handle = Open(context.handle, descriptor.handle, iostream.handle);
	}

	public interface Callback {
		int Dive(byte[] dive, byte[] fingerprint);
	}

	public interface Events {
		void Waiting();
		void Progress(double percentage);
		void Devinfo(int model, int firmware, int serial);
		void Clock(long devtime, long systime);
		void Vendor(byte[] data);
	}

	public void Foreach(Callback callback)
	{
		Foreach(handle, callback);
	}

	public void SetFingerprint(byte[] fingerprint)
	{
		SetFingerprint(handle, fingerprint);
	}

	public void SetEvents(Events events)
	{
		SetEvents(handle, events);
	}

	static {
		System.loadLibrary("divecomputer-java");
	}
}
