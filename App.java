import org.libdivecomputer.Context;

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
	}
}
