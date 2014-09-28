
package servlets;

import beans.*;
import com.mysql.jdbc.Connection;
import com.mysql.jdbc.Statement;
import general.StringFixer;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.LinkedList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;



public class ChangePrivacy extends HttpServlet {


    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException, ClassNotFoundException, SQLException {
        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();

        HttpSession session = request.getSession();

         StringFixer str = new StringFixer();


        Access access = (Access)session.getAttribute("access");
        if(access == null) {
            session.setAttribute("access", new Access());
            access = (Access)session.getAttribute("access");
        }

        LinkedList<String> msgs = (LinkedList<String>)session.getAttribute("msgs");
        if(msgs == null) {
            session.setAttribute("msgs", new LinkedList<String>());
            msgs = (LinkedList<String>)session.getAttribute("msgs");
        }

        User user = (User)session.getAttribute("user");
         if(user == null) {
            session.setAttribute("user", new User());
            user = (User)session.getAttribute("user");
        }
        String usrname = user.getUsername();


        if(user.getUsername() == null) {
            access.set(true);
            msgs.add("NotLoggedIn");
            request.getRequestDispatcher("errorUser.jsp").forward(request, response);
            return;
        }


        String file = request.getParameter("file");
        String privacy = request.getParameter("privacy");
        String path_file= request.getParameter("path_file");


        if(file == null || privacy == null || path_file == null){
            access.set(true);
            msgs.add("nullparameter");
            request.getRequestDispatcher("errorUser.jsp").forward(request, response);
            return;

        }

        

        Connection con;
        try {
                Class.forName("com.mysql.jdbc.Driver");
                con = (Connection) DriverManager.getConnection("jdbc:mysql://localhost/test","root","2507");
                ResultSet rs = null;
                Statement st = (Statement) con.createStatement();

                String priv = null;

                out.println(privacy);

                if(privacy.equals("public")){
                    priv = "private";

                    String delete = "DELETE FROM public_files WHERE name = "
                                                    + str.addQuotes(file) + " and owner = "
                                                    + str.addQuotes(usrname);
                    st.executeUpdate(delete);

                }
                else if(privacy.equals("private")){
                    priv = "public";
                  
                    String select = "SELECT * FROM fs_"+usrname+" WHERE name = "
                                                    + str.addQuotes(file) + " and path = "
                                                    + str.addQuotes(path_file);

                    rs = st.executeQuery(select);

                    //String type  =  null;
                    long size = 0;
                    int id = 0;
                    int price =0,downloads = 0;
                    String category = null;
                    java.sql.Timestamp  sqlDate = null;

                    while (rs.next()) {
                                size = rs.getLong("size");
                                id = rs.getInt("id");
                                category = rs.getString("category");
                                sqlDate = rs.getTimestamp("date");
                                price = rs.getInt("price");
                                downloads = rs.getInt("downloads");
                    }

                    String table1 = "INSERT INTO public_files (id,name,owner,category,date, size,price,downloads) VALUES("
                                                +id+","
                                                +str.addQuotes(file)+","
                                                +str.addQuotes(usrname)+","
                                                +str.addQuotes(category)+","
                                                +"'"+sqlDate+"',"
                                                +size+","
                                                +price+","
                                                +downloads+
                                                ")";
                 st.executeUpdate(table1);
                }
                
                String update = "UPDATE fs_"+usrname+" SET privacy = "
                                                + str.addQuotes(priv)
                                                + " WHERE name = "
                                                + str.addQuotes(file)+ " and path = "
                                                + str.addQuotes(path_file);

                st.executeUpdate(update);
                access.set(true);
                request.getRequestDispatcher("ManageAll").include(request, response);

                rs.close();
                st.close();
                con.close();

        } finally {
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
        try {
            processRequest(request, response);
        } catch (ClassNotFoundException ex) {
            Logger.getLogger(ChangePer.class.getName()).log(Level.SEVERE, null, ex);
        } catch (SQLException ex) {
            Logger.getLogger(ChangePer.class.getName()).log(Level.SEVERE, null, ex);
        }
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
        try {
            processRequest(request, response);
        } catch (ClassNotFoundException ex) {
            Logger.getLogger(ChangePer.class.getName()).log(Level.SEVERE, null, ex);
        } catch (SQLException ex) {
            Logger.getLogger(ChangePer.class.getName()).log(Level.SEVERE, null, ex);
        }
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




