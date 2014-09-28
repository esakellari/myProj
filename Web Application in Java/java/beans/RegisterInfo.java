
package beans;



public class RegisterInfo {

    String name, nameComment;
    String surname, surnameComment;
    String email, emailComment;
    String cardCode, cardCodeComment;
    String username, usernameComment;
    String password, passwordComment;
    String repass, repassComment;


    public RegisterInfo() {
        name     = "";      nameComment     = "";
        surname  = "";      surnameComment  = "";
        email    = "";      emailComment    = "";
        cardCode = "";      cardCodeComment = "";
        username = "";      usernameComment = "";
        password = "";      passwordComment = "";
        repass   = "";      repassComment   = "";
    }


    public String getName() {
        return name;
    }
    public String getNameComment() {
        return nameComment;
    }
    public String getSurname() {
        return surname;
    }
    public String getSurnameComment() {
        return surnameComment;
    }
    public String getEmail() {
        return email;
    }
    public String getEmailComment() {
        return emailComment;
    }
    public String getCardCode() {
        return cardCode;
    }
    public String getCardCodeComment() {
        return cardCodeComment;
    }
    public String getUsername() {
        return username;
    }
    public String getUsernameComment() {
        return usernameComment;
    }
    public String getPassword() {
        return password;
    }
    public String getPasswordComment() {
        return passwordComment;
    }
    public String getRepass() {
        return repass;
    }
    public String getRepassComment() {
        return repassComment;
    }

    
    public void setName(String value) {
        name = value;
    }
    public void setNameComment(String value) {
        nameComment = value;
    }
    public void setSurname(String value) {
        surname = value;
    }
    public void setSurnameComment(String value) {
        surnameComment = value;
    }
    public void setEmail(String value) {
        email = value;
    }
    public void setEmailComment(String value) {
        emailComment = value;
    }
    public void setCardCode(String value) {
        cardCode = value;
    }
    public void setCardCodeComment(String value) {
        cardCodeComment = value;
    }
    public void setUsername(String value) {
        username = value;
    }
    public void setUsernameComment(String value) {
        usernameComment = value;
    }
    public void setPassword(String value) {
        password = value;
    }
    public void setPasswordComment(String value) {
        passwordComment = value;
    }
    public void setRepass(String value) {
        repass = value;
    }
    public void setRepassComment(String value) {
        repassComment = value;
    }
}
