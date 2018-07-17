package controller;

import java.io.IOException;
import java.sql.SQLException;
import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import model.JDBCBean;

//Created on : 27-Nov-2017, 13:16:36, Author: Frazer, Sasha, Jack
public class AdminController extends HttpServlet {

    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        //get JDBC Bean
        JDBCBean bean = (JDBCBean) getServletContext().getAttribute("JDBCBean");

        //Find JSP that refered resource
        String requestingView = request.getParameter("viewId");
        getServletContext().log("Admin Controller received a request from " + requestingView);

        String include;
        switch (requestingView) {

            //Manage Members
            case "/ListMembers":
                getMemberById(bean, request, request.getParameter("selectedMember"));
                include = "/docs/admin/ManageMember";
                break;
            case "/ListBalance":
                getMemberById(bean, request, request.getParameter("selectedMember"));
                include = "/docs/admin/ManageMember";
                break;
            case "/ManageMember":
                suspendOrResumeMember(bean, request);
                include = "/docs/admin/ManageMember";
                break;

            //Manage Claims
            case "/ListClaims":
                getClaim(bean, request, request.getParameter("selectedClaim"));
                getMemberByClaimId(bean, request, request.getParameter("selectedClaim"));
                include = "/docs/admin/ManageClaim";
                break;
            case "/ManageClaim":
                acceptOrRejectClaim(bean, request);
                include = "/docs/admin/ManageClaim";
                break;

            //Manage Applications
            case "/ListApplications":
                getMemberById(bean, request, request.getParameter("selectedMember"));
                include = "/docs/admin/ManageApplication";
                break;
            case "/ManageApplication":
                approveApplication(bean, request);
                include = "/docs/admin/ManageApplication";
                break;

            //Manage Turnover
            case "/ManageTurnover":
                chargeMembers(bean, request);
                include = "/docs/admin/ManageTurnover";
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

    //Returns selected memebers details, including all claims and payments
    protected void getMemberById(JDBCBean bean, HttpServletRequest request, String memberID) {

        //Generate lists for required tables
        ArrayList requestedMember = new ArrayList();
        ArrayList requestedClaims = new ArrayList();
        ArrayList requestedPayments = new ArrayList();

        try {
            requestedMember = bean.sqlQueryToArrayList("SELECT * FROM MEMBERS WHERE \"id\"='" + memberID + "'");
            requestedClaims = bean.sqlQueryToArrayList("SELECT * FROM CLAIMS WHERE \"mem_id\"='" + memberID + "'");
            requestedPayments = bean.sqlQueryToArrayList("SELECT * FROM PAYMENTS WHERE \"mem_id\"='" + memberID + "'");
        } catch (SQLException ex) {
            System.out.println("SQL failed to execute in AdminController, getMember. " + ex);
        }

        request.setAttribute("requestedMember", requestedMember.get(0));
        request.setAttribute("requestedClaims", requestedClaims);
        request.setAttribute("requestedPayments", requestedPayments);
    }

    //Returns member associated with claim ID provided
    protected void getMemberByClaimId(JDBCBean bean, HttpServletRequest request, String claimID) {

        //Get member ID
        String memberID = (String) getClaim(bean, request, claimID).get(1);

        //Get member bu ID
        getMemberById(bean, request, memberID);

    }

    //Gets selected claim by ID
    protected ArrayList getClaim(JDBCBean bean, HttpServletRequest request, String claimID) {

        //Get selected claim
        ArrayList claim = new ArrayList();

        try {
            claim = (ArrayList) bean.sqlQueryToArrayList("SELECT * FROM CLAIMS WHERE \"id\"=" + claimID).get(0);
        } catch (SQLException ex) {
            System.out.println("SQL failed to execute in AdminController, getClaim. " + ex);
        }

        request.setAttribute("requestedClaim", claim);

        return claim;
    }

    //Sets status of members account
    protected void suspendOrResumeMember(JDBCBean bean, HttpServletRequest request) {

        //Split into action and member ID
        String action = request.getParameter("manageMemberAction").split("_")[0];
        String memberID = request.getParameter("manageMemberAction").split("_")[1];

        if (action.equalsIgnoreCase("suspend")) {
            bean.executeSQLUpdate("UPDATE MEMBERS SET \"status\"='SUSPENDED' WHERE \"id\"='" + memberID + "'");
            bean.executeSQLUpdate("UPDATE USERS SET \"status\"='SUSPENDED' WHERE \"id\"='" + memberID + "'");
        } else if (action.equalsIgnoreCase("resume")) {
            bean.executeSQLUpdate("UPDATE MEMBERS SET \"status\"='APPROVED' WHERE \"id\"='" + memberID + "'");
            bean.executeSQLUpdate("UPDATE USERS SET \"status\"='APPROVED' WHERE \"id\"='" + memberID + "'");
        }
        getMemberById(bean, request, memberID);
    }

    //Upgrades provisional memeber to full member or rejects application
    protected void approveApplication(JDBCBean bean, HttpServletRequest request) {

        //Split into action and member ID
        String action = request.getParameter("manageMemberAction").split("_")[0];
        String memberID = request.getParameter("manageMemberAction").split("_")[1];

        if (action.equalsIgnoreCase("approve")) {
            bean.executeSQLUpdate("UPDATE MEMBERS SET \"status\"='APPROVED' WHERE \"id\"='" + memberID + "'");
            bean.executeSQLUpdate("UPDATE USERS SET \"status\"='APPROVED' WHERE \"id\"='" + memberID + "'");
       }

        getMemberById(bean, request, memberID);
    }

    //Sets status of selected claim
    protected void acceptOrRejectClaim(JDBCBean bean, HttpServletRequest request) {

        //Split into action and claim ID
        String action = request.getParameter("manageClaimAction").split("_")[0];
        String claimID = request.getParameter("manageClaimAction").split("_")[1];

        if (action.equalsIgnoreCase("accept")) {
            bean.executeSQLUpdate("UPDATE CLAIMS SET \"status\"='APPROVED' WHERE \"id\"=" + claimID);
        } else if (action.equalsIgnoreCase("reject")) {
            bean.executeSQLUpdate("UPDATE CLAIMS SET \"status\"='REJECTED' WHERE \"id\"=" + claimID);
        }
        getMemberByClaimId(bean, request, claimID);
    }

    //Charge members yearly fee
    protected void chargeMembers(JDBCBean bean, HttpServletRequest request) {

        //Get sum of all approved claims and number of active members
        ArrayList approvedClaims = new ArrayList();
        ArrayList activeMembers = new ArrayList();

        DateFormat df = new SimpleDateFormat("yyyy-MM-dd");
        Calendar cal2 = Calendar.getInstance();
        cal2.add(Calendar.YEAR, -1);
        Date lastYear = cal2.getTime();
        String formattedDate = df.format(lastYear);
        
        try {
            approvedClaims = (ArrayList) bean.sqlQueryToArrayList("SELECT SUM(\"amount\") FROM CLAIMS WHERE \"status\"='APPROVED' AND \"date\" > '" + formattedDate + "'").get(0);
            activeMembers = (ArrayList) bean.sqlQueryToArrayList("SELECT COUNT(*) FROM MEMBERS WHERE \"status\"='APPROVED'").get(0);
        } catch (SQLException ex) {
            System.out.println("SQL failed to execute in AdminController, chargeMembers. " + ex);
        }

        //Calculate charge for each member
        Double sumOfClaims = (Double) approvedClaims.get(0);
        Integer sumOfActiveMembers = (Integer) activeMembers.get(0);
        Double individualMemberCharge = sumOfClaims / sumOfActiveMembers;
        DecimalFormat decf = new DecimalFormat("####.##");
        String membersCharge = decf.format(individualMemberCharge);

        //Add charge to active members balance
        bean.executeSQLUpdate("UPDATE MEMBERS SET \"balance\"=\"balance\" + " + membersCharge + " WHERE \"status\"='APPROVED'");

        request.setAttribute("membersCharge", membersCharge);
    }
}
