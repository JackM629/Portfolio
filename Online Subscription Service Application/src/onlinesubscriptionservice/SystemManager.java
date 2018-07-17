 /*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package onlinesubscriptionservice;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Scanner;

/**
 * Jack Martin
 * 15009787
 */
public class SystemManager {
    
    //Array declarations
    ArrayList<Member> members = new ArrayList<>();
    ArrayList<Item> items = new ArrayList<>();
    ArrayList<Transaction> transactions = new ArrayList<>();
    Item itemToBeBooked;
    
    public String addMember(String newMemberIDNumber, String newMemberName) {
        
        boolean memberIDAvailable = true;
        
        //Carries out validation checks on inputted values, returns an appropriate error message if a value is invalid
        if (newMemberIDNumber.length() != 5) {
            return "Your Member ID Number must be 5 characters long";
        }
        else if (stringNumeric(newMemberIDNumber) == false) {
            return "Your Member ID Number must be an integer";
        }
        else if (newMemberName.length() < 2 || newMemberName.length() > 30) {
            return "Your Member Name must be between 2 and 30 characters long";
        }
        else {
            
            //Checks that inputted member ID number is not already taken
            for (int i = 0; i < members.size(); i++) {
                Member workingMember = members.get(i);
                int workingMemberIDNumber = workingMember.getMemberID();
                if (workingMemberIDNumber == Integer.parseInt(newMemberIDNumber)) {
                    memberIDAvailable = false;
                }
            }
            
            //Adds new member if member ID number is not taken
            if (memberIDAvailable == true) {
                members.add(new Member(Integer.parseInt(newMemberIDNumber), newMemberName, 0, 0));
                return null;
            }
            
            //Returns error message if member ID number is taken
            else {
                return "That Member ID Number is already taken";
            }
        }
    }
    
    public String addItem(String newItemIDNumber, String newItemName, String newItemInitialNumberAvailable) {
        
        boolean itemIDAvailable = true;
        
        //Carries out validation checks on inputted values, returns an appropriate error message if a value is invalid
        if (newItemIDNumber.length() != 4) {
            return "Your Item ID Number must be 4 characters long";
        }
        else if (stringNumeric(newItemIDNumber) == false) {
            return "Your Item ID Number must be an integer";
        }
        else if (newItemName.length() < 2 || newItemName.length() > 30) {
            return "Your Item Name must be between 2 and 30 characters long";
        }
        else if (stringNumeric(newItemInitialNumberAvailable) == false) {
            return "The Number Available must be an integer";
        }
        else {
            
            //Checks that inputted member ID number is not already taken
            for (int i = 0; i < items.size(); i++) {
                Item workingItem = items.get(i);
                int workingItemIDNumber = workingItem.getItemID();
                if (workingItemIDNumber == Integer.parseInt(newItemIDNumber)) {
                    itemIDAvailable = false;
                }
            }
            
            //Adds new member if member ID number is not taken
            if (itemIDAvailable == true) {
                items.add(new Item(Integer.parseInt(newItemIDNumber), newItemName, Integer.parseInt(newItemInitialNumberAvailable), 10));
                return null;
            }
            
            //Returns error message if member ID number is taken
            else {
                return "That Item ID Number is already taken";
            }
        }
    }
    
    public String addMemberTextFile() {
        Scanner readMember = new Scanner(RunGUI.class.getResourceAsStream("NewMemberInfo"));
        String workingLine;
        String newMemberIDNumber;
        String newMemberName;
        int workingField;
        char workingChar;
        int i;
        int lineCount = 1;
        boolean memberIDAvailable = true;
        
        while (readMember.hasNextLine()) {
            newMemberIDNumber = "";
            newMemberName = "";
            workingLine = readMember.nextLine();
            workingField = 0;
            i = 0;
            
            //Splits each line up into a member ID number and a member name
            while (workingField < 2 && i < workingLine.length()) {
                workingChar = workingLine.charAt(i);
                if (workingChar == ',') {
                    workingField++;
                }
                else if (workingField == 0) {
                    newMemberIDNumber = newMemberIDNumber + workingChar;
                }
                else {
                    newMemberName = newMemberName + workingChar;
                }
                i++;
            }
            
            //Carries out validation checks on inputted values, returns an appropriate error message if a value is invalid
            if (newMemberIDNumber.length() != 5) {
                return "The member ID number on line" + lineCount + " is not 5 characters long";
            }
            else if (stringNumeric(newMemberIDNumber) == false) {
                return "The member ID number on line " + lineCount + " is not numerical";
            }
            else if (newMemberName.length() < 2 || newMemberName.length() > 30) {
                return "The member name on line" + lineCount + " is not between 2 and 30 characters long";
            }
            else {
                    
                //Checks that inputted member ID number is not already taken
                for (int j = 0; j < members.size(); j++) {
                    Member workingMember = members.get(j);
                    int workingMemberIDNumber = workingMember.getMemberID();
                    if (workingMemberIDNumber == Integer.parseInt(newMemberIDNumber)) {
                        memberIDAvailable = false;
                    }
                }
                    
                if (memberIDAvailable == true) {
                    members.add(new Member(Integer.parseInt(newMemberIDNumber), newMemberName, 0, 0));
                }
            
                //Returns error message if member ID number is taken
                else {
                    return "The member ID number on line " + lineCount + " is already taken";
                }
                lineCount++;
            }
        }
        return null;
    }
    
    public String addItemTextFile() {
        Scanner readItem = new Scanner(RunGUI.class.getResourceAsStream("NewItemInfo"));
        String workingLine;
        String newItemIDNumber;
        String newItemName;
        String newItemInitialNumberAvailable;
        int workingField;
        char workingChar;
        int i;
        int lineCount = 1;
        boolean itemIDAvailable = true;
        
        while (readItem.hasNextLine()) {
            newItemIDNumber = "";
            newItemName = "";
            newItemInitialNumberAvailable = "";
            workingLine = readItem.nextLine();
            workingField = 0;
            i = 0;
            
            //Splits each line up into a item ID number, an item name and the initial number of the item avaiable
            while (workingField < 3 && i < workingLine.length()) {
                workingChar = workingLine.charAt(i);
                if (workingChar == ',') {
                    workingField++;
                }
                else if (workingField == 0) {
                    newItemIDNumber = newItemIDNumber + workingChar;
                }
                else if (workingField == 1) {
                    newItemName = newItemName + workingChar;
                }
                else {
                    newItemInitialNumberAvailable = newItemInitialNumberAvailable + workingChar;
                }
                i++;
            }
            
            //Carries out validation checks on inputted values, returns an appropriate error message if a value is invalid
            if (newItemIDNumber.length() != 4) {
                return "The item ID number on line" + lineCount + " is not 4 characters long";
            }
            else if (stringNumeric(newItemIDNumber) == false) {
                return "The item ID number on line " + lineCount + " is not numerical";
            }
            else if (newItemName.length() < 2 || newItemName.length() > 30) {
                return "The item name on line" + lineCount + " is not between 2 and 30 characters long";
            }
            else if (stringNumeric(newItemInitialNumberAvailable) == false) {
                return "The item number available on line " + lineCount + " is not numerical" + newItemIDNumber + " & " + newItemName + " & " + newItemInitialNumberAvailable;
            }
            else {
                    
                //Checks that inputted member ID number is not already taken
                for (int j = 0; j < items.size(); j++) {
                    Item workingItem = items.get(j);
                    int workingItemIDNumber = workingItem.getItemID();
                    if (workingItemIDNumber == Integer.parseInt(newItemIDNumber)) {
                        itemIDAvailable = false;
                    }
                }
                    
                if (itemIDAvailable == true) {
                    items.add(new Item(Integer.parseInt(newItemIDNumber), newItemName, Integer.parseInt(newItemInitialNumberAvailable), 10));
                }
            
                //Returns error message if member ID number is taken
                else {
                    return "The item ID number on line " + lineCount + " is already taken";
                }
                lineCount++;
            }
        }
        return null;
    }
    
    public String searchItems(String searchTerm) {
        String results = "";
        
        //Iterates through the items array list to see if any matches are found with the search term
        for (int i = 0; i < items.size(); i++) {
            Item workingItem = items.get(i);
            int workingItemIDNumber = workingItem.getItemID();
            String workingItemName = workingItem.getName();
            int workingItemCurrentNumberAvailable = workingItem.getItemNumberAvailable();
            if (workingItemName.contains(searchTerm)) {
                results = results + "ID: " + workingItemIDNumber + ", Name: " + workingItemName + ", Number Available: " + workingItemCurrentNumberAvailable + "\n";
            }
        }
        return results;
    }
    
    public String validateFindItemSearchTerm(String searchID) {
        
        //Checks that the search term is a 4 digit number and there are items in the array list to search through
        if (searchID.length() != 4) {
            return "The Item ID must be 4 digits long";
        }
        else if (stringNumeric(searchID) == false) {
            return "The Item ID must be an integer";
        }
        else if (items.isEmpty()) {
            return "Items need to be added first";
        }
        else {
            return null;
        }
    }
    
    public String findItem(String searchID) {
        String itemPreview = "";
        int searchIDNum = Integer.parseInt(searchID);
        
        //Iterates through the item array list to see if the search term matches any of the item IDs
        for (int i = 0; i < items.size(); i++) {
            Item workingItem = items.get(i);
            int workingItemIDNumber = workingItem.getItemID();
            String workingItemName = workingItem.getName();
            int workingItemCurrentNumberAvailable = workingItem.getItemNumberAvailable();
            if (searchIDNum == workingItemIDNumber) {
                itemPreview = "ID: " + workingItemIDNumber + ", Name: " + workingItemName + ", Number Available: " + workingItemCurrentNumberAvailable;
                itemToBeBooked = workingItem;
            }
        }
        return itemPreview;
    }
    
    public String requestBooking(String transactionIDNumber, String memberIDNumber, String transactionDate) {
        int itemIDNumber = itemToBeBooked.getItemID();
        Member workingMember = null;
        Member memberToBeBooked = null;
        int memberOffset = -1;
        int itemOffset = -1;
        int i = 0;
        Item workingItem = null;
        int workingMemberIDNumber;
        int workingItemIDNumber;
        if (itemToBeBooked.getName().isEmpty()) {
            return "Please search for an item first";
        }
        String transactionIDNumberError = validateTransactionIDNumber(transactionIDNumber);
        String transactionDateError = validateTransactionDate(transactionDate);
        String transactionDuplicationError = validateTransactionDuplication(transactionIDNumber, Integer.parseInt(memberIDNumber), itemIDNumber);
        if (transactionIDNumberError != null) {
            return transactionIDNumberError;
        }
        if (transactionDateError != null) {
            return transactionDateError;
        }
        if (transactionDuplicationError != null) {
            return transactionDuplicationError;
        }
        for (i = 0; i < members.size(); i++) {
            workingMember = members.get(i);
            workingMemberIDNumber = workingMember.getMemberID();
            if (Integer.parseInt(memberIDNumber) == workingMemberIDNumber) {
                memberToBeBooked = workingMember;
                memberOffset = i;
            }
        }
        if (memberToBeBooked == null) {
            return "Please enter a valid member ID number";
        }
        for (i = 0; i < items.size(); i++) {
            workingItem = items.get(i);
            workingItemIDNumber = workingItem.getItemID();
            if (itemIDNumber == workingItemIDNumber) {
                itemOffset = i;
            }
        }
        int itemNumberAvailable = itemToBeBooked.getItemNumberAvailable();
        int itemRequestsAvailable = itemToBeBooked.getRequestNumberAvailable();
        int memberItemsBooked = memberToBeBooked.getItemsBooked();
        int memberItemsRequested = memberToBeBooked.getItemsRequested();
        if (itemNumberAvailable > 0) {
            if (memberItemsBooked < 5) {
                transactions.add(new Transaction(transactionIDNumber, itemIDNumber, Integer.parseInt(memberIDNumber), transactionDate, true));
                memberItemsBooked++;
                workingMember.setItemsBooked(memberItemsBooked);
                workingMember.setItemsRequested(memberItemsRequested);
                workingMember.setMemberID(memberToBeBooked.getMemberID());
                workingMember.setMemberName(memberToBeBooked.getMemberName());
                members.set(memberOffset, workingMember);
                itemNumberAvailable--;
                workingItem.setItemID(itemToBeBooked.getItemID());
                workingItem.setName(itemToBeBooked.getName());
                workingItem.setRequestNumberAvailable(itemRequestsAvailable);
                workingItem.setNumberAvailable(itemNumberAvailable);
                items.set(itemOffset, workingItem);
                return "Transaction successful, item booked";
            }
            else if (memberItemsRequested < 5) {
                transactions.add(new Transaction(transactionIDNumber, itemIDNumber, Integer.parseInt(memberIDNumber), transactionDate, false));
                memberItemsRequested++;
                workingMember.setItemsBooked(memberItemsBooked);
                workingMember.setItemsRequested(memberItemsRequested);
                workingMember.setMemberID(memberToBeBooked.getMemberID());
                workingMember.setMemberName(memberToBeBooked.getMemberName());
                members.set(memberOffset, workingMember);
                itemRequestsAvailable--;
                workingItem.setNumberAvailable(itemNumberAvailable);
                workingItem.setItemID(itemToBeBooked.getItemID());
                workingItem.setName(itemToBeBooked.getName());
                workingItem.setRequestNumberAvailable(itemRequestsAvailable);
                items.set(itemOffset, workingItem);
                return "Transaction successful, item requested";
            }
            else {
                return "Transaction unsuccessful, member cannot carry out any more transactions at this moment in time";
            }
        }
        else if (itemRequestsAvailable > 0) {
            if (memberItemsRequested < 5) {
                transactions.add(new Transaction(transactionIDNumber, itemIDNumber, Integer.parseInt(memberIDNumber), transactionDate, false));
                memberItemsRequested++;
                workingMember.setItemsRequested(memberItemsRequested);
                members.set(memberOffset, workingMember);
                itemRequestsAvailable--;
                workingItem.setNumberAvailable(itemRequestsAvailable);
                items.set(itemOffset, workingItem);
                return "Transaction successful, item requested";
            }
            else {
                return "Transaction unsuccessful, item is not currently available for booking and member cannot carry out any more requests";
            }
        }
        else {
            return "Transaction unsuccessful, item is currently fully booked";
        }
    }
    
    public String validateTransactionDuplication(String transactionID, int memberID, int itemID) {
        for (int i = 0; i < transactions.size(); i++) {
            Transaction workingTransaction = transactions.get(i);
            int workingMemberIDNumber = workingTransaction.getMemberID();
            int workingItemIDNumber = workingTransaction.getItemID();
            if (memberID == workingMemberIDNumber && itemID == workingItemIDNumber) {
                return "That member already has that item";
            }
        }
        return null;
    }
    
    public boolean validateTransaction(String transactionID, String transactionDate) {
        Transaction workingTransaction = null;
        String workingTransactionIDNumber;
        int transactionOffset = -1;
        for (int i = 0; i < transactions.size(); i++) {
            workingTransaction = transactions.get(i);
            workingTransactionIDNumber = workingTransaction.getTransactionID();
            if (transactionID.equals(workingTransactionIDNumber)) {
                transactionOffset = i;
            }
        }
        workingTransaction = transactions.get(transactionOffset);
        String transactionDateStr = workingTransaction.getDate();
        String yearStr = transactionDateStr.charAt(0) + "" + transactionDateStr.charAt(1) + "" + transactionDateStr.charAt(2) + "" + transactionDateStr.charAt(3);
        String dayStr = transactionDateStr.charAt(5) + "" + transactionDateStr.charAt(6);
        String monthStr = transactionDateStr.charAt(8) + "" + transactionDateStr.charAt(9);
        int year = Integer.parseInt(yearStr);
        int month = Integer.parseInt(monthStr);
        int day = Integer.parseInt(dayStr);
        Calendar currentDate = Calendar.getInstance();
        Calendar endDate = Calendar.getInstance();
        endDate.set(year, month, day);;
        endDate.add(Calendar.DATE, 7);
        if (currentDate.before(endDate)) {
            return true;
        }
        else {
            return false;
        }
    }
    
    public String validateMemberIDNumber(String memberID) {
        if (memberID.length() != 5) {
            return "Your member ID number must be 5 characters long";
        }
        else if (stringNumeric(memberID) == false) {
            return "Your user ID number may only consist of integers";
        }
        else {
            return null;
        }
    }
    
    public String viewCurrentBookings(String memberID) {
        String results = null;
        for (int i = 0; i < transactions.size(); i++) {
            Transaction workingTransaction = transactions.get(i);
            int workingMemberIDNumber = workingTransaction.getMemberID();
            if (Integer.parseInt(memberID) == workingMemberIDNumber) {
                String workingTransactionID = workingTransaction.getTransactionID();
                int workingItemID = workingTransaction.getItemID();
                String workingTransactionDate = workingTransaction.getDate();
                boolean workingTransactionItemInStock = workingTransaction.getItemInStock();
                if (validateTransaction(workingTransactionID, workingTransactionDate)) {
                    if (workingTransactionItemInStock == true) {
                        if (results == null) {
                            results = "Transaction ID: " + workingTransactionID + ", Item ID: " + workingItemID + ", Member ID: " + workingMemberIDNumber + ", TransactionDate: " + workingTransactionDate + "\n";
                        }
                        else {
                            results = results + "Transaction ID: " + workingTransactionID + ", Item ID: " + workingItemID + ", Member ID: " + workingMemberIDNumber + ", TransactionDate: " + workingTransactionDate + "\n";
                        }
                    }
                }
                else {
                    Member workingMember;
                    Item workingItem;
                    int memberOffset = -1;
                    int itemOffset = -1;
                    int transactionMemberID = workingTransaction.getMemberID();
                    int itemID = workingTransaction.getItemID();
                    for (i = 0; i < members.size(); i++) {
                        workingMember = members.get(i);
                        int workingTransactionMemberIDNumber = workingMember.getMemberID();
                        if (transactionMemberID == workingTransactionMemberIDNumber) {
                            memberOffset = i;
                        }
                    }
                    for (i = 0; i < items.size(); i++) {
                        workingItem = items.get(i);
                        int workingItemIDNumber = workingItem.getItemID();
                        if (itemID == workingItemIDNumber) {
                            itemOffset = i;
                        }
                    }
                    workingMember = members.get(memberOffset);
                    workingItem = items.get(itemOffset);
                    int workingMemberItemsBooked = workingMember.getItemsBooked();
                    int workingItemsAvailable = workingItem.getItemNumberAvailable();
                    workingMemberItemsBooked--;
                    workingItemsAvailable++;
                    workingMember.setItemsBooked(workingMemberItemsBooked);
                    workingItem.setNumberAvailable(workingItemsAvailable);
                    members.set(memberOffset, workingMember);
                    items.set(itemOffset, workingItem); 
                }
            }
        }
        return results;
    }
    
    public String validateTransactionIDNumber(String transactionIDNumber) {
        if (transactionIDNumber.length() != 6) {
            return "Your transaction ID number must be 6 characters long";
        }
        if (Character.isDigit(transactionIDNumber.charAt(0)) || Character.isDigit(transactionIDNumber.charAt(1)) || Character.isDigit(transactionIDNumber.charAt(2)) == false || Character.isDigit(transactionIDNumber.charAt(3)) == false || Character.isDigit(transactionIDNumber.charAt(4)) == false || Character.isDigit(transactionIDNumber.charAt(5)) == false) {
            return "Your transaction ID must be in the format LLDDDD where each L is a letter and each D is a digit";
        }
        else {
            for (int i = 0; i < transactions.size(); i++) {
                Transaction workingTransaction = transactions.get(i);
                String workingTransactionIDNumber = workingTransaction.getTransactionID();
                if (transactionIDNumber.equals(workingTransactionIDNumber)) {
                    return "That transaction ID number is already taken";
                }
            }
            return null;
        }
    }
    
    public String validateTransactionDate(String transactionDate) {
        if (transactionDate.length() != 10) {
            return "Your transaction date must be in the format DDDD/DD/DD where each D is a digit. The first four digits are for the year, the next two for the month and the final two for the day";
        }
        if (Character.isDigit(transactionDate.charAt(0)) == false || Character.isDigit(transactionDate.charAt(1)) == false || Character.isDigit(transactionDate.charAt(2)) == false || Character.isDigit(transactionDate.charAt(3)) == false || transactionDate.charAt(4) != '/' || Character.isDigit(transactionDate.charAt(5)) == false || Character.isDigit(transactionDate.charAt(6)) == false || transactionDate.charAt(7) != '/' || Character.isDigit(transactionDate.charAt(8)) == false || Character.isDigit(transactionDate.charAt(9)) == false) {
            return "Your transaction date must be in the format DDDD/DD/DD where each D is a digit. The first four digits are for the year, the next two for the month and the final two for the day";
        }
        else {
            return null;
        }
    }
    
    public static boolean stringNumeric(String str) {
        try {
            int i = Integer.parseInt(str);
        }
        catch (NumberFormatException x) {
            return false;
        }
        return true;
    }
}
