#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Task structure
struct Task {
    int taskId;
    string taskDescription;

    Task(int id, const string& description) : taskId(id), taskDescription(description) {}
};

// Node structure for the linked list
struct Node {
    Task data;
    Node* next;

    Node(Task task) : data(task), next(nullptr) {}
};

// TaskScheduler class
class TaskScheduler {
private:
    Node* head; // Points to the head of the linked list

public:
    // Constructor
    TaskScheduler() : head(nullptr) {
        // Load tasks from file when the scheduler is created
        loadTasksFromFile();
    }

    // Destructor
    ~TaskScheduler() {
        // Save tasks to file before the scheduler is destroyed
        saveTasksToFile();
        // Release allocated memory when the scheduler is destroyed
        clear();
    }

    // Function to insert a task at the rear of the list
    void insertTask(const Task& task) {
        Node* newNode = new Node(task);

        if (!head) {
            // If the list is empty, set the new node as the head
            head = newNode;
        } else {
            // Traverse to the end of the list and insert the new node
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }

        // Save tasks to file after insertion
        saveTasksToFile();
    }

    // Function to delete a task from any position in the list
    void deleteTask(int taskId) {
        if (!head) {
            cout << "Task scheduler is empty.\n";
            return;
        }

        Node* current = head;
        Node* prev = nullptr;

        // Traverse the list to find the task with the specified ID
        while (current && current->data.taskId != taskId) {
            prev = current;
            current = current->next;
        }

        if (!current) {
            cout << "Task with ID " << taskId << " not found.\n";
            return;
        }

        // Remove the node from the list
        if (!prev) {
            // If the task to be deleted is at the head
            head = current->next;
        } else {
            prev->next = current->next;
        }

        delete current; // Free the memory of the deleted node
        
        cout << "Task ID " << taskId << " has been deleted.\n";

        // Save tasks to file after deletion
        saveTasksToFile();
    }

    // Function to display all tasks in the scheduler
    void displayTasks() {
        if (!head) {
            cout << "Task scheduler is empty.\n";
            return;
        }

        Node* current = head;
        while (current) {
            cout << "Task ID: " << current->data.taskId << ", Description: " << current->data.taskDescription << "\n";
            current = current->next;
        }
    }

    // Function to clear the entire scheduler
    void clear() {
        Node* current = head;
        Node* nextNode;

        while (current) {
            nextNode = current->next;
            delete current; // Free the memory of each node
            current = nextNode;
        }

        head = nullptr; // Set head to null after clearing the list
    }

    // Function to load tasks from a file
    void loadTasksFromFile() {
        ifstream file("tasks.txt");
        if (file.is_open()) {
            int taskId;
            string taskDescription;

            while (file >> taskId) {
                file.ignore(); // Ignore the space after taskId
                getline(file, taskDescription);

                insertTask(Task(taskId, taskDescription));
            }

            file.close();
        }
    }

    // Function to save tasks to a file
    void saveTasksToFile() {
        ofstream file("tasks.txt");
        if (file.is_open()) {
            Node* current = head;

            while (current) {
                file << current->data.taskId << " " << current->data.taskDescription << "\n";
                current = current->next;
            }

            file.close();
        }
    }
};

int main() {
    TaskScheduler scheduler;

    // Menu for user input
    int choice;
    do {
        cout << "\nTask Scheduler Menu:\n";
        cout << "1. Insert Task\n";
        cout << "2. Delete Task\n";
        cout << "3. Display Tasks\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int taskId;
                string taskDescription;
                cout << "Enter Task ID: ";
                cin >> taskId;
                cin.ignore(); // Clear the input buffer
                cout << "Enter Task Description: ";
                getline(cin, taskDescription);

                scheduler.insertTask(Task(taskId, taskDescription));
                break;
            }
            case 2: {
                int taskId;
                cout << "Enter Task ID to delete: ";
                cin >> taskId;
                scheduler.deleteTask(taskId);
                break;
            }
            case 3:
                cout << "\nTasks in the scheduler:\n";
                scheduler.displayTasks();
                break;
            case 4:
                cout << "Exiting the program.\n\n";
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
        }
    } while (choice != 4);

    return 0;
}