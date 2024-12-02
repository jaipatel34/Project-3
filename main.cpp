
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace std;

//Athlete struct
struct Athlete {
    int id;
    std::string name;
    std::string sport;
    std::string event;
    double race_time;
    std::string country;
};


// Function to load data from CSV file
vector<Athlete> loadAthleteData(const string& filename) {
    vector<Athlete> athletes;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return athletes;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        istringstream stream(line);
        Athlete athlete;
        string value;

        getline(stream, value, ',');
        try {
            athlete.id = stoi(value);
        } catch (const invalid_argument& e) {
            cerr << "Invalid ID value: " << value << endl;
            continue;
        }

        // Read Athlete Name (second value)
        getline(stream, athlete.name, ',');

        // Read Sport (third value)
        getline(stream, athlete.sport, ',');

        // Read Event (fourth value)
        getline(stream, athlete.event, ',');

        // Read Race Time (fifth value)
        getline(stream, value, ',');
        try {
            athlete.race_time = stod(value);
        } catch (const invalid_argument& e) {
            cerr << "Invalid race time value: " << value << endl;
            continue;
        }

        // Read Country (sixth value)
        getline(stream, athlete.country, ',');

        athletes.push_back(athlete);
    }
    file.close();
    return athletes;
}

// Merge Sort implementation
void merge(std::vector<Athlete>& athletes, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<Athlete> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = athletes[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = athletes[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].race_time <= R[j].race_time) {
            athletes[k++] = L[i++];
        } else {
            athletes[k++] = R[j++];
        }
    }

    while (i < n1) {
        athletes[k++] = L[i++];
    }
    while (j < n2) {
        athletes[k++] = R[j++];
    }
}

void mergeSort(std::vector<Athlete>& athletes, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(athletes, left, mid);
        mergeSort(athletes, mid + 1, right);
        merge(athletes, left, mid, right);
    }
}


// Quick Sort implementation
int partition(std::vector<Athlete>& athletes, int low, int high) {
    double pivot = athletes[high].race_time;
    int i = (low - 1);
    for (int j = low; j < high; ++j) {
        if (athletes[j].race_time <= pivot) {
            ++i;
            std::swap(athletes[i], athletes[j]);
        }
    }
    std::swap(athletes[i + 1], athletes[high]);
    return (i + 1);
}

void quickSort(std::vector<Athlete>& athletes, int low, int high) {
    if (low < high) {
        int pi = partition(athletes, low, high);
        quickSort(athletes, low, pi - 1);
        quickSort(athletes, pi + 1, high);
    }
}

// Display using SFML
void displayBestPerformance(sf::RenderWindow& window, const Athlete& bestAthlete) {
    sf::Font font;
    font.loadFromFile("arial.ttf");

    window.clear(sf::Color::White);
    string row = "Best Performance: " + bestAthlete.name + " | Time: " + to_string(bestAthlete.race_time) + " seconds";
    sf::Text text(row, font, 20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(10, 10);
    window.draw(text);

    window.display();
}
std::vector<Athlete> filterBySportAndEvent(const std::vector<Athlete>& athletes, const std::string& sport, const std::string& event) {
    std::vector<Athlete> filtered;
    for (const auto& athlete : athletes) {
        if (athlete.sport == sport && athlete.event == event) {
            filtered.push_back(athlete);
        }
    }
    return filtered;
}
void promptInput(sf::RenderWindow& window, sf::Font& font, std::string& sport, std::string& event) {
    // Prompts
    sf::Text sportPrompt("Enter Sport: ", font, 30);
    sportPrompt.setPosition(50, 50);

    sf::Text eventPrompt("Enter Event: ", font, 30);
    eventPrompt.setPosition(50, 100);

    // Options for sports and events
    sf::Text sportOptions("Available Sports:\n1. Swimming\n2. Cycling\n3. Rowing", font, 20);
    sportOptions.setPosition(50, 200);

    sf::Text eventOptions("Available Events:\n1. 50m Butterfly\n2. 200m Freestyle\n3. Floor Routine", font, 20);
    eventOptions.setPosition(50, 300);

    // User input
    sf::Text sportText("", font, 30);
    sf::Text eventText("", font, 30);
    sportText.setPosition(sportPrompt.getPosition().x + sportPrompt.getGlobalBounds().width + 5, sportPrompt.getPosition().y);
    eventText.setPosition(eventPrompt.getPosition().x + eventPrompt.getGlobalBounds().width + 5, eventPrompt.getPosition().y);

    // Input handling
    sf::Text inputText("", font, 30);
    std::string currentInput = "";
    bool enteringSport = true;

    sf::RectangleShape cursor(sf::Vector2f(2, 30)); // Thin vertical line
    cursor.setFillColor(sf::Color::White);

    sf::Clock clock;
    bool cursorVisible = true;

    while (window.isOpen()) {
        sf::Event eventSFML;
        while (window.pollEvent(eventSFML)) {
            if (eventSFML.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (eventSFML.type == sf::Event::TextEntered) {
                if (eventSFML.text.unicode == '\b' && !currentInput.empty()) {
                    currentInput.pop_back();
                } else if (eventSFML.text.unicode == '\r') {
                    if (enteringSport) {
                        sport = currentInput;
                        sportText.setString(sport);
                        enteringSport = false;
                        currentInput = "";
                    } else {
                        event = currentInput;
                        eventText.setString(event);
                        return;
                    }
                } else if (eventSFML.text.unicode < 128) {
                    currentInput += static_cast<char>(eventSFML.text.unicode);
                }
            }
        }

        // Update input text and cursor position
        inputText.setString(currentInput);

        if (enteringSport) {
            inputText.setPosition(sportText.getPosition().x + sportText.getGlobalBounds().width + 5, sportText.getPosition().y);
        } else {
            inputText.setPosition(eventText.getPosition().x + eventText.getGlobalBounds().width + 5, eventText.getPosition().y);
        }

        cursor.setPosition(inputText.getPosition().x + inputText.getGlobalBounds().width + 2, inputText.getPosition().y);

        if (clock.getElapsedTime().asMilliseconds() > 500) {
            cursorVisible = !cursorVisible;
            clock.restart();
        }

        window.clear(sf::Color::Black);
        window.draw(sportPrompt);
        window.draw(eventPrompt);
        window.draw(sportText);
        window.draw(eventText);
        window.draw(inputText);

        window.draw(sportOptions);
        window.draw(eventOptions);

        if (cursorVisible) {
            window.draw(cursor);
        }

        window.display();
    }
}

// Main function
int main() {
    vector<Athlete> athletes = loadAthleteData("athlete_data.csv");
    //vector<Athlete> athletes = loadAthleteData("olympic_athlete_performance_data.csv");

    if (athletes.empty()) {
        cerr << "No athlete data available!" << endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "Best Performance");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Failed to load font!" << endl;
        return 1;
    }

    string sport, event;

    promptInput(window, font, sport, event);

    vector<Athlete> filteredAthletes = filterBySportAndEvent(athletes, sport, event);

    if (filteredAthletes.empty()) {
        cout << "No athletes found for the specified sport and event!" << endl;
        return 1;
    }

    // Measure time for Merge Sort
    auto startMerge = chrono::high_resolution_clock::now();
    mergeSort(filteredAthletes, 0, filteredAthletes.size() - 1);
    auto endMerge = chrono::high_resolution_clock::now();
    chrono::duration<double> durationMerge = endMerge - startMerge;
    cout << "Merge Sort Time: " << durationMerge.count() << " seconds" << endl;

    // Measure time for Quick Sort
    auto startQuick = chrono::high_resolution_clock::now();
    quickSort(filteredAthletes, 0, filteredAthletes.size() - 1);
    auto endQuick = chrono::high_resolution_clock::now();
    chrono::duration<double> durationQuick = endQuick - startQuick;
    cout << "Quick Sort Time: " << durationQuick.count() << " seconds" << endl;

    // Display the best performance
    Athlete bestAthlete = filteredAthletes[0];

    // Display using SFML
    displayBestPerformance(window, bestAthlete);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    // best performance details
    cout << "Best Performance:" << endl;
    cout << "Athlete: " << bestAthlete.name << endl;
    cout << "Time: " << bestAthlete.race_time << " seconds" << endl;
    cout << "Country: " << bestAthlete.country << endl;

    return 0;
}
