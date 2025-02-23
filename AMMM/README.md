# Chess Tournament Scheduling  

This project focuses on scheduling a round-robin chess tournament with an odd number **n** of contestants, ensuring fairness in white/black assignments and optimizing player availability preferences.  

## Objectives  

- **Tournament Scheduling:** Assigning matchups across **n** time slots while ensuring each contestant plays exactly once per round and sits out exactly once.  
- **Fair White/Black Distribution:** Balancing the number of times each contestant plays as white or black.  
- **Optimizing Player Preferences:** Contestants allocate **100 preference points** across time slots, and the goal is to maximize the overall satisfaction score of the schedule.  

## Solution Methods  

1. **Integer Linear Programming (ILP):**  
   - Formulating the problem as an ILP and solving it with **CPLEX**.  

2. **Heuristic Algorithms:**  
   - Implementing:  
     - A **greedy constructive algorithm**.  
     - A **greedy algorithm + local search** for refinement.  
     - **GRASP metaheuristic**, reusing local search for further optimization.  

3. **Instance Generation & Parameter Tuning:**  
   - Generating random problem instances.  
   - Tuning the **α parameter** of GRASP.  
   - Running scalability tests with increasing problem sizes (1–30 min per instance).  

4. **Performance Comparison:**  
   - Evaluating ILP and heuristic solutions in terms of **computation time** and **solution quality**.  

## Deliverables  

- **Code implementation** of ILP and heuristic methods.  
- **Benchmark results** comparing different approaches.  
- **Final report & presentation** summarizing findings.  
