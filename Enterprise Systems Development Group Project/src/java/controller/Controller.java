package controller;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

//Created on : 27-Nov-2017, 13:16:36, Author: Frazer, Sasha, Jack
@WebServlet(name = "Controller", urlPatterns = {"/Controller", "/docs/*"})
public class Controller extends HttpServlet {

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
	 
        HttpSession session = request.getSession();

        //Always Forward to main.jsp
        String mainPage = "/WEB-INF/docs/main.jsp";

        //Find requested resource
        String requestPath = request.getRequestURI().substring(request.getContextPath().length());

        getServletContext().log("Controller received a request for " + requestPath);

        String include;
        switch (requestPath) {
            case "/Controller":
                include = "mainStart.jsp";
                break;

            //Login		
            case "/docs/Login":
                include = "Login.jsp";
                break;

            //Registration		
            case "/docs/Registration":
                include = "Registration.jsp";
                break;

            //Admin		
            case "/docs/admin/AdminDashboard":
                include = "admin/AdminDashboard.jsp";
                break;
            case "/docs/admin/ListMembers":
                include = "admin/ListMembers.jsp";
                break;
            case "/docs/admin/ListBalance":
                include = "admin/ListBalance.jsp";
                break;
            case "/docs/admin/ManageMember":
                include = "admin/ManageMember.jsp";
                break;
            case "/docs/admin/ListClaims":
                include = "admin/ListClaims.jsp";
                break;
            case "/docs/admin/ManageClaim":
                include = "admin/ManageClaim.jsp";
                break;
            case "/docs/admin/ListApplications":
                include = "admin/ListApplications.jsp";
                break;
            case "/docs/admin/ManageApplication":
                include = "admin/ManageApplication.jsp";
                break;
            case "/docs/admin/ManageTurnover":
                include = "admin/ManageTurnover.jsp";
                break;

            //Users		
            case "/docs/user/UserDashboard":
                include = "user/UserDashboard.jsp";
                break;

            case "/docs/user/ManageUserBalance":
                include = "user/ManageUserBalance.jsp";
                break;

            case "/docs/user/UserBalance":
                include = "user/UserBalance.jsp";
                break;

            case "/docs/user/UserMakeClaim":
                include = "user/UserClaimForm.jsp";
                break;

            case "/docs/user/UserClaimConfirm":
                include = "user/UserClaimConfirm.jsp";
                break;

            case "/docs/user/UserMakePayment":
                include = "user/UserPaymentForm.jsp";
                break;

            case "/docs/user/UserPaymentConfirm":
                include = "user/UserPaymentConfirm.jsp";
                break;

            case "/docs/user/ManageUserClaims":
                include = "user/ManageUserClaims.jsp";
                break;

            case "/docs/user/ListUserClaims":
                include = "user/ListUserClaims.jsp";
                break;

            case "/docs/user/ManageUserPayments":
                include = "user/ManageUserPayments.jsp";
                break;

            case "/docs/user/ListUserPayments":
                include = "user/ListUserPayments.jsp";
                break;

            case "/docs/RegistrationSuccessful":
                include = "RegistrationSuccessful.jsp";
                break;

            default:
                include = "/docs/Error404.jsp";
        }

        request.setAttribute("included", include);

        request.getRequestDispatcher(mainPage).forward(request, response);
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
