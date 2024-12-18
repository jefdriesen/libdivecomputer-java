package org.libdivecomputer;

import java.util.List;

public class Descriptor extends Handle
{
	private native void Free(long handle);
	private native String Vendor(long handle);
	private native String Product(long handle);
	private native int Type(long handle);
	private native int Model(long handle);
	private native int Transports(long handle);

	static public native List<Descriptor> Iterator();

	private Descriptor(long handle)
	{
		this.handle = handle;
	}

	public String Vendor()
	{
		return Vendor(handle);
	}

	public String Product()
	{
		return Product(handle);
	}

	public int Type()
	{
		return Type(handle);
	}

	public int Model()
	{
		return Model(handle);
	}

	public int Transports()
	{
		return Transports(handle);
	}

	@Override
	public void close()
	{
		Free(handle);
		handle = 0;
	}

	static {
		System.loadLibrary("divecomputer-java");
	}
}
