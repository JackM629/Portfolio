/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package guitartabdeveloper;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Jack Martin (15009787)
 * 
 */

public class DbMgr {
    
    Connection connection = null;
    Statement stmt = null;
    ResultSet rs = null;
    
    public DbMgr() {
        
    }
    
    public DbMgr(String query) {
        
    }
    
    public void setConnection(Connection con) {
        connection = con;
    }
    
    public void getRows() {
        

    }
    
    public void add(String[] str){
        
        PreparedStatement ps = null;
        
        try {
            ps = connection.prepareStatement("INSERT INTO guitarTabs VALUES (?,?,?)",PreparedStatement.RETURN_GENERATED_KEYS);
            ps.setString(1, str[0].trim()); 
            ps.setString(2, str[1]);
            ps.setString(3, str[2]);
            ps.executeUpdate();
        
            ps.close();
            System.out.println("1 row added.");
        } 
        
        catch (SQLException ex) {
            Logger.getLogger(DbMgr.class.getName()).log(Level.SEVERE, null, ex);
        }
         
    }
    
    public void search(String searchTerm) {
        
    }
    
    public void connect() {
        
        String str = "select * from tabs";
        
        DbMgr dbmgr = new DbMgr(str);
        Connection con = null;
        
        try {
            Class.forName("org.apache.derby.jdbc.ClientDriver");
            con = DriverManager.getConnection("jdbc:derby://localhost:1527/tabs","Jack","1234");
        } catch(ClassNotFoundException | SQLException e) {
            System.out.println("Connection error");
        }
        System.out.println("Database connected");
        
        dbmgr.setConnection(con);
        
    }
    
}
