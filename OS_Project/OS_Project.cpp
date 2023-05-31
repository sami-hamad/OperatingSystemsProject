//Sami Mohammed Hamad - 20200880
#include <iostream>
#include <fstream>
using namespace std;

//ofstream fout("output.txt");
ifstream fin("input.txt");

void fillMatrices(int* available, int** max, int** allocation);
void calculateNeed(int** need, int** max, int** allocation);
void printMatrices(int** need, int** max, int** allocation, int* available);
bool isSafeState(int** need, int** allocation, int* available, bool* finish, int *sequence);
bool partialSequence(const bool* finish);
void printSequence(const int* sequence, const bool* finish);

int PROCESS, RESOURCE;

int main() {
    cout << "\n\nWelcome, this is an implementation of Banker's algorithm to avoid deadlocks!\n\n";
    
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
    bool *finish = new bool[PROCESS];//finished processes
    int *sequence = new int[PROCESS];//sequence of the processes
    int seqptr = 0;//last process added to the sequence
    
    for (int i = 0; i < PROCESS; ++i) {
        allocation[i] = new int[RESOURCE];
        max[i] = new int[RESOURCE];
        need[i] = new int[RESOURCE];
        finish[i] = false;
        sequence[i] = -1;//this is useful when there's partial sequences (to know where to stop when printing the sequence) 
                         //because the sequence matrix is partially filled
    }
    
    
    //filling available, max, and allocation matrices from the user
    fillMatrices(available, max, allocation);
    
    //calculating need matrix
    calculateNeed(need, max, allocation);
    
    //printing all matrices
    printMatrices(need, max, allocation, available);

    //checking for safe state
    bool safe = isSafeState(need, allocation, available, finish, sequence);


    //if the system is in a safe state
    if (safe) {
        cout << "\n\nThe System is in a safe state, and the sequence is ";
        printSequence(sequence, finish);
    }
    else {//system is not safe
        if(!partialSequence(finish))//if there's a partial sequence or not
            cout << "\n\nThe system is in an unsafe, and there's no partial sequence.";
        else {
            cout << "\n\nThe system is in an unsafe state, but the partial sequence is ";
            printSequence(sequence, finish);
        }
    }
    
    
    
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
void fillMatrices(int* available, int** max, int** allocation) {
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
void calculateNeed(int** need, int** max, int** allocation) {
    for (int i = 0; i < PROCESS; ++i) {
        for (int j = 0; j < RESOURCE; ++j) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}
void printMatrices(int** need, int** max, int** allocation, int* available) {
    cout << "\nALLOCATED MATRIX:\n";
    for (int i = 0; i < PROCESS; ++i) {
        cout << "P" << i << " : ";
        for (int j = 0; j < RESOURCE; ++j) {
            cout << allocation[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\nMAX MATRIX:\n";
    for (int i = 0; i < PROCESS; ++i) {
        cout << "P" << i << " : ";
        for (int j = 0; j < RESOURCE; ++j) {
            cout << max[i][j] << " ";
        }
        cout << "\n";
    }

    cout<<"\nNEED MATRIX:\n";
    for (int i = 0; i < PROCESS; ++i) {
        cout << "P" << i << " : ";
       for (int j = 0; j < RESOURCE; ++j) {
           cout << need[i][j] << " ";
       }
       cout<<"\n";
    }
}

bool isSafeState(int** need, int** allocation,int* available, bool* finish,int *sequence) {

    int* work = new int[RESOURCE];
    cout << "\n\nWORK:" << endl;
    for (int w = 0; w < RESOURCE; w++) {
        work[w] = available[w];
        cout << work[w] << " ";
    }
    cout << endl;

    int counter = 0;
    while (counter<PROCESS) {//if all processes where executed successfully (all finish[] = true)

        bool O = false;//flag to indicate if there's no any valid processes through one iteration 

        for (int i = 0; i < PROCESS; i++) {
            if (!finish[i]) {
                bool needlessthanwork = true;
                for (int j = 0; j < RESOURCE; j++) {
                    if (need[i][j] > work[j]) {
                        needlessthanwork = false;
                        break;
                    }
                }
                if (needlessthanwork) {//if the need resources of a process is less than the work resources
                    sequence[counter] = i;
                    counter++;
                    finish[i] = true;
                    O = true;
                    for (int q = 0; q < RESOURCE; q++) {
                        work[q] += allocation[i][q];
                        cout << work[q] << " ";
                    }
                    cout << endl;
                }
            }
        }
        if (!O) {//there were no any executed process after one whole scan
            delete[]work;
            return false;
        }
    }

    //all the process where successfully executed (safe state system)
    delete[]work;
    return true;

}
bool partialSequence(const bool* finish) {
    for (int i = 0; i < PROCESS; i++) {
        if (finish[i]) return true;
    }
    return false;
}
void printSequence(const int* sequence, const bool* finish) {
    cout << "< ";
    for (int i = 0; i < PROCESS; i++) {
        if (sequence[i]>-1) {
            cout << "P" << sequence[i]<< " ";
        }
    }
    cout << ">\n";
}