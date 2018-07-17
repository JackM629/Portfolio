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

public class CreditCardNoVerification {
    
    public String verifyInput(String creditCardNo) {
        
        //Puts all digits from credit card number into array. If non-digit character found, credit card number not valid. Dashes are ignored
        int[] creditCardNoDigits = new int[30];
        char[] creditCardNoChars = new char[30];
        
        //Regex to remove any non-digits
        String creditCardNum = creditCardNo.replaceAll("\\D+","");
        if (creditCardNum.length() != 16) {
            return "The credit card number needs to contain 16 \n digits. Any other characters will be ignored";
        }
        
        //Converts inputted credit card number to array of digits
        for (int i=0; i<16; i++) {
            creditCardNoChars[i] = creditCardNo.charAt(i);
            creditCardNoDigits[i] = Character.getNumericValue(creditCardNoChars[i]);
        }
        
        //Carries out lun check
        int j = 0;
        while (j < 16) {
            creditCardNoDigits[j] = creditCardNoDigits[j] * 2;
            if (creditCardNoDigits[j] > 9) {
                creditCardNoDigits[j] = creditCardNoDigits[j] - 9;
            }
            j = j + 2;
        }
        
        int total = 0;
        for (int i=0; i<16; i++) {
            total = total + creditCardNoDigits[i];
            System.out.println(total);
        }
        
        total = total % 10;
        
        //Outputs appropriate result
        if (total == 0) {
            return "This credit card number is valid";
        }
        else {
            return "This credit card number is invalid";
        }
        
    }
    
}
