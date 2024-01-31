/******************************************************************************
* Author: Isra Mohamed 
* Project 4: Restaurant Inspections
* November 1st, 2023
* This program displays information about inspections on different restaurants in Chicago 
* Class: CS 141, Fall 2023
* Systems: ZyBook Lab and Replit *
*******************************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

/* Description: class introducting Restaurant object with multiple get functions and function assigning information to each variable*/
class Restaurant{
    public:
        /*Description: assigns private variables with corresponding info from copyFile function
        * Inputs: restName (string holding restaurant name info), restAddress (string holding restaurant address info), restDate (string holding restaurant date info), restResult (string holding restaurant result info), restRisk (string holding restaurant risk info), restNeighborhood (string holding restaurant neihborhood info)
        * Outputs: void function (holds values of restaurant object)*/
        void SetRestaurantValues(string restName, string restAddress, string restDate, string restResult, string restRisk, string restNeighborhood){
            name = restName;
            date = restDate;
            address = restAddress;
            inspectResult = restResult;
            neighborhood = restNeighborhood;
            risk = restRisk;
        }
        vector<Restaurant>copyFile(string dataFile);
        //getters
        string getName(){
            return name;
        }
        string getDate(){
            return date;
        }
        string getAddress(){
            return address;
        }
        string getResult(){
            return inspectResult;
        }
        string getNeighborhood(){
            return neighborhood;
        }
        string getRisk(){
            return risk;
        }

    private:
        //variables each Restaurant object will have (not directly seen by user)
        string name;
        string date;
        string address;
        string inspectResult;
        string neighborhood;
        string risk;
};

/* Description: public function which finds each specific restaurant info between commas in data file lines
* Inputs: dataFile (string name of user file input)
* Outputs: restaurantList (returns vector filled with Restaurant objects for each restaurant in data file)*/
vector<Restaurant> Restaurant::copyFile(string dataFile){
    ifstream restList;//input board stream
    restList.open(dataFile);//opens file

    vector<Restaurant> restaurantList;
    string line;
    while(getline(restList, line)){
        string name;
        string date;
        string address;
        string inspectResult;
        string neighborhood;
        string risk;

        //finds name, address, date, risk, result, and neighborhood using substring
        int firstCommaIndex = line.find(',');
        name = line.substr(0, firstCommaIndex);
        
        int secondCommaIndex = line.find(',', firstCommaIndex+1);
        int addressLength = secondCommaIndex-firstCommaIndex-1;
        address = line.substr(firstCommaIndex+1, addressLength);

        int thirdCommaIndex = line.find(',', secondCommaIndex+1);
        date = line.substr(secondCommaIndex+1, 10);

        int fourthCommaIndex = line.find(',', thirdCommaIndex+1);
        risk = line.substr(thirdCommaIndex+1, 1);

        int fifthCommaIndex = line.find(',', fourthCommaIndex+1);
        int resultLength = fifthCommaIndex-fourthCommaIndex-1;
        inspectResult = line.substr(fourthCommaIndex+1, resultLength);

        int sixthCommaIndex = line.find(',', fifthCommaIndex+1);
        int neighborhoodLength = sixthCommaIndex-fifthCommaIndex-1;
        neighborhood = line.substr(fifthCommaIndex+1, neighborhoodLength);


        Restaurant rest;//restaurant object
        rest.SetRestaurantValues(name, address, date, inspectResult, risk, neighborhood);//calls function to assign values to each private variable
        restaurantList.push_back(rest);//adds every type restauraunt object to restaurant list
    }
    return restaurantList;
}

/* Description: calculates number of total restaurants and adds dates of all passing restaurants in a vector 
* Inputs: numRestaurants (integer representing number of restaurants in file), numPassing (integer representing number of passing restaurants in file), restaurantList (vector with all restaurants in data file), passDates (vector with all dates from passing restaurants (starts empty))
* Outputs: void function (changes number of restaurants variable from main function and fills passDates vector with all passing dates)*/
void passRests(int& numRestaurants, int& numPassing, vector<Restaurant>restaurantList, vector<string>& passDates){
    numRestaurants = 0;
    numPassing = 0;

    for(Restaurant rest: restaurantList){//iterates through all restaurants in file
        numRestaurants++;
        if(rest.getResult() == "Pass" || rest.getResult() == "Conditional"){
            numPassing++;
            passDates.push_back(rest.getDate());//adds passing date to passDates vector
        }
    }
}

/* Description: calculates percentage of risk type in data file
* Inputs: restaurantList (vector with all restaurants in data file)
* Outputs: void function (displays percentage of restaurants that are high, low, and medium risk)*/
void percentRisk(vector<Restaurant> restaurantList){
    double riskHigh = 0.0;
    double riskLow = 0.0;
    double riskMid = 0.0;

    int numRestaurants = 0;
    for(Restaurant rest : restaurantList){//iterates through all restaurants in file
        numRestaurants++;//counts total number of restaurants in file
        if(rest.getRisk()=="H"){
            riskHigh++;//counts total number of restaurants that are high risk
        }else if(rest.getRisk()=="L"){
            riskLow++;//counts total number of restaurants that are low risk
        }else{
            riskMid++;//counts total number of restaurants that are medium risk
        }
    }
    
    //calculates risk percentages
    riskHigh = (riskHigh/numRestaurants)*100;
    riskLow = (riskLow/numRestaurants)*100;
    riskMid = (riskMid/numRestaurants)*100;

    //outputting each percentage with only one decimal place (setprecision(1))
    cout<<"High risk: "<<fixed<<setprecision(1)<<riskHigh<<"%"<<endl;
    cout<<"Medium risk: "<<fixed<<setprecision(1)<<riskMid<<"%"<<endl;
    cout<<"Low risk: "<<fixed<<setprecision(1)<<riskLow<<"%"<<endl;
}

/* Description: prints out header for option 3 output
* Inputs: none
* Outputs: void function (displays header titles and dashes)*/
void header(){
    cout << setw(27)<< left << "Neighborhood" << setw(7)<< left << "Passed" << setw(15)<< left << "Cond. Pass" <<left << "Failed" << endl;
    cout << setw(27)<< left << "============" << setw(7)<< left << "======" << setw(15)<< left << "==========" << left << "======" << endl;
}

/* Description: calculates most recent date inspection from restaurants that passed inspection
* Inputs: recentDate (a string that holds most recent date nums), passDates (vector of dates that are passing)
* Outputs: void function (stores most recent passing date in variable recentDate)*/
void dateRecent(string& recentDate, vector<string> passDates){//recentDate variable in main function also needs to be changed (&)
    string comparison = "00000000";//string holding current greatest date nums which is passing
    recentDate = "00000000";
    for (string date : passDates) {//iterating through vector of passing dates
        // Convert the date to a numerical format (YYYYMMDD)
        int dateNumeric = stoi(date.substr(6, 4) + date.substr(0, 2) + date.substr(3, 2));
        int comparisonNumeric = stoi(comparison.substr(6, 4) + comparison.substr(0, 2) + comparison.substr(3, 2));

        if (dateNumeric >= comparisonNumeric){//checks if current passing date is more recent (greater) than compared recent date so far
            comparison = date;
            recentDate = date;
        }
    }
}

/* Description: prints out all inspection information for restaurants containing user inputted word in restaurant name
* Inputs: restaurantList (vector with all restaurants in data file), searchWord (string word that user wants to search in restaurant names)
* Outputs: void function (displays restaurant names, addresses, dates, and results that have the user word in the restaurant name)*/
void printSearchedRestaurants(vector<Restaurant>restaurantList, string searchWord){
    for(unsigned int i = 0; i<searchWord.size(); i++){
        searchWord.at(i) = toupper(searchWord.at(i));//capitalizes each letter in user input string
    }
    for(Restaurant rest: restaurantList){//iterates through all restaurants in file
        //if finding the user string in restaurantList was NOT a failure
        if(rest.getName().find(searchWord)!=string::npos){//(string::npos basically means failure)
            cout<<endl<<"Restaurant: "<<rest.getName()<<endl;
            cout<<"Address: "<<rest.getAddress()<<endl;
            cout<<"Inspection Date: "<<rest.getDate()<<endl;
            cout<<"Inspection Result: "<<rest.getResult()<<endl;
        }else{
            continue;
        }
    }
}

/* Description: prints out inspection results for each neighborhood in data file
* Inputs: restaurantList (vector with all restaurants in data file), neighborhoodList (vector with all different neighborhoods in data file (starts empty))
* Outputs: void function (displays number of restaurants for each result type in each neighborhood)*/
void neighborhoodInfo(vector<Restaurant>restaurantList, vector<string> neighborhoodList){
    for(Restaurant rest: restaurantList){//iterates through all restaurants in file
        bool notRepeat = true;
        for(string n: neighborhoodList){//iterates through all neighborhoods in file
            if(n==rest.getNeighborhood()){
                notRepeat = false;
                break;
            }
        }
        if(notRepeat == true){
            neighborhoodList.push_back(rest.getNeighborhood());//adds neighborhood to neighborhood vector
        }
    }
    int numPassed;
    int numFailed;
    int numCond;
    for(string n: neighborhoodList){//iterates through all neighborhoods in file
        numPassed = 0;
        numFailed = 0;
        numCond = 0;
        for(Restaurant r: restaurantList){//iterates through all restaurants in file
            if(n==r.getNeighborhood()){
                if(r.getResult()=="Pass"){
                    numPassed++;
                }else if(r.getResult()=="Fail"){
                    numFailed++;
                }else{
                    numCond++;
                }
            }
        }
    cout << setw(27)<< left << n << setw(6) << right << numPassed << setw(11) << right << numCond << setw(11) << right << numFailed << endl; 
    }
}

int main() {
    //will all be assigned values using outside functions(&)
    vector<Restaurant> restaurantList;//empty vector of Restaurant objects 
    vector<string> passDates;//empty vector of strings
    vector<string> neighborhoodList;//empty vector of strings
    string recentDate;//empty string

    cout<<"Enter the data file to use: ";
    string file;
    cin>>file;
    cout<<endl;

    Restaurant rest;
    restaurantList = rest.copyFile(file);//assigns empty Restaurant vector with all restaurants in file
    
    //will be assigned values using outside functions(&)
    int numRestaurants;
    int numPassing;

    while(true){//so that prompt repeats after every choice chosen
        cout << "Select a menu option: " << endl;
        cout << "   1. Display overall inspection information" << endl;
        cout << "   2. Display risk percentages" << endl;
        cout << "   3. Display passing numbers by neighborhood" << endl;
        cout << "   4. Search for restaurant by name" << endl;
        cout << "   5. Exit" << endl;
        cout << "Your choice: ";
        
        int option = 0;
        cin>>option;

        if(option==1){
            passRests(numRestaurants, numPassing, restaurantList, passDates);//assigns numRestaurants variable
            dateRecent(recentDate, passDates);//assigns numPassing variable and mostRecent variable
            cout<<"Number of restaurants: "<<numRestaurants<<endl;
            cout<<"Number that pass: "<<numPassing<<endl;
            for(Restaurant r: restaurantList){
                //if finding the most recent passing date in restaurantList was NOT a failure
                if(r.getDate().find(recentDate)!=string::npos){//(string::npos basically means failure)
                    cout<<"Most recent passing inspection was of "<<r.getName()<<" on "<<r.getDate()<<endl; 
                }           
            }
        }
        else if(option==2){
            percentRisk(restaurantList);
        }
        else if(option==3){
            header();
            neighborhoodInfo(restaurantList, neighborhoodList);
        }else if(option == 4){
            cout<<"Enter restaurant to search for: ";
            string searchWord;
            cin.ignore();//need to flush the newline character in the buffer first
            getline(cin, searchWord);
            printSearchedRestaurants(restaurantList, searchWord);//finds restaurants with a name that includes searchWord
        }else{
            break;//if any other number is chosen
        }
    }
    return 0;
}