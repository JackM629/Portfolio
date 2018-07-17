package isbncreditcardverification;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Jack Martin (15009787)
 * 
 */

public class ISBNVerification {
    
    public String verifyInput(String isbn) {
        
        //Puts all digits from isbn into array. If non-digit character found, isbn not valid. Dashes are ignored
        int isbnDigits[] = new int[20];
        
        String isbnNum = isbn.replaceAll("\\D+","");
        if (isbnNum.length() != 10) {
            return "The ISBN number needs to contain 10 \n digits. Any other characters will be ignored";
        }
        
        for (int i=0; i<10; i++) {
            isbnDigits[i] = isbnNum.charAt(i);
        }
        
        //Carries out calculations to test whether isbn is valid and returns appropriate result
        int total = 0;
        for (int i=0; i<isbn.length() ; i++) {
            total = total + isbnDigits[i] * (i + 1);
        }
        
        total = total % 11;
        
        if (total == 0) {
            return "This ISBN number is valid";
        }
        else {
            return "This ISBN number is invalid";
        }
        
    }
    
}
