/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package onlinesubscriptionservice;

/**
 * Jack Martin
 * 15009787
 */
public class Transaction {
    
    private String transactionID;
    private int itemID;
    private int memberID;
    private String date;
    private boolean itemInStock;

    public String getTransactionID() {
        return transactionID;
    }
    
    public int getItemID() {
        return itemID;
    }
    
    public int getMemberID() {
        return memberID;
    }
    
    public String getDate(){
        return date;
    }
    
    public boolean getItemInStock() {
        return itemInStock;
    }

    public void setTransactionID(String transactionID) {
        this.transactionID = transactionID;
    }
    
    public void setItemID(int itemID) {
        this.itemID = itemID;
    }
    
    public void setMemberID(int memberID) {
        this.memberID = memberID;
    }
    
    public void setDate(String date) {
        this.date = date;
    }
    
    public void setItemInStock(boolean itemInStock) {
        this.itemInStock = itemInStock;
    }

    public Transaction(String transactionID, int itemID, int memberID, String date, boolean itemInStock) {
        this.transactionID = transactionID;
        this.itemID = itemID;
        this.memberID = memberID;
        this.date = date;
        this.itemInStock = itemInStock;
    }

}
