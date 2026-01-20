#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// --- Constants ---
char *SUBJECTS[] = {"CS101", "CH101", "M101", "HU103", "HU105"};
const int FIXED_CLASSES = 10; // 10 classes are fixed per subject
const int TOTAL_SUBJECTS = 5;

// --- Data Structure ---
struct Student {
    int rollNo;         
    char fname[20];
    char mname[20];
    char lname[20];
    char stream[20];
    
    int subTotal[5];    // This will always remain 10
    int subAttended[5]; // User will increment this (max up to 10)
};

// Global Variables
struct Student batch[50];
int count = 0;
const char *FILENAME = "attendance_db.txt";

// --- Prototypes ---
void loadData();
void saveData();
void addStudent();
void markAttendance();
void viewAttendance();
void checkEligibility();
void printFullName(int index);

// --- Main Program ---
int main() {
    int choice;
    loadData(); 

    while(1) {
        printf("\n=== Student Attendance System (Fixed 10 Classes Logic) ===\n");
        printf("1. Add New Student\n");
        printf("2. Mark Attendance\n");
        printf("3. View Attendance Records\n");
        printf("4. Check Exam Eligibility\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addStudent(); break;
            case 2: markAttendance(); break;
            case 3: viewAttendance(); break;
            case 4: checkEligibility(); break;
            case 5: saveData(); printf("Exiting... Bye!\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

// --- Helper Functions ---
void printFullName(int i) {
    printf("%s ", batch[i].fname);
    if (strcmp(batch[i].mname, ".") != 0) {
        printf("%s ", batch[i].mname);
    }
    printf("%s", batch[i].lname);
}

// --- File Handling ---
void saveData() {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) return;
    
    fprintf(fp, "%d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %s %s %s ", 
                batch[i].rollNo, batch[i].fname, batch[i].mname, batch[i].lname, batch[i].stream);
        
        for(int j=0; j<5; j++) fprintf(fp, "%d ", batch[i].subTotal[j]);
        for(int j=0; j<5; j++) fprintf(fp, "%d ", batch[i].subAttended[j]);
        
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void loadData() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Note: No previous database found. Starting fresh.\n");
        return;
    }
    fscanf(fp, "%d", &count);
    for (int i = 0; i < count; i++) {
        fscanf(fp, "%d %s %s %s %s", 
               &batch[i].rollNo, batch[i].fname, batch[i].mname, &batch[i].lname, batch[i].stream);
        
        for(int j=0; j<5; j++) fscanf(fp, "%d", &batch[i].subTotal[j]);
        for(int j=0; j<5; j++) fscanf(fp, "%d", &batch[i].subAttended[j]);
    }
    fclose(fp);
}

// --- Features ---

void addStudent() {
    int streamChoice;
    if (count >= 50) { printf("Batch Full!\n"); return; }
    
    printf("\n--- Add New Student ---\n");

    printf("Choose Stream:\n1. BTech CSE1\n2. BTech CSE2\n3. BTech IT1\n4. BTech IT2\nSelect: ");
    scanf("%d", &streamChoice);

    switch(streamChoice) {
        case 1: strcpy(batch[count].stream, "BTech-CSE1"); break;
        case 2: strcpy(batch[count].stream, "BTech-CSE2"); break;
        case 3: strcpy(batch[count].stream, "BTech-IT1"); break;
        case 4: strcpy(batch[count].stream, "BTech-IT2"); break;
        default: strcpy(batch[count].stream, "General"); break;
    }

    printf("Enter First Name: "); scanf("%s", batch[count].fname);
    printf("Enter Middle Name ('.' if none): "); scanf("%s", batch[count].mname);
    printf("Enter Last Name: "); scanf("%s", batch[count].lname);

    batch[count].rollNo = 100 + (count + 1); 
    
    // --- CHANGE 1: Default Total Classes = 10 ---
    for(int j=0; j<5; j++) {
        batch[count].subTotal[j] = FIXED_CLASSES; // This will be set to 10, not 0
        batch[count].subAttended[j] = 0;
    }
    
    printf("Student Registered! Assigned Roll No: %d\n", batch[count].rollNo);
    count++;
    saveData();
}

void markAttendance() {
    int rno, i, found = 0, subChoice;
    printf("\n--- Mark Attendance (Total 10 Classes per Subject) ---\n");
    printf("Enter Roll No: ");
    scanf("%d", &rno);
    
    for (i = 0; i < count; i++) {
        if (batch[i].rollNo == rno) {
            found = 1;
            printf("Found: "); printFullName(i); printf("\n");

            printf("Select Subject:\n1. CS101\n2. CH101\n3. M101\n4. HU103\n5. HU105\nChoice: ");
            scanf("%d", &subChoice);

            if (subChoice >= 1 && subChoice <= 5) {
                int index = subChoice - 1;
                
                // --- CHANGE 2: Check limit before marking ---
                // Since max classes are 10, attendance cannot exceed that
                if (batch[i].subAttended[index] < FIXED_CLASSES) {
                    batch[i].subAttended[index]++; 
                    // NOTE: subTotal is NOT increased, it is fixed (10).
                    
                    printf("Attendance Marked for %s! (%d/10)\n", SUBJECTS[index], batch[i].subAttended[index]);
                    saveData();
                } else {
                    printf("Error: Attendance Full (10/10) for this subject! Cannot mark more.\n");
                }
            } else {
                printf("Invalid Subject Choice!\n");
            }
            break;
        }
    }
    if (!found) printf("Student not found!\n");
}

void viewAttendance() {
    int i, j;
    int grandTotal = TOTAL_SUBJECTS * FIXED_CLASSES; // 5 * 10 = 50 Fixed
    int grandAttended;
    float percentage;
    
    printf("\n--- Attendance Records (Out of 50 Classes) ---\n");
    printf("Roll\tName\t\t\tStream\t\tAttended/50\t%%\n"); 
    printf("----------------------------------------------------------------------------\n");
    
    for (i = 0; i < count; i++) {
        grandAttended = 0;

        // Sum only the attended classes
        for(j=0; j<5; j++) {
            grandAttended += batch[i].subAttended[j];
        }

        // --- CHANGE 3: Logic Update ---
        // Formula: (Total Attended / 50) * 100
        percentage = ((float)grandAttended / grandTotal) * 100;
        
        printf("%d\t", batch[i].rollNo);
        printFullName(i);
        
        if (strlen(batch[i].fname) + strlen(batch[i].lname) < 8) printf("\t\t\t");
        else printf("\t\t");

        printf("%s", batch[i].stream);
        if (strlen(batch[i].stream) < 8) printf("\t\t");
        else printf("\t");

        printf("%d/%d\t\t%.1f%%\n", grandAttended, grandTotal, percentage);
    }
}

void checkEligibility() {
    int i, j, grandAttended;
    int grandTotal = 50; // Fixed
    float percentage;
    
    printf("\n--- Exam Eligibility (>75%% of 50 Classes) ---\n");
    printf("Roll\tName\t\t\tStatus\t\tScore\n");
    printf("------------------------------------------------------\n");
    
    for (i = 0; i < count; i++) {
        grandAttended = 0;
        for(j=0; j<5; j++) grandAttended += batch[i].subAttended[j];

        percentage = ((float)grandAttended / grandTotal) * 100;
        
        printf("%d\t", batch[i].rollNo);
        printFullName(i);
        
        if (strlen(batch[i].fname) + strlen(batch[i].lname) < 8) printf("\t\t\t");
        else printf("\t\t");
        
        if (percentage >= 75.0) printf("ALLOWED \t");
        else printf("NOT ALLOWED \t");

        printf("[%.1f%%]", percentage);
        printf("\n");
    }
}