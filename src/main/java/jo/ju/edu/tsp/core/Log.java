package jo.ju.edu.tsp.core;

import org.jetbrains.annotations.NotNull;

public class Log {
    public static void info(@NotNull String msg) {
        msg("INFO: ", msg);
    }

    public static void debug(@NotNull String msg) {
        msg("DEBUG: ", msg);
    }

    public static void error(@NotNull String msg) {
        msg("ERROR: ", msg);
    }

    public static void warn(@NotNull String msg) {
        msg("WARNING: ", msg);
    }

    public static void msg(@NotNull String type, @NotNull String msg) {
        System.out.println(type + msg);
    }
}
