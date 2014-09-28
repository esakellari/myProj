
package servlets;

import beans.*;
import general.*;
import java.util.*;
import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;




public class Cart extends HttpServlet {


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
        LinkedList<String> msgs = (LinkedList<String>)session.getAttribute("msgs");
        if(msgs == null) {
            session.setAttribute("msgs", new LinkedList<String>());
            msgs = (LinkedList<String>)session.getAttribute("msgs");
        }

        ArrayList<CartElement> cart = (ArrayList<CartElement>)session.getAttribute("cart");
        if(cart == null) {
            cart = new ArrayList<CartElement>();
        }



        String cmd = request.getParameter("cmd");

        if(cmd == null) {
            msgs.add("WrongParams");
            access.set(true);
            request.getRequestDispatcher("errorUser.jsp").forward(request, response);

        } else if(cmd.compareTo("add") == 0) {
            try {
                String name = request.getParameter("name");
                int id      = Integer.parseInt(request.getParameter("id"));
                int size    = Integer.parseInt(request.getParameter("size"));
                int price   = Integer.parseInt(request.getParameter("price"));

                cart.add(new CartElement(name, id, size, price));
            } catch(NumberFormatException e) {
                ;
            } finally {
                response.sendRedirect( response.encodeRedirectURL(path.get()) );
            }

        } else if(cmd.compareTo("remove") == 0) {
            try {
                int index = Integer.parseInt(request.getParameter("index"));

                cart.remove(index);
            } catch(NumberFormatException e) {
                ;
            } finally {
                response.sendRedirect( response.encodeRedirectURL(path.get()) );
            }

        } else if(cmd.compareTo("clear") == 0) {
            cart.clear();
            response.sendRedirect( response.encodeRedirectURL(path.get()) );

        } else if (cmd.compareTo("buy") == 0) {
            response.sendRedirect("CompletePurchase");

        } else {
            msgs.add("WrongParams");
            access.set(true);
            request.getRequestDispatcher("errorUser.jsp").forward(request, response);

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
