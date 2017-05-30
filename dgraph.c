/*** File: dgraph.c - Implements Directed Random Graphs ***/
/*
 *   Shane Saunders
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dgraph.h"


#define UNDEFINED -1



/* We take MaxCost as being unconnected and 0 to (MaxCost - 1) as the cost
 * of connected edges
 */
const int MaxEdgeCost = 1000;
const int MinEdgeCost = 1;


/* Always use vertex number 0 for the starting vertex in a graph. */
const int StartVertex = 0;



/* dgraph_blank() - creates a graph with n vertices but no edges.  Can be used
 * when an outside algorithm generates the graphs edges.
 */
dgraph_t *dgraph_blank(int n)
{
    dgraph_t *g;
    dgraph_vertex_t *vertices;
    int i;

    g = malloc(sizeof(dgraph_t));
    vertices = g->vertices = malloc(n*sizeof(dgraph_vertex_t));
    g->n = n;

    for(i = 0; i < n; i++) {
        vertices[i].first_edge = vertices[i].last_edge = NULL;
    }

    return g;
}



/* dgraph_rnd_sparse() - creates a directed random graph with all vertices
 * reachable from the starting vertex.  Parameter n is the size of the graph,
 * and parameter edge_factor is the average OUT set size for vertices.
 * Returns a pointer to the random graph created.
 * For freeing space used by the random graph see dgraph_free().
 */
dgraph_t *dgraph_rnd_sparse(int n, double edge_factor)
{
    int i, j, k, m;
    int *c, *size;
    int costDiff, n_edges, edge_count;
    dgraph_t *g;
    dgraph_vertex_t *vertex, *vertices;
    dgraph_edge_t *edge_ptr;

    /* Allocate space for the graph. */
    g = malloc(sizeof(dgraph_t));
    g->vertices = malloc(n*sizeof(dgraph_vertex_t));
    g->n = n;

    /* Allocate space for temporary arrays. */
    c = malloc(n * sizeof(int));
    size = malloc(n * sizeof(int));

    /* Initialise variables and arrays. */
    costDiff = (MaxEdgeCost - MinEdgeCost);
    n_edges = edge_factor * n;
    vertices = g->vertices;
    for(i = 0; i < n; i++) {
	vertices[i].first_edge = NULL;
	vertices[i].last_edge = NULL;

        c[i] = i;
	size[i] = 1;
    }


    /* For each vertex except the starting vertex, set up one random edge
     * pointing to it.  We do not set up a random edge pointing to the starting
     * vertex, that is, vertex number 0.
     */
    for(j = 1; j < n; j++) {

        /* Choose one of the available 'from' vertices at random. */
        k = rand() % (n - size[j]);

        /* Find the unused vertex, i, that corresponds to k.
         */
        i = -1; m = -1;
        while(m < k) {
            i++;
            if(c[i] != c[j]) m++;
        }

        /* In order to avoid creating cycles in the sub-graph, we update the
	 * array c[] to keep track of the connected parts in the sub-graph.
	 * Do this by updating all vertices with connection number c[i] to
	 * have connection number c[j].
	 */
        m = c[i];
        for(k = 0; k < n; k++) {
            if(c[k] == m) {
                c[k] = c[j];
                size[k] += size[j];
            }
        }

	/* Add the new edge to the random graph. */
	add_new_edge(&vertices[i], j, MinEdgeCost + (rand() % costDiff));
    }

    /* We are done with using c[] and size[]. */
    free(c);
    free(size);

    /* Initialise edge_count and calculate the number of edges required. */
    edge_count = n-1;
    n_edges = n * edge_factor;

    /* Create the random graph.  Edges from vertex i to vertex j.
     * Variable k keeps track of the number of vertices in the out set
     * of the current vertex.
     */
    while(edge_count < n_edges) {

        /* Select random i and j */
        i = rand() % n;  j = rand() % n;
	vertex = &vertices[i];

        /* The cost for the edge from a vertex to itself is zero. */
        if (i != j) {

	    /* Scan the existing edges to make sure that an edge to j does not
	     * already exist.
	     */
            edge_ptr = vertex->first_edge;
	    while(edge_ptr) {
                if(edge_ptr->vertex_no == j) break;
		edge_ptr = edge_ptr->next;
	    }

	    /* If the edge to j does not already exist create the new edge. */
	    if(!edge_ptr) {
                add_new_edge(vertex, j, MinEdgeCost + (rand() % costDiff));
		edge_count++;
	    }
        }
    }

    return g;
}



/* dgraph_rnd_dense() - creates a directed random graph with all vertices
 * reachable from the starting vertex.  Parameter n is the size of the graph,
 * and parameter prob is the probability of edge existence.
 * Returns a pointer to the random graph created.
 */
dgraph_t *dgraph_rnd_dense(int n, double prob)
{
    int i, j, k, m;
    int *c, *size;
    int *sources;
    double x;
    int costDiff;
    dgraph_t *g;
    dgraph_vertex_t *vertex, *vertices;

    /* Allocate space for the graph. */
    g = malloc(sizeof(dgraph_t));
    g->vertices = malloc(n*sizeof(dgraph_vertex_t));
    g->n = n;

    /* Allocate space for temporary arrays. */
    c = malloc(n * sizeof(int));
    size = malloc(n * sizeof(int));
    sources = malloc(n * sizeof(int));

    /* Initialise variables and arrays. */
    costDiff = (MaxEdgeCost - MinEdgeCost);
    vertices = g->vertices;
    for(i = 0; i < n; i++) {
	vertices[i].first_edge = NULL;
	vertices[i].last_edge = NULL;

        c[i] = i;
	size[i] = 1;
	sources[i] = UNDEFINED;
    }

    /* For each vertex, except the starting vertex, set up one random edge
     * pointing to it.  We do not set up a random edge pointing to the starting
     * vertex, that is, vertex number 0.
     */
    for(j = 1; j < n; j++) {

        /* Choose one of the available 'from' vertices at random. */
        k = rand() % (n - size[j]);

        /* Find the unused vertex, i, that corresponds to k, and record the
         * connection in the array sources[].
         */
        i = -1; m = -1;
        while(m < k) {
            i++;
            if(c[i] != c[j]) m++;
        }
        sources[j] = i;

        /* In order to avoid creating cycles in the sub-graph, we update the
	 * array c[] to keep track of the connected parts in the sub-graph.
	 * Do this by updating all vertices with connection number c[i] to
	 * have connection number c[j].
	 */
        m = c[i];
        for(k = 0; k < n; k++) {
            if(c[k] == m) {
                c[k] = c[j];
                size[k] += size[j];
            }
        }
    }

    /* We are done with using c[] and size[]. */
    free(c);
    free(size);

    /* We note that we must change the probability of edge existence slightly
     * since n-1 edges have already been determined.
     */
    prob = (prob*n - 1) / (n - 1);

    /* Create the random graph.  Edges from vertex i to vertex j.
     * Variable k keeps track of the number of vertices in the out set
     * of the current vertex.
     */
    for(i = 0; i < n; i++) {

        vertex = &vertices[i];

        for(j = 0; j < n; j++) {

            /* Only create edges that are not from a vertex to itself.
	     */
            if (i != j) {

                /* Create and edge from vertex i to vertex j with probability
                 * prob.  All edges in the connecting sub-graph are created.
                 */
                x = (double) rand() / RAND_MAX;
                if (x <= prob || sources[j] == i) {
		    add_new_edge(vertex, j, MinEdgeCost + (rand() % costDiff));
                }
            }
        }
    }

    /* We are done with using sources[]. */
    free(sources);

    return g;
}



/* dgraph_rnd_tree_parts() - create a random graph made up of tree parts.
 * n_trees - the number of trees.
 * tree_size - number of vertices per tree.
 * branch_f - branching factor of trees.
 * ext_f - Number of additional external edges into each tree root.
 *         Note that each leaf vertex of a tree automatically causes the
 *         creation of an edge to the root vertex of some tree.
 * The user should ensure that:
 *     1.  tree_size > branch_f.  (do not allow edge from a root vertex
 *                                 to itself.)
 *     2.  n_trees >= branch_f.
 * Otherwise the algorithm may run indefinitely trying to generate the graph.
 */
dgraph_t *dgraph_rnd_tree_parts(int n_trees, int tree_size,
			       int branch_f, int ext_f)
{
    dgraph_t *g;
    dgraph_vertex_t *vertex, *vertices;
    dgraph_edge_t *edge_ptr;
    int i, j, k, r, n;
    int v, w;
    int exists;
    int tree_start, tree_end;
    int child_start, child_end;
    int costDiff;
    int *stack, tos, *visited, vertex_count;

    /* For converting rand() result to edge costs. */
    costDiff = (MaxEdgeCost - MinEdgeCost);

    /* Calculate the number of vertices in the graph. */
    n = n_trees * tree_size;

    /* Stack and boolean array for reachability DFS. */
    stack = malloc(n * sizeof(int));
    visited = malloc(n * sizeof(int));

  start_graph_gen:
    /* Allocate space for the graph. */
    g = malloc(sizeof(dgraph_t));
    vertices = g->vertices = malloc(n*sizeof(dgraph_vertex_t));
    g->n = n;

    /* Initialise vertex entries in the graph structure. */
    for(i = 0; i < n; i++) {
        vertices[i].first_edge = vertices[i].last_edge = NULL;
    }

    /* The root vertex of tree i is located at i * tree_size in the array.
     * Here i ranges from 0 to n_trees - 1.
     */

    /* Create edges for each tree. */
    for(i = 0; i < n_trees; i++) {
	tree_start = i * tree_size;
	tree_end = tree_start + tree_size;

	/* Cycle through each vertex in the tree. */
        for(j = 0; j < tree_size; j++) {
	    v = tree_start + j;
                /* Array index for the jth vertex in the tree. */
            vertex = &vertices[v];

            /* The children of the jth vertex are at branch_f * j + 1,
	     * branch_f * j + 2, ..., branch_f * j + branch_f in the tree.
	     */
            child_start = tree_start + branch_f * j + 1;
	    child_end = child_start + branch_f;

	    /* Create an edge from v to each child entry in the array.
	     * If the tree is not large enough to hold a child, an edge
	     * from the child to a random root vertex is created.
	     */
	    for(k = child_start; k < child_end; k++) {
		if(k < tree_end) {
                    add_new_edge(vertex, k, MinEdgeCost + (rand() % costDiff));
		}
		else {
                    /* Select a random root vertex entry which is not already
		     * in v's edge list.
		     */
		    do {
			r = tree_size * (rand() % n_trees);
			/* Check that edge (v,r) does not already exist. */
			exists = 0;
			edge_ptr = vertex->first_edge;
			while(edge_ptr) {
			    if(edge_ptr->vertex_no == r) {
				exists = 1;
				break;
			    }
			    edge_ptr = edge_ptr->next;
			}
		    } while(exists);
		    add_new_edge(vertex, r, MinEdgeCost + (rand() % costDiff));
		}
	    }
	}
    }

    /* Create additional external edges into the root vertex of each tree. */
    for(i = 0; i < n_trees; i++) {
        for(j = 0; j < ext_f; j++) {
	    /* The root vertex entry, w, is the destination. */
	    w = i * tree_size;

	    /* Keep trying random source vertices until an edge that does not
	     * already exist is found.
	     */
	    do {
                /* Choose a source vertex, v, at random which is not equal to
		 * w.
		 */
	        v = rand() % (n-1);
		if(v >= w) v++;
	        vertex = &vertices[v];

	        /* Check that edge (v,w) does not already exist. */
		exists = 0;
		edge_ptr = vertex->first_edge;
		while(edge_ptr) {
                    if(edge_ptr->vertex_no == w) {
			exists = 1;
			break;
		    }
		    edge_ptr = edge_ptr->next;
		}
	    } while(exists);

	    add_new_edge(vertex, w, MinEdgeCost + (rand() % costDiff));
	}
    }


    /* Test reachability of vertices from StartVertex using a DFS. */
    memset(visited, 0, n * sizeof(int));
    vertex_count = 0;
    tos = 0;
    visited[StartVertex] = 1;
    stack[tos++] = StartVertex;
    while(tos) {
        v = stack[--tos];
	vertex_count++;
	vertex = &vertices[v];
	edge_ptr = vertex->first_edge;
	while(edge_ptr) {
	    w = edge_ptr->vertex_no;
	    if(!visited[w]) {
		visited[w] = 1;
		stack[tos++] = w;
	    }
	    edge_ptr = edge_ptr->next;
	}
    }
    if(vertex_count != n) {
	/* Not reachable, so try again. */
	dgraph_free(g);
	goto start_graph_gen;
    }

    free(stack);
    free(visited);

    return g;
}



/* dgraph_rnd_acyclic_parts() - create a random graph made up of acyclic parts.
 * n_trees - the number of acyclic parts.
 * tree_size - number of vertices per acyclic part.
 * branch_f - branching factor of acyclic parts.
 * ext_f - Number of additional external edges into each acyclic parts root.
 *         Note that each leaf vertex of a tree automatically causes the
 *         creation of an edge to the root vertex of some tree.
 * The user should ensure that:
 *     1.  tree_size > branch_f.  (do not allow edge from a root vertex
 *                                 to itself.)
 *     2.  n_trees >= branch_f.
 * Otherwise the algorithm may run indefinitely trying to generate the graph.
 */
dgraph_t *dgraph_rnd_acyclic_parts(int n_trees, int tree_size,
			          int branch_f, int ext_f)
{
    dgraph_t *g;
    dgraph_vertex_t *vertex, *vertices;
    dgraph_edge_t *edge_ptr;
    int i, j, k, r, n;
    int v, w;
    int exists;
    int tree_start, tree_end;
    int child_start, child_end;
    int costDiff;
    int *stack, tos, *visited, vertex_count;

    /* For converting rand() result to edge costs. */
    costDiff = (MaxEdgeCost - MinEdgeCost);

    /* Calculate the number of vertices in the graph. */
    n = n_trees * tree_size;

    /* Stack and boolean array for reachability DFS. */
    stack = malloc(n * sizeof(int));
    visited = malloc(n * sizeof(int));

  start_graph_gen:
    /* Allocate space for the graph. */
    g = malloc(sizeof(dgraph_t));
    vertices = g->vertices = malloc(n*sizeof(dgraph_vertex_t));
    g->n = n;

    /* Initialise vertex entries in the graph structure. */
    for(i = 0; i < n; i++) {
        vertices[i].first_edge = vertices[i].last_edge = NULL;
    }

    /* The root vertex of ac part i is located at i * tree_size in the array.
     * Here i ranges from 0 to n_trees - 1.
     */

    /* Create edges for each ac part. */
    for(i = 0; i < n_trees; i++) {
	tree_start = i * tree_size;
	tree_end = tree_start + tree_size;

	/* Cycle through each vertex in the ac part. */
        for(j = 0; j < tree_size; j++) {
	    v = tree_start + j;
                /* Array index for the jth vertex in the ac part. */
            vertex = &vertices[v];

            /* The immediate children of the jth vertex are at
	     * branch_f * j + 1, branch_f * j + 2, ..., branch_f * j + branch_f
	     * in the ac part.
	     */
            child_start = tree_start + branch_f * j + 1;
	    child_end = child_start + branch_f;


	    /* Create an edge from v to each child entry in the array.
	     * If the ac part is not large enough to hold a child, an edge
	     * from the child to a random root vertex is created.
	     */
	    for(k = child_start; k < child_end; k++) {
		if(k < tree_end) {
                    add_new_edge(vertex, k, MinEdgeCost + (rand() % costDiff));
		}
		else {
                    /* Select a random root vertex entry which is not already
		     * in v's edge list.
		     */
		    do {
			r = tree_size * (rand() % n_trees);
			/* Check that edge (v,r) does not already exist. */
			exists = 0;
			edge_ptr = vertex->first_edge;
			while(edge_ptr) {
			    if(edge_ptr->vertex_no == r) {
				exists = 1;
				break;
			    }
			    edge_ptr = edge_ptr->next;
			}
		    } while(exists);
		    add_new_edge(vertex, r, MinEdgeCost + (rand() % costDiff));
		}
	    }

	    /* To make the acyclic structure, edges are created which point to
	     * the grandchildren.
	     */
	    child_start = tree_start + branch_f*(branch_f * j + 1) + 1;
	    child_end = child_start + branch_f * branch_f;

	    for(k = child_start; k < child_end; k++) {
		if(k < tree_end) {
                    add_new_edge(vertex, k, MinEdgeCost + (rand() % costDiff));
		}
		else {
                    /* Select a random root vertex entry which is not already
		     * in v's edge list.
		     */
		    do {
			r = tree_size * (rand() % n_trees);
			/* Check that edge (v,r) does not already exist. */
			exists = 0;
			edge_ptr = vertex->first_edge;
			while(edge_ptr) {
			    if(edge_ptr->vertex_no == r) {
				exists = 1;
				break;
			    }
			    edge_ptr = edge_ptr->next;
			}
		    } while(exists);
		    add_new_edge(vertex, r, MinEdgeCost + (rand() % costDiff));
		}
	    }
	}
    }

    /* Create additional external edges into the root vertex of each ac part.
     */
    for(i = 0; i < n_trees; i++) {
        for(j = 0; j < ext_f; j++) {
	    /* The root vertex entry, w, is the destination. */
	    w = i * tree_size;

	    /* Keep trying random source vertices until an edge that does not
	     * already exist is found.
	     */
	    do {
                /* Choose a source vertex, v, at random which is not equal to
		 * w.
		 */
	        v = rand() % (n-1);
		if(v >= w) v++;
	        vertex = &vertices[v];

	        /* Check that edge (v,w) does not already exist. */
		exists = 0;
		edge_ptr = vertex->first_edge;
		while(edge_ptr) {
                    if(edge_ptr->vertex_no == w) {
			exists = 1;
			break;
		    }
		    edge_ptr = edge_ptr->next;
		}
	    } while(exists);

	    add_new_edge(vertex, w, MinEdgeCost + (rand() % costDiff));
	}
    }


    /* Test reachability of vertices from StartVertex using a DFS. */
    memset(visited, 0, n * sizeof(int));
    vertex_count = 0;
    tos = 0;
    visited[StartVertex] = 1;
    stack[tos++] = StartVertex;
    while(tos) {
        v = stack[--tos];
	vertex_count++;
	vertex = &vertices[v];
	edge_ptr = vertex->first_edge;
	while(edge_ptr) {
	    w = edge_ptr->vertex_no;
	    if(!visited[w]) {
		visited[w] = 1;
		stack[tos++] = w;
	    }
	    edge_ptr = edge_ptr->next;
	}
    }
    if(vertex_count != n) {
	/* Not reachable, so try again. */
	dgraph_free(g);
	goto start_graph_gen;
    }

    free(stack);
    free(visited);

    return g;
}



/* dgraph_free() - frees space used by a directed random graph.
 */
void dgraph_free(dgraph_t *g)
{
    int i;
    dgraph_vertex_t *vertices;
    dgraph_edge_t *edge_ptr, *next_edge_ptr;

    vertices = g->vertices;

    for(i = 0; i < g->n; i++) {
        edge_ptr = vertices[i].first_edge;

	while(edge_ptr) {
	    next_edge_ptr = edge_ptr->next;
	    free(edge_ptr);
	    edge_ptr = next_edge_ptr;
	}
    }

    free(vertices);

    free(g);
}



/* add_new_edge() - adds a new edge to the edge list of the vertex pointed to
 * by `vertex'.  The edge has destination vertex number dest_vertex_no, and
 * distance dist.
 */
void add_new_edge(dgraph_vertex_t *vertex, int dest_vertex_no, long dist)
{
    dgraph_edge_t *new_edge, *last_edge;

    new_edge = malloc(sizeof(dgraph_edge_t));
    new_edge->vertex_no = dest_vertex_no;
    new_edge->dist = dist;
    new_edge->next = NULL;

    if((last_edge = vertex->last_edge)) {
	last_edge->next = new_edge;
    }
    else {
	vertex->first_edge = new_edge;
    }
    vertex->last_edge = new_edge;
}

void dgraph_add_new_edge(dgraph_t *g, int vertex, int vertex_dest){
	add_new_edge(&(g->vertices[vertex]),vertex_dest,0);
}

void dgraph_print(const dgraph_t *g){
	int i;
	for(i = 0 ;i < g->n; i++){
		printf("%d	", i);
		dgraph_edge_t *current_edge = g->vertices[i].first_edge;
		while (current_edge != NULL){
			printf("%d ", current_edge->vertex_no);
			current_edge = current_edge->next;
		}
		printf("\n");
	}
}





