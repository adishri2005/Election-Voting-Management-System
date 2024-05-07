#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for a single voter
typedef struct Voter {
    int id;             // Voter ID
    struct Voter *next; // Pointer to the next voter
} Voter;

// Define a structure for a candidate
typedef struct {
    char name[100];     // Candidate name
    int votes;          // Number of votes received
    Voter *voters_head; // Pointer to the head of the linked list of voters who voted for this candidate
} Candidate;

Candidate *candidates = NULL;  // Array of candidates
int num_candidates = 0;         // Number of candidates

// Function prototypes
void add_candidate(const char *name);
int find_candidate(const char *name);
void register_vote(int voter_id, const char *candidate_name);
void display_results();
void save_data();
void load_data();
void free_memory();

int main() {
    load_data();  // Load previously saved data
    int choice;
    char name[100];
    int voter_id;

    // Main menu loop
    while (1) {
        printf("\n••••••• Election Voting Management System •••••••\n");
        printf("\n1. Add Candidate\n2. Cast Vote\n3. Display Results\n4. Save and Exit\nEnter your choice: ");
        fgets(name, sizeof(name), stdin);  // Flush stdin
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter candidate name: ");
                scanf(" %[^\n]s", name);  // Read string with spaces
                add_candidate(name);
                break;
            case 2:
                printf("Enter voter ID: ");
                scanf("%d", &voter_id);
                printf("Enter candidate name: ");
                scanf(" %[^\n]s", name);  // Read string with spaces
                register_vote(voter_id, name);
                break;
            case 3:
                display_results();
                break;
            case 4:
                save_data();
                free_memory();  // Free the allocated memory
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

// Function to add a new candidate
void add_candidate(const char *name) {
    if (find_candidate(name) != -1) {
        printf("Candidate '%s' already exists.\n", name);
        return;
    }
    // Allocate memory for a new candidate
    candidates = realloc(candidates, (num_candidates + 1) * sizeof(Candidate));
    // Initialize the new candidate
    strcpy(candidates[num_candidates].name, name);
    candidates[num_candidates].votes = 0;
    candidates[num_candidates].voters_head = NULL;
    num_candidates++;
    printf("Candidate '%s' added successfully.\n", name);
}

// Function to find a candidate by name
int find_candidate(const char *name) {
    for (int i = 0; i < num_candidates; i++) {
        if (strcmp(candidates[i].name, name) == 0) {
            return i;  // Candidate found, return index
        }
    }
    return -1;  // Candidate not found
}

// Function to register a vote
void register_vote(int voter_id, const char *candidate_name) {
    int idx = find_candidate(candidate_name);
    if (idx == -1) {
        printf("Candidate not found.\n");
        return;
    }
    // Check if the voter has already voted for this candidate
    Voter *current = candidates[idx].voters_head;
    while (current) {
        if (current->id == voter_id) {
            printf("Voter %d has already voted.\n", voter_id);
            return;
        }
        current = current->next;
    }
    // Allocate memory for a new voter
    Voter *newVoter = (Voter *)malloc(sizeof(Voter));
    newVoter->id = voter_id;
    newVoter->next = candidates[idx].voters_head;
    candidates[idx].voters_head = newVoter;
    candidates[idx].votes++;
    printf("Vote registered for %s by voter %d.\n", candidate_name, voter_id);
}

// Function to display election results
void display_results() {
    printf("\nElection Results:\n");
    for (int i = 0; i < num_candidates; i++) {
        printf("%s: %d votes\n", candidates[i].name, candidates[i].votes);
    }
}

// Function to save election data to a file
void save_data() {
    FILE *file = fopen("voting_data.txt", "w");
    if (!file) {
        printf("Failed to save data.\n");
        return;
    }
    for (int i = 0; i < num_candidates; i++) {
        fprintf(file, "%s %d\n", candidates[i].name, candidates[i].votes);
    }
    fclose(file);
    printf("Data saved successfully.\n");
}

// Function to load election data from a file
void load_data() {
    FILE *file = fopen("voting_data.txt", "r");
    if (!file) return;  // No data file, start fresh
    char name[100];
    int votes;
    // Read data from the file and add candidates
    while (fscanf(file, "%99s %d", name, &votes) == 2) {
        add_candidate(name);
        candidates[num_candidates - 1].votes = votes;  // Set votes from loaded data
    }
    fclose(file);
}

// Function to free allocated memory
void free_memory() {
    for (int i = 0; i < num_candidates; i++) {
        // Free the linked list of voters for each candidate
        Voter *current = candidates[i].voters_head;
        while (current) {
            Voter *tmp = current;
            current = current->next;
            free(tmp);
        }
    }
    free(candidates);  // Free the array of candidates
}
