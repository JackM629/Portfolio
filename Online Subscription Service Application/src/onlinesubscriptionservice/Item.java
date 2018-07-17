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

public class Item {
    
    private int itemID;
    private String name;
    private int itemNumberAvailable;
    private int requestNumberAvailable;
    
    public int getItemID() {
        return itemID;
    }
    
    public String getName() {
        return name;
    }
    
    public int getItemNumberAvailable() {
        return itemNumberAvailable;
    }
    
    public int getRequestNumberAvailable() {
        return requestNumberAvailable;
    }
    
    public void setItemID(int itemID) {
        this.itemID = itemID;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public void setNumberAvailable(int itemNumberAvailable) {
        this.itemNumberAvailable = itemNumberAvailable;
    }
    
    public void setRequestNumberAvailable(int requestNumberAvailable) {
        this.requestNumberAvailable = requestNumberAvailable;
    }

    public Item(int itemID, String name, int itemNumberAvailable, int requestNumberAvailable) {
        this.itemID = itemID;
        this.name = name;
        this.itemNumberAvailable = itemNumberAvailable;
        this.requestNumberAvailable = requestNumberAvailable;
    }

}
