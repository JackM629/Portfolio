/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package bchgenerator;

import java.util.Scanner;

/**
 *
 * @author Jack Martin (15009787)
 * 
 */

public class BCHGenerator {

    /**
     * @param args the command line arguments
     */
    
    public static void main(String[] args) {
        // TODO code application logic here
        
        // Declares variables
        char digits[] = new char[6];
        int convertedDigits[] = new int[6];
        String sixDigitInput = null;
        int s1 = 10, s2 = 10, s3 = 10, s4 = 10;
        
        while (validateSyndrome(s1) == false || validateSyndrome(s2) == false || validateSyndrome(s3) == false || validateSyndrome(s4) == false) {
            sixDigitInput = getSixDigitInput();
        
            // Converts characters from input to integers
            for (int i = 0; i < 6; i++) {
                digits[i] = sixDigitInput.charAt(i);
                convertedDigits[i] = Character.getNumericValue(digits[i]);
            }
        
            // Carries out calculations to get 4 syndromes
            s1 = ((4 * convertedDigits[0]) + (10 * convertedDigits[1]) + (9 * convertedDigits[2]) + (2 * convertedDigits[3]) + convertedDigits[4] + (7 * convertedDigits[5])) % 11;
            s2 = ((7 * convertedDigits[0]) + (8 * convertedDigits[1]) + (7 * convertedDigits[2]) + (convertedDigits[3]) + (9 * convertedDigits[4]) + (6 * convertedDigits[5])) % 11;
            s3 = ((9 * convertedDigits[0]) + (convertedDigits[1]) + (7 * convertedDigits[2]) + (8 * convertedDigits[3]) + (7 * convertedDigits[4]) + (7 * convertedDigits[5])) % 11;
            s4 = (convertedDigits[0] + (2 * convertedDigits[1]) + (9 * convertedDigits[2]) + (10 * convertedDigits[3]) + (4 * convertedDigits[4]) + convertedDigits[5]) % 11;
            
            if (validateSyndrome(s1) == false || validateSyndrome(s2) == false || validateSyndrome(s3) == false || validateSyndrome(s4) == false) {
                System.out.println("Unusable number");
            }
            else {
                System.out.println(convertedDigits[0] + "" + convertedDigits[1] + "" + convertedDigits[2] + "" + convertedDigits[3] + "" + convertedDigits[4] + "" + convertedDigits[5] + "" + s1 + "" + s2 + "" + s3 + "" + s4);
            }
            
        }
        
        System.out.println("Encoding = " + sixDigitInput + "" + s1 + "" + s2 + "" + s3 + "" + s4);

    }
    
    public static String getSixDigitInput() {
        
        Scanner input = new Scanner(System.in);
        boolean inputValid = false;
        String sixDigitInput = null;
        
        // Gets 6 digit input from user
        while (inputValid == false) {
            System.out.println("Please enter six digits");
            sixDigitInput = input.next();
            inputValid = validateNum(sixDigitInput);
            if (inputValid == false) {
                System.out.println("Input invalid");
            }
        }
        return sixDigitInput;
    }
    
    public static boolean validateNum(String num) {
        //Checks if given number is valid
        if (num.length() == 6) {
            try {
               Float.parseFloat(num);
            }
            catch (NumberFormatException e) {
                return false;
            }
            return true;
        }
        return false;
    }
    
    public static boolean validateSyndrome(int s) {
        // Checks if syndrome is valid
        if (s == 10) {
            return false;
        }
        else {
               return true;
        }
    }
}
