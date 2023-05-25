#include <iostream>
#include <fstream>
using namespace std;



    ofstream fout("output.txt");
    ifstream fin("input.txt");
void fillMatrices(int* available, int** max, int** allocation, int PROCESS, int RESOURCE);
void calculateNeed(int** need, int** max, int** allocation, int PROCESS, int RESOURCE);
void printMatrices(int** need, int** max, int** allocation, int* available, int PROCESS, int RESOURCE);

int main() {
    cout << "\n\nWelcome, this is an implementation of Banker’s algorithm to "
            "avoid deadlocks!\n\n";
    
    
    int PROCESS, RESOURCE;
    
    do {
        cout << "Enter the number of processes: ";
        fin >> PROCESS;
        if (PROCESS < 1)
          cout << "Please enter a valid number of processes (>0)\n";
    } while (PROCESS < 1);
    
    do {
        cout << "Enter the number of resources: ";
        fin >> RESOURCE;
        if (RESOURCE < 1)
          cout << "Please enter a valid number of resources (>1)\n";
    } while (RESOURCE < 1);
    
    int **allocation = new int *[PROCESS];//allocated resources matrix
    int **max = new int *[PROCESS];//maximum resources matrix
    int *available = new int[RESOURCE];//available resources
    int **need = new int *[PROCESS];//need matrix
    int *finish = new int[PROCESS];//finished processes
    int *sequence = new int[PROCESS];//sequence of the processes
    int seqptr = 0;//last process added to the sequence
    
    for (int i = 0; i < PROCESS; ++i) {
        allocation[i] = new int[RESOURCE];
        max[i] = new int[RESOURCE];
        need[i] = new int[RESOURCE];
        finish[i] = false;
    }
    
    
    //filling available, max, and allocation matrices
    fillMatrices(available, max, allocation, PROCESS, RESOURCE);
    
    //filling need matrix
    calculateNeed(need, max, allocation, PROCESS, RESOURCE);
    
    
    int counter = 0;
    for (int i = 0; seqptr<PROCESS; i = (i + 1) % PROCESS) {
        bool valid = true;
        counter++;
        for (int j = 0; j < RESOURCE; j++) {
            if (finish[i] || need[i][j] > available[j]){
                valid = false;
                break;
            }
        }
        if (valid) {
            finish[i] = true;
            for (int j = 0; j < RESOURCE; j++) {
                available[j] += allocation[i][j];
                fout << available[j];
            }
            sequence[seqptr++] = i;
            fout << endl;
        }
    }

    printMatrices(need, max, allocation, available, PROCESS, RESOURCE);
    
    fout << "\n\nThe seqq is <";
    for (int i = 0; i < PROCESS; i++) {
        fout << "P" << (sequence[i]) << ",";
    }
    fout << " >\n";
    
    
    
    // Cleaning up dynamic memory
    for (int i = 0; i < PROCESS; ++i) {
        delete[] allocation[i];
        delete[] max[i];
        delete[] need[i];
    }
    delete[] allocation;
    delete[] max;
    delete[] available;
    delete[] need;
    delete[] finish;
    
    return 0;
}
void fillMatrices(int* available, int** max, int** allocation, int PROCESS, int RESOURCE) {
    cout << "\nEnter the currently allocated resources to each process:\n\n";
    for (int i = 0; i < PROCESS; ++i) {
        cout << "Process " << i + 1 << ":\n";
        for (int j = 0; j < RESOURCE; ++j) {
            cout << "\t\t\tResource " << (char)(j + 65) << ": ";
            fin >> allocation[i][j];
        }
    }

    cout << "\nEnter the maximum number of resources for each process:\n";
    for (int i = 0; i < PROCESS; ++i) {
        cout << "Process " << i + 1 << ":\n";
        for (int j = 0; j < RESOURCE; ++j) {
            cout << "\t\t\tResource " << (char)(j + 65) << ": ";
            fin >> max[i][j];
        }
    }

    cout << "Enter the number of available instances of each resource:\n";
    for (int i = 0; i < RESOURCE; ++i) {
        cout << "Resource " << (char)(i + 65) << ": ";
        fin >> available[i];
    }
}
void calculateNeed(int** need, int** max, int** allocation, int PROCESS, int RESOURCE) {
    for (int i = 0; i < PROCESS; ++i) {
        for (int j = 0; j < RESOURCE; ++j) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}
void printMatrices(int** need, int** max, int** allocation, int* available, int PROCESS, int RESOURCE) {


    fout << "\nALLOCATED MATRIX:\n";
    for (int i = 0; i < PROCESS; ++i) {
        fout << "Process " << i + 1 << ": ";
        for (int j = 0; j < RESOURCE; ++j) {
            fout << allocation[i][j] << "\t";
        }
        fout << "\n";
    }
    fout << "\nMAX MATRIX:\n";
    for (int i = 0; i < PROCESS; ++i) {
        fout << "Process " << i + 1 << ": ";
        for (int j = 0; j < RESOURCE; ++j) {
            fout << max[i][j] << "\t";
        }
        fout << "\n";
    }
     fout<<"\nNEED MATRIX:\n";
     for (int i = 0; i < PROCESS; ++i) {
       fout<<"Process "<<i+1<<": ";
       for (int j = 0; j < RESOURCE; ++j) {
         fout << need[i][j]<<"\t";
       }
       fout<<"\n";
     }


}
