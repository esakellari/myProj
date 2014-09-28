

package servlets;

import beans.*;
import com.mysql.jdbc.Connection;
import com.mysql.jdbc.Statement;
import general.StringFixer;
import java.io.IOException;
import java.io.PrintWriter;
//import java.security.Timestamp;
import java.sql.DriverManager;
import java.sql.ResultSet;
//import java.sql.SQLException;
import java.util.LinkedList;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;





public class uploadFile extends HttpServlet {
   
   
    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException {

        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();

        HttpSession session = request.getSession();


        Access access = (Access)session.getAttribute("access");
        if(access == null) {
            session.setAttribute("access", new Access());
            access = (Access)session.getAttribute("access");
        }
        
        

        StringFixer str = new StringFixer();

        User user = (User)session.getAttribute("user");
         if(user == null) {
            session.setAttribute("user", new User());
            user = (User)session.getAttribute("user");
        }
        String usrname = user.getUsername();

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

        String file = str.fixWhiteChars(request.getParameter("datafileName"));
        String privacy  = request.getParameter("privacy");
        String folder = request.getParameter("folder");
        String category = request.getParameter("Category");

        boolean mistakes=false;

        if(file == null || file.isEmpty()){
           msgs.add("NoFileName");
           access.set(true);
           request.getRequestDispatcher("upload.jsp").forward(request, response);
           out.close();
           return;
        }

        if(privacy == null ){
            msgs.add("Privacy");

            mistakes = true;
            //request.getRequestDispatcher("upload.jsp").forward(request, response);
            //out.close();
            //return;
        }

        if(folder == null ){
            msgs.add("location");

            mistakes = true;
            //request.getRequestDispatcher("upload.jsp").forward(request, response);
            //out.close();
            //return;
        }

        int Size = 0;
        long size_= 0;
        long kb =1024;
        long mb = 1024*1024;
        long gb = 1024*1024*1024;
        String bytes =null;
       
        long sp = 0;

        int a = file.indexOf('.');

        String  ending = file.substring(a+1);

        if(file.length()>128){
             msgs.add("longFileName");
             mistakes =true;
           
        }
   
            bytes = request.getParameter("bytes");
        if(bytes == null){
            msgs.add("WrongSize");
            mistakes =true;
         }
         String size  = request.getParameter("size");
         Size = Integer.parseInt(size);

         if(Size<0){
                msgs.add("NegSize");
                mistakes =true;
         }

         if(bytes!=null){
            if (bytes.equals("kb")){
                size_ = Size*kb;
            }
            else if (bytes.equals("mb")){
                size_ = Size*mb;
            }
            else if (bytes.equals("gb")){
                size_=Size*gb;
            }
        }
        
        if(mistakes == false) {
        
                Connection con = null;
		try{
                    Class.forName("com.mysql.jdbc.Driver");
                    con = (Connection) DriverManager.getConnection("jdbc:mysql://localhost/test","root","2507");

                    //RequestDispatcher rd = request.getRequestDispatcher("asxeto.jsp");
                  
                            ResultSet rs = null;
                            Statement st = (Statement) con.createStatement();

                            

                            if(folder!=null && folder.equals("inSpecial")){
                                String select = "SELECT * FROM fs_"+usrname+" WHERE type = 'folder' AND property = "+str.addQuotes(ending);

                                rs = st.executeQuery(select);

                                if(rs.next()==false){

                                    msgs.add("PropertyDoesnotExist");
                                    access.set(true);
                                    request.getRequestDispatcher("upload.jsp").forward(request, response);
                                    out.close();
                                    return;

                                }else {
                                  
                                    if((ending.equalsIgnoreCase(rs.getString("property")))){
                                        path_file = rs.getString("path")+"-"+rs.getString("name");
                                       

                                    }

                                }
                            }


                            String query  = "SELECT space,space_used,space_public,charge"
                                    + " FROM clients WHERE username="+str.addQuotes(usrname);
                            rs = st.executeQuery(query);

                            long taken = 0,sp_public = 0;
                            float charge = 0;
                            while (rs.next()) {
                                sp = rs.getLong("space");
                                taken = rs.getLong("space_used");
                                sp_public = rs.getLong("space_public");
                                charge = rs.getFloat("charge");

                            }
                            long Taken = taken/gb;
                            long Space = sp/gb;

                            if(taken + size_ >sp){
                                access.set(true);
                                request.getRequestDispatcher("failupload.jsp").include(request, response);
                            }else{
                                long filesize = 0;
                                long new_taken = 0;
                               
                                        filesize = size_;
                                        new_taken = Taken + size_;
                       
                                

                                String table0 = "INSERT INTO idfiles VALUES()";

                                st.executeUpdate(table0);
                                ResultSet res = st.executeQuery("SELECT COUNT(*) FROM idfiles");
                                int id = 0;

                                while (res.next()){
                                    id = res.getInt(1);
                                }
                                
                               String type = "data";
                               String property = null;

                               st.executeQuery("SELECT name FROM fs_"+usrname+" WHERE name = " + str.addQuotes(file)+" AND path = "+ str.addQuotes(path_file));

                               ResultSet ts = st.getResultSet();
                                boolean alreadyExists = ts.next();
                               if(alreadyExists == true){
                                    msgs.add("fileExists");
                                    mistakes  =true;
    
                               }

                               java.sql.Timestamp  sqlDate = new java.sql.Timestamp(new java.util.Date().getTime());
                               
                               String table = "INSERT INTO fs_"+usrname+" (id,name,path,type,size,privacy,price,money_earned,downloads,property,category,date)"
                                                + " VALUES("
                                                + id +","
                                                + str.addQuotes(file)+","
                                                + str.addQuotes(path_file)+","
                                                + str.addQuotes(type)+","
                                                + filesize+","
                                                + str.addQuotes(privacy)+","
                                                + 0+","
                                                + 0+","
                                                + 0+","
                                                +str.addQuotes(property)+","
                                                +str.addQuotes(category)+","
                                                +"'"+sqlDate+"'"+")";                                                

                                st.executeUpdate(table);

                                if(privacy != null && privacy.equals("public")){

                                    String table1 = "INSERT INTO public_files (id,name,owner,category,date,size,price,downloads) "
                                                + "VALUES("
                                                + id +","
                                                + str.addQuotes(file)+","
                                                + str.addQuotes(usrname)+","
                                                + str.addQuotes(category)+","
                                                + "'"+sqlDate+"'"+","
                                                + filesize+","
                                                + 0+","
                                                + 0+")";

                                    st.executeUpdate(table1);

                                    String update = "UPDATE clients SET space_public = "
                                                + (sp_public+filesize)
                                                + " WHERE username = "
                                                +str.addQuotes(usrname);

                                    st.executeUpdate(update);

                                    update = "UPDATE clients SET charge = "+(charge+(filesize/gb))+"WHERE username = "
                                            +str.addQuotes(usrname);

                                    st.executeUpdate(update);
                                }
                             
                                
                                String update = "UPDATE clients SET space_used = "
                                                + (taken+size_)
                                                + " WHERE username = "
                                                +str.addQuotes(usrname);

                                    st.executeUpdate(update);
                                    access.set(true);
                                    request.getRequestDispatcher("succUpload.jsp").include(request, response);
                            }


                            rs.close();
                            st.close();
                            con.close();



                        }catch (Exception s){
                    out.println(s.getMessage());
                    access.set(true);
                    request.getRequestDispatcher("errorGeneral.jsp").include(request, response);

                       

		}
		 finally {

            if(mistakes == true){
                access.set(true);
                request.getRequestDispatcher("upload.jsp").forward(request, response);
            }
        }
        }
        if(mistakes == true){
                access.set(true);
                request.getRequestDispatcher("upload.jsp").forward(request, response);
        }
        out.close();

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
