
package servlets;

import beans.*;
import com.mysql.jdbc.Connection;
import com.mysql.jdbc.Statement;
import general.StringFixer;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.DriverManager;
//import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.LinkedList;
//import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;



public class ChangePer extends HttpServlet {
   

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
        String price = request.getParameter("price");
        
      
        String idstr = (String)session.getAttribute("id");
        String name = (String) session.getAttribute("name");
        //String priceAttr = (String) session.getAttribute("priceAttr");
        String initPrice = (String) session.getAttribute("initialPrice");
        String path_file = (String) session.getAttribute("path_file");

        session.setAttribute("initialPrice",initPrice);
        session.setAttribute("pathAttr",path_file);
        session.setAttribute("nameAttr",name);
        session.setAttribute("priceAttr",price);
        session.setAttribute("idAttr",idstr);

        if( price == null || price.isEmpty()){

                     msgs.add("NoPrice");
                     access.set(true);
                     request.getRequestDispatcher("ChangePer.jsp").forward(request, response);
                     out.close();
                     return;
                }

        Connection con;
        try {
                Class.forName("com.mysql.jdbc.Driver");
                con = (Connection) DriverManager.getConnection("jdbc:mysql://localhost/test","root","2507");
                Statement st = (Statement) con.createStatement();

     
                int price2=0;

                if(price!=null)
                        price2 = Integer.parseInt(price);
                
                if(price2<0){
                     msgs.add("NegPrice");
                     access.set(true);
                     request.getRequestDispatcher("ChangePer.jsp").forward(request, response);
                     out.close();
                     return;
                 }


                int id = Integer.parseInt(idstr);
                out.println(id);
                String update = "UPDATE public_files SET price = "
                                                + price2 + " WHERE name = "
                                                + str.addQuotes(name)+" and "
                                                + "owner = "
                                                + str.addQuotes(usrname)+ " and id = "
                                                +"'"+id+"'";
                 st.executeUpdate(update);

                String update1 = "UPDATE fs_"+usrname+ " SET price = "
                                                + price2 
                                                + " WHERE name = "
                                                + str.addQuotes(name) + " and id = "
                                                +"'"+id+"'";
                st.executeUpdate(update1);
                request.getRequestDispatcher("ManagePublic").include(request, response);


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




