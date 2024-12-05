# Who's the Best Athlete?

## Project Description
"Who's the Best Athlete?" is a C++ application that analyzes and shows sports performance data using CSV files. The program allows users to:
- Filter athletes by sport and event.
- Sort athletes by performance time using **Merge Sort** and **Quick Sort**.
- Compare sorting algorithm performance in terms of execution time.
- Display detailed information and congratulate the best athlete in an interactive graphical interface by **SFML** (Simple and Fast Multimedia Library).

---

## Features
1. **CSV Data Handling**  
   The program reads athlete data from a CSV file, processes it into a `struct`, and stores it in a `vector`.

2. **Sorting Algorithms**
   - **Merge Sort**
   - **Quick Sort**

3. **User Input**  
   Users input their desired **sport** and **event** using a graphical interface.

4. **Performance Metrics**  
   Displays the execution time for both sorting algorithms.

5. **Graphical Result**  
   SFML is used to show:
   - The fastest athlete's information.
   - Execution times of sorting algorithms.
   - A congratulatory message.

---

## How It Works
1. **Data Loading**  
   The program reads athlete data from a CSV file (e.g., `sports_performance_dataset.csv`).
   
3. **User Input**  
   A graphical interface asks the user to input a sport and an event.

4. **Filtering**  
   Filters athletes by the selected sport and event.

5. **Sorting**  
   Applies Merge Sort and Quick Sort to sort the filtered athletes by best performance time.

6. **Best Athlete**  
   Displays the fastest athlete's information along with the sorting times.

---

## Prerequisites
   - **C++14 or later**
   - **SFML** (Simple and Fast Multimedia Library)
   - A valid **CSV dataset** with the following columns in this order:  
     `ID, Name, Sport, Event, Performance Time, Country`

---

## How to Use
1. **Launch the Program**  
   - Enter a valid sport (e.g., Swimming)
   - Enter a valid event (e.g., Freestyle)

3. **View Results**
   -  Sorting times for Merge Sort and Quick Sort.
   -  Best athlete's details.
   -  A congratulatory message.
   
4. **Close the Window**
   - Close the graphical window to exit the program.
 
