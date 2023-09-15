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

				// Parse the samples.
				parser.Foreach(new Parser.Callback() {
					@Override
					public void Sample(int type) {
						//System.out.format("Sample: type=%d\n", type);
					}
				});

				// Continue downloading dives.
				return 1;
			}
		});
	}
}
