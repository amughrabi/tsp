package jo.ju.edu.tsp.set;

import org.jetbrains.annotations.NotNull;

import java.io.File;

public class SetInstance {
    public final static SetDetails
            SIMPLE   = f("simple", 5, "simple5.xml"),
            DJB2036  = f("djb2036", 2036, "djb2036.xml"),
            IDA8197  = f("ida8197", 8197, "ida8197.xml"),
            PCB3038  = f("pcb3038", 3038, "pcb3038.xml"),
            RL1304   = f("rl1304", 1304,"rl1304.xml"),
            RL5934   = f("rl5934", 5934,"rl5934.xml"),
            XMC10150 = f("xmc10150", 10150, "xmc10150.xml"),
            XSC6880  = f("xsc6880", 6880, "xsc6880.xml"),
            CH130    = f("ch130", 130, "ch130.xml"),
            D198     = f("d198", 198, "d198.xml"),
            EIL76    = f("eil76", 176, "eil76.xml"),
            WI29     = f("wi29", 29, "wi29.xml"),
            BGB4355  = f("bgb4355", 4355, "bgb4355.xml"),
            bayg29   = f("bayg29", 29, "bayg29.xml"),
            bays29   = f("bays29", 29, "bays29.xml"),
            berlin52 = f("berlin52", 52, "berlin52.xml"),
            brazil58 = f("brazil58", 58, "brazil58.xml"),
            gr17     = f("gr17", 17, "gr17.xml"),
            gr21     = f("gr21", 21, "gr21.xml"),
            gr24     = f("gr24", 24, "gr24.xml"),
            gr48     = f("gr48", 48, "gr48.xml"),
            rd400    = f("rd400", 400, "rd400.xml"),
            dca1389  = f("dca1389", 1389, "dca1389.xml"),
            u1817    = f("u1817", 1817, "u1817.xml"),
            xqc2175  = f("xqc2175", 2175, "xqc2175.xml");


    private static @NotNull SetDetails f(@NotNull String name, int size, @NotNull String uri) {
        return new SetDetails(new File(SetInstance.class.getClassLoader().getResource("dataset/must/" + uri).getFile()),
                size, name);
    }
}
