import org.libdivecomputer.Context;
import org.libdivecomputer.Descriptor;
import org.libdivecomputer.IOStream;
import org.libdivecomputer.Serial;
import org.libdivecomputer.Custom;
import org.libdivecomputer.Device;
import org.libdivecomputer.Parser;

public class App
{
	public static void main(String[] args)
	{
		// Setup the context.
		Context context = new Context();
		context.SetLogLevel(Context.LogLevel.DC_LOGLEVEL_ALL);
		context.SetLogFunc(new Context.LogFunc() {
			@Override
			public void Log(int loglevel, String file, int line, String function, String message) {
				String loglevels[] = {"NONE", "ERROR", "WARNING", "INFO", "DEBUG", "ALL"};
				int seconds = 0, microseconds = 0;
				System.out.format("[%d.%06d] %s: %s [in %s:%d (%s)]\n",
					seconds, microseconds,
					loglevels[loglevel], message, file, line, function);
			}
		});

		// Choose a device descriptor.
		Descriptor descriptor = null;
		for	(Descriptor desc: Descriptor.Iterator())
		{
			System.out.format("%s %s\n",
				desc.Vendor(),
				desc.Product());
			if (desc.Type() == 0x60002) {
				descriptor = desc;
				break;
			}
		}

		// Setup an I/O stream.
		IOStream iostream = new Serial(context, "/tmp/ttyS0");
		/*IOStream iostream = new Custom(context, 0x20, new Custom.Callback() {
			@Override
			public void SetTimeout(int timeout) {
				System.out.format("SetTimeout\n");
			}
		});*/

		// Setup the device handle.
		Device device = new Device(context, descriptor, iostream);

		device.SetFingerprint(null);

		// Download the dives.
		device.Foreach(new Device.Callback() {
			@Override
			public int Dive(byte[] dive, byte[] fingerprint) {
				System.out.format("Received dive (%d bytes)\n",
					dive.length);

				// Setup the parser.
				Parser parser = new Parser(device, dive);
				//Parser parser = new Parser(context, descriptor, dive);

				Parser.Datetime dt = parser.GetDatetime();
				if (dt.timezone == Parser.Datetime.None) {
					System.out.format ("%04d-%02d-%02d %02d:%02d:%02d\n",
						dt.year, dt.month, dt.day,
						dt.hour, dt.minute, dt.second);
				} else {
					System.out.format("%04d-%02d-%02d %02d:%02d:%02d %+03d:%02d\n",
						dt.year, dt.month, dt.day,
						dt.hour, dt.minute, dt.second,
						dt.timezone / 3600, (Math.abs(dt.timezone) % 3600) / 60);
				}

				Parser.Gasmix[] gasmixes = parser.GetGasmixes();
				for (int i = 0; i < gasmixes.length; i++) {
					System.out.format("Gasmix %d: %.1f %.1f %.1f\n",
						i,
						gasmixes[i].helium * 100.0,
						gasmixes[i].oxygen * 100.0,
						gasmixes[i].nitrogen * 100.0);
				}

				Parser.Tank[] tanks = parser.GetTanks();
				for (int i = 0; i < tanks.length; i++) {
					System.out.format("Tank %d: %d %d %.1f %.1f %.1f %.1f %d\n",
						i,
						tanks[i].gasmix,
						tanks[i].type,
						tanks[i].volume,
						tanks[i].workpressure,
						tanks[i].beginpressure,
						tanks[i].endpressure,
						tanks[i].usage);
				}

				// Parse the samples.
				parser.Foreach(new Parser.Callback() {
					@Override
					public void Time(int value) {
						System.out.format("Time: %d\n", value);
					}
					@Override
					public void Depth(double value) {
						System.out.format("Depth: %.2f\n", value);
					}
					@Override
					public void Pressure(int tank, double value) {
						System.out.format("Pressure: tank=%d, value=%.2f\n", tank, value);
					}
					@Override
					public void Temperature(double value) {
						System.out.format("Temperature: %.2f\n", value);
					}
					@Override
					public void Event(int type, int time, int flags, int value) {
						System.out.format("Event: type=%d, time=%d, flags=%d, value=%d\n", type, time, flags, value);
					}
					@Override
					public void Rbt(int value) {
						System.out.format("Rbt: %d\n", value);
					}
					@Override
					public void Heartbeat(int value) {
						System.out.format("Heartbeat: %d\n", value);
					}
					@Override
					public void Bearing(int value) {
						System.out.format("Bearing: %d\n", value);
					}
					@Override
					public void Vendor(int type, byte[] value) {
						System.out.format("Vendor: type=%d, size=%d\n", type, value.length);
					}
					@Override
					public void Setpoint(double value) {
						System.out.format("Setpoint: %.2f\n", value);
					}
					@Override
					public void Ppo2(int sensor, double value) {
						System.out.format("Ppo2: sensor=%d, value=%.2f\n", sensor, value);
					}
					@Override
					public void Cns(double value) {
						System.out.format("Cns: %.1f\n", value * 100.0);
					}
					@Override
					public void Deco(int type, int time, double depth, int tts) {
						System.out.format("Deco: type=%d, time=%d, depth=%.2f, tts=%d\n", type, time, depth, tts);
					}
					@Override
					public void Gasmix(int value) {
						System.out.format("Gasmix: %d\n", value);
					}
				});

				// Continue downloading dives.
				return 1;
			}
		});
	}
}
