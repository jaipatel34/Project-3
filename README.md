# Who's the Best Athlete?

## Project Description
"Who's the Best Athlete?" is a C++ application that analyzes and displays sports performance data using CSV files. The program allows users to:
- Filter athletes by sport and event.
- Sort athletes by performance time using **Merge Sort** and **Quick Sort**.
- Compare sorting algorithm performance in terms of execution time.
- Display detailed information and congratulate the best athlete in an interactive graphical interface powered by **SFML** (Simple and Fast Multimedia Library).

---

## Features
1. **CSV Data Handling**  
   The program reads athlete data from a CSV file, processes it into a `struct`, and stores it in a `vector`.

2. **Sorting Algorithms**
   - **Merge Sort**: Stable and efficient for large datasets.
   - **Quick Sort**: Faster for most scenarios with average-case performance.

3. **Interactive User Input**  
   Users input their desired **sport** and **event** using a graphical interface.

4. **Performance Metrics**  
   Displays the execution time for both sorting algorithms.

5. **Graphical Output**  
   SFML is used to display:
   - The fastest athlete's information.
   - Execution times of sorting algorithms.
   - A congratulatory message.

---

## How It Works
1. **Data Loading**  
   The program reads athlete data from a CSV file (e.g., `sports_performance_dataset.csv`).  
   Data is stored in a vector of `Athlete` structs.

2. **User Input**  
   A graphical interface prompts the user to input a sport and an event.

3. **Filtering**  
   Filters athletes by the selected sport and event.

4. **Sorting**  
   Applies Merge Sort and Quick Sort to sort the filtered athletes by performance time.

5. **Best Athlete**  
   Displays the fastest athlete's details along with the sorting times.

---

## Prerequisites
- **C++17 or later**
- **SFML** (Simple and Fast Multimedia Library)
  - Download from [SFML's official website](https://www.sfml-dev.org/).
- A valid **CSV dataset** with the following columns:  
  `ID, Name, Sport, Event, Performance Time, Country`

---

## Usage
1. **Launch the Program**  
   Enter a valid sport (e.g., Swimming)
   Enter a valid event (e.g., Freestyle)

2. **View Results**
-  Sorting times for Merge Sort and Quick Sort.
-  Best athlete's details.
-  A congratulatory message.
3. **Close the Window**
   Close the graphical window to exit the program.
 
