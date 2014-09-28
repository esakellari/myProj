
package servlets;

import beans.*;
import general.*;
import java.io.*;
import java.sql.*;
import java.util.LinkedList;
import java.util.Date;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;




public class Register extends HttpServlet {


    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException {

        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();


        HttpSession session = request.getSession();

        Path path = (Path)session.getAttribute("path");
        if(path == null) {
            session.setAttribute("path", new Path());
            path = (Path)session.getAttribute("path");
        }
        Access access = (Access)session.getAttribute("access");
        if(access == null) {
            session.setAttribute("access", new Access());
            access = (Access)session.getAttribute("access");
        }
        User user = (User)session.getAttribute("user");
        if(user == null) {
            session.setAttribute("user", new User());
            user = (User)session.getAttribute("user");
        }
        LinkedList<String> msgs = (LinkedList<String>)session.getAttribute("msgs");
        if(msgs == null) {
            session.setAttribute("msgs", new LinkedList<String>());
            msgs = (LinkedList<String>)session.getAttribute("msgs");
        }


        if(user.getUsername() != null) {
            System.out.println("User \"" + user.getUsername() + "\": Tried to Register, while already logged in");
            msgs.add("AlreadyLoggedIn");
            access.set(true);
            request.getRequestDispatcher("errorUser.jsp").forward(request, response);
            out.close();
            return;
        }


        StringFixer sf = new StringFixer();

        String name     = sf.fixWhiteChars(request.getParameter("name"));
        String surname  = sf.fixWhiteChars(request.getParameter("surname"));
        String cardCode = request.getParameter("card_code");
        String email    = sf.fixWhiteChars(request.getParameter("email"));
        String username = sf.fixWhiteChars(request.getParameter("username"));
        String password = request.getParameter("password");
        String repass   = request.getParameter("repass");


        RegisterInfo ri = new RegisterInfo();
        boolean tryAgain = false;

        tryAgain |= !isNameValid(name, ri);
        tryAgain |= !isSurnameValid(surname, ri);
        tryAgain |= !isCardCodeValid(cardCode, ri);
        tryAgain |= !isEmailValid(email, ri);
        try {
            tryAgain |= !isUsernameValid(username, ri);
        } catch(Exception e) {
            access.set(true);
            request.getRequestDispatcher("errorGeneral.jsp").forward(request, response);
            out.close();
            return;
        }
        boolean isPassValid = isPasswordValid(password, ri);
        tryAgain |= !isPassValid;
        tryAgain |= !isRepassValid(repass, password, ri);
        if(isPassValid == false) {
            ri.setRepassComment("");
        }

        if(tryAgain == true) {
            ri.setPassword("");
            ri.setRepass("");

            access.set(true);
            session.setAttribute("rinfo", ri);
            request.getRequestDispatcher("register.jsp").forward(request, response);
            session.removeAttribute("rinfo");
            out.close();
            return;
        }



        // Ean proxwrisei i ektelesi se auto to simeio, simainei oti ta pedia pou sublirwse
        // o xristis gia eggrafi einai ola egura, kai einai etoimi na ginei i eggrafi.
        //

        String forwardTo = "errorGeneral.jsp";
        Connection conn = null;
        try {
            Class.forName("com.mysql.jdbc.Driver");
            conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/test","root","2507");
            try {
                Statement st = conn.createStatement();

                Date currentDate = new Date();
                long msec = currentDate.getTime();
                long days = msec/(24 * 60 * 60 * 1000);

                st.executeUpdate("INSERT INTO clients "
                               + "(username, password, name, surname, email, card_code,"
                               + " space, space_used, space_bonus, space_public, money_earned, charge, last_sign_in)"
                               + " VALUES("
                               + sf.addQuotes(username) + ","
                               + sf.addQuotes(password) + ","
                               + sf.addQuotes(name)     + ","
                               + sf.addQuotes(surname)  + ","
                               + sf.addQuotes(email)    + ","
                               + sf.addQuotes(cardCode) + ","
                               + 0             + ","
                               + 0             + ","
                               + 0             + ","
                               + 0             + ","
                               + 0             + ","
                               + 0             + ","
                               + days          + ")");

                st.executeUpdate("CREATE TABLE fs_" + username + "("
                               + "id            INTEGER,"
                               + "name          VARCHAR(128) NOT NULL,"
                               + "path          VARCHAR(128) NOT NULL,"
                               + "type          VARCHAR(8),"
                               + "size          BIGINT,"
                               + "privacy       VARCHAR(8),"
                               + "price         INTEGER,"
                               + "money_earned  INTEGER,"
                               + "downloads     INTEGER,"
                               + "property      VARCHAR(45), "
                               + "category      VARCHAR(45),"
                               + "date          TIMESTAMP,"
                               + "PRIMARY KEY(`name`,`path`))");

                st.close();
                conn.close();

                user.setUsername(username);
                user.setPassword(password);
                user.setName    (name);
                user.setSurname (surname);
                user.setEmail   (email);
                user.setCardCode(cardCode);

                System.out.println("Registration: Successful");
                forwardTo = "welcomeUser.jsp";

            } catch(SQLException e) {
                System.out.println("ERROR [Register.processRequest >> *]: SQLException");
            }
        } catch(ClassNotFoundException e) {
            System.out.println("ERROR [Register.processRequest >> Class.forName]: ClassNotFoundException");
        } catch(SQLException e) {
            System.out.println("ERROR [Register.processRequest >> DriverManager.getConnection]: SQLException");
        } finally {
            access.set(true);
            request.getRequestDispatcher(forwardTo).forward(request, response);
            out.close();
        }
    }




    // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP <code>GET</code> method.
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Handles the HTTP <code>POST</code> method.
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Returns a short description of the servlet.
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "Short description";
    }// </editor-fold>



    // Oi parakatw sunartiseis tis morfis "is*Valid" elegxoun an to ekastote
    // pedio pou edwse o xristis einai eguro. Analoga me tin eugorotita tou
    // pediou thetoun times kai minimata sto adikeimeno 'ri' wste na ektupwthoun
    // oi aparaitites plirofories sto xristi.
    // Epistrefoun 'true' an einai eguro to pedio, alliws 'false'.
    //

    boolean isNameValid(String name, RegisterInfo ri) {
        if(name == null) {
            return false;
        } else if(name.isEmpty() == true) {
            ri.setNameComment("Please give your Name");
            return false;
        } else if(name.length() > 45) {
            ri.setNameComment("Name cannot be more than 45 characters");
            return false;
        } else {
            ri.setName(name);
            return true;
        }
    }

    boolean isSurnameValid(String surname, RegisterInfo ri) {
        if(surname == null) {
            return false;
        } else if(surname.isEmpty() == true) {
            ri.setSurnameComment("Please give your surname");
            return false;
        } else if(surname.length() > 45) {
            ri.setSurnameComment("Surame cannot be more than 45 characters");
            return false;
        } else {
            ri.setSurname(surname);
            return true;
        }
    }

    boolean isCardCodeValid(String cardCode, RegisterInfo ri) {
        if(cardCode == null) {
            return false;
        } else if(cardCode.isEmpty() == true) {
            ri.setCardCodeComment("Please give your Credit Card Code");
            return false;
        } else if(cardCode.length() > 45) {
            ri.setCardCodeComment("Credit Card Code cannot be more than 45 characters");
            return false;
        } else {
            ri.setCardCode(cardCode);
            return true;
        }
    }

    boolean isEmailValid(String email, RegisterInfo ri) {
        if(email == null) {
            return false;
        } else if(email.isEmpty() == true) {
            ri.setEmailComment("Please give your E-mail");
            return false;
        } else if(email.length() > 45) {
            ri.setEmailComment("E-mail cannot be more than 45 characters");
            return false;
        } else {
            ri.setEmail(email);
            return true;
        }
    }

    boolean isUsernameValid(String username, RegisterInfo ri)
    throws Exception {
        if(username == null) {
            return false;
        } else if(username.isEmpty() == true) {
            ri.setUsernameComment("Please select a Username");
            return false;
        } else if(username.contains(" ") == true) {
            ri.setUsernameComment("Username cannot contain whitespace characters");
            return false;
        } else if(doesUsernameExist(username) == true) {
            ri.setUsernameComment("Username \""+username+"\" already exists. Please select another username");
            return false;
        } else if(username.length() > 45) {
            ri.setUsernameComment("Username cannot be more than 45 characters");
            return false;
        } else {
            ri.setUsername(username);
            return true;
        }
    }

    boolean isPasswordValid(String password, RegisterInfo ri) {
        if(password == null) {
            return false;
        } else if(password.isEmpty() == true) {
            ri.setPasswordComment("Please select a password");
            return false;
        } else if(password.length() <= 4) {
            ri.setPasswordComment("Password must be bigger than 4 characters");
            return false;
        } else if(password.length() > 45) {
            ri.setPasswordComment("Password cannot be more than 45 characters");
            return false;
        } else {
            ri.setPassword(password);
            return true;
        }
    }

    boolean isRepassValid(String repass, String password, RegisterInfo ri) {
        if(repass == null) {
            return false;
        } else if(repass.isEmpty() == true) {
            ri.setRepassComment("Please re-type the Password");
            return false;
        } else if(repass.compareTo(password) != 0) {
            ri.setRepassComment("Please re-type the Password correctly");
            return false;
        } else {
            ri.setRepass(repass);
            return true;
        }
    }



    // Psaxnei sti vasi sto table [clients] ean to 'username' yparxei idi.
    //
    boolean doesUsernameExist(String username)
    throws Exception {

        StringFixer sf = new StringFixer();
        Connection conn = null;
        boolean alreadyExists = false;
        try {
            Class.forName("com.mysql.jdbc.Driver");
            conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/test","root","2507");
            try {
                Statement st = conn.createStatement();
                st.executeQuery("SELECT username FROM clients WHERE username="
                                + sf.addQuotes(sf.fixForEQUAL(username)) );

                ResultSet rs = st.getResultSet();
                alreadyExists = rs.next();

                rs.close();
                st.close();
                conn.close();
            } catch(SQLException e) {
                System.out.println("ERROR [Register.doesUsernameExist >> *]: SQLException");
                throw new Exception();
            }
        } catch(ClassNotFoundException e) {
            System.out.println("ERROR [Register.doesUsernameExist >> Class.forName]: ClassNotFoundException");
            throw new Exception();
        } catch (SQLException e) {
            System.out.println("ERROR [Register.doesUsernameExist >> DriverManager.getConnection]: SQLException");
            throw new Exception();
        }
        return alreadyExists;
    }

}