#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ---------- CONSTANTS ---------- */
#define MAX_STUDENTS 50
#define TOTAL_SUBJECTS 5
#define FIXED_CLASSES 10

const char *SUBJECTS[TOTAL_SUBJECTS] = {
    "CS101", "CH101", "M101", "HU103", "HU105"
};

const char *FILENAME = "attendance_db.txt";

/* ---------- STRUCTURE ---------- */
struct Student {
    int rollNo;
    char fname[20];
    char mname[20];
    char lname[20];
    char stream[20];
    int subAttended[TOTAL_SUBJECTS];
};

/* ---------- GLOBALS ---------- */
struct Student batch[MAX_STUDENTS];
int count = 0;

/* ---------- PROTOTYPES ---------- */
void loadData();
void saveData();
void addStudent();
void markAttendance();
void viewAttendance();
void checkEligibility();
void printFullName(int i);
int getTotalAttended(int i);

/* ---------- MAIN ---------- */
int main() {
    int choice;
    loadData();

    while (1) {
        printf("\n=== Student Attendance System (Fixed 10 Classes) ===\n");
        printf("1. Add New Student\n");
        printf("2. Mark Attendance\n");
        printf("3. View Attendance Records\n");
        printf("4. Check Exam Eligibility\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: markAttendance(); break;
            case 3: viewAttendance(); break;
            case 4: checkEligibility(); break;
            case 5:
                saveData();
                printf("Exiting... Bye!\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
}

/* ---------- HELPERS ---------- */
void printFullName(int i) {
    printf("%s ", batch[i].fname);
    if (strcmp(batch[i].mname, ".") != 0)
        printf("%s ", batch[i].mname);
    printf("%s", batch[i].lname);
}

int getTotalAttended(int i) {
    int sum = 0;
    for (int j = 0; j < TOTAL_SUBJECTS; j++)
        sum += batch[i].subAttended[j];
    return sum;
}

/* ---------- FILE HANDLING ---------- */
void saveData() {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("Error: Unable to save data!\n");
        return;
    }

    fprintf(fp, "%d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %s %s %s ",
                batch[i].rollNo,
                batch[i].fname,
                batch[i].mname,
                batch[i].lname,
                batch[i].stream);

        for (int j = 0; j < TOTAL_SUBJECTS; j++)
            fprintf(fp, "%d ", batch[i].subAttended[j]);

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
        fscanf(fp, "%d %19s %19s %19s %19s",
               &batch[i].rollNo,
               batch[i].fname,
               batch[i].mname,
               batch[i].lname,
               batch[i].stream);

        for (int j = 0; j < TOTAL_SUBJECTS; j++)
            fscanf(fp, "%d", &batch[i].subAttended[j]);
    }
    fclose(fp);
}

/* ---------- FEATURES ---------- */
void addStudent() {
    int streamChoice;
    char confirm;

    if (count >= MAX_STUDENTS) {
        printf("Batch Full!\n");
        return;
    }

    printf("\n--- Add New Student ---\n");

    printf("Choose Stream:\n");
    printf("1. BTech CSE1\n");
    printf("2. BTech CSE2\n");
    printf("3. BTech IT1\n");
    printf("4. BTech IT2\n");
    printf("Select: ");
    scanf("%d", &streamChoice);

    switch (streamChoice) {
        case 1: strcpy(batch[count].stream, "BTech-CSE1"); break;
        case 2: strcpy(batch[count].stream, "BTech-CSE2"); break;
        case 3: strcpy(batch[count].stream, "BTech-IT1"); break;
        case 4: strcpy(batch[count].stream, "BTech-IT2"); break;
        default: strcpy(batch[count].stream, "General");
    }

    printf("Enter First Name: ");
    scanf("%19s", batch[count].fname);

    printf("Enter Middle Name ('.' if none): ");
    scanf("%19s", batch[count].mname);

    printf("Enter Last Name: ");
    scanf("%19s", batch[count].lname);

    batch[count].rollNo = 100 + count + 1;

    for (int j = 0; j < TOTAL_SUBJECTS; j++)
        batch[count].subAttended[j] = 0;

    /* ---------- SUMMARY ---------- */
    printf("\n--- Student Summary ---\n");
    printf("Name   : ");
    printFullName(count);
    printf("\nStream : %s\n", batch[count].stream);
    printf("Roll   : %d\n", batch[count].rollNo);

    printf("\nPress C & Enter key to confirm registration : ");
    scanf(" %c", &confirm);

    if (confirm == 'C' || confirm == 'c') {
        count++;
        saveData();
        printf("\nStudent Registered Successfully!\n");
    } else {
        printf("\nRegistration Cancelled.\n");
    }
}

void markAttendance() {
    int rno, subChoice;

    printf("\n--- Mark Attendance ---\n");
    printf("Enter Roll No: ");
    scanf("%d", &rno);

    for (int i = 0; i < count; i++) {
        if (batch[i].rollNo == rno) {
            printf("Student Found: ");
            printFullName(i);
            printf("\n");

            printf("Select Subject:\n");
            for (int s = 0; s < TOTAL_SUBJECTS; s++)
                printf("%d. %s\n", s + 1, SUBJECTS[s]);

            printf("Choice: ");
            scanf("%d", &subChoice);

            if (subChoice < 1 || subChoice > TOTAL_SUBJECTS) {
                printf("Invalid subject choice!\n");
                return;
            }

            int idx = subChoice - 1;

            if (batch[i].subAttended[idx] < FIXED_CLASSES) {
                batch[i].subAttended[idx]++;
                printf("Attendance marked for %s (%d/%d)\n",
                       SUBJECTS[idx],
                       batch[i].subAttended[idx],
                       FIXED_CLASSES);
                saveData();
            } else {
                printf("Attendance already full (10/10) for this subject!\n");
            }
            return;
        }
    }
    printf("Student not found!\n");
}

void viewAttendance() {
    int total = TOTAL_SUBJECTS * FIXED_CLASSES;

    printf("\n--- Attendance Records (Out of %d Classes) ---\n", total);
    printf("Roll   Name                    Stream        Att/Total   %%\n");
    printf("-----------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        int att = getTotalAttended(i);
        float per = (att * 100.0f) / total;

        printf("%-6d ", batch[i].rollNo);
        printFullName(i);
        printf("   %-12s   %2d/%2d     %5.1f%%\n",
               batch[i].stream, att, total, per);
    }
}

void checkEligibility() {
    int total = TOTAL_SUBJECTS * FIXED_CLASSES;

    printf("\n--- Exam Eligibility (Minimum 75%%) ---\n");
    printf("Roll   Name                    Status        %%\n");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        int att = getTotalAttended(i);
        float per = (att * 100.0f) / total;

        printf("%-6d ", batch[i].rollNo);
        printFullName(i);

        if (per >= 75.0)
            printf("   ALLOWED     ");
        else
            printf("   NOT ALLOWED ");

        printf("%5.1f%%\n", per);
    }
}
