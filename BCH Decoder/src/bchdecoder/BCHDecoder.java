/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package bchdecoder;

import java.util.Scanner;

/**
 *
 * @author Jack Martin (15009787)
 * 
 */

public class BCHDecoder {

    /**
     * @param args the command line arguments
     */
    
    public static int convertedDigits[] = new int[10];
    public static char digits[] = new char[10];
    public static String tenDigitInput;
    public static int s1, s2, s3, s4;
    
    public static class nineDigitResult {
        private int position;
        private int value;
        
        public nineDigitResult(int position, int value) {
            this.position = position;
            this.value = value;
        }
        public int getPosition() {
            return position;
        }
        public int getValue() {
            return value;
        }
    }
    
    public static void main(String[] args) {
        // TODO code application logic here
        
        tenDigitInput = getTenDigitInput();
        convertInput();
        calcSyndromes();
        correctErrors();

    }
    
    public static void correctErrors() {
        // Gives output if there are no errors
        if (s1 == 0 && s2 == 0 && s3 == 0 && s4 == 0) {
            System.out.println("\nNo errors");
        }
        else {
            // Calculates p, q and r to locate errors
            int p = Math.floorMod(Math.floorMod((s2 * s2), 11) - Math.floorMod((s1 * s3), 11), 11);
            int q = Math.floorMod(Math.floorMod((s1 * s4), 11) - Math.floorMod((s2 * s3), 11), 11);
            int r = Math.floorMod(Math.floorMod((s3 * s3), 11) - Math.floorMod((s2 * s4), 11), 11);
            
            System.out.println("\nP: " + p + "\nQ: " + q + "\nR: " + r);
            
            if (p == 0 && q == 0 && r == 0) {
                int i = s2 / s1;
                // Test for 3+ errors
                if (i == 0) {
                    System.out.println("\nMore than 2 errors");
                }
                else {
                    // Corrects single error and outputs result
                    System.out.println("\nSingle error\nMagnitude: " + s1 + "\nPosition: " + i);
                    convertedDigits[i - 1] = convertedDigits[i - 1] - s1;
                    System.out.println("\nCorrect codeword: " + convertedDigits[0] + "" + convertedDigits[1] + "" + convertedDigits[2] + "" + convertedDigits[3] + "" + convertedDigits[4] + "" + convertedDigits[5] + "" + convertedDigits[6] + "" + convertedDigits[7] + "" + convertedDigits[8] + "" + convertedDigits[9]);
                }
            }
            else {
                // Calculations for i
                double iWorking = -4 * p * r;
                iWorking = getMod(iWorking) + getMod(q * q);
                iWorking = getMod(iWorking);
                iWorking = getSqrt((int)iWorking);
                
                // Test for 3+ errors
                if (iWorking == 11) {
                    System.out.println("\nMore than 2 errors");
                }
                
                else {
                    // Calculations for i
                    int i = (int)iWorking;
                    i = getMod(i);
                    i = getMod(-q) + i;
                    i = getMod(i);
                    i = divide(i, getMod(2 * p));
                    
                    // Test for 3+ errors
                    if (i == 0) {
                        System.out.println("\nMore than 2 errors");
                    }
                    
                    else {
                        // Calculations for j
                        double jWorking = -4 * p * r;
                        jWorking = getMod(jWorking) + getMod(q * q);
                        jWorking = getMod(jWorking);
                        jWorking = getSqrt((int)jWorking);
                        
                        // Test for 3+ errors
                        if (jWorking == 11) {
                            System.out.println("\nMore than 2 errors");
                        }
                        else {
                            // Calculations for j
                            int j = (int)jWorking;
                            j = getMod(j);
                            j = getMod(-q) - j;
                            j = getMod(j);
                            j = divide(j, getMod(2 * p));
                            
                            // Test for 3+ errors
                            if (j == 0) {
                                System.out.println("\nMore than 2 errors");
                            }
                            else {
                                // Calculates a and b to correct double error
                                int b = add(multiply(i, s1), -s2);
                                b = divide(b, add(i, -j));
                                
                                int a = add(s1, -b);
                        
                                // Corrects double error and outputs result
                                convertedDigits[i - 1] = getMod(convertedDigits[i - 1] - a);
                                convertedDigits[j - 1] = getMod(convertedDigits[j - 1] - b);
                                if (checkDigits(convertedDigits[0], convertedDigits[1], convertedDigits[2], convertedDigits[3], convertedDigits[4], convertedDigits[5], convertedDigits[6], convertedDigits[7], convertedDigits[8], convertedDigits[9]) == false) {
                                    System.out.println("\nMore than 2 errors");
                                }
                                else {
                                    System.out.println("\nDouble error\nMagnitude: " + a + " Position: " + i + "\nMagnitude: " + b + " Position: " + j);
                                    System.out.println("\nCorrect codeword: " + convertedDigits[0] + "" + convertedDigits[1] + "" + convertedDigits[2] + "" + convertedDigits[3] + "" + convertedDigits[4] + "" + convertedDigits[5] + "" + convertedDigits[6] + "" + convertedDigits[7] + "" + convertedDigits[8] + "" + convertedDigits[9]);
                                }
                            }   
                        }   
                    }   
                }
            }   
        }
    }
    
    public static void convertInput() {
        // Converts characters from input to integers
        for (int i = 0; i < 10; i++) {
            digits[i] = tenDigitInput.charAt(i);
            convertedDigits[i] = Character.getNumericValue(digits[i]);
        }
    }
    
    public static void calcSyndromes() {
        // Carries out calculations to get 4 syndromes
        s1 = Math.floorMod((convertedDigits[0] + convertedDigits[1] + convertedDigits[2] + convertedDigits[3] + convertedDigits[4] + convertedDigits[5] + convertedDigits[6] + convertedDigits[7] + convertedDigits[8] + convertedDigits[9]), 11);
        s2 = Math.floorMod((convertedDigits[0] + (2 * convertedDigits[1]) + (3 * convertedDigits[2]) + (4 * convertedDigits[3]) + (5 * convertedDigits[4]) + (6 * convertedDigits[5]) + (7 * convertedDigits[6]) + (8 * convertedDigits[7]) + (9 * convertedDigits[8]) + (10 * convertedDigits[9])), 11);
        s3 = Math.floorMod((convertedDigits[0] + (4 * convertedDigits[1]) + (9 * convertedDigits[2]) + (5 * convertedDigits[3]) + (3 * convertedDigits[4]) + (3 * convertedDigits[5]) + (5 * convertedDigits[6]) + (9 * convertedDigits[7]) + (4 * convertedDigits[8]) + convertedDigits[9]), 11);
        s4 = Math.floorMod((convertedDigits[0] + (8 * convertedDigits[1]) + (5 * convertedDigits[2]) + (9 * convertedDigits[3]) + (4 * convertedDigits[4]) + (7 * convertedDigits[5]) + (2 * convertedDigits[6]) + (6 * convertedDigits[7]) + (3 * convertedDigits[8]) + (10 * convertedDigits[9])), 11);
        
        System.out.println("\nS1: " + s1 + "\nS2: " + s2 + "\nS3: " + s3 + "\nS4: " + s4);
    }
    
    public static boolean checkDigits(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, int d9, int d10) {
        // Ensures no digit is 10 or more
        return !(d1 > 9 || d2 > 9 || d3 > 9 || d4 > 9 || d5 > 9 || d6 > 9 || d7 > 9 || d8 > 9 || d9 > 9 || d10 > 9);
    }
    
    public static double getMod(double x) {
        // Calculates modulus of given value of double
        while (x >= 11) {
            x = x - 11;
        }
        while (x < 0) {
            x = x + 11;
        }
        return x;
    }
    
    public static String getTenDigitInput() {
        
        // Variable declarations
        Scanner input = new Scanner(System.in);
        boolean inputValid = false;
        String tenDigitInput = null;
        
        // Gets 10 digit input from user
        while (inputValid == false) {
            System.out.println("Please enter the ten digits output by the generator");
            tenDigitInput = input.next();
            inputValid = validateNum(tenDigitInput);
            if (inputValid == false) {
                System.out.println("Input invalid");
            }
        }
        return tenDigitInput;
    }
    
    public static boolean validateNum(String num) {
        //Checks if given number is valid
        if (num.length() == 10) {
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
    
    public static int getMod(int x) {
        // Calculats modulus value of int
        while (x < 0) {
            x = x + 11;
        }
        while (x >= 11) {
            x = x - 11;
        }
        return x;
    }
    
    public static int getSqrt(int x) {
        // Gives modulus of the square root of given value
        for (int i = 0; i < 11; i++) {
            int y = x + (11 * i);
            if (y == 1 || y == 4 || y == 9 || y == 16 || y == 25 || y == 36 || y == 49 || y == 64 || y == 81 || y == 100) {
                y = (int)Math.sqrt(y);
                return y;
            }
        }
        return 11;
    }
    
    public static int add(int x, int y) {
        
        //Carries out operation (x + y) % 11 and returns result
        x = getMod(x);
        y = getMod(y);
        
        int z = x + y;
        
        z = getMod(z);
        
        return z;
        
    }
    
    public static int multiply(int x, int y) {
        
        //Carries out operation (x * y) % 11 and returns result
        x = getMod(x);
        y = getMod(y);
        
        int z = x * y;
        
        z = getMod(z);
        
        return z;
        
    }
    
    public static int divide(int x, int y) {
        
        //Carries out operation (x / y) % 11 and returns result
        
        x = getMod(x);
        y = getMod(y);
        
        int z = 0;
        
        if (y == 0) {
            return 0;
        }
        
        for (int count = 0; count < 11; count++) {
            z = (count * 11) + x;
            if (z % y == 0) {
                return z / y;
            }
        }
        
        return 0;
        
    }
    
}
