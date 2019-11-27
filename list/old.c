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
node *createNode(int *nodes, graph *g, int x) {
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

//Parses string and creates graph
graph *parseString(char string[]) {
    char delim[] = ",-";
    char *str = strtok(string, delim);
    int *nodes;
    nodes = malloc(sizeof(int));

    graph *g = newGraph();

    int c = 0;
    while(str != NULL) {
        nodes = realloc(nodes, (c+1) * sizeof(int));
        nodes[c] = atoi(str);
        c++;
        str = strtok(NULL, delim);
    }

    for(int i = 0; i < c; i = i + 2) {
        node *first = createNode(nodes, g, nodes[i]);
        node *second = createNode(nodes, g, nodes[i+1]);
        addEdge(first, second);
    }
    
    free(nodes);
    return g;
}

//Validates input using regular expression, input must be zero or more "a-b," ending with "a-b"
int checkInput(char *str) {
    regex_t r;
    regcomp(&r, "^([[:digit:]]+[\\-][[:digit:]]+[,])*([[:digit:]]+[\\-][[:digit:]]+)$", REG_EXTENDED);
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
    assert(checkInput("1") == 1);
    assert(checkInput("1-2,") == 1);
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
