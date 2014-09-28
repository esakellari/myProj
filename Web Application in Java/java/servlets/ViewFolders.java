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
//import java.util.Date;
import java.util.Vector;
import java.util.LinkedList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;




public class ViewFolders extends HttpServlet {



    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException, ClassNotFoundException, SQLException {
        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();

        HttpSession session = request.getSession();

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


        if(user.getUsername() == null) {
            access.set(true);
            msgs.add("NotLoggedIn");
            request.getRequestDispatcher("errorUser.jsp").forward(request, response);
            return;
        }


         StringFixer str = new StringFixer();

        String path_file = (String) session.getAttribute("new_path2");
        String path2 = request.getParameter("new_path");

        
        String init_folder = request.getParameter("file");

       /* if(path2.isEmpty() || path_file.isEmpty() || init_folder.isEmpty()){
            access.set(true);
            msgs.add("emptyPath");
            request.getRequestDispatcher("errorUser.jsp").forward(request, response);
            return;

        }*/
        Vector<String> array = new Vector<String>();
        Vector<String> paths = new Vector<String>();
        Vector<String> privacy = new Vector<String>();
        Vector<String> property =  new Vector<String>();
        Vector<String> downloads = new Vector<String>();
        Vector<String> sizes = new Vector<String>();
        Vector<String> category =  new Vector<String>();
        Vector<Integer> ids = new Vector<Integer>();

       
        Connection con;
        try {
                try{
                        Class.forName("com.mysql.jdbc.Driver");
                        con = (Connection) DriverManager.getConnection("jdbc:mysql://localhost/test","root","2507");

                        try{ 
                                Statement st = (Statement) con.createStatement();
                                ResultSet rs = null;
                                
                                rs = st.executeQuery("SELECT space,space_used FROM clients WHERE username="+str.addQuotes(user.getUsername()));
                                while(rs.next()){

                                    long left=0;
                                    left = rs.getLong("space")-rs.getLong("space_used");

                                    
                                }

                                if( path2 == null ? "root" == null : path2.equals("root")  ){
                                    
                                    session.setAttribute("new_path2" , path2);
                                
                                    rs = st.executeQuery("SELECT * FROM fs_"+user.getUsername()+" WHERE path="+str.addQuotes(path2));

                                    while(rs.next()){
                                        array.add(rs.getString("name"));
                                        paths.add(rs.getString("path"));
                                        privacy.add(rs.getString("privacy"));
                                        property.add(rs.getString("property"));
                                        category.add(rs.getString("category"));
                                        ids.add(rs.getInt("id"));

                                        //String bb = null;
                                        long size =rs.getLong("size");
                                        String f_size = str.decodeSize(size);
                                        sizes.add(f_size);

                                        String Downloads = Integer.toString(rs.getInt("downloads"));
                                        downloads.add(Downloads);



                                    }

                                    session.setAttribute("arrayfiles",array);
                                    session.setAttribute("paths",paths);
                                    session.setAttribute("privacy", privacy);
                                    session.setAttribute("property", property);
                                    session.setAttribute("sizes",sizes);
                                    session.setAttribute("down",downloads);
                                    session.setAttribute("category",category);
                                    session.setAttribute("ids",ids);

                                    access.set(true);
                                    request.getRequestDispatcher("viewfolders.jsp").include(request, response);

                                }else if(path2.compareTo("root") != 0){

                                    session.setAttribute("new_path2" , path2);
                                 
                                    rs = st.executeQuery("SELECT * FROM fs_"+user.getUsername()+" WHERE path="+str.addQuotes(path2));

                                    while(rs.next()){
                                        array.add(rs.getString("name"));
                                        paths.add(rs.getString("path"));
                                        privacy.add(rs.getString("privacy"));
                                        property.add(rs.getString("property"));
                                        category.add(rs.getString("category"));

                                        long size =rs.getLong("size");
                                        String f_size = str.decodeSize(size);
                                        sizes.add(f_size);

                                        String Downloads = Integer.toString(rs.getInt("downloads"));
                                        downloads.add(Downloads);

                                    }

                                    session.setAttribute("arrayfiles",array);
                                    session.setAttribute("paths",paths);
                                    session.setAttribute("privacy", privacy);
                                    session.setAttribute("property", property);
                                    session.setAttribute("sizes",sizes);
                                    session.setAttribute("down",downloads);
                                    session.setAttribute("category",category);

                                    access.set(true);
                                    request.getRequestDispatcher("viewfolders.jsp").include(request, response);
                                }

                                else if(init_folder == null && path_file!= null){

                                    session.setAttribute("new_path2" , path_file);
                               
                                    rs = st.executeQuery("SELECT * FROM fs_"+user.getUsername()+" WHERE path="+str.addQuotes(path_file));

                                    while(rs.next()){
                                        array.add(rs.getString("name"));
                                        paths.add(rs.getString("path"));
                                        privacy.add(rs.getString("privacy"));
                                        property.add(rs.getString("property"));
                                        category.add(rs.getString("category"));

                                        //String bb = null;
                                        long size =rs.getLong("size");
                                        String f_size = str.decodeSize(size);
                                        sizes.add(f_size);

                                        String Downloads = Integer.toString(rs.getInt("downloads"));
                                        downloads.add(Downloads);
                                    }
                                        session.setAttribute("arrayfiles",array);
                                        session.setAttribute("paths",paths);
                                        session.setAttribute("privacy", privacy);
                                        session.setAttribute("property", property);
                                        session.setAttribute("sizes",sizes);
                                        session.setAttribute("down",downloads);
                                        session.setAttribute("category",category);

                                        access.set(true);
                                        request.getRequestDispatcher("viewfolders.jsp").include(request, response);

                                   
                                }


                                rs.close();
                                st.close();



                    }catch(SQLException s){
                            out.println(s.getMessage());
                    }

                    con.close();

                }catch(SQLException se){
                            out.println(se.getMessage());
                }
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
            Logger.getLogger(ViewFolders.class.getName()).log(Level.SEVERE, null, ex);
        } catch (SQLException ex) {
            Logger.getLogger(ViewFolders.class.getName()).log(Level.SEVERE, null, ex);
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
            Logger.getLogger(ViewFolders.class.getName()).log(Level.SEVERE, null, ex);
        } catch (SQLException ex) {
            Logger.getLogger(ViewFolders.class.getName()).log(Level.SEVERE, null, ex);
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
