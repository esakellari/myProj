
package servlets;

import beans.*;
import general.*;
import java.io.*;
import java.sql.*;
import java.util.Date;
import java.util.LinkedList;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;



public class SignIn extends HttpServlet {


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



        StringFixer sf = new StringFixer();


        if(user.getUsername() != null) {    // Ean o xristis pou kanei login einai idi signed in.
            System.out.println("User \"" + user.getUsername() + "\": Tried to Register, while already logged in");
            msgs.add("AlreadyLoggedIn");
            access.set(true);
            request.getRequestDispatcher("errorUser.jsp").include(request, response);
            out.close();
            return;
        }


        String username = sf.fixWhiteChars(request.getParameter("username"));
        String password = request.getParameter("password");

        if(username == null || username.isEmpty() == true
        || password == null || password.isEmpty() == true) {
            msgs.add("SignIn:Failure");
            response.sendRedirect( response.encodeRedirectURL(path.get()) );
            out.close();
            return;
        }


        String toPage = null;
        Connection conn = null;
        try {
            Class.forName("com.mysql.jdbc.Driver");
            conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/test","root","2507");
            try {
                Statement st = conn.createStatement();

                st.executeQuery("SELECT * FROM clients WHERE username=" + sf.addQuotes(sf.fixForEQUAL(username)) );

                ResultSet rs = st.getResultSet();
                if(rs.next() == true) {
                    if(password.compareTo(rs.getString("password")) == 0) {

                        user.setUsername(username);
                        user.setPassword(password);
                        user.setName    (rs.getString("name"));
                        user.setSurname (rs.getString("surname"));
                        user.setEmail   (rs.getString("email"));
                        user.setCardCode(rs.getString("card_code"));

                        Date currentDate = new Date();
                        long msec = currentDate.getTime();
                        long days = msec/(24 * 60 * 60 * 1000);

                        rs = st.executeQuery("SELECT * FROM clients WHERE username = "+sf.addQuotes(username));

                        long last_sign_in = 0;
                        long space = 0,space_bonus = 0,space_public =0;
                        double charge = 0;
                         while(rs.next()){

                             last_sign_in = rs.getLong("last_sign_in");
                             space = rs.getLong("space");
                             space_bonus = rs.getLong("space_bonus");
                             space_public = rs.getLong("space_public");
                             charge = rs.getDouble("charge");
                         }

                        long daysPassed = 0;
                        long mb = 1024*1024;
                        long gb =1024*1024*1024;

                        daysPassed = days - last_sign_in;



                        st.executeUpdate("UPDATE clients SET last_sign_in = "+days+" WHERE username = "+sf.addQuotes(username));

                        st.executeUpdate("UPDATE clients SET space = "+(space+daysPassed*mb)+", space_bonus = "+(space_bonus+daysPassed*mb)
                                +" WHERE username = "+sf.addQuotes(username));

                        st.executeUpdate("UPDATE clients SET charge = "+(charge+(daysPassed*space_public/gb))+" WHERE username = "+sf.addQuotes(username));


                        System.out.println("SignIn: Successful [user:"+username+"]");
                        toPage = "welcomeUser.jsp";

                    } else {
                        System.out.println("SignIn Failed: Wrong Password by user \"" + username + "\"");
                        msgs.add("SignIn:Failure");
                        toPage = path.get();
                    }
                } else {
                    System.out.println("SignIn Failed: Username \"" + username + "\" does not exist");
                    msgs.add("SignIn:Failure");
                    toPage = path.get();
                }

                rs.close();
                st.close();
                conn.close();

            } catch(SQLException e) {
                System.out.println("ERROR [SignIn.processRequest >> *]: SQLException");
                toPage = "errorGeneral.jsp";
            }
        } catch(ClassNotFoundException e) {
            System.out.println("ERROR [SignIn.processRequest >> Class.forName]: ClassNotFoundException");
            toPage = "errorGeneral.jsp";
        } catch(SQLException e) {
            System.out.println("ERROR [SignIn.processRequest >> DriverManager.getConnection]: SQLException");
            toPage = "errorGeneral.jsp";
        } finally {
            if(toPage.compareTo(path.get()) == 0) {
                response.sendRedirect( response.encodeRedirectURL(toPage) );
            } else {
                access.set(true);
                request.getRequestDispatcher(toPage).include(request, response);
            }
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
}
