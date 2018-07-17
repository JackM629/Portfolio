package DataSet2;

import java.util.Random;
import java.util.Scanner;

/**
 *
 * @author Jack Martin
 * 
 */

public class DataSet2 {
    
    int candidatesPerGeneration = 500;
    int numberOfGenerations = 1000;
    int candidates[][] = new int[candidatesPerGeneration][42];
    Scanner readData = new Scanner(DataSet2.class.getResourceAsStream("data2.txt"));
    int ruleBase[][] = new int[64][7];
    int candidateFitnesses[] = new int[candidatesPerGeneration];
    int selectedCandidates[][] = new int[candidatesPerGeneration][42];
    int crossoverProbability = 100;
    int mutationProbability = 1;
    Random rnd = new Random();
    
    public static void main(String[] args) {

        DataSet2 dataSet2 = new DataSet2();
        
        dataSet2.resetCandidateFitnesses();
        dataSet2.getRules();
        dataSet2.generateGenes();
           
        for (int i = 0; i < dataSet2.numberOfGenerations; i++) {
      
            dataSet2.resetCandidateFitnesses();
            dataSet2.getCandidateFitnesses();
            dataSet2.tournamentSelection();
            dataSet2.crossover();
            dataSet2.mutation();
            dataSet2.getTotalFitness();
        } 
        
    }
    
    public void resetCandidateFitnesses() {
        //Resets all candidate fitnesses for new generation
        for (int x = 0; x < candidatesPerGeneration; x++) {
            candidateFitnesses[x] = 0;
        }
    }
    
    public void getRules() {
        //Reads rules in from text file
        for (int x = 0; x < 42; x++) {
            String currentRule = readData.nextLine();
            for (int y = 0; y < 6; y++) {
                ruleBase[x][y] = Character.getNumericValue(currentRule.charAt(y));
            }
            ruleBase[x][6] = Character.getNumericValue(currentRule.charAt(7));
        }
    }
    
    public void generateGenes() {
        //Generates 42 random genes per candidate
        for (int x = 0; x < candidatesPerGeneration; x++) {
            for (int y = 0; y < 42; y++) {
                candidates[x][y] = rnd.nextInt(3);
            }
        }
    }
    
    public void getCandidateFitnesses() {
        //Compares each candidate to each rule from the data set
        boolean ruleValid = true;
        int candidateRules[][] = new int[6][7];
        for (int x = 0; x < candidatesPerGeneration; x++) {
            for (int y = 0; y < 6; y++) {
                for (int i = 0; i < 7; i++) {
                    candidateRules[y][i] = candidates[x][(y * 6) + i];
                }
            }
            for (int y = 0; y < 6; y++) {
                for (int z = 0; z < 64; z++) {
                    ruleValid = true;
                    for (int i = 0; i < 7; i++) {
                        if (candidates[y][i] != 2 && candidates[y][i] != ruleBase[z][i]) {
                            ruleValid = false;
                        }
                    }
                    if (ruleValid == true) {
                        candidateFitnesses[x]++;
                    }
                }
            }
        }
    }
    
    public void getTotalFitness() {
        //Calculates and prints the average fitness of the generation
        int generationFitness = 0;
        for (int x = 0; x < candidatesPerGeneration; x++) {
            generationFitness = generationFitness + candidateFitnesses[x];
        }
        generationFitness = generationFitness / candidatesPerGeneration;
        System.out.println(generationFitness);
    }
    
    public void tournamentSelection() {
        //Chooses 2 random candidates at a time and chooses the best one from each pair to be a parent in the next generation
        for (int x = 0; x < candidatesPerGeneration; x++) {
            
            int randomCandidate1 = rnd.nextInt(candidatesPerGeneration);
            int randomCandidate2 = rnd.nextInt(candidatesPerGeneration);
                
            //Uses first random candidate if fitness is higher
            if (candidateFitnesses[randomCandidate1] > candidateFitnesses[randomCandidate2]) {
                for (int y = 0; y < 42; y++) {
                    selectedCandidates[x][y] = candidates[randomCandidate1][y];
                }
            }
            
            //Uses second random candidate if fitness is higher
            else {
                for (int y = 0; y < 42; y++) {
                    selectedCandidates[x][y] = candidates[randomCandidate2][y];
                }
            }
        }
    }
    
    public void crossover() {
        //Carries out crossover on a number of candidates. More likely to happen if crossover probability is higher
        for (int x = 0; x < candidatesPerGeneration; x = x + 2) {
            int rand = rnd.nextInt(100);
            int parent1[] = new int[42];
            int parent2[] = new int[42];
            int child1[] = new int[42];
            int child2[] = new int[42];
            for (int y = 0; x < 42; x++) {
                parent1[y] = candidates[x][y];
                parent2[y] = candidates[x + 1][y];
                child1[y] = candidates[x][y];
                child2[y] = candidates[x][y];
            }
            if (rand < crossoverProbability) {
                int crossoverPoint = rnd.nextInt(42);
                for (int y = crossoverPoint; y < 42; y++) {
                    child1[y] = parent2[y];
                    child2[y] = parent1[y];
                }
            }
            for (int y = 0; y < 42; y++) {
                candidates[x][y] = child1[y];
                candidates[x + 1][y] = child2[y];
            }
        }
    }
    
    public void mutation() {
        //Carries out mutation on random genes. More likely to occur if mutation probability is higher
        for (int x = 0; x < candidatesPerGeneration; x++) {
            for (int y = 0; y < 42; y++) {
                int rand = rnd.nextInt(100);
                if (rand < mutationProbability) {
                    candidates[x][y] = rnd.nextInt(3);
                }
            }
        }
    }
}