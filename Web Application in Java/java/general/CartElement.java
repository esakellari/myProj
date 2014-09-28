
package general;

import java.io.Serializable;



public class CartElement implements Serializable {

    String name;
    int id;
    int size;
    int price;


    public CartElement(String Name, int Id, int Size, int Price) {
        name  = Name;
        id    = Id;
        size  = Size;
        price = Price;
    }

    public String getName() {
        return name;
    }
    public int getId() {
        return id;
    }
    public int getSize() {
        return size;
    }
    public int getPrice() {
        return price;
    }

}
