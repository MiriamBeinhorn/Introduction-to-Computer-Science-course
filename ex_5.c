/**********
 Name: Miriam Beinhorn
 ID: 212340442
 Assignment: 5
**********/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ex_5.h"

int main() {
    Worker *workersArr[MAX_WORKERS];
    Project *projectsArr[MAX_PROJECTS];
    int workersCount = 0, projectsCount = 0;
    int exit = 0, optionChoice;
    while (!exit) {
        optionChoice = menu();
        switch (optionChoice) {
            case JOIN_COMPANY: {
                joinCompany(workersArr, &workersCount);
                break;
            }
            case OPEN_PROJECT: {
                openNewProject(workersArr, workersCount, projectsArr, &projectsCount);
                break;
            }
            case JOIN_PROJECT: {
                joinProject(workersArr, workersCount, projectsArr, projectsCount);
                break;
            }
            case SEE_PROJECTS_IN_WORK: {
                displayAllProjects(projectsArr, projectsCount);
                break;
            }
            case WHICH_PEOPLE_IN_WHAT_PROJECTS: {
                displayAllWorkers(workersArr, workersCount);
                break;
            }
            case WORK_ON_PROJECT: {
                workOnProject(workersArr, workersCount, projectsCount);
                break;
            }
            case LEAVE_COMPANY: {
                leaveCompany(workersArr, &workersCount, projectsArr, &projectsCount);
                break;
            }
            case EXIT_PROGRAM: {
                freeAllAllocations(workersArr, workersCount,
                                   projectsArr, projectsCount);
                exit = 1;
                break;
            }
            default: {
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    }
    return 0;
}

int menu() { //printing the menu and get the user's choice
    int option;
    printf("Welcome!\nPlease select the desired option:\n");
    printf("1. Join the company\n");
    printf("2. Open a new project\n");
    printf("3. Join an existing project\n");
    printf("4. See what projects are in work\n");
    printf("5. See which people are in what projects\n");
    printf("6. Work on existing project\n");
    printf("7. Leave the company\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &option);
    return option;
}

void cleanBuffer() { //cleaning the buffer
    scanf("%*[^\n]");
    scanf("%*c");
}

void didAllocationFail(void *ptr) { //check  if dynamic allocation failed
    if (ptr == NULL) {
        printf("ALLOCATION FAILED");
        exit(1);
    }
}

char *getString() { //get a string from the user
    int count = 1; //we get at least 1 char
    char *ptr = (char *) malloc(sizeof(char));
    didAllocationFail(ptr);
    char *temp;
    scanf("%c", ptr);//get the first char from the user
    if (*ptr == '\n') {
        free(ptr);
        return NULL;
    }
    temp = ptr;
    while (*temp != '\n') { //get chars until the user press 'ENTER'
        count++;
        ptr = (char *) realloc(ptr, (count) * sizeof(char));
        didAllocationFail(ptr);
        //realloc might change the location of the string, so we go back from the start:
        temp = ptr + ((count - 1) * sizeof(char));
        scanf("%c", temp);
    }
    *temp = '\0'; //change the last char from '\n' to '\0'
    return ptr;
}

//make sure there isn't already a worker with this name in the company:
int checkWorkerName(Worker *workersArr[], char *name, int workersCount) {
    if (workersCount == 0)
        return 0;
    for (int i = 0; i < workersCount; i++) {
        if (strcmp(name, workersArr[i]->name) == 0) {
            printf("A worker with this name already exists.\n"
                   "There is not enough space in this company for both of us\n");
            return 1;
        }
    }
    return 0;
}

//make sure there isn't already a project with this name in the company:
int checkProjectName(Project *projectsArr[], char *name, int projectsCount) {
    if (projectsCount == 0)
        return 0;
    for (int i = 0; i < projectsCount; i++) {
        if (strcmp(name, projectsArr[i]->name) == 0) {
            printf("A project with this name already exists.\nBetter be more creative\n");
            return 1;
        }
    }
    return 0;
}

//check if the worker is already part of the project:
int isWorkerInProject(Project *project, char *workerName) {
    for (int i = 0; i < project->workerCount; ++i) {
        if (strcmp(workerName, project->workers[i]->name) == 0) {
            printf("This worker is already part of the project '%s'.\n", project->name);
            return 1;
        }
    }
    return 0;
}

int isNumInRange(int choice, int maxChoice) { //make sure a number is between 1 to a specific maximum
    if ((choice <= 0) || (choice > maxChoice))
        return 0;
    return 1;
}

void joinCompany(Worker *workersArr[], int *workersCount) {
    if (*workersCount == MAX_WORKERS) {
        printf("There is already maximum amount of workers in the company\n");
        return;
    }
    printf("Enter the name of the new worker: ");
    cleanBuffer();
    char *name = getString();
    if (name == NULL) {
        printf("Invalid name\n");
        return;
    }
    if (checkWorkerName(workersArr, name, *workersCount)) { //if a worker with this name already exists
        free(name);
        return;
    }
    workersArr[*workersCount] = malloc(sizeof(Worker)); //make place for the new worker
    didAllocationFail(workersArr[*workersCount]);
    workersArr[*workersCount]->name = name;
    workersArr[*workersCount]->projectCount = 0; //all workers begin with 0 projects
    workersArr[*workersCount]->projects = NULL; //all workers start without projects
    (*workersCount)++;
}

void openNewProject(Worker *workersArr[], int workersCount, Project *projectsArr[], int *projectsCount) {
    const int WORKERS_NUM = 4; //maximum amount of workers on the project
    if (!workersCount) { //in order to open a project, you must have workers
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }
    if (*projectsCount == MAX_PROJECTS) {
        printf("There is already a maximum amount of projects in the company\n");
        return;
    }
    int chosenWorker;
    char *projectName;
    printf("Who are you? Choose a number:\n");
    for (int i = 0; i < workersCount; i++) {
        printf("%d. %s\n", i + 1, workersArr[i]->name);
    }
    printf("Enter your choice: ");
    scanf("%d", &chosenWorker);
    if (!isNumInRange(chosenWorker, workersCount)) {
        printf("Invalid choice\n");
        return;
    }
    chosenWorker--; //get the 'spot' of this worker in the workers array
    printf("Enter the name of the new project: ");
    cleanBuffer();
    projectName = getString();
    if (projectName == NULL) {
        printf("Invalid name\n");
        return;
    }
    //check if there already is project with this name:
    if (checkProjectName(projectsArr, projectName, *projectsCount)) {
        free(projectName);
        return;
    }
    projectsArr[*projectsCount] = malloc(sizeof(Project)); //make place for the new project
    didAllocationFail(projectsArr[*projectsCount]);
    projectsArr[*projectsCount]->name = projectName;
    projectsArr[*projectsCount]->workerCount = 1; //Every project begins with 1 worker
    projectsArr[*projectsCount]->features = NULL;
    //every project have an array of maximum 4 workers:
    projectsArr[*projectsCount]->workers = malloc(sizeof(Worker *) * WORKERS_NUM);
    didAllocationFail(projectsArr[*projectsCount]->workers);
    projectsArr[*projectsCount]->workers[0] = workersArr[chosenWorker]; //this is the first worker on the project
    workersArr[chosenWorker]->projects = realloc(workersArr[chosenWorker]->projects,
                                                 (workersArr[chosenWorker]->projectCount+1)*sizeof(Project*));
    didAllocationFail(workersArr[chosenWorker]->projects);
    workersArr[chosenWorker]->projects[workersArr[chosenWorker]->projectCount] = projectsArr[*projectsCount];
    workersArr[chosenWorker]->projectCount++;
    (*projectsCount)++;
}
void joinProject(Worker *workersArr[], int workersCount, Project *projectsArr[], int projectsCount) {
    const int MAX_WORKERS_IN_PROJECT = 4;
    if (!workersCount) { //in this option, must have workers in the company
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }
    if (!projectsCount) { //in this option, must have projects in the company
        printf("There are no projects in the company yet, please create one first.\n");
        return;
    }
    int chosenWorker, chosenProject;
    printf("Select a worker to join a project:\n");
    for (int i = 0; i < workersCount; i++) {
        printf("%d. %s\n", i + 1, workersArr[i]->name);
    }
    printf("Enter the worker's number: ");
    scanf("%d", &chosenWorker);
    if (!isNumInRange(chosenWorker, workersCount)) {
        printf("Invalid worker selection6\n");
        return;
    }
    chosenWorker--; //get the workers 'spot'
    printf("Select a project to join:\n");
    for (int i = 0; i < projectsCount; i++) {
        printf("%d. %s (Workers: %d)\n", i + 1, projectsArr[i]->name, projectsArr[i]->workerCount);
    }
    printf("Enter the project's number: ");
    scanf("%d", &chosenProject);
    if (!isNumInRange(chosenProject, projectsCount)) {
        printf("Invalid project selection\n");
        return;
    }
    chosenProject--; //get the workers 'spot'
    //if the worker is already in the project:
    if (isWorkerInProject(projectsArr[chosenProject], workersArr[chosenWorker]->name)) {
        return;
    }
    //you can't have more than 4 workers on a project:
    if (projectsArr[chosenProject]->workerCount == MAX_WORKERS_IN_PROJECT) {
        printf("Project is full\n");
        return;
    }
    projectsArr[chosenProject]->workers[projectsArr[chosenProject]->workerCount] = workersArr[chosenWorker];
    projectsArr[chosenProject]->workerCount++;
    workersArr[chosenWorker]->projects = realloc(workersArr[chosenWorker]->projects,
                                                 (workersArr[chosenWorker]->projectCount+1)*sizeof(Project*));
    didAllocationFail(workersArr[chosenWorker]->projects);
    workersArr[chosenWorker]->projects[workersArr[chosenWorker]->projectCount] = projectsArr[chosenProject];
    workersArr[chosenWorker]->projectCount++;
}

void displayAllProjects(Project *projectsArr[], int projectsCount) {
    if (!projectsCount) { //if there are no projects
        printf("There are no projects to display.\n");
        return;
    }
    FeatureNode *currentFeature;
    for (int i = 0; i < projectsCount; ++i) { //go over all projects
        printf("Project: %s\n", projectsArr[i]->name);
        printf("  Workers in this project:\n");
        for (int j = 0; j < projectsArr[i]->workerCount; ++j) {
            printf("    - %s\n", projectsArr[i]->workers[j]->name);
        }
        printf("  Features of the project:\n");
        if (projectsArr[i]->features == NULL) { //if there are no features in the project
            printf("    No features\n");
        } else { //if there are features to print
            currentFeature = projectsArr[i]->features;
            while (currentFeature != NULL) {
                printf("  - %s\n", currentFeature->feature);
                currentFeature = currentFeature->next;
            }
        }
        printf("\n");
    }
}

void displayAllWorkers(Worker *workersArr[], int workersCount) {
    if (!workersCount) { //if there are no workers
        printf("There are no workers to display.\n");
        return;
    }
    for (int i = 0; i < workersCount; ++i) { //go over all workers
        printf("Worker: %s\n", workersArr[i]->name);
        if (!workersArr[i]->projectCount) { //if the worker don't have projects
            printf("  Not involved in any projects.\n");
        } else { //if the worker has projects
            printf("  Projects this worker is involved in:\n");
            for (int j = 0; j < workersArr[i]->projectCount; ++j) {
                printf("    - %s\n", workersArr[i]->projects[j]->name);
            }
        }
        printf("\n");
    }
}

void workOnProject(Worker *workersArr[], int workersCount, int projectsCount) {
    if (!workersCount) { //in this option, must have workers in the company
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }
    if (!projectsCount) { //in this option, must have projects in the company
        printf("There are no projects in the company yet, please create a project first.\n");
        return;
    }
    int chosenWorker, chosenProject;
    char *addOrRemove;
    printf("Select a worker:\n");
    for (int i = 0; i < workersCount; ++i) {
        printf("%d. %s\n", i + 1, workersArr[i]->name);
    }
    printf("Enter the worker's number: ");
    scanf("%d", &chosenWorker);
    if (!isNumInRange(chosenWorker, workersCount)) {
        printf("Invalid choice\n");
        return;
    }
    chosenWorker--; //get the workers 'spot' in the workers array
    if (!workersArr[chosenWorker]->projectCount) { //if the worker has no projects
        printf("This worker is not involved in any projects.\n");
        return;
    }
    printf("Select a project:\n");
    for (int i = 0; i < workersArr[chosenWorker]->projectCount; ++i) { //print the projects of the worker
        printf("%d. %s\n", i + 1, workersArr[chosenWorker]->projects[i]->name);
    }
    printf("Enter the project number: ");
    scanf("%d", &chosenProject);
    if (!isNumInRange(chosenProject, workersArr[chosenWorker]->projectCount)) {
        printf("Invalid project selection\n");
        return;
    }
    chosenProject--;
    printf("Do you want to add or remove a feature? (add/remove): ");
    cleanBuffer();
    addOrRemove = getString();
    if (strcmp(addOrRemove, "add") == 0) { //if the user chose 'add'
        free(addOrRemove);
        addFeatureToProject(workersArr[chosenWorker]->projects[chosenProject]);
    } else if (strcmp(addOrRemove, "remove") == 0) { //if the user chose 'remove'
        free(addOrRemove);
        removeFeatureFromProject(workersArr[chosenWorker]->projects[chosenProject]);
    } else { //if the user chose something other than 'add' or 'remove'
        free(addOrRemove);
        printf("Invalid choice\n");
    }
}

void addFeatureToProject(Project *project) {
    char *featureName;
    printf("Enter the new feature: ");
    featureName = getString();
    if (project->features == NULL) { //if there are no features in the project yet
        project->features = malloc(sizeof(FeatureNode));
        didAllocationFail(project->features);
        project->features->feature = featureName;
        project->features->next = NULL;
        return;
    }
    FeatureNode *newFeature;
    newFeature = project->features; //go to the first feature
    while (newFeature != NULL) { //get to the last feature
        if (strcmp(newFeature->feature, featureName) == 0) { //make sure there isn't another feature with this name
            printf("Feature already exists\n");
            free(featureName);
            return;
        }
        if (newFeature->next == NULL) //if we got to the last feature
            break;
        newFeature = newFeature->next;
    }
    newFeature->next = malloc(sizeof(FeatureNode)); //make place for the feature
    didAllocationFail(newFeature->next);
    newFeature = newFeature->next;
    newFeature->feature = featureName;
    newFeature->next = NULL;
}

void removeFeatureFromProject(Project *project) {
    if (project->features == NULL) {
        printf("There are no features to remove.\n");
        return;
    }
    FeatureNode *currentFeature = project->features;
    int count = 0, choice;
    printf("Select a feature to remove:\n");
    while (currentFeature != NULL) { //go over all features
        count++;
        printf("%d. %s\n", count, currentFeature->feature);
        currentFeature = currentFeature->next;
    }
    printf("Enter your choice: ");
    scanf("%d", &choice);
    if (!isNumInRange(choice, count)) {
        printf("Invalid choice\n");
        return;
    }
    if (count == 1) { //if there is only 1 feature
        free(project->features->feature); //free the feature
        free(project->features);
        project->features = NULL;
        return;
    }
    if (choice == 1) { //if the user chose to remove the first feature
        currentFeature = project->features; //keep a pointer to the feature we want to remove
        project->features = project->features->next; //turn the second feature to the first one
        free(currentFeature->feature); //free the feature
        free(currentFeature);
        return;
    }
    FeatureNode *pre, *curr, *next;
    pre = project->features;
    for (int i = 1; i < choice - 1; ++i) { //reach to the node that's before the wanted one
        pre = pre->next;
    }
    curr = pre->next; // pointing at the wanted node
    next = curr->next;
    pre->next = next; //connect 'next' to 'pre'
    free(curr->feature);
    free(curr);
}

void leaveCompany(Worker *workersArr[], int *workersCount, Project *projectsArr[], int *projectsCount) {
    if (*workersCount == 0) { //must have workers first
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }
    int chosenWorker;
    printf("Select a worker to leave the company:\n");
    for (int i = 0; i < *workersCount; ++i) { //print all workers
        printf("%d. %s\n", i + 1, workersArr[i]->name);
    }
    printf("Enter the worker's number: ");
    scanf("%d", &chosenWorker);
    if (!isNumInRange(chosenWorker, *workersCount)) {
        printf("Invalid choice\n");
        return;
    }
    chosenWorker--; //get the worker's 'spot' in the workers array
    Project *currentProject;
    for (int i = 0;
         i < workersArr[chosenWorker]->projectCount; ++i) { //see which of the worker's projects we need to delete
        currentProject = workersArr[chosenWorker]->projects[i];
        if (currentProject->workerCount == 1) { //if this is the only worker in the project, we need to delete it
            freeFeatures(currentProject);
            int projectIndex = 0;
            //find the project in the projects array:
            while (strcmp(currentProject->name, projectsArr[projectIndex]->name) != 0) {
                projectIndex++;
            }
            for (int j = projectIndex; j < (*projectsCount)-1; ++j) {
                projectsArr[j] = projectsArr[j + 1];
            }
            (*projectsCount)--;
            projectsArr[*projectsCount] = NULL;
            free(currentProject->workers);
            free(currentProject->name);
            free(currentProject);
        } else { //if there are other workers in the project, we only need to remove this worker from the project
            int index = 0;
            while (strcmp(currentProject->workers[index]->name, workersArr[chosenWorker]->name) != 0)
                index++;
            for (int j = index; j < currentProject->workerCount - 1; ++j) {
                currentProject->workers[j] = currentProject->workers[j + 1];
            }
            currentProject->workerCount--;
            currentProject->workers[currentProject->workerCount] = NULL;
        }
    }
    //now we remove the worker from the workers array:
    Worker *currentWorker = workersArr[chosenWorker];
    int index = 0;
    while (strcmp(currentWorker->name, workersArr[index]->name) != 0) {
        index++;
    } //move the other workers 1 spot back
    for (int i = index; i < (*workersCount)-1; ++i) {
        workersArr[i] = workersArr[i + 1];
    }
    (*workersCount)--;
    workersArr[*workersCount] = NULL;
    free(currentWorker->projects);
    free(currentWorker->name);
    free(currentWorker);
}
void freeFeatures (Project* project) {
    if (project->features != NULL) { //if there are features in the project, we'll delete them
        FeatureNode *start, *middle, *end;
        start = project->features;
        while (start->next != NULL) { //while there is more than 1 feature
            middle = start;
            while (middle->next->next != NULL) //go to the one before last node
                middle = middle->next;
            end = middle->next;
            middle->next = NULL;
            free(end->feature);
            free(end);
        }
        free(start->feature);
        free(start);
    }
}
void freeAllAllocations(Worker *workersArr[], int workersCount, Project *projectsArr[], int projectsCount) {
    for (int i = 0; i < projectsCount; ++i) { //free all projects
        freeFeatures(projectsArr[i]);
        free(projectsArr[i]->workers);
        free(projectsArr[i]->name);
        free(projectsArr[i]); //free the project
    }
    for (int i = 0; i < workersCount; ++i) { //free all workers
        free(workersArr[i]->projects); //the projects themselves already freed
        free(workersArr[i]->name);
        free(workersArr[i]);
    }
    printf("Exiting...");
}