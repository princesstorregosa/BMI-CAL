#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <jansson.h>
#include <string.h>

int main() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    char createTableQuery[] = "CREATE TABLE IF NOT EXISTS bmi_data (age INTEGER, height INTEGER, weight INTEGER, gender TEXT, bmi REAL);";
    rc = sqlite3_exec(db, createTableQuery, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    int age, height, weight;
    char gender;

    printf("Enter age: ");
    scanf("%d", &age);

    printf("Enter height (in cm): ");
    scanf("%d", &height);

    printf("Enter weight (in kg): ");
    scanf("%d", &weight);

    printf("Enter gender (M/F): ");
    scanf(" %c", &gender);

    // Validating input
    if (age <= 0 || height <= 0 || weight <= 0 || (gender != 'M' && gender != 'F')) {
        printf("All fields are required!\n");
    } else {
        // Calculating BMI
        float bmi = weight / ((height / 100.0) * (height / 100.0));
        printf("BMI: %.2f\n", bmi);

        // Determining BMI category
        char result[20];
        if (bmi < 18.5) {
            strcpy(result, "Underweight");
        } else if (bmi >= 18.5 && bmi <= 24.9) {
            strcpy(result, "Healthy");
        } else if (bmi >= 25 && bmi <= 29.9) {
            strcpy(result, "Overweight");
        } else if (bmi >= 30 && bmi <= 34.9) {
            strcpy(result, "Obese");
        } else if (bmi >= 35) {
            strcpy(result, "Extremely obese");
        }

        printf("Result: %s\n", result);

        // Inserting data into the database
        char insertQuery[100];
        sprintf(insertQuery, "INSERT INTO bmi_data (age, height, weight, gender, bmi) VALUES (%d, %d, %d, '%c', %.2f);",
                age, height, weight, gender, bmi);
        rc = sqlite3_exec(db, insertQuery, 0, 0, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to insert data: %s\n", sqlite3_errmsg(db));
        }
    }

    sqlite3_close(db);

    return 0;
}

