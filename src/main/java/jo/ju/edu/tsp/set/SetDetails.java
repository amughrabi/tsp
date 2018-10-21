package jo.ju.edu.tsp.set;

import java.io.File;

public class SetDetails {
    private File file;
    private int size;
    private String name;

    public SetDetails(File file, int size, String name) {
        this.file = file;
        this.size = size;
        this.name = name;
    }

    public File getFile() {
        return file;
    }

    public int getSize() {
        return size;
    }

    public String getName() {
        return name;
    }
}