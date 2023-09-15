package org.libdivecomputer;

public class Custom extends IOStream
{
	private native long Open(long context, int transport, Callback callback);

	public interface Callback {
		default void SetTimeout(int timeout) {};
		default void SetBreak(boolean value) {};
		default void SetDtr(boolean value) {};
		default void SetRts(boolean value) {};
		default int GetLines() {return 0;};
		default int GetAvailable() {return 0;};
		default void Configure(int baudrate, int databits, int parity, int stopbits, int flowcontrol) {};
		default void Poll(int timeout) {};
		default byte[] Read() {return null;};
		default void Write(byte[] data) {};
		default void Ioctl(int request, byte[] data) {};
		default void Flush() {};
		default void Purge(int direction) {};
		default void Sleep(int milliseconds) {};
		default void Close() {};
	}

	public Custom(Context context, int transport, Callback callback)
	{
		this.handle = Open(context.handle, transport, callback);
	}

	static {
		System.loadLibrary("divecomputer-java");
	}
}
