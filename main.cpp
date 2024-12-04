
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace std;

//Athlete struct
struct Athlete {
    int id{};
    string name;
    string sport;
    string event;
    float performance_time{};
    string country;
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
            athlete.performance_time = stod(value);
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
void merge(vector<Athlete>& athletes, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<Athlete> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = athletes[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = athletes[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].performance_time <= R[j].performance_time) {
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

void mergeSort(vector<Athlete>& athletes, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(athletes, left, mid);
        mergeSort(athletes, mid + 1, right);
        merge(athletes, left, mid, right);
    }
}


// Quick Sort implementation
int partition(vector<Athlete>& athletes, int low, int high) {
    double pivot = athletes[high].performance_time;
    int i = (low - 1);
    for (int j = low; j < high; ++j) {
        if (athletes[j].performance_time <= pivot) {
            ++i;
            swap(athletes[i], athletes[j]);
        }
    }
    swap(athletes[i + 1], athletes[high]);
    return (i + 1);
}

void quickSort(vector<Athlete>& athletes, int low, int high) {
    if (low < high) {
        int pi = partition(athletes, low, high);
        quickSort(athletes, low, pi - 1);
        quickSort(athletes, pi + 1, high);
    }
}

vector<Athlete> filterBySportAndEvent(const vector<Athlete>& athletes, const string& sport, const string& event) {
    vector<Athlete> filtered;
    for (const auto& athlete : athletes) {
        if (athlete.sport == sport && athlete.event == event) {
            filtered.push_back(athlete);
        }
    }
    return filtered;
}
//vector<Athlete> athletes = loadAthleteData("athlete_data.csv");
//string sport, event;


//vector<Athlete> filteredAthletes = filterBySportAndEvent(athletes, sport, event);

// Display using SFML
void displayBestPerformance(sf::RenderWindow& window, const Athlete& bestAthlete, double mergeSortTime, double quickSortTime) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Failed to load font!" << endl;
        return;
    }

    window.clear(sf::Color::White);



    // Display Merge Sort Time
    sf::Text mergeSortText("Merge Sort Time: " +to_string(mergeSortTime) + " seconds", font, 20);
    mergeSortText.setFillColor(sf::Color::Black);
    mergeSortText.setPosition(10, 10);
    window.draw(mergeSortText);

    // Display Quick Sort Time
    sf::Text quickSortText("Quick Sort Time: " + to_string(quickSortTime) + " seconds", font, 20);
    quickSortText.setFillColor(sf::Color::Black);
    quickSortText.setPosition(10, 40);
    window.draw(quickSortText);

    // Display Best Athlete Details
    string athleteDetails = "Best Performance:\nID: "+to_string(bestAthlete.id) + "\nAthlete: " + bestAthlete.name +
                            "\nTime: " + to_string(bestAthlete.performance_time) +
                            " seconds\nCountry: " + bestAthlete.country;
    sf::Text athleteText(athleteDetails, font, 20);
    athleteText.setFillColor(sf::Color::Black);
    athleteText.setPosition(10, 80);
    window.draw(athleteText);

    window.display();
}


void promptInput(sf::RenderWindow& window, sf::Font& font, string& sport, string& event) {
    // Prompts
    sf::Text sportPrompt("Enter Sport: ", font, 30);
    sportPrompt.setPosition(50, 50);

    sf::Text eventPrompt("Enter Event: ", font, 30);
    eventPrompt.setPosition(50, 100);

    // Options for sports and events
    sf::Text sportOptions("Available Sports:\n1. Athletics\n2. Cycling\n3. Skiing\n4. Swimming\n5. Gymnastics", font, 20);
    sportOptions.setPosition(50, 200);

    sf::Text eventOptions("Available Events:\n1. Athletics Events: 100m OR Marathon\n2. Cycling Events: Road Cycling OR Mountain Biking\n3. Skiing Events: Giant Slalom OR Super-G\n4. Swimming Events: Freestyle OR Butterfly\n5. Gymnastics Events: Floor Exercise OR Vault", font, 20);
    eventOptions.setPosition(50, 350);

    // User input
    sf::Text sportText("", font, 30);
    sf::Text eventText("", font, 30);
    sportText.setPosition(sportPrompt.getPosition().x + sportPrompt.getGlobalBounds().width + 5, sportPrompt.getPosition().y);
    eventText.setPosition(eventPrompt.getPosition().x + eventPrompt.getGlobalBounds().width + 5, eventPrompt.getPosition().y);

    // Input handling
    sf::Text inputText("", font, 30);
    string currentInput = "";
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


int main() {
    //vector<Athlete> athletes = loadAthleteData("athlete_data.csv");
    vector<Athlete> athletes = loadAthleteData("sports_performance_dataset.csv");

    if (athletes.empty()) {
        cerr << "No athlete data available!" << endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "Athlete Performance");

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


    // Measure time for Quick Sort
    auto startQuick = chrono::high_resolution_clock::now();
    quickSort(filteredAthletes, 0, filteredAthletes.size() - 1);
    auto endQuick = chrono::high_resolution_clock::now();
    chrono::duration<double> durationQuick = endQuick - startQuick;


    // Display the best performance
    Athlete bestAthlete = filteredAthletes[0];

    displayBestPerformance(window, bestAthlete, durationMerge.count(), durationQuick.count());

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }


    return 0;
}

