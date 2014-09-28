
package beans;


public class User {

    String name, surname, email, cardCode, username, password;


    public User() {
        name     = null;
        surname  = null;
        email    = null;
        cardCode = null;
        username = null;
        password = null;
    }

    public void setName(String value) {
        name = value;
    }
    public void setSurname(String value) {
        surname = value;
    }
    public void setEmail(String value) {
        email = value;
    }
    public void setCardCode(String value) {
        cardCode = value;
    }
    public void setUsername(String value) {
        username = value;
    }
    public void setPassword(String value) {
        password = value;
    }


    public String getName() {
        return name;
    }
    public String getSurname() {
        return surname;
    }
    public String getEmail() {
        return email;
    }
    public String getCardCode() {
        return cardCode;
    }
    public String getUsername() {
        return username;
    }
    public String getPassword() {
        return password;
    }
    
}
