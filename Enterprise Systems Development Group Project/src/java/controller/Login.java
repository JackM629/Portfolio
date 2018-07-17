package controller;

import java.io.IOException;
import java.sql.SQLException;
import java.util.ArrayList;
import model.JDBCBean;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

//Created on : 27-Nov-2017, 13:16:36, Author: Frazer, Sasha, Jack

public class Login extends HttpServlet {

    /**
     * Processes requests for both HTTP <code>GET</code> and <code>POST</code>
     * methods.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        JDBCBean bean = (JDBCBean) getServletContext().getAttribute("JDBCBean");
        String username = request.getParameter("username");
        String password = request.getParameter("password");
        String errorMessage = "";
        boolean loginValidation = false;
        HttpSession session = request.getSession();

        ArrayList<ArrayList<Object>> requestedUserDetail = new ArrayList<>();

        
        try {
            requestedUserDetail = bean.sqlQueryToArrayList("SELECT * FROM ROOT.USERS WHERE \"id\"='" + username + "'");
        } catch (SQLException ex) {
            System.out.println("SQL Login statement failed to execute! ");
            ex.printStackTrace();
        }
        

        // Checks if the getUsername or password is empty 
        if (username == null || password == null || username.length() == 0 || password.length() == 0) {
            loginValidation = true;
            errorMessage = "<div class=\"error\">\n"
                    + "  <span class=\"errorMessageBtn\" onclick=\"this.parentElement.style.display='none';\">&times;</span> \n"
                    + "  <strong>Username and password are required</strong> \n"
                    + "</div>";
        } else if (requestedUserDetail.isEmpty() || !requestedUserDetail.get(0).get(1).equals(password)) {
            loginValidation = true;
            errorMessage = "<div class=\"error\">\n"
                    + "  <span class=\"errorMessageBtn\" onclick=\"this.parentElement.style.display='none';\">&times;</span> \n"
                    + "  <strong>Invalid Login details</strong> \n"
                    + "</div>";
        } else {
            loginValidation = false;
        }

        // If user provides incorrect information, load the login page again
        if (loginValidation) {
            request.setAttribute("ErrorMessage", errorMessage);
            RequestDispatcher view = request.getRequestDispatcher("/docs/Login");
            view.forward(request, response);
            // If the user is admin redirect to admin page
        } else if (bean.exists(username,password,"ADMIN")) {
            //Making it thread safe
            synchronized (session) {
                // Store user info in Session
                session.setAttribute("adminUsername", username);
            }
            Cookie userID = new Cookie("username", username);
            //Store user info in Cookie
            response.addCookie(userID);

            response.sendRedirect(request.getContextPath() + "/docs/admin/AdminDashboard");
        }
         else {
            //Making it thread safe
            synchronized (session) {
                // Store user info in Session
                session.setAttribute("username", username);
            }
            //Session will expire in 20 mins
            session.setMaxInactiveInterval(20 * 60);
            Cookie userID = new Cookie("username", username);
            //Store user info in Cookie
            response.addCookie(userID);
            /* If a POST is been successful, you would want to redirect the request, so that the request 
                    won't be resubmitted when the user refreshes the request*/
            response.sendRedirect(request.getContextPath() + "/docs/user/UserDashboard");
        }
    }

    // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP <code>GET</code> method.
     *
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
     *
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
     *
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "Short description";
    }// </editor-fold>

}
