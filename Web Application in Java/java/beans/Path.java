
package beans;



public class Path {

    String path;

    
    public Path() {
        path = "Home";
    }
    public Path(String value) {
        path = value;
    }

    public String get() {
        return path;
    }

    public void set(String value) {
        path = value;
    }
}
