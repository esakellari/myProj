

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
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;




public class RemoveFile extends HttpServlet {
   

    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException, ClassNotFoundException, SQLException {
        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();

        HttpSession session = request.getSession();
        

        User user = (User)session.getAttribute("user");
         if(user == null) {
            session.setAttribute("user", new User());
            user = (User)session.getAttribute("user");
        }
        String usrname = user.getUsername();


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


        if(user.getUsername() == null) {
            access.set(true);
            msgs.add("NotLoggedIn");
            request.getRequestDispatcher("errorUser.jsp").forward(request, response);
            return;
        }

        StringFixer str = new StringFixer();


        String file = request.getParameter("file");
        String privacy = request.getParameter("privacy");
        String path_file = (String) request.getParameter("path_file");
        String id = (String)request.getParameter("id");

        if(file == null || privacy == null || path_file == null || id == null){
         access.set(true);
         request.getRequestDispatcher("errorGeneral.jsp").include(request, response);
         return;
        }


        Connection con;
        try {
                Class.forName("com.mysql.jdbc.Driver");
                con = (Connection) DriverManager.getConnection("jdbc:mysql://localhost/test","root","2507");
                ResultSet rs = null;
                Statement st = (Statement) con.createStatement();

                String select = "SELECT size FROM fs_"+usrname+" WHERE name = "+ str.addQuotes(file);
                rs = st.executeQuery(select);

                double size = 0;


                while (rs.next()) {
                    size = rs.getDouble("size");
                }


                select = "SELECT space_used,space_public FROM clients WHERE username = "+ str.addQuotes(usrname);
                rs = st.executeQuery(select);

                double space_used = 0,space_public = 0;

                while (rs.next()) {
                    space_used = rs.getDouble("space_used");
                    space_public = rs.getDouble("space_public");
                }


                String delete = "DELETE FROM fs_"+usrname+" WHERE name = "
                                            + str.addQuotes(file)+ " and path = "
                                            + str.addQuotes(path_file);

                st.executeUpdate(delete);
                
                String update = "UPDATE clients SET space_used = "+(space_used-size)+"WHERE username = "+str.addQuotes(usrname);
                
                st.executeUpdate(update);

                 if(privacy.equals("public")){

                     

                    String delete2 = null;

                    delete2 = "DELETE FROM public_files WHERE name = "
                                            + str.addQuotes(file) + " and owner = "
                                            + str.addQuotes(usrname)+" and id = "
                                            +str.addQuotes(id);

                    st.executeUpdate(delete2);

                    update = "UPDATE clients SET space_public = "+(space_public-size)+"WHERE username = "+str.addQuotes(usrname);
                    st.executeUpdate(update);
                }

                access.set(true);
                request.getRequestDispatcher("ManageAll").include(request, response);

                rs.close();
                st.close();
                con.close();

        } catch(SQLException s){
                            out.println(s.getMessage());
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




