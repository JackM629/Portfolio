/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// https://netbeans.org/competition/win-with-netbeans/mysql-client.html

package guitartabdeveloper;

import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.Scanner;
import javax.swing.DefaultComboBoxModel;

/**
 *
 * @author Jack Martin (15009787)
 * 
 */

public class TabWriter {
    
    int notes[][] = new int[10000][8];
    int noteLengths[] = new int[10000];
    boolean notesDotted[] = new boolean[10000];
    int otherOptions[] = new int[10000];
    int noteTypes[] = new int[10000];
    int beatsPerBar[] = new int[10000];
    int numberOfNotes = 0;
    int currentStrings = -1;
    int noteLength = -1;
    boolean noteDotted = false;
    int otherOption = -1;
    int noteType = -1;
    int currentBeatsPerBar = 4;
    int dashesPerBarLimit = 64;
    String[] currentTuning = new String[]{"E", "B", "G", "D", "A", "E"};
    boolean newLine = false;
    String[] standardStringList = new String[]{"1", "2", "3", "4", "5", "6"};
    String[] bassStringList = new String[]{"1", "2", "3", "4"};
    String[] sevenStringList = new String[]{"1", "2", "3", "4", "5", "6", "7"};
    String[] eightStringList = new String[]{"1", "2", "3", "4", "5", "6", "7", "8"};
    DefaultComboBoxModel standardStringModel = new DefaultComboBoxModel(standardStringList);
    DefaultComboBoxModel bassStringModel = new DefaultComboBoxModel(bassStringList);
    DefaultComboBoxModel sevenStringModel = new DefaultComboBoxModel(sevenStringList);
    DefaultComboBoxModel eightStringModel = new DefaultComboBoxModel(eightStringList);
    
    public void setStringsNo(int strings) {
        // Sets number of strings and default tuning
        currentStrings = strings;
        if (strings == 6) {
            currentTuning = new String[]{"E", "B", "G", "D", "A", "E"};
        }
        else if (strings == 4) {
            currentTuning = new String[]{"G", "D", "A", "E"};
        }
        else if (strings == 7) {
            currentTuning = new String[]{"E", "B", "G", "D", "A", "E", "B"};
        }
        else {
            currentTuning = new String[]{"E", "B", "G", "D", "A", "E", "B", "F#"};
        }
        resetStrings(currentStrings);
    }
    
    public void finishChord() {
        // Increments number of notes to signify chord ending
        numberOfNotes++;
    }
    
    public String undoLastNote() {
        if (otherOptions[numberOfNotes] == 0 || otherOptions[numberOfNotes] == 2 || otherOptions[numberOfNotes] == -1) {
            numberOfNotes--;
        }
        for (int i = 0; i < currentStrings; i++) {
            notes[numberOfNotes][i] = -1;
        }
        noteLengths[numberOfNotes] = -1;
        notesDotted[numberOfNotes] = false;
        noteTypes[numberOfNotes] = -1;
        otherOptions[numberOfNotes] = -1;
        beatsPerBar[numberOfNotes] = -1;
        return displayOutput();
    }
    
    public int selectionValid(int fret) {
        if ((otherOption == 1) && (noteType != 0)) {
            return 1;
        }
        else if ((otherOption != 0) && (fret == 0) && (noteType == 1 || noteType == 2 || noteType == 3 || noteType == 5 || noteType == 7 || noteType == 8 || noteType == 9 || noteType == 10 || noteType == 11 || noteType == 13)) {
            return 2;
        }
        else if (otherOption == 0 && noteType != 0) {
            return 3;
        }
        else {
            return 0;
        }
    }
    
    public void addNote(int string, int fret) {
        switch (otherOption) {
            case 2:
                // If single note option selected, save selected fret and string
                string--;
                for (int i = 0; i < currentStrings; i++) {
                    if (i == string) {
                        notes[numberOfNotes][i] = fret;
                    }
                    else {
                        notes[numberOfNotes][i] = -1;
                    }
                }   
                // Save note length, type, single/chord option, dotted option and current beats per bar
                // Increment number of notes
                noteLengths[numberOfNotes] = noteLength;
                notesDotted[numberOfNotes] = noteDotted;
                noteTypes[numberOfNotes] = noteType;
                otherOptions[numberOfNotes] = otherOption;
                beatsPerBar[numberOfNotes] = currentBeatsPerBar;
                numberOfNotes++;
                break;
            case 1:
                // If part of chord option selected save selected fret and string
                string--;
                for (int i = 0; i < currentStrings; i++) {
                    if (i == string) {
                        notes[numberOfNotes][i] = fret;
                    }
                    // Save note length, type, single/chord option, dotted option and current beats per bar
                    // Increment number of notes
                    noteLengths[numberOfNotes] = noteLength;
                    notesDotted[numberOfNotes] = noteDotted;
                    noteTypes[numberOfNotes] = noteType;
                    otherOptions[numberOfNotes] = otherOption;
                    beatsPerBar[numberOfNotes] = currentBeatsPerBar;
                }   break;
            case 0:
                // Save note length, type, single/chord option, dotted option and current beats per bar
                // Increment number of notes
                for (int i = 0; i < currentStrings; i++) {
                    notes[numberOfNotes][i] = 25;
                }
                noteLengths[numberOfNotes] = noteLength;
                notesDotted[numberOfNotes] = noteDotted;
                noteTypes[numberOfNotes] = noteType;
                otherOptions[numberOfNotes] = otherOption;
                numberOfNotes++;
                break;
        }
    }
    
    public void saveSong(String songName, String artistName) {
        
        Writer tabTextWriter = null;
        Writer songDataWriter = null;

        try {
            tabTextWriter = new BufferedWriter(new OutputStreamWriter(
            new FileOutputStream(songName + "-" + artistName + "-" + currentStrings), "utf-8"));
            String output = displayOutput();
            tabTextWriter.write(output);
        } catch (IOException ex) {
            
        } finally {
            try {
                tabTextWriter.close();
            } catch (Exception ex) {
            }
        }
        
        try {
            songDataWriter = new BufferedWriter(new OutputStreamWriter(
            new FileOutputStream(songName + "-" + artistName + "-" + currentStrings + " Data"), "utf-8"));
            String output = "";
            for (int i = 0; i < numberOfNotes; i++) {
                for (int j = 0; j < currentStrings; j++) {
                    output += notes[i][j] + " ";
                }
                output += noteLengths[i] + " ";
                output += notesDotted[i] + " ";
                output += otherOptions[i] + " ";
                output += noteTypes[i] + " ";
                output += beatsPerBar[i] + " ";
            }
            songDataWriter.write(output);
        } catch (IOException ex) {
            
        } finally {
            try {
                songDataWriter.close();
            } catch (Exception ex) {
            }
        }
        
    }
    
    public String loadSong(String songName, String artistName, String noOfStrings) throws FileNotFoundException {
        resetStrings(Integer.parseInt(noOfStrings));
        setStringsNo(Integer.parseInt(noOfStrings));
        Scanner dataReader = new Scanner(new FileReader(songName + "-" + artistName + "-" + currentStrings + " Data"));
        int i = 0;
        int j = 0;
        int strings = Integer.parseInt(noOfStrings);
        while (dataReader.hasNext()) {
            String next = dataReader.next();
                switch (i) {
                    case 0:
                        notes[j][0] = Integer.parseInt(next);
                        break;
                    case 1:
                        notes[j][1] = Integer.parseInt(next);
                        break;
                    case 2:
                        notes[j][2] = Integer.parseInt(next);
                        break;
                    case 3:
                        notes[j][3] = Integer.parseInt(next);
                        break;
                    case 4:
                        switch (strings) {
                            case 4:
                                noteLengths[j] = Integer.parseInt(next);
                                break;
                            default:
                                notes[j][4] = Integer.parseInt(next);
                                break;
                        }
                        break;
                    case 5:
                        switch (strings) {
                            case 4:
                                if (next == "true") {
                                    notesDotted[j] = true;
                                }
                                else if (next == "false") {
                                    notesDotted[j] = false;
                                }
                                else {
                                    System.out.println("Error loading file");
                                }
                                break;
                            default:
                                notes[j][5] = Integer.parseInt(next);
                                break;
                        }
                        break;
                    case 6:
                        switch (strings) {
                            case 4:
                                otherOptions[j] = Integer.parseInt(next);
                                break;
                            case 6:
                                noteLengths[j] = Integer.parseInt(next);
                                break;
                            default:
                                notes[j][6] = Integer.parseInt(next);
                                break;
                        }
                        break;
                    case 7:
                        switch (strings) {
                            case 4:
                                noteTypes[j] = Integer.parseInt(next);
                                break;
                            case 6:
                                if ("true".equals(next)) {
                                    notesDotted[j] = true;
                                }
                                else if ("false".equals(next)) {
                                    notesDotted[j] = false;
                                }
                                else {
                                    System.out.println("Error loading file");
                                }
                                break;
                            case 7:
                                noteLengths[j] = Integer.parseInt(next);
                                break;
                            default:
                                notes[j][7] = Integer.parseInt(next);
                                break;
                        }
                        break;
                    case 8:
                        switch (strings) {
                            case 4:
                                beatsPerBar[j] = Integer.parseInt(next);
                                break;
                            case 6:
                                otherOptions[j] = Integer.parseInt(next);
                                break;
                            case 7:
                                if (next == "true") {
                                    notesDotted[j] = true;
                                }
                                else if (next == "false") {
                                    notesDotted[j] = false;
                                }
                                else {
                                    System.out.println("Error loading file");
                                }
                                break;
                            default:
                                noteLengths[j] = Integer.parseInt(next);
                                break;
                        }
                        break;
                    case 9:
                        switch (strings) {
                            case 6:
                                noteTypes[j] = Integer.parseInt(next);
                                break;
                            case 7:
                                otherOptions[j] = Integer.parseInt(next);
                                break;
                            default:
                                if (next == "true") {
                                    notesDotted[j] = true;
                                }
                                else if (next == "false") {
                                    notesDotted[j] = false;
                                }
                                else {
                                    System.out.println("Error loading file");
                                }
                                break;
                        }
                        break;
                    case 10:
                        switch (strings) {
                            case 6:
                                beatsPerBar[j] = Integer.parseInt(next);
                                break;
                            case 7:
                                noteTypes[j] = Integer.parseInt(next);
                                break;
                            default:
                                otherOptions[j] = Integer.parseInt(next);
                                break;
                        }
                        break;
                    case 11:
                        switch (strings) {
                            case 7:
                                beatsPerBar[j] = Integer.parseInt(next);
                                break;
                            default:
                                noteTypes[j] = Integer.parseInt(next);
                        }
                        break;
                    case 12:
                        beatsPerBar[j] = Integer.parseInt(next);
                        break;
            }
            i++;
            if ((i == 9 && strings == 4) || (i == 11 && strings == 6) || (i == 12 && strings == 7) || (i == 13 && strings == 8)) {
                i = 0;
                j++;
            }
        }
        return displayOutput();
    }
    
    public String displayLineStart(String output, int i) {
        if (currentTuning[i].length() == 1) {
            output = currentTuning[i] + " |-";
        }
        else {
            output = currentTuning[i] + "|-";
        }
        return output;
    }
    
    public boolean testOutputComplete(int noteCount) {
        boolean complete = true;
        for (int i = 0; i < currentStrings; i++) {
            if (notes[noteCount][i] != -1) {
                complete = false;
            }
        }
        return complete;
    }
    
    public String displayOutput() {
        int dashLength = 0;
        String output = "";
        boolean nextString = false;
        boolean newLine = false;
        int charsInLine = 0;
        boolean outputComplete = false;
        int stringNo = 0;
        int noteCount = 0;
        int noteCountOffset = 0;
        int dashesInBar = 0;
        int dashesInBarOffset = 0;
        
        output = displayLineStart(output, stringNo);
        charsInLine = 4;        

        while (outputComplete == false) {
            if (newLine == true) {
                stringNo = 0;
                output += "\n\n";
                output += displayLineStart(output, stringNo);
                newLine = false;
            }
            else if (nextString == true) {
                if (stringNo == currentStrings - 1) {
                    dashesInBarOffset = dashesInBar;
                    output += "\n";
                    newLine = true;
                    nextString = false;
                    charsInLine = 4;
                }
                else {
                    dashesInBar = dashesInBarOffset;
                    stringNo++;
                    noteCount = noteCountOffset;
                    charsInLine = 4;
                    output += "\n" + displayLineStart(output, stringNo);
                    nextString = false;
                }
            }
            else if (testOutputComplete(noteCount)) {
                if (stringNo == currentStrings - 1) {
                    outputComplete = true;
                }
                else {
                    nextString = true;
                }
            }
            else if (charsInLine >= 150) {
                nextString = true;
                if (stringNo == currentStrings - 1) {
                    noteCountOffset = noteCount;
                }
            }
            else if (notes[noteCount][stringNo] == 25) {
                dashLength = getDashCount(noteCount);
                for (int i = 0; i < dashLength; i++) {
                    output += "-";
                }
                charsInLine = getCharsInLine(output);
                noteCount++;
            }
            else {
                dashLength = getDashCount(noteCount);
                if (notes[noteCount][stringNo] != -1) {
                    if (notes[noteCount][stringNo] > 9) {
                        dashLength = dashLength - 2;
                        output += notes[noteCount][stringNo];
                    }
                    else {
                        dashLength--;
                        output += notes[noteCount][stringNo];
                    }
                    switch (noteTypes[noteCount]) {
                        case 1:
                            output += "b";
                            dashLength--;
                            break;
                        case 2:
                            output += "br";
                            dashLength = dashLength - 2;
                            break;
                        case 3:
                            output += "pb";
                            dashLength = dashLength - 2;
                            break;
                        case 4:
                            output += "h";
                            dashLength--;
                            break;
                        case 5:
                            output += "p";
                            dashLength--;
                            break;
                        case 6:
                            output += "/";
                            dashLength--;
                            break;
                        case 7:
                            output += "\\";
                            dashLength--;
                            break;
                        case 8:
                            output += "~~~";
                            dashLength = dashLength - 3;
                            break;
                        case 9:
                            output += "(h)";
                            dashLength = dashLength - 3;
                            break;
                        case 10:
                            output += "(ph)";
                            dashLength = dashLength - 4;
                            break;
                        case 11:
                            output += "T";
                            dashLength--;
                            break;
                        case 12:
                            output += "x";
                            dashLength--;
                            break;
                        case 13:
                            output += "pbr";
                            dashLength = dashLength - 3;
                            break;
                        default:
                            break;
                    }
                }
                dashesInBar = dashesInBar - getIgnoredDashes(stringNo, noteCount);
                    for (int i = 0; i < dashLength; i++) {
                    output += "-";
                    dashesInBar++;
                    if (dashesInBar == beatsPerBar[noteCount] * 16) {
                        output += "|-";
                        if (i != dashLength - 1) {
                            if (notes[noteCount][stringNo] != -1) {
                                output += "(" + notes[noteCount][stringNo] + ")";
                            }
                            else {
                                for (int j = 0; j < getIgnoredDashes(stringNo, noteCount) + 2; j++) {
                                    output += "-";
                                }
                            }
                        }
                        dashesInBar = 0;
                    }
                }
                charsInLine = getCharsInLine(output);
                noteCount++;
            }
        }
        
        return output;
    }
    
    public int getIgnoredDashes(int stringNo, int noteCount) {
        int ignoredDashes = 0;
        if (notes[noteCount][stringNo] != -1) {
            return 0;
        }
        else {
            for (int i = 0; i < currentStrings; i++) {
                if (notes[noteCount][stringNo] > 9) {
                    ignoredDashes = 2;
                }
                else {
                    if (ignoredDashes < 2) {
                        ignoredDashes = 1;
                    }
                }
            }
            if (noteTypes[noteCount] == 1 || noteTypes[noteCount] == 4 || noteTypes[noteCount] == 5 || noteTypes[noteCount] == 6 || noteTypes[noteCount] == 7 || noteTypes[noteCount] == 11 || noteTypes[noteCount] == 12) {
                ignoredDashes++;
            }
            else if (noteTypes[noteCount] == 2 || noteTypes[noteCount] == 3) {
                ignoredDashes = ignoredDashes + 2;
            }
            else if (noteTypes[noteCount] == 8 || noteTypes[noteCount] == 9 || noteTypes[noteCount] == 13) {
                ignoredDashes = ignoredDashes + 3;
            }
            else if (noteTypes[noteCount] == 10) {
                ignoredDashes = ignoredDashes + 4;
            }
        }
        return ignoredDashes;
    }
    
    public int getCharsInLine(String output) {
        int i = 0;
        int j = 0;
        int charsInLine = 0;
        char s = output.charAt(0);
        while (i != output.length()) {
            s = output.charAt(i);
            if (s == '\n') {
                j++;
                i++;
                charsInLine = 0;
            }
            else {
                i++;
                charsInLine++;
            }
        }
        return charsInLine;
    } 
    
    public int getDashCount(int noteCount) {
        // Sets amount of dashes according to amount of digits in fret
        int dashLength = 0;
        for (int i = 0; i < currentStrings; i++) {
            if (notes[noteCount][i] != -1 && notes[noteCount][i] != 25) {
                if (notes[noteCount][i] > 9) {
                    dashLength = 2;
                }
                else {
                    dashLength = 1;
                }
                switch (noteTypes[noteCount]) {
                    case 1:
                        dashLength++;
                        break;
                    case 2:
                        dashLength = dashLength + 2;
                        break;
                    case 3:
                        dashLength = dashLength + 2;
                        break;
                    case 4:
                        dashLength++;
                        break;
                    case 5:
                        dashLength++;
                        break;
                    case 6:
                        dashLength++;
                        break;
                    case 7:
                        dashLength++;
                        break;
                    case 8:
                        dashLength = dashLength + 3;
                        break;
                    case 9:
                        dashLength = dashLength + 3;
                        break;
                    case 10:
                        dashLength = dashLength + 4;
                        break;
                    case 11:
                        dashLength++;
                        break;
                    case 12:
                        dashLength++;
                        break;
                    case 13:
                        dashLength = dashLength + 3;
                        break;
                }
            }
        }
        // Adds to amount of dashes according to note length
        switch (noteLengths[noteCount]) {
            case 32:
                dashLength = dashLength + 2;
                if (notesDotted[noteCount]) {
                    dashLength = dashLength + 1;
                }
                break;
            case 16:
                dashLength = dashLength + 4;
                if (notesDotted[noteCount]) {
                    dashLength = dashLength + 2;
                }
                break;
            case 8: 
                dashLength = dashLength + 8;
                if (notesDotted[noteCount]) {
                    dashLength = dashLength + 4;
                }
                break;
            case 4:
                dashLength = dashLength + 16;
                if (notesDotted[noteCount]) {
                    dashLength = dashLength + 8;
                }
                break;
            case 2: 
                dashLength = dashLength + 32;
                if (notesDotted[noteCount]) {
                    dashLength = dashLength + 16;
                }
                break;
            case 1: 
                dashLength = dashLength + 64;
                if (notesDotted[noteCount]) {
                    dashLength = dashLength + 32;
                }
                break;
        }
        return dashLength;
    }
    
    public void setTuning(String tuning) {
        for (int i = 0; i < currentStrings; i++) {
            currentTuning[i] = "";
        }
        int j = 0;
        for (int i = 0; i < tuning.length(); i++) {
            if (tuning.charAt(i) == ' ') {
                j++;
            }
            else {
                currentTuning[j] = currentTuning[j] + tuning.charAt(i);
            }
        }
    }
    
    public String validateTuning(String tuning) {
        int i = 0;
        int j = 1;
        int charCount = 0;
        while (charCount < tuning.length()) {
            if (tuning.charAt(i) == ' ') {
                j++;
                i = 0;
                charCount++;
            }
            else if (validChar(tuning.charAt(charCount)) == false) {
                return "Tuning must consist of upper case letters from A to G and sharps/flats (#/b) only and notes must be separated by a space";
            }
            else if (tuning.charAt(charCount) == '#' && i == 0) {
                return "Please enter a note before the sharp";
            }
            else if (tuning.charAt(charCount) == 'b' && i == 0) {
                return "Please enter a note before the flat";
            }
            else if (i > 2) {
                return "Please separate each string's tuning with one space";
            }
            else if (tuning.charAt(0) == ' ' || tuning.charAt(tuning.length() - 1) == ' ') {
                return "Please do not start or end with a space";
            }
            else {
                i++;
                charCount++;
            }
        }
        if (j == currentStrings) {
            return null;
        }
        else if (j > currentStrings) {
            return "Tuning invalid, you have entered more than " + currentStrings + " strings, make sure you have separated notes with one space only";
        }
        else {
            return "Tuning invalid, you have entered less than " + currentStrings + " strings, make sure you have separated notes with one space only";
        }
    }
    
    public boolean validChar(char c) {
        if (c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F' || c == 'G' || c == '#' || c == 'b') {
            return true;
        }
        else {
            return false;
        }
    }
    
    public void setBeatsPerBar(int input) {
        currentBeatsPerBar = input;
    }
    
    public boolean stringNumeric(String string) {
        try {
            Integer.parseInt(string);
        }
        catch (NumberFormatException e) {
            return false;
        }
        return true;
    }
    
    public void setNoteType(int type) {
        noteType = type;
    }

    public void setNextNoteLength(int length) {
        noteLength = length;
    } 
    
    public void setNoteDotted(boolean dotted) {
        noteDotted = dotted;
    }
    
    public void setOtherOption(int other) {
        otherOption = other;
    }
    
    public void resetStrings(int updatedStrings) {
        for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < 8; j++) {
                notes[i][j] = -1;
            }
            noteLengths[i] = -1;
            notesDotted[i] = false;
            otherOptions[i] = -1;
            noteTypes[i] = -1;
        }
        currentStrings = updatedStrings;
        numberOfNotes = 0;
    }
    
    public int getCurrentStrings() {
        return currentStrings;
    }
    
}
    

