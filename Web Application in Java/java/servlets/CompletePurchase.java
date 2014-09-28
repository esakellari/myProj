
package servlets;

import beans.*;
import general.*;
import java.util.*;
import java.sql.*;
import java.io.*;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;




public class CompletePurchase extends HttpServlet {


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

        ArrayList<CartElement> cart = (ArrayList<CartElement>)session.getAttribute("cart");
        if(cart == null) {
            cart = new ArrayList<CartElement>();
        }


        String cmd = request.getParameter("cmd");

        if(cmd == null) {
            access.set(true);
            request.getRequestDispatcher("completePurchase.jsp").include(request, response);

        } else if(cmd.compareTo("cancel") == 0) {
            response.sendRedirect( response.encodeRedirectURL(path.get()) );

        } else if(cmd.compareTo("buy") == 0) {

            String ccCustomer = null;

            if(user == null  ||  user.getUsername() == null) {
                String cc = request.getParameter("card_code");
                if(cc == null  ||  cc.isEmpty() == true) {
                    msgs.add("GiveCardCode");
                    access.set(true);
                    request.getRequestDispatcher("completePurchase.jsp").include(request, response);
                    out.close();
                    return;
                } else {
                    ccCustomer = cc;
                }
            } else {
                ccCustomer = user.getCardCode();
            }



            StringFixer sf = new StringFixer();
            ArrayList<String> failed = new ArrayList<String>();
            ArrayList<String> bought = new ArrayList<String>();

            Connection conn = null;
            try {
                Class.forName("com.mysql.jdbc.Driver");
                conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/test","root","2507");


                Iterator<CartElement> it = cart.iterator();
                while(it.hasNext() == true) {

                    CartElement elem = it.next();
                    try {
                        Statement st = conn.createStatement();
                        ResultSet rs = null;

                        
                        st.executeQuery("SELECT * FROM public_files WHERE id = '" + elem.getId() + "'");
                        rs = st.getResultSet();
                        if(rs.next() == false) {            // Elegxei ean to arxeio yparxei akoma ston
                            failed.add(elem.getName());     // pinaka twn public files.
                            continue;
                        }

                        
                        int downloads = rs.getInt("downloads");
                        String owner  = rs.getString("owner");

                        //if(owner.compareTo(user.getUsername()) == 0) {
                          //  failed.add(elem.getName());
                            //continue;
                       // }

                        // Pairnei tis aparaitites plirofories tou idioktiti tou arxeiou apo ton pinaka "clients"
                        // kai tou idiou tou arxeiou apo ton adistoixo pinaka arxeiwn tou idioktiti.
                        //
                        st.executeQuery("SELECT money_earned,card_code FROM clients WHERE username = " + sf.addQuotes(owner));
                        rs = st.getResultSet();
                        rs.next();
                        int total_money_earned = rs.getInt("money_earned");
                        String ccSeller        = rs.getString("card_code");

                        st.executeQuery("SELECT money_earned FROM fs_" + owner + " WHERE id = '" + elem.getId() + "'");
                        rs = st.getResultSet();
                        rs.next();
                        int file_money_earned = rs.getInt("money_earned");


                        if(doTransaction(ccSeller, ccCustomer) == true) {
                            total_money_earned += elem.getPrice();
                            file_money_earned += elem.getPrice();
                            ++downloads;

                            bought.add(elem.getName());

                            st.executeUpdate("UPDATE clients SET money_earned='" + total_money_earned
                                           + "' WHERE username = " + sf.addQuotes(owner));
                            st.executeUpdate("UPDATE fs_" + owner + " SET money_earned='" + file_money_earned
                                           + "' WHERE id = '" + elem.getId() + "'");
                            st.executeUpdate("UPDATE fs_" + owner + " SET downloads='" + downloads
                                           + "' WHERE id = '" + elem.getId() + "'");
                            st.executeUpdate("UPDATE public_files SET downloads='" + downloads
                                           + "' WHERE id = '" + elem.getId() + "'");
                        } else {
                            failed.add(elem.getName());
                        }

                        st.close();

                    } catch(SQLException e) {
                        System.out.println("ERROR [CompletePurchase.processRequest >> *]: SQLException");
                        access.set(true);
                        request.getRequestDispatcher("errorGeneral.jsp").forward(request, response);
                    }
                }

                session.setAttribute("failed", failed);
                session.setAttribute("bought", bought);
                access.set(true);
                request.getRequestDispatcher("purchaseDone.jsp").forward(request, response);
                session.removeAttribute("bought");
                session.removeAttribute("failed");

                conn.close();

            } catch(ClassNotFoundException e) {
                System.out.println("ERROR [CompletePurchase.processRequest >> Class.forName]: ClassNotFoundException");
                access.set(true);
                request.getRequestDispatcher("errorGeneral.jsp").forward(request, response);
            } catch(SQLException e) {
                System.out.println("ERROR [CompletePurchase.processRequest >> DriverManager.getConnection]: SQLException");
                access.set(true);
                request.getRequestDispatcher("errorGeneral.jsp").forward(request, response);
            }

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



    boolean doTransaction(String ccSeller, String ccCustomer) {
        return true;
    }
}
