#include <iostream>
#include <fstream>
using namespace std;

int PROCESS, RESOURCE;

void calculateNeed(int **need, int **max, int **allocation) {
  for (int i = 0; i < PROCESS; ++i) {
    for (int j = 0; j < RESOURCE; ++j) {
      need[i][j] = max[i][j] - allocation[i][j];
    }
  }

  // cout<<"\nNEED MATRIX:\n";
  // for (int i = 0; i < PROCESS; ++i) {
  //   cout<<"Process "<<i+1<<": ";
  //   for (int j = 0; j < RESOURCE; ++j) {
  //     cout << need[i][j]<<"\t";
  //   }
  //   cout<<"\n";
  // }
}

int main() {
  cout << "\n\nWelcome, this is an implementation of Banker’s algorithm to "
          "avoid deadlocks!\n\n";

  ofstream fout("output.txt");
  ifstream fin("input.txt");


  do {
    cout << "Enter the number of processes: ";
    cin >> PROCESS;
    if (PROCESS < 1)
      cout << "Please enter a valid number of processes (>0)\n";
  } while (PROCESS < 1);

  do {
    cout << "Enter the number of resources: ";
    cin >> RESOURCE;
    if (RESOURCE < 1)
      cout << "Please enter a valid number of resources (>1)\n";
  } while (RESOURCE < 1);

  int **allocation = new int *[PROCESS];
  int **max = new int *[PROCESS];
  int *available = new int[RESOURCE];
  int **need = new int *[PROCESS];

  for (int i = 0; i < PROCESS; ++i) {
    allocation[i] = new int[RESOURCE];
    max[i] = new int[RESOURCE];
    need[i] = new int[RESOURCE];
  }

  cout << "\nEnter the currently allocated resources to each process:\n\n";
  for (int i = 0; i < PROCESS; ++i) {
    cout << "Process " << i + 1 << ":\n";
    for (int j = 0; j < RESOURCE; ++j) {
      cout << "\t\t\tResource " << (char)(j + 65) << ": ";
      cin >> allocation[i][j];
    }
  }

  cout << "\nEnter the maximum number of resources for each process:\n";
  for (int i = 0; i < PROCESS; ++i) {
    cout << "Process " << i + 1 << ":\n";
    for (int j = 0; j < RESOURCE; ++j) {
      cout << "\t\t\tResource " << (char)(j + 65) << ": ";
      cin >> max[i][j];
    }
  }

  cout << "Enter the number of available instances of each resource:\n";
  for (int i = 0; i < RESOURCE; ++i) {
    cout << "Resource " << (char)(i + 65) << ": ";
    cin >> available[i];
  }

  calculateNeed(need, max, allocation);



  // Clean up dynamic memory
  for (int i = 0; i < PROCESS; ++i) {
    delete[] allocation[i];
    delete[] max[i];
    delete[] need[i];
  }
  delete[] allocation;
  delete[] max;
  delete[] available;
  delete[] need;

  return 0;
}
