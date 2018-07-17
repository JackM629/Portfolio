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

public class Member {

    private int memberID;
    private String memberName;
    private int itemsBooked;
    private int itemsRequested;

    public int getMemberID() {
        return memberID;
    }
    
    public String getMemberName() {
        return memberName;
    }
    
    public int getItemsBooked() {
        return itemsBooked;
    }
    
    public int getItemsRequested() {
        return itemsRequested;
    }

    public void setMemberID(int memberID) {
        this.memberID = memberID;
    }

    public void setMemberName(String memberName) {
        this.memberName = memberName;
    }
    
    public void setItemsBooked(int itemsBooked) {
        this.itemsBooked = itemsBooked;
    }
    
    public void setItemsRequested(int itemsRequested) {
        this.itemsRequested = itemsRequested;
    }

    public Member(int memberID, String memberName, int itemsBooked, int itemsRequested) {
        this.memberID = memberID;
        this.memberName = memberName;
        this.itemsBooked = itemsBooked;
        this.itemsRequested = itemsRequested;
    }

}
