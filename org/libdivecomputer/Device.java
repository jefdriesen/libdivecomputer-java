package org.libdivecomputer;

public class Device extends Handle
{
	private native long Open(long context, long descriptor, long iostream);
	private native void Foreach(long handle, Callback callback);
	private native void SetFingerprint(long handle, byte[] fingerprint);
	private native void SetEvents(long handle, Events events);
	private native void SetCancel(long handle, Cancel cancel);

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

	public interface Cancel {
		boolean Cancel();
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

	public void SetCancel(Cancel cancel)
	{
		SetCancel(handle, cancel);
	}

	static {
		System.loadLibrary("divecomputer-java");
	}
}
