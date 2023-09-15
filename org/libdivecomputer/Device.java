package org.libdivecomputer;

public class Device
{
	public long handle;
	private native long Open(long context, long descriptor, long iostream);
	private native void Foreach(long handle, Callback callback);

	public Device(Context context, Descriptor descriptor, IOStream iostream)
	{
		this.handle = Open(context.handle, descriptor.handle, iostream.handle);
	}

	public interface Callback {
		int Dive(byte[] dive, byte[] fingerprint);
	}

	public void Foreach(Callback callback)
	{
		Foreach(handle, callback);
	}

	static {
		System.loadLibrary("divecomputer-java");
	}
}
