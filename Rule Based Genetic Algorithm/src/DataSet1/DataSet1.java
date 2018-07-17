package DataSet1;

import java.util.Random;
import java.util.Scanner;

/**
 *
 * @author Jack Martin
 * 
 */

public class DataSet1 {
    
    int candidatesPerGeneration = 500;
    int numberOfGenerations = 1000;
    int candidates[][] = new int[candidatesPerGeneration][36];
    Scanner readData = new Scanner(DataSet1.class.getResourceAsStream("data1.txt"));
    int ruleBase[][] = new int[32][6];
    int candidateFitnesses[] = new int[candidatesPerGeneration];
    int selectedCandidates[][] = new int[candidatesPerGeneration][36];
    int crossoverProbability = 100;
    int mutationProbability = 1;
    Random rnd = new Random();
    
    public static void main(String[] args) {

        DataSet1 dataSet1 = new DataSet1();
        
        dataSet1.resetCandidateFitnesses();
        dataSet1.getRules();
        dataSet1.generateGenes();
            
        for (int i = 0; i < dataSet1.numberOfGenerations; i++) {
      
            dataSet1.resetCandidateFitnesses();
            dataSet1.getCandidateFitnesses();
            dataSet1.tournamentSelection();
            dataSet1.crossover();
            dataSet1.mutation();
            dataSet1.getTotalFitness();
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
        for (int x = 0; x < 32; x++) {
            String currentRule = readData.nextLine();
            for (int y = 0; y < 5; y++) {
                ruleBase[x][y] = Character.getNumericValue(currentRule.charAt(y));
            }
            ruleBase[x][5] = Character.getNumericValue(currentRule.charAt(6));
        }
    }
    
    public void generateGenes() {
        //Generates 36 random genes per candidate
        for (int x = 0; x < candidatesPerGeneration; x++) {
            for (int y = 0; y < 36; y++) {
                candidates[x][y] = rnd.nextInt(3);
            }
        }
    }
    
    public void getCandidateFitnesses() {
        //Compares each candidate to each rule from the data set
        boolean ruleValid = true;
        int candidateRules[][] = new int[6][6];
        for (int x = 0; x < candidatesPerGeneration; x++) {
            for (int y = 0; y < 6; y++) {
                for (int i = 0; i < 6; i++) {
                    candidateRules[y][i] = candidates[x][(y * 6) + i];
                }
            }
            for (int y = 0; y < 6; y++) {
                for (int z = 0; z < 32; z++) {
                    ruleValid = true;
                    for (int i = 0; i < 6; i++) {
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
                for (int y = 0; y < 36; y++) {
                    selectedCandidates[x][y] = candidates[randomCandidate1][y];
                }
            }
            
            //Uses second random candidate if fitness is higher
            else {
                for (int y = 0; y < 36; y++) {
                    selectedCandidates[x][y] = candidates[randomCandidate2][y];
                }
            }
        }
    }
    
    public void crossover() {
        //Carries out crossover on a number of candidates. More likely to happen if crossover probability is higher
        for (int x = 0; x < candidatesPerGeneration; x = x + 2) {
            int rand = rnd.nextInt(100);
            int parent1[] = new int[36];
            int parent2[] = new int[36];
            int child1[] = new int[36];
            int child2[] = new int[36];
            for (int y = 0; x < 36; x++) {
                parent1[y] = candidates[x][y];
                parent2[y] = candidates[x + 1][y];
                child1[y] = candidates[x][y];
                child2[y] = candidates[x][y];
            }
            if (rand < crossoverProbability) {
                int crossoverPoint = rnd.nextInt(36);
                for (int y = crossoverPoint; y < 36; y++) {
                    child1[y] = parent2[y];
                    child2[y] = parent1[y];
                }
            }
            else {
                for (int y = 0; y < 36; y++) {
                    candidates[x][y] = child1[y];
                    candidates[x + 1][y] = child2[y];
                }
            }
        }
    }
    
    public void mutation() {
        //Carries out mutation on random genes. More likely to occur if mutation probability is higher
        for (int x = 0; x < candidatesPerGeneration; x++) {
            for (int y = 0; y < 36; y++) {
                int rand = rnd.nextInt(100);
                if (rand < mutationProbability) {
                    candidates[x][y] = rnd.nextInt(3);
                }
            }
        }
    }
}