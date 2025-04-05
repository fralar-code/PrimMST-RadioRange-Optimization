#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include "Graph.h"
using namespace std;

//function used to skip lines from the text file when an input error occurs
void skipLine(ifstream &in, int n){
    for(int i=0;i<n;i++){
        in.ignore(numeric_limits<std::streamsize>::max(),'\n');
    }
}

int main(){
    int N, S, V, x, y;
    ifstream in("input.txt");
    if(in){
        if (in.eof()){
            cout << "File empty\n";
        }
        else{
            in >> N;
            //loop to scan the acquisition operations as many times as the number of tests N taken from input
            for (int i = 0; i < N; i++){
                in >> S;
                in >> V;
                //check on the number of satellite channels, in case of error skip to the next test
                if (S < 1 || S > 100){
                    cout << "Error in test " << i+1 <<"! Enter a number of satellite channels between 1 and 100!" << endl<< endl;
                    skipLine(in,V+1);
                    continue;
                }
               //check on the number of outposts, in case of error skip to the next test
                if (V <= S || V > 500){
                    cout << "Error in test " << i+1 <<"! Enter a number of outposts greater than the number of satellites and less than 500!" << endl<< endl;
                    skipLine(in,V+1);
                    continue;
                }

                //we use the Coords class to acquire the coordinates (x,y)
                vector<Coords> outpost;
                bool error=false;
                for (int j = 0; j < V; j++){
                    in >> x >> y;
                    //coordinate check
                    if (x < 0 || x > 10000 || y < 0 || y > 10000){
                        error=true;
                        cout << "Error in test " << i+1 <<"! Enter coordinates between 0 and 10000!" << endl<< endl;
                        skipLine(in,V-j);
                        break;
                    }
                    outpost.push_back(Coords (x,y));
                }
                //in case of error on coordinates skip to the next test
                if(error) continue;

                //graph creation curr_graph
                Graph curr_graph = Graph(S,V,outpost);

                //curr_graph.printGraph();

                //calculation of the MST
                curr_graph.computePrimMST();

                cout << "Test " << i+1 << ": " <<endl;
                //print of the minimum D needed to connect the network
                cout << curr_graph.determineMinRadioRange() << setprecision(2) << fixed << endl;
                //setprecision(2) << fixed is used to display only 2 digits after the decimal point
                cout << endl;
            }
        }
    }
}