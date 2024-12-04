
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
vector<Athlete> GetAthleteCSVData(const string& filename) {
    vector<Athlete> athletes;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "File is not opening" << filename << endl;
        return athletes;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        istringstream stream(line);
        Athlete athlete;
        string value;

        getline(stream, value, ',');
        athlete.id = stoi(value);


        // Read Athlete Name (second value)
        getline(stream, athlete.name, ',');

        // Read Sport (third value)
        getline(stream, athlete.sport, ',');

        // Read Event (fourth value)
        getline(stream, athlete.event, ',');

        // Read Race Time (fifth value)
        getline(stream, value, ',');
        athlete.performance_time = stod(value);


        // Read Country (sixth value)
        getline(stream, athlete.country, ',');

        athletes.push_back(athlete);
    }
    file.close();
    return athletes;
}

void merge(vector<Athlete>& athletes, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;
    vector<Athlete> L(n1);
    vector<Athlete> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = athletes[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = athletes[middle + 1 + i];

    int i = 0;
    int j=0;
    int q=left;
    while (i < n1 && j < n2) {
        if (L[i].performance_time <= R[j].performance_time) {
            athletes[q] = L[i];
            i++;
        } else {
            athletes[q] = R[j];
            j++;

        }
        q++;
    }

    while (i < n1) {
        athletes[q++] = L[i++];

    }

    while (j < n2) {
        athletes[q++] = R[j++];
    }
}

void mergeSort(vector<Athlete>& athletes, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        mergeSort(athletes, left, middle);
        mergeSort(athletes, middle + 1, right);

        merge(athletes, left, middle, right);
    }
}



int partition(vector<Athlete>& athletes, int low, int high) {
    double piv = athletes[high].performance_time;
    int part = low - 1; // Partition index

    for (int j = low; j < high; ++j) {
        if (athletes[j].performance_time <= piv) {
            part++;
            swap(athletes[part], athletes[j]);
        }
    }
    swap(athletes[part + 1], athletes[high]);
    return part + 1;
}

void quickSort(vector<Athlete>& athletes, int low, int high) {
    if (low < high) {
        int piv = partition(athletes, low, high);

        quickSort(athletes, low, piv - 1);
        quickSort(athletes, piv + 1, high);
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

// Display using SFML
void displayBestPerformance(sf::RenderWindow& window, const Athlete& bestAthlete, double mergeSortTime, double quickSortTime) {
    sf::Font font;
    font.loadFromFile("arial.ttf");

    window.clear(sf::Color::White);



    // Merge Sort Time
    sf::Text mergeSortText("Merge Sort Time: " +to_string(mergeSortTime) + " seconds", font, 20);
    mergeSortText.setFillColor(sf::Color::Black);
    mergeSortText.setPosition(10, 10);
    window.draw(mergeSortText);

    // Quick Sort Time
    sf::Text quickSortText("Quick Sort Time: " + to_string(quickSortTime) + " seconds", font, 20);
    quickSortText.setFillColor(sf::Color::Black);
    quickSortText.setPosition(10, 40);
    window.draw(quickSortText);

    // Best Athlete Details
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
    sf::Text sportPrompt("Enter Sport: ", font, 30);
    sportPrompt.setPosition(50, 50);

    sf::Text eventPrompt("Enter Event: ", font, 30);
    eventPrompt.setPosition(50, 100);

    sf::Text sportOptions("Available Sports:\n1. Athletics\n2. Cycling\n3. Skiing\n4. Swimming\n5. Gymnastics", font, 20);
    sportOptions.setPosition(50, 200);

    sf::Text eventOptions("Available Events:\n1. Athletics Events: 100m OR Marathon\n2. Cycling Events: Road Cycling OR Mountain Biking\n3. Skiing Events: Giant Slalom OR Super-G\n4. Swimming Events: Freestyle OR Butterfly\n5. Gymnastics Events: Floor Exercise OR Vault", font, 20);
    eventOptions.setPosition(50, 350);

    sf::Text sportText("", font, 30);
    sf::Text eventText("", font, 30);
    sportText.setPosition(sportPrompt.getPosition().x + sportPrompt.getGlobalBounds().width + 5, sportPrompt.getPosition().y);
    eventText.setPosition(eventPrompt.getPosition().x + eventPrompt.getGlobalBounds().width + 5, eventPrompt.getPosition().y);

    sf::Text inputText("", font, 30);
    string currentInput = "";
    bool enteringSport = true;

    sf::RectangleShape cursor(sf::Vector2f(2, 30));
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
                }
                else if (eventSFML.text.unicode == '\r') {
                    if (enteringSport) {
                        sport = currentInput;
                        sportText.setString(sport);
                        enteringSport = false;
                        currentInput = "";
                    }
                    else {
                        event = currentInput;
                        eventText.setString(event);
                        return;
                    }
                }
                else if (eventSFML.text.unicode < 128) {
                    currentInput += static_cast<char>(eventSFML.text.unicode);
                }
            }
        }

        inputText.setString(currentInput);

        if (!enteringSport) {
            inputText.setPosition(eventText.getPosition().x + eventText.getGlobalBounds().width + 5, eventText.getPosition().y);

        }
        else {
            inputText.setPosition(sportText.getPosition().x + sportText.getGlobalBounds().width + 5, sportText.getPosition().y);
        }

        cursor.setPosition(inputText.getPosition().x + inputText.getGlobalBounds().width + 2, inputText.getPosition().y);

        if (clock.getElapsedTime().asMilliseconds() > 400) {
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
    //vector<Athlete> athletes = GetAthleteCSVData("athlete_data.csv");
    vector<Athlete> athletes = GetAthleteCSVData("sports_performance_dataset.csv");

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

