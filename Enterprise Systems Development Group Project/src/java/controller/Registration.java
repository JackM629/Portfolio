/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package controller;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import model.JDBCBean;

//Created on : 27-Nov-2017, 13:16:36, Author: Frazer, Sasha

public class Registration extends HttpServlet {

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

        String firstName = request.getParameter("firstName");
        String lastName = request.getParameter("lastName");
        String name = firstName + " " + lastName;
        String username = "";
        String password = "";
        String streetNumber = request.getParameter("houseNumber");
        String streetName = request.getParameter("streetName");
        String city = request.getParameter("city");
        String postcode = request.getParameter("postcode");
        String country = request.getParameter("country");

        String address = streetNumber + ", " + streetName + ", " + city + ", " + postcode + ", " + country;
        String dob = request.getParameter("DOB").replaceAll("/", "-");
        System.out.println("DOB = " + dob);
        //DOR to current date in YYYY-MM-DD format
        SimpleDateFormat sqlDateFormatForRegistration = new SimpleDateFormat("yyyy-MM-dd");
        String dor = sqlDateFormatForRegistration.format(Calendar.getInstance().getTime());
        //password in DDMMYY format

        float RegistrationFee = 10;
        String defaultStatus = "APPLIED";

        String errorMessageForRegistration = "";
        boolean registrationValidation = false;

        if (firstName.isEmpty() || lastName.isEmpty() || streetNumber.isEmpty() || streetName.isEmpty()
                || city.isEmpty() || postcode.isEmpty() || country.isEmpty() || dob.isEmpty()) {
            registrationValidation = true;
            errorMessageForRegistration = "<div class=\"error\">\n"
                    + "  <span class=\"errorMessageBtn\" onclick=\"this.parentElement.style.display='none';\">&times;</span> \n"
                    + "  <strong>All the fields are required</strong> \n"
                    + "</div>";
        } else {
            //generating username
            username = (firstName.charAt(0) + "-" + lastName).toLowerCase();
            String editDOBForDatabase = dob.replaceAll("(..)-(..)-(....)", "$3-$2-$1");
            //password in DDMMYY format
            password = request.getParameter("DOB").replaceAll("(..)(..)-(..)-(..)", "$4$3$2");
            bean.executeSQLUpdate("INSERT INTO ROOT.MEMBERS(\"id\", \"name\", \"address\", \"dob\", \"dor\", \"status\", \"balance\")"
                    + "VALUES (" + "'" + username + "','" + name + "','" + address + "','" + editDOBForDatabase + "','" + dor + "','" + defaultStatus + "'," + RegistrationFee + ")");
           //ROOT.USERS 
            bean.executeSQLUpdate("INSERT INTO ROOT.USERS(\"id\", \"password\", \"status\")"
                    + "VALUES (" + "'" + username + "','" + password + "','" + defaultStatus + "'" + ")");
            registrationValidation = false;
        }
        if (registrationValidation) {
            request.setAttribute("ErrorMessageForRegistration", errorMessageForRegistration);
            RequestDispatcher view = request.getRequestDispatcher("/docs/Registration");
            view.forward(request, response);
        } else {
            request.setAttribute("registeredUsername", username);
            request.setAttribute("registeredUsernamePassword", password);
            request.getRequestDispatcher("/docs/RegistrationSuccessful").forward(request, response);
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
