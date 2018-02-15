#include <stdlib.h>

struct queue {
	double enterTime;
	struct queue *next;
};

int insertIntoQueue(struct queue **queuePtrRef, double enterTime) {
	struct queue *current = *queuePtrRef;
	int queueLength = 0;
	if (current == NULL)
		*queuePtrRef = current = malloc(sizeof(struct queue));
	else {
		queueLength++;
		while (current->next != NULL) {
			current = current->next;
			queueLength++;
		}
		current->next = malloc(sizeof(struct queue));
		current = current->next;
	}
	current->enterTime = enterTime;
	current->next = NULL;

	return queueLength;
}

double deleteFromQueue(struct queue **queuePtrRef) {
	if (*queuePtrRef == NULL)
		return -1;
	struct queue *current = *queuePtrRef;
	double enterTime = current->enterTime;
	*queuePtrRef = current->next;
	free(current);
	return enterTime;
}

int queueLength(struct queue **queuePtrRef) {
	if (queuePtrRef == NULL)
		return -1;
	struct queue *current = *queuePtrRef;
	int length = 0;
	for (length = 0; current != NULL; length++)
		current = current->next;
	return length;
}
