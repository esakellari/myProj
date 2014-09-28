
package servlets;

import beans.*;
import java.util.LinkedList;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;




public class SignOut extends HttpServlet {


    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException {

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


        if(user.getUsername() == null) {        // Ean o xristis pou kanei signout den einai signed in.
            System.out.println("Simple user tried to 'Sign Out' while not logged in");
            access.set(true);
            msgs.add("NotLoggedIn");
            request.getRequestDispatcher("errorUser.jsp").forward(request, response);
            return;
        } else {
            session.setAttribute("user", new User());   // "katharizei" ti session variable "user".
            response.sendRedirect( response.encodeRedirectURL(path.get()) );
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
