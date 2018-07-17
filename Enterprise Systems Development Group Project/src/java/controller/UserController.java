/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package controller;

import java.io.IOException;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import model.JDBCBean;

//Created on : 27-Nov-2017, 13:16:36, Author: Frazer, Sasha, Jack

public class UserController extends HttpServlet {

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

        JDBCBean bean = (JDBCBean) getServletContext().getAttribute("JDBCBean");

        //Find JSP that refered resource
        String requestingView = request.getParameter("viewId");
        getServletContext().log("User Controller received a request from " + requestingView);

        String include;
        switch (requestingView) {
            case "/UserBalance":
                getBalance(bean, request);
                include = "/docs/user/UserBalance";
                break;
            case "/UserMakeClaim":
                makeClaim(bean, request);
                include = "/docs/user/UserClaimConfirm";
                break;
            case "/UserMakePayment":
                makePayment(bean, request);
                include = "/docs/user/UserPaymentConfirm";
                break;
            case "/UserListClaims":
                listUserTable(bean, request, "Claims");
                include = "/docs/user/ListUserClaims";
                break;
            case "/UserListPayments":
                listUserTable(bean, request, "payments");
                include = "/docs/user/ListUserPayments";
                break;
            default:
                include = "/docs/Error404.jsp";
        }

        request.getRequestDispatcher(include).forward(request, response);

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

    public void getBalance(JDBCBean bean, HttpServletRequest request) {
        String user = request.getParameter("username");
        ArrayList currentBalanceArr = checkBalance(bean, user);
        Double currentBal = (Double) currentBalanceArr.get(0);
        request.setAttribute("balance", currentBal);

    }

    private void makeClaim(JDBCBean bean, HttpServletRequest request) {
        ArrayList temp;
        String user = request.getParameter("username");
        String rationale = request.getParameter("rationale");
        double amount = Double.parseDouble(request.getParameter("amount"));
        try {
            if (isMember(bean, user)) {
                temp = getRowNum1(bean, "'id'", "Claims");
                bean.executeSQLUpdate("INSERT INTO ROOT.CLAIMS VALUES (" + ((int) temp.get(0) + 1) + ",'" + user + "','" + new java.sql.Date(Calendar.getInstance().getTime().getTime()) + "','" + rationale + "'," + "'SUBMITTED'" + "," + amount + ")");
              
                request.setAttribute("confirm", "succeeded");
            } else {
                request.setAttribute("confirm", "failed, not paid member");
            }
        } catch (SQLException ex) {
            request.setAttribute("confirm", "failed");
            System.out.println("SQL failed to execute in UserController, makeClaim! " + ex);
        }
    }

    public ArrayList getRowNum1(JDBCBean bean, String column, String table) throws SQLException {
        return (ArrayList) bean.sqlQueryToArrayList("SELECT COUNT(\"id\") FROM ROOT.Claims").get(0);
    }
    
    public ArrayList getRowNum2(JDBCBean bean, String column, String table) throws SQLException {
        return (ArrayList) bean.sqlQueryToArrayList("SELECT COUNT(\"id\") FROM ROOT.Payments").get(0);
    }
    
    

    public void makePayment(JDBCBean bean, HttpServletRequest request) {
        ArrayList currentBalanceArr, numOfRows;
        String user = request.getParameter("username");
        String paymentType = request.getParameter("paymentType");
        float amount = Float.parseFloat(request.getParameter("amount"));
        SimpleDateFormat sqlDateFormatForRegistration = new SimpleDateFormat("yyyy-MM-dd");
        String date = sqlDateFormatForRegistration.format(Calendar.getInstance().getTime());
        SimpleDateFormat sqlDateFormatForRegistration1 = new SimpleDateFormat("HH:mm:ss");
        String time = sqlDateFormatForRegistration1.format(Calendar.getInstance().getTime());
        try {
            currentBalanceArr = checkBalance(bean, user);
            Double currentBal = (Double) currentBalanceArr.get(0);
            request.setAttribute("balance", currentBal);
            if (amount <= 0) {
                request.setAttribute("balance", currentBal);
                request.setAttribute("confirm", "failed, incorrect value");
            } else if (currentBal >= amount) {
                numOfRows = getRowNum2(bean, "id", "Payments");
                bean.executeSQLUpdate("INSERT INTO PAYMENTS VALUES (" + ((int) numOfRows.get(0) + 1) + ",'" + user + "','" + paymentType + "'," + amount + ",'" + date + "','" + time + "')");
                double newbal = currentBal - amount;
                bean.executeSQLUpdate("UPDATE MEMBERS SET \"balance\"= "+ newbal + "WHERE \"id\"='" + user + "'");
                request.setAttribute("balance", (currentBal - amount));
                request.setAttribute("confirm", "succeeded");
            } else {
                request.setAttribute("confirm", "failed, lack of funds");
            }
        } catch (SQLException ex) {
            request.setAttribute("confirm", "failed");
            System.out.println("SQL failed to execute in UserController, makePayment! " + ex);
        }
    }

    private ArrayList checkBalance(JDBCBean bean, String user) {
        try {
            return (ArrayList) bean.sqlQueryToArrayList("SELECT \"balance\" FROM ROOT.Members WHERE \"id\"='" + user + "'").get(0);
        } catch (SQLException ex) {
            Logger.getLogger(UserController.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }

    private void listUserTable(JDBCBean bean, HttpServletRequest request, String table) {
        ArrayList list;
        try {
            list = (ArrayList) bean.sqlQueryToArrayList("SELECT * FROM " + table + " WHERE \"mem_id\"='" + request.getParameter("username") + "'");
            request.setAttribute("data", list);
        } catch (SQLException ex) {
            Logger.getLogger(UserController.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    private boolean isMember(JDBCBean bean, String user) {
//        SELECT Members.id, Members.status FROM Members INNER JOIN users ON Members.status=users.status 
//        AND Members.id=users.id AND Members.id='t-fisher'
        ArrayList userStatus;
        try {
            userStatus = (ArrayList) bean.sqlQueryToArrayList("SELECT MEMBERS.\"status\" FROM MEMBERS INNER JOIN USERS ON MEMBERS.\"status\"=USERS.\"status\" AND MEMBERS.\"id\"=USERS.\"id\" AND MEMBERS.\"id\"='" + user + "'").get(0);
            if (((String) userStatus.get(0)).equals("APPROVED")) {
                return true;
            }
        } catch (SQLException ex) {
            Logger.getLogger(UserController.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }
        return false;
    }

}
