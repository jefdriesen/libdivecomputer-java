package org.libdivecomputer;

import java.util.List;

public class Descriptor
{
	public long handle;
	private native String Vendor(long handle);
	private native String Product(long handle);
	private native int Type(long handle);
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

	public int Transports()
	{
		return Transports(handle);
	}

	static {
		System.loadLibrary("divecomputer-java");
	}
}
