package org.libdivecomputer;

public class Context extends Handle
{
	private native long New();
	private native long Free(long handle);
	private native void SetLogLevel(long handle, int loglevel);
	private native void SetLogFunc(long handle, LogFunc logfunc);

	public enum LogLevel {
		DC_LOGLEVEL_NONE,
		DC_LOGLEVEL_ERROR,
		DC_LOGLEVEL_WARNING,
		DC_LOGLEVEL_INFO,
		DC_LOGLEVEL_DEBUG,
		DC_LOGLEVEL_ALL;
	}

	public interface LogFunc {
		void Log(int loglevel, String file, int line, String function, String message);
	}

	public Context()
	{
		this.handle = New();
	}

	public void SetLogLevel(LogLevel loglevel)
	{
		SetLogLevel(handle, loglevel.ordinal());
	}

	public void SetLogFunc(LogFunc logfunc)
	{
		SetLogFunc(handle, logfunc);
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
