package org.libdivecomputer;

public abstract class IOStream extends Handle
{
	private native void Close(long handle);

	@Override
	public void close()
	{
		Close(handle);
		handle = 0;
	}

	static {
		System.loadLibrary("divecomputer-java");
	}
}

