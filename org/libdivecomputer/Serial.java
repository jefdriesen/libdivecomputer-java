package org.libdivecomputer;

public class Serial extends IOStream
{
	private native long Open(long context, String devname);

	public Serial(Context context, String devname)
	{
		this.handle = Open(context.handle, devname);
	}

	static {
		System.loadLibrary("divecomputer-java");
	}
}
