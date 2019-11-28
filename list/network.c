#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <assert.h>

typedef struct node {
    int x;
    int currentItems;
    struct node **edges;
} node;

typedef struct graph {
    int n;
    struct node **array;
} graph;

node *newNode(int x) {
    node *new;
    new = malloc(sizeof(node));
    new->edges = malloc(sizeof(node));
    new->currentItems = 0;
    new->x = x;

    return new;
}

graph *newGraph() {
    graph *new;
    new = malloc(sizeof(graph));
    new->array = malloc(sizeof(node));
    new-> n = 0;

    return new;
}

void freeGraph(graph *g) {
    for (int i = 0; i < g->n; i++ ) {
        free(g->array[i]->edges);
        free(g->array[i]);
    }
    free(g->array);
    free(g);
}

//Checks if an edge exists for a node, returns 1 if edge exists, 0 if not
int edgeExists(node *src, node *dst) {
    for (int i = 0; i < src->currentItems; i++) {
        if (src->edges[i] == dst) return 1;
    }
    return 0;
}

//Reallocates space for new edge then adds edge
void addEdge(node *src, node *dst) {
    int r = edgeExists(src, dst);
    if (r == 0) {
        src->edges = realloc(src->edges, (src->currentItems + 1) * sizeof(node));
        src->edges[src->currentItems] = dst;
        src->currentItems++;
    }
}

//Reallocates sapce for new node then adds node
void addNode(graph *g, node *n) {
    g->array = realloc(g->array, (g->n + 1) * sizeof(node));
    g->array[g->n] = n;
    g->n++;
}

//Returns node pointer from node value, NULL is returned if no node found
node *getNode(graph *g, int x) {
    for (int i = 0; i < g->n; i++) {
        if (g->array[i]->x == x) {
            return (g->array[i]);
        }
    }
    return NULL;
}

//Prints edges of node in format (a -> b)(a -> c)
//Or (a) in the case the node does not go anywhere
void printEdges(node *n) {
    if (n->currentItems > 0) {
        for (int i = 0; i < n->currentItems; i++) {
            printf("(%d -> %d)", n->x, n->edges[i]->x);
        }
        printf("\n");
    } else {
        printf("(%d)\n",n->x);
    }
}

//Prints structure for entire graph
void printStructure(graph *g) {
    for(int i = 0; i < g->n; i++) {
        printEdges(g->array[i]);
    }
}

//If node does not already exist, creates node and adds it to the graph
node *createNode(graph *g, int x) {
    node *node = getNode(g, x);
       if (node == NULL) {
            node = newNode(x);
            addNode(g, node);
       }
    return node;
}

//Finds avg number of edges each node has, returns a float which is rounded to 1d.p when printed
float avgEdges(graph *g) {
    float total = 0;
    for (int i = 0; i < g->n; i++) {
        total = total + g->array[i]->currentItems;
    }
    return total / g->n;
}

//Finds total number of edges in a graph
int numEdges(graph *g) {
    int num = 0;
    for (int i = 0; i < g->n; i++) {
        num = num + g->array[i]->currentItems;
    }
    return num;
}

int existsInArray(int n, int *visited, const int curr) {
    for (int i = 0; i < curr; i++) {
        printf("curr: %d x: %d visited: %d\n",curr,n, visited[i]);
        printf("T?: %d\n",(n == visited[i]));
        if (n == visited[i]) return 1;
    }
    return 0;
}

int nodeSearch(node *n, int *visited, int *curr) {
    int children = n->currentItems;
    for (int i = 0; i < children; i++) {
        //printf("i: %d pointer: %d node: %d\n",i,*curr, n->x); 
        //printf("x: %d\n",n->edges[i]->x);
        int r = existsInArray(n->edges[i]->x, visited, *curr);
        if (r == 1) return 0;
        else {
            //visited = realloc(visited, (*curr + 1) * sizeof(int));
            visited[*curr] = n->edges[i]->x;
            *curr = *curr + 1;
        }
    } 
    return 1;
}

int isTree(graph *g) {
    int visited[200];
    //visited = malloc(sizeof(int));
    int curr = 0;

    for (int i = 0; i < (g->n - 1); i++) {
        int r = nodeSearch(g->array[i], visited, &curr);
        if (r == 0) return 0;
    }
    return 1;
}


// int checkHamiltonian(graph *g) {
    // int *visited;
    // visited = malloc(sizeof(int));

    // for (int i = 0; i < (g->n - 1); i++) {
        
    // }

    // return 0;
// }

// int checkHamiltonian(graph *g) {
    // for (int i = 0; i < g->n; i++) {
        // node *start = g->array[i];
        // static int *visited;
        // visited = malloc(sizeof(int));
        // visited[0] = start->x;
        // int c = 1;
        // for (int i = 0; i < start->currentItems - 1; i++) {
            // int r = existsInArray(start->edges[i], visited);
            // if (r == 0) {
                // visited = realloc(visited, (c+1)*sizeof(int));
                // visited[c] = start->edges[i]->x;
                // c++;
            // }
        // }
        // int size = sizeof(visited) / sizeof(int);
        // for (int i = 0; i < size - 1; i++) {
            // printf("%d ",visited[i]);
        // }
        // printf("\n");
        // printf("size: %d size g: %d\n",size, g->n);
        // if (size == (g->n)) return 1;
        // free(visited);
    // }
    // return 0;
// }

//Parses string and creates graph
graph *parseString(char string[]) {
    char delim[] = ",";
    char *str = strtok(string, delim);

    graph *g = newGraph();

    while(str != NULL) {
        if (str[1] == '-') {
            node *first = createNode(g, atoi(&str[0]));
            node *second = createNode(g, atoi(&str[2]));
            addEdge(first, second);
        } else {
            createNode(g, atoi(&str[0]));
        }
        str = strtok(NULL, delim);
    }

    return g;
}

//Validates input using regular expression, input must be zero or more "a-b," or "a," ending with "a-b" or "a"
int checkInput(char *str) {
    regex_t r;
    regcomp(&r, "^([[:digit:]]+[\\-][[:digit:]]+[,]|[[:digit:]]+[,])*([[:digit:]]+[\\-][[:digit:]]+|[[:digit:]]+)$", REG_EXTENDED);
    int status = regexec(&r, str, (size_t) 0, NULL, 0);
    regfree(&r);
    return status;
}

//Testing
void freeNode(node *n) {
    free(n->edges);
    free(n);
}

void testCheckInput() {
    assert(checkInput("1-2") == 0);
    assert(checkInput("1-2,2-3") == 0);
    assert(checkInput("1,2-3") == 0);
    assert(checkInput("1-2,3") == 0);
    assert(checkInput("1") == 0);
    assert(checkInput("1-2,") == 1);
    assert(checkInput("1-") == 1);
    assert(checkInput("abc") == 1);
}

void testNode() {
    node *n = newNode(5);
    assert(n->currentItems == 0);
    assert(n->x == 5);
    node *m = newNode(2);
    addEdge(n,m);
    assert(n->currentItems == 1);
    assert(n->edges[0] == m);
    
    freeNode(m);
    freeNode(n);
}

void testGraph() {
    graph *g = newGraph();
    assert(g->n == 0);
    node *n = newNode(5);
    addNode(g, n);
    assert(g->n == 1);
    assert(g->array[0] == n);
    
    freeGraph(g);
}

void testEdgeExists() {
    node *n = newNode(5);
    node *m = newNode(2);
    assert(edgeExists(n, m) == 0);
    addEdge(n,m);
    assert(edgeExists(n, m) == 1);

    freeNode(n);
    freeNode(m);
}

void testGetNode() {
    graph *g = newGraph();
    assert(getNode(g, 2) == NULL);
    node *n = newNode(5);
    addNode(g, n);
    assert(getNode(g, 2) == NULL);
    assert(getNode(g, 5) == n);

    freeGraph(g);
}

void testAvgEdges() {
    char s[] = "1-2";
    graph *g = parseString(s);
    assert(avgEdges(g) == 0.5);
    freeGraph(g);

    char x[] = "1-2,2-2";
    g = parseString(x);
    assert(avgEdges(g) == 1.0);
    freeGraph(g);
}

void testNumEdges() {
    char s[] = "1-2,1-3,2-3,3-1";
    graph *g = parseString(s);
    assert(numEdges(g) == 4);

    freeGraph(g);
}

void test() {
    testCheckInput();
    testNode();
    testGraph();
    testEdgeExists();
    testGetNode();
    testAvgEdges();
    testNumEdges();
    printf("All tests pass\n");
}

int main(int n, char *args[n]) {
    if (n == 2) {
        int r = checkInput(args[1]);
        if (r == 0) {
            graph *g = parseString(args[1]);
            printf("Visual represntation:\n");
            printStructure(g);
            printf("Number of nodes: %d\n", g->n);
            printf("Number of edges: %d\n",numEdges(g));
            printf("Average number of edges per node: %.1f\n", avgEdges(g));
            printf("Is tree?: %d\n", isTree(g));
            //printf("Hamiltonian?: %d\n",checkHamiltonian(g));
            freeGraph(g);
        } else {
            printf("Error with inputs\n");
            exit(1);
        }
    } else { 
        test();
    }
   return 0;
}
