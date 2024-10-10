package org.libdivecomputer;

public class Parser extends Handle
{
	private native long New(long device, byte[] data);
	private native long New2(long context, long descriptor, byte[] data);
	private native void Free(long handle);
	private native void Foreach(long handle, Callback callback);
	private native void GetDatetime(long handle, Datetime datetime);
	private native void GetSalinity(long handle, Salinity salinity);
	private native void GetDecomodel(long handle, Decomodel decomodel);
	private native void GetGasmix(long handle, Gasmix gasmix, int idx);
	private native void GetTank(long handle, Tank tank, int idx);
	private native int GetFieldInt(long handle, int field);
	private native double GetFieldDouble(long handle, int field);

	private int DC_FIELD_DIVETIME = 0;
	private int DC_FIELD_MAXDEPTH = 1;
	private int DC_FIELD_AVGDEPTH = 2;
	private int DC_FIELD_GASMIX_COUNT = 3;
	private int DC_FIELD_GASMIX = 4;
	private int DC_FIELD_SALINITY = 5;
	private int DC_FIELD_ATMOSPHERIC = 6;
	private int DC_FIELD_TEMPERATURE_SURFACE = 7;
	private int DC_FIELD_TEMPERATURE_MINIMUM = 8;
	private int DC_FIELD_TEMPERATURE_MAXIMUM = 9;
	private int DC_FIELD_TANK_COUNT = 10;
	private int DC_FIELD_TANK = 11;
	private int DC_FIELD_DIVEMODE = 12;
	private int DC_FIELD_DECOMODEL = 13;

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

	public class Salinity {
		public int type;
		public double density;
	}

	public class Gasmix {
		public double helium;
		public double oxygen;
		public double nitrogen;
		public int usage;
	}

	public class Tank {
		public int gasmix;
		public int type;
		public double volume;
		public double workpressure;
		public double beginpressure;
		public double endpressure;
		public int usage;
	}

	public class Decomodel {
		public int type;
		public int conservatism;
		public int gf_high;
		public int gf_low;
	}

	public interface Callback {
		void Time(int value);
		void Depth(double value);
		void Pressure(int tank, double value);
		void Temperature(double value);
		void Event(int type, int time, int flags, int value);
		void Rbt(int value);
		void Heartbeat(int value);
		void Bearing(int value);
		void Vendor(int type, byte[] value);
		void Setpoint(double value);
		void Ppo2(int sensor, double value);
		void Cns(double value);
		void Deco(int type, int time, double depth, int tts);
		void Gasmix(int value);
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

	public int GetDivetime()
	{
		return GetFieldInt(handle, DC_FIELD_DIVETIME);
	}

	public double GetMaxdepth()
	{
		return GetFieldDouble(handle, DC_FIELD_MAXDEPTH);
	}

	public double GetAvgdepth()
	{
		return GetFieldDouble(handle, DC_FIELD_AVGDEPTH);
	}

	public Gasmix[] GetGasmixes()
	{
		int count = GetFieldInt(handle, DC_FIELD_GASMIX_COUNT);
		Gasmix[] gasmixes = new Gasmix[count];
		for(int i = 0; i < gasmixes.length; i++) {
			Gasmix gasmix = new Gasmix();
			GetGasmix(handle, gasmix, i);
			gasmixes[i] = gasmix;
		}
		return gasmixes;
	}

	public Salinity GetSalinity()
	{
		Salinity salinity = new Salinity();
		GetSalinity(handle, salinity);
		return salinity;
	}

	public double GetAtmospheric()
	{
		return GetFieldDouble(handle, DC_FIELD_ATMOSPHERIC);
	}

	public double GetTemperatureSurface()
	{
		return GetFieldDouble(handle, DC_FIELD_TEMPERATURE_SURFACE);
	}

	public double GetTemperatureMin()
	{
		return GetFieldDouble(handle, DC_FIELD_TEMPERATURE_MINIMUM);
	}

	public double GetTemperatureMax()
	{
		return GetFieldDouble(handle, DC_FIELD_TEMPERATURE_MAXIMUM);
	}

	public Tank[] GetTanks()
	{
		int count = GetFieldInt(handle, DC_FIELD_TANK_COUNT);
		Tank[] tanks = new Tank[count];
		for(int i = 0; i < tanks.length; i++) {
			Tank tank = new Tank();
			GetTank(handle, tank, i);
			tanks[i] = tank;
		}
		return tanks;
	}

	public int GetDivemode()
	{
		return GetFieldInt(handle, DC_FIELD_DIVEMODE);
	}

	public Decomodel GetDecomodel()
	{
		Decomodel decomodel = new Decomodel();
		GetDecomodel(handle, decomodel);
		return decomodel;
	}

	public void Foreach(Callback callback)
	{
		Foreach(handle, callback);
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
