/*
 * Banker's Algorithm - Deadlock Avoidance
 * Safety algorithm + resource-request algorithm
 */
#include <stdio.h>

#define MAXP 10
#define MAXR 10

/* Edit these three lines with your own details */
#define STUDENT_NAME  "Your Name"
#define STUDENT_UID   "Your UID"
#define STUDENT_BATCH "Your Batch"

static int n, m;                       /* processes, resource types */
static int alloc[MAXP][MAXR];          /* Allocation matrix */
static int maxc[MAXP][MAXR];           /* Maximum matrix */
static int need[MAXP][MAXR];           /* Need = Max - Allocation */
static int avail[MAXR];                /* Available vector */

static void printVector(const int v[])
{
    int j;
    printf("[");
    for (j = 0; j < m; j++)
        printf("%s%d", j ? ", " : "", v[j]);
    printf("]");
}

static void printRow(const int v[])
{
    int j;
    for (j = 0; j < m; j++)
        printf("%3d", v[j]);
    printf("   |");
}

static void printState(void)
{
    int i;
    printf("\nProcess |%-*s|%-*s|%-*s|\n", m * 3 + 3, " Allocation", m * 3 + 3, " Max", m * 3 + 3, " Need");
    for (i = 0; i < n; i++)
    {
        printf("P%-6d |", i + 1);
        printRow(alloc[i]);
        printRow(maxc[i]);
        printRow(need[i]);
        printf("\n");
    }
    printf("\nAvailable = ");
    printVector(avail);
    printf("\n");
}

static int readMatrix(int a[][MAXR])
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            if (scanf("%d", &a[i][j]) != 1 || a[i][j] < 0)
                return 0;
    return 1;
}

static void computeNeed(void)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = maxc[i][j] - alloc[i][j];
}

/* Safety algorithm. Returns 1 if a safe sequence exists and stores it in seq. */
static int isSafe(int verbose, int seq[])
{
    int work[MAXR], finish[MAXP] = {0};
    int i, j, count = 0, progress;

    for (j = 0; j < m; j++)
        work[j] = avail[j];

    do
    {
        progress = 0;
        for (i = 0; i < n; i++)
        {
            int ok = !finish[i];
            for (j = 0; ok && j < m; j++)
                if (need[i][j] > work[j])
                    ok = 0;
            if (!ok)
                continue;

            if (verbose)
            {
                printf("  P%d: Need ", i + 1);
                printVector(need[i]);
                printf(" <= Work ");
                printVector(work);
            }
            for (j = 0; j < m; j++)
                work[j] += alloc[i][j];
            finish[i] = 1;
            seq[count++] = i;
            progress = 1;
            if (verbose)
            {
                printf(" -> runs, releases resources, Work = ");
                printVector(work);
                printf("\n");
            }
        }
    } while (progress && count < n);

    return count == n;
}

static void printSequence(const int seq[])
{
    int i;
    printf("Safe sequence: ");
    for (i = 0; i < n; i++)
        printf("%sP%d", i ? " -> " : "", seq[i] + 1);
    printf("\n");
}

static void checkSafety(void)
{
    int seq[MAXP];
    printf("\nSafety algorithm steps:\n");
    if (isSafe(1, seq))
    {
        printf("Result: SAFE state.\n");
        printSequence(seq);
    }
    else
        printf("Result: UNSAFE state. No order lets every process finish.\n");
}

/* Resource-request algorithm */
static void makeRequest(void)
{
    int p, j, req[MAXR], seq[MAXP];
    int saveAvail[MAXR], saveAlloc[MAXR], saveNeed[MAXR];

    printf("Enter process number (1-%d): ", n);
    if (scanf("%d", &p) != 1 || p < 1 || p > n)
    {
        printf("Invalid process number.\n");
        return;
    }
    p--;
    printf("Enter request for %d resource types: ", m);
    for (j = 0; j < m; j++)
        if (scanf("%d", &req[j]) != 1 || req[j] < 0)
        {
            printf("Invalid request.\n");
            return;
        }

    printf("\nRequest by P%d = ", p + 1);
    printVector(req);
    printf("\n");

    for (j = 0; j < m; j++)
        if (req[j] > need[p][j])
        {
            printf("Denied: P%d has exceeded its maximum claim.\n", p + 1);
            return;
        }
    for (j = 0; j < m; j++)
        if (req[j] > avail[j])
        {
            printf("P%d must wait: the resources are not available right now.\n", p + 1);
            return;
        }

    /* pretend to allocate, then test the new state */
    for (j = 0; j < m; j++)
    {
        saveAvail[j] = avail[j];
        saveAlloc[j] = alloc[p][j];
        saveNeed[j] = need[p][j];
        avail[j] -= req[j];
        alloc[p][j] += req[j];
        need[p][j] -= req[j];
    }

    if (isSafe(0, seq))
    {
        printf("Granted: the system stays in a SAFE state.\n");
        printSequence(seq);
    }
    else
    {
        for (j = 0; j < m; j++)
        {
            avail[j] = saveAvail[j];
            alloc[p][j] = saveAlloc[j];
            need[p][j] = saveNeed[j];
        }
        printf("Denied: granting this request would leave the system in an UNSAFE state.\n");
    }
}

int main(void)
{
    int i, j, choice;

    printf("==============================================\n");
    printf(" Banker's Algorithm - Deadlock Avoidance\n");
    printf(" Name: %s | UID: %s | Batch: %s\n", STUDENT_NAME, STUDENT_UID, STUDENT_BATCH);
    printf("==============================================\n\n");

    printf("Enter number of processes (max %d) and resource types (max %d): ", MAXP, MAXR);
    if (scanf("%d %d", &n, &m) != 2 || n < 1 || n > MAXP || m < 1 || m > MAXR)
    {
        printf("Invalid size.\n");
        return 1;
    }

    printf("Enter the Allocation matrix (%d x %d):\n", n, m);
    if (!readMatrix(alloc))
    {
        printf("Invalid input.\n");
        return 1;
    }
    printf("Enter the Maximum matrix (%d x %d):\n", n, m);
    if (!readMatrix(maxc))
    {
        printf("Invalid input.\n");
        return 1;
    }
    printf("Enter the Available vector (%d values):\n", m);
    for (j = 0; j < m; j++)
        if (scanf("%d", &avail[j]) != 1 || avail[j] < 0)
        {
            printf("Invalid input.\n");
            return 1;
        }

    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            if (alloc[i][j] > maxc[i][j])
            {
                printf("Error: Allocation of P%d exceeds its Maximum.\n", i + 1);
                return 1;
            }

    computeNeed();
    printState();
    checkSafety();

    for (;;)
    {
        printf("\n1. Make a resource request\n2. Show current state\n");
        printf("3. Run the safety algorithm again\n4. Exit\nEnter your choice: ");
        if (scanf("%d", &choice) != 1)
            break;
        if (choice == 1)
            makeRequest();
        else if (choice == 2)
            printState();
        else if (choice == 3)
            checkSafety();
        else if (choice == 4)
            break;
        else
            printf("Invalid choice.\n");
    }
    printf("\nProgram ended.\n");
    return 0;
}
