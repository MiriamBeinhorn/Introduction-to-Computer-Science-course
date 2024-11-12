#ifndef ex_5_H
#define ex_5_H

#define MAX_WORKERS 50
#define MAX_PROJECTS 100
#define JOIN_COMPANY 1
#define OPEN_PROJECT 2
#define JOIN_PROJECT 3
#define SEE_PROJECTS_IN_WORK 4
#define WHICH_PEOPLE_IN_WHAT_PROJECTS 5
#define WORK_ON_PROJECT 6
#define LEAVE_COMPANY 7
#define EXIT_PROGRAM 8

typedef struct Worker Worker;
typedef struct FeatureNode FeatureNode;
typedef struct Project Project;

struct Worker
{
    char *name;
    Project **projects;
    int projectCount;
};

struct FeatureNode
{
    char *feature;
    struct FeatureNode *next;
};

struct Project
{
    char *name;
    Worker **workers;
    int workerCount;
    FeatureNode *features;
};

int menu();
void cleanBuffer();
void didAllocationFail (void *ptr);
char* getString ();
int checkWorkerName (Worker* workersArr[], char* name, int workersCount);
int checkProjectName (Project* projectsArr[], char* name, int projectsCount);
int isWorkerInProject (Project* project, char* workerName);
int isNumInRange (int choice, int maxChoice);
void joinProject (Worker *workersArr[], int workersCount, Project *projectsArr[], int projectsCount);
void joinCompany(Worker *workersArr[], int *workersCount);
void openNewProject(Worker *workersArr[], int workersCount, Project *projectsArr[], int *projectsCount);
void displayAllProjects(Project *projectsArr[], int projectsCount);
void displayAllWorkers(Worker *workersArr[], int workersCount);
void workOnProject(Worker *workersArr[], int workersCount, int projectsCount);
void addFeatureToProject(Project *project);
void removeFeatureFromProject(Project *project);
void leaveCompany(Worker *workersArr[], int *workersCount, Project *projectsArr[], int *projectsCount);
void freeFeatures (Project* project);
void freeAllAllocations(Worker *workers[], int workerCount, Project *projects[], int projectCount);

#endif