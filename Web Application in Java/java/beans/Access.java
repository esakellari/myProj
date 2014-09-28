
package beans;



public class Access {

    boolean access;


    public Access() {
        access = false;
    }
    public Access(boolean value) {
        access = value;
    }


    public boolean get() {
        return access;
    }

    public void set(boolean value) {
        access = value;
    }
}
