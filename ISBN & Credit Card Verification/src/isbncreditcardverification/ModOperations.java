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

import java.util.Scanner;

public class ModOperations {
    
    public void modOperations() {
        
        Scanner input = new Scanner(System.in);
    
        //Gets valid input from user for x
        String x = null;
        boolean xValid = false;
        while (xValid == false) {
            System.out.println("Enter x");
            x = input.next();
            xValid = validateNum(x);
        }
        
        //Gets valid input from user for mod operation
        String op = null;
        boolean opValid = false;
        while (opValid == false) {
            System.out.println("Enter mod operation: a for addition, m for multiplication, d for division");
            op = input.next();
            opValid = validateOp(op);
        }
        
        //Gets valid input from user for y
        String y = null;
        boolean yValid = false;
        while (yValid == false) {
            System.out.println("Enter y");
            y = input.next();
            yValid = validateNum(y);
        }
               
        //Selects appropriate algorithm for calculation
        switch (op) {
            case "a": add(Integer.parseInt(x),Integer.parseInt(y));
                break;
            case "m": multiply(Integer.parseInt(x),Integer.parseInt(y));
                break;
            case "d": divide(Integer.parseInt(x),Integer.parseInt(y));
                break;
        }
    
    }
    
    public void add(int x, int y) {
        
        //Carries out operation (x + y) % 11 and outputs result
        Math.abs(x);
        Math.abs(y);
        
        int modX = x % 11;
        int modY = y % 11;
        
        System.out.println("The solution is " + (modX + modY) % 11);
        
    }
    
    public void multiply(int x, int y) {
        
        //Carries out operation (x * y) % 11 and outputs result
        Math.abs(x);
        Math.abs(y);
        
        int modX = x % 11;
        int modY = y % 11;
        
        System.out.println("The solution is " + (modX * modY) % 11);
        
    }
    
    public void divide(int x, int y) {
        
        //Carries out operation (x / y) % 11 and outputs result/s
        int solutions[] = new int[12];
        
        Math.abs(x);
        Math.abs(y);
        
        int modX = x % 11;
        int modY = y % 11;
        
        int j = 0;
        int workingNum = 0;
        
        for (int i = 0; i < 12; i++) {
            workingNum = modX + (12 * i);
            if (workingNum % modY == 0) {
                solutions[j] = workingNum / modY;
                j++;
            }
        }
        
        if (solutions[0] == 0) {
            System.out.println("There are no solutions");
        }
        else if (solutions[1] == 0) {
            System.out.println("The only solution is " + solutions[0]);
        }
        else {
            System.out.println("There are " + j + " solutions. These are ");
            j = 0;
            while (solutions[j] != 0) {
                System.out.print(solutions[j] + ", ");
            }
        }
        
    }
    
    public boolean validateNum(String num) {
        //Checks if given number is valid
        try {
            Integer.parseInt(num);
        }
        catch (NumberFormatException e) {
            return false;
        }
        return true;
    }
    
    public boolean validateOp(String op) {
        //Checks if given operation is valid
        return "a".equals(op) || "m".equals(op) || "d".equals(op);
    }

}
