
package servlets;

import beans.*;
import general.*;
import com.mysql.jdbc.Connection;
import com.mysql.jdbc.Statement;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.util.LinkedList;
//import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;




public class CreateFolder extends HttpServlet {


    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException {

        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();


        HttpSession session = request.getSession();
        

        StringFixer str = new StringFixer();


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



        String path_file = (String) session.getAttribute("new_path2");
        session.setAttribute("new_path2", path_file);

        String file = str.fixWhiteChars(request.getParameter("FolderName"));

        String property = request.getParameter("property");


     

        if(file == null || file.isEmpty()){
            msgs.add("NoFolderName");
            access.set(true);
            request.getRequestDispatcher("upload.jsp").forward(request, response);
            out.close();
            return;
        }

       if(property == null || property.isEmpty() ){
            msgs.add("NoProperty");
            access.set(true);
            request.getRequestDispatcher("upload.jsp").forward(request, response);
            out.close();
            return;
        }

         if(file.length()>128){
            msgs.add("longFileName");
            access.set(true);
            request.getRequestDispatcher("upload.jsp").forward(request, response);
            out.close();
            return;
        }



        try {
                Connection con = null;
                Class.forName("com.mysql.jdbc.Driver");
                con = (Connection) DriverManager.getConnection("jdbc:mysql://localhost/test","root","2507");

                ResultSet rs = null;
                Statement st = (Statement) con.createStatement();

                String table0 = "INSERT INTO idfiles VALUES()";

                st.executeUpdate(table0);
                ResultSet res = st.executeQuery("SELECT COUNT(*) FROM idfiles");

                int id = 0;
                while (res.next()){
                    id = res.getInt(1);
                }
                String privacy = null;
                String type = "folder";

                st.executeQuery("SELECT name FROM fs_"+usrname+" WHERE name = " + str.addQuotes(file)+" AND path = "+ str.addQuotes(path_file));

                ResultSet ts = st.getResultSet();
                boolean alreadyExists = ts.next();

                if(alreadyExists == true){
                        msgs.add("folderExists");
                        access.set(true);
                        request.getRequestDispatcher("upload.jsp").forward(request, response);
                        out.close();
                        return;
                }

                st.executeQuery("SELECT * FROM fs_"+usrname+" WHERE property = " + str.addQuotes(property));

                ts = st.getResultSet();

                if(ts.next()){
                        msgs.add("propertyExists");
                        access.set(true);
                        request.getRequestDispatcher("upload.jsp").forward(request, response);
                        out.close();
                        return;
                }
                java.sql.Timestamp  sqlDate = new java.sql.Timestamp(new java.util.Date().getTime());
                String table = "INSERT INTO fs_"+usrname+" (id,name,path,type,size,privacy,price,money_earned,downloads,property,category,date)"
                                + " VALUES("
                                + id +","
                                + str.addQuotes(file)+","
                                + str.addQuotes(path_file)+ ","
                                + str.addQuotes(type)+","
                                + 0+ ","
                                + str.addQuotes(privacy)+","
                                + 0+","
                                + 0+","
                                + 0+","
                                + str.addQuotes(property)+","
                                + "'none',"
                                + "'"+sqlDate+"'"
                                +")";

               
                st.executeUpdate(table);
                
                st.close();
                con.close();

                access.set(true);
                request.getRequestDispatcher("succUpload.jsp").include(request, response);
}
	catch (Exception e){
            System.out.println(e.getMessage());
            access.set(true);
            request.getRequestDispatcher("errorGeneral.jsp").include(request, response);
               
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
