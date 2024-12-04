
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
        cerr << "File is not opening" << endl;
        return athletes;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        istringstream stream(line);
        Athlete athlete;
        string val;

        // Read ID Number (first value)
        getline(stream, val, ',');
        athlete.id = stoi(val);

        // Read Athlete Name (second value)
        getline(stream, athlete.name, ',');

        // Read Sport (third value)
        getline(stream, athlete.sport, ',');

        // Read Event (fourth value)
        getline(stream, athlete.event, ',');

        // Read Race Time (fifth value)
        getline(stream, val, ',');
        athlete.performance_time = stod(val);

        // Read Country (sixth value)
        getline(stream, athlete.country, ',');

        athletes.push_back(athlete);
    }
    file.close();
    return athletes;
}

//Pre-Lecture Video Cited
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
        }
        else {
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

//Pre- Lecture Video Cited
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
    int part = low - 1;

    for (int j = low; j < high; j++) {
        if (athletes[j].performance_time <= piv) {
            part++;
            swap(athletes[part], athletes[j]);
        }
    }
    swap(athletes[part + 1], athletes[high]);
    return part + 1;
}

//Pre- Lecture Video Cited
void quickSort(vector<Athlete>& athletes, int low, int high) {
    if (low < high) {
        int piv = partition(athletes, low, high);

        quickSort(athletes, low, piv - 1);
        quickSort(athletes, piv + 1, high);
    }
}

//look at event and sport 
vector<Athlete> checkEandS(const vector<Athlete>& athletes, const string& sport, const string& event) {
    vector<Athlete> newSE;
    for (const auto& athlete : athletes) {
        if (athlete.event == event && athlete.sport == sport) {
            newSE.push_back(athlete);
        }
    }
    return newSE;
}

//first screen
void UserInputScreen(sf::RenderWindow& window, sf::Font& font, string& sport, string& event) {
    sf::Text SportName("Enter Sport: ", font, 30);
    SportName.setPosition(50, 50);

    sf::Text EventName("Enter Event: ", font, 30);
    EventName.setPosition(50, 100);

    sf::Text AvailableSports("Available Sports:\n1. Athletics\n2. Cycling\n3. Skiing\n4. Swimming\n5. Gymnastics", font, 20);
    AvailableSports.setPosition(50, 200);

    sf::Text AvailableEvents("Available Events:\n1. Athletics Events: 100m OR Marathon\n2. Cycling Events: Road Cycling OR Mountain Biking\n3. Skiing Events: Giant Slalom OR Super-G\n4. Swimming Events: Freestyle OR Butterfly\n5. Gymnastics Events: Floor Exercise OR Vault", font, 20);
    AvailableEvents.setPosition(50, 350);

    sf::Text UserSport("", font, 30);
    sf::Text UserEvent("", font, 30);
    UserSport.setPosition(SportName.getPosition().x + SportName.getGlobalBounds().width + 5, SportName.getPosition().y);
    UserEvent.setPosition(EventName.getPosition().x + EventName.getGlobalBounds().width + 5, EventName.getPosition().y);

    sf::Text inputAnswer("", font, 30);
    string currAnswer = "";
    bool newSport = true;

    sf::RectangleShape cursor(sf::Vector2f(2, 30));
    cursor.setFillColor(sf::Color::White);

    sf::Clock clock;
    bool cursorOutput = true;

    while (window.isOpen()) {
        sf::Event WindowE;
        while (window.pollEvent(WindowE)) {
            if (WindowE.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (WindowE.type == sf::Event::TextEntered) {
                if (!currAnswer.empty() && WindowE.text.unicode == '\b') {
                    currAnswer.pop_back();
                }
                else if (WindowE.text.unicode == '\r') {
                    if (newSport) {
                        sport = currAnswer;
                        UserSport.setString(sport);
                        newSport = false;
                        currAnswer = "";
                    }
                    else {
                        event = currAnswer;
                        UserEvent.setString(event);
                        return;
                    }
                }
                else if (WindowE.text.unicode < 128) {
                    currAnswer += char(WindowE.text.unicode);
                }
            }
        }

        inputAnswer.setString(currAnswer);

        if (!newSport) {
            inputAnswer.setPosition(UserEvent.getPosition().x + UserEvent.getGlobalBounds().width + 5, UserEvent.getPosition().y);

        }
        else {
            inputAnswer.setPosition(UserSport.getPosition().x + UserSport.getGlobalBounds().width + 5, UserSport.getPosition().y);
        }

        cursor.setPosition(inputAnswer.getPosition().x + inputAnswer.getGlobalBounds().width + 2, inputAnswer.getPosition().y);

        if (clock.getElapsedTime().asMilliseconds() > 400) {
            cursorOutput = !cursorOutput;
            clock.restart();
        }

        window.clear(sf::Color::Black);
        window.draw(inputAnswer);
        window.draw(AvailableSports);
        window.draw(AvailableEvents);
        window.draw(SportName);
        window.draw(EventName);
        window.draw(UserSport);
        window.draw(UserEvent);


        if (cursorOutput) {
            window.draw(cursor);
        }

        window.display();
    }
}

// Second Screen
void PerformanceTimeAndInfoScreen(sf::RenderWindow& window, const Athlete& bestAthlete, double mergeSortTime, double quickSortTime) {
    sf::Font font;
    font.loadFromFile("arial.ttf");

    window.clear(sf::Color::White);
    

    // Merge Sort Time
    sf::Text mergeSortInfo("Merge Sort Time: " +to_string(mergeSortTime) + " seconds", font, 20);
    mergeSortInfo.setFillColor(sf::Color::Black);
    mergeSortInfo.setPosition(10, 10);
    window.draw(mergeSortInfo);

    // Quick Sort Time
    sf::Text quickSortInfo("Quick Sort Time: " + to_string(quickSortTime) + " seconds", font, 20);
    quickSortInfo.setFillColor(sf::Color::Black);
    quickSortInfo.setPosition(10, 40);
    window.draw(quickSortInfo);

    // Best Athlete Info
    string athleteInfo = "Best Performance:\nID: "+to_string(bestAthlete.id) + "\nAthlete: " + bestAthlete.name +
                            "\nTime: " + to_string(bestAthlete.performance_time) +
                            " seconds\nCountry: " + bestAthlete.country;
    sf::Text athleteText(athleteInfo, font, 20);
    athleteText.setFillColor(sf::Color::Black);
    athleteText.setPosition(10, 80);
    window.draw(athleteText);

    string upperName = bestAthlete.name;
    transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);

    string Congrats = "CONGRATULATIONSSSSS \n" +upperName +"!!!!!!!!!";
    sf::Text congratsText(Congrats, font, 55);
    congratsText.setFillColor(sf::Color::Blue);
    congratsText.setPosition(10, 200);
    window.draw(congratsText);

    string CongratsN = "WOOOOOOOOOOOOOOO\nHOOOOOOOOOOOOOOOO\n!!!!!!!!!!!!!";
    sf::Text congratssText(CongratsN, font, 55);
    congratssText.setFillColor(sf::Color(245, 170, 0));
    congratssText.setPosition(10, 350);
    window.draw(congratssText);


    window.display();
}





int main() {
    // Dataset with 100
    //vector<Athlete> athletes = GetAthleteCSVData("sports_performance_dataset_TEST.csv");

    // Dataset with 100001
    vector<Athlete> athletes = GetAthleteCSVData("sports_performance_dataset.csv");

    if (athletes.empty()) {
        cerr << "Athlete Data is not available!\nPlease Try Again" << endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "Who's the Best Athlete!");

    sf::Font font;
    font.loadFromFile("arial.ttf");

    string sport;
    string event;

    UserInputScreen(window, font, sport, event);

    vector<Athlete> NewAthletes = checkEandS(athletes, sport, event);

    if (NewAthletes.empty()) {
        cout << "No athlete found for your input of the sport and event!" << endl;
        return 1;
    }

    // Measure time for Merge Sort
    auto firstMerge = chrono::high_resolution_clock::now();
    mergeSort(NewAthletes, 0, NewAthletes.size() - 1);
    auto secondMerge = chrono::high_resolution_clock::now();
    chrono::duration<double> TotalMergeTime = secondMerge - firstMerge;


    // Measure time for Quick Sort
    auto firstQuick = chrono::high_resolution_clock::now();
    quickSort(NewAthletes, 0, NewAthletes.size() - 1);
    auto secondQuick = chrono::high_resolution_clock::now();
    chrono::duration<double> TotalQuickTime = secondQuick - firstQuick;


    // Show the best time
    Athlete bestAthlete = NewAthletes[0];

    PerformanceTimeAndInfoScreen(window, bestAthlete, TotalMergeTime.count(), TotalQuickTime.count());

    while (window.isOpen()) {
        sf::Event even;
        while (window.pollEvent(even)) {
            if (even.type == sf::Event::Closed)
                window.close();
        }
    }


    return 0;
}

