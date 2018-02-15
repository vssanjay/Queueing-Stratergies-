#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "queue.c"
#define ITERATIONS 10
#define NUMBER_OF_PACKETS 10000
#define QUEUE_SIZE 10


double avgQueueLength, avgWaitingTime, blockingProbability;

int pickOne(int items) {
	return rand() % items;
}

void simulate(double lambda, double mu) {
	double timestep = (1 / lambda) / 100; // the timestep will be 100th of the average packet arrival time
	int steps = 0;
	struct queue *queue1Ptr = NULL, *queue2Ptr = NULL;
	int queue1Length = 0, queue2Length = 0;
	int arrivals = 0, totalQueueLength = 0, totalPacketsDropped = 0;
	double totalWaitingTime = 0;

	while (arrivals < NUMBER_OF_PACKETS) {
		if (exponentialGenerator(lambda, timestep)) { // there is an arrival
			arrivals++; // increment the nubmer of arrivals
/* random assignment */ if (pickOne(2)) {  // packet has been assigned to queue 1
				totalQueueLength += queue1Length;
				if (queue1Length < QUEUE_SIZE) {
					insertIntoQueue(&queue1Ptr, timestep * steps);
					queue1Length++;
				} else {
					totalPacketsDropped++;
				}
			} else  { // packet has been assigned to queue 2
				totalQueueLength += queue2Length;
				if (queue2Length < QUEUE_SIZE) {
					insertIntoQueue(&queue2Ptr, timestep * steps);
					queue2Length++;
				} else {
					totalPacketsDropped++;
				}
			}
		}
		if (exponentialGenerator(mu, timestep) && queue1Length > 0) {
			double waitingTime = timestep * steps - deleteFromQueue(&queue1Ptr);
			totalWaitingTime += waitingTime;
			queue1Length--;
		}
		if (exponentialGenerator(mu, timestep) && queue2Length > 0) {
			double waitingTime = timestep * steps - deleteFromQueue(&queue2Ptr);
			totalWaitingTime += waitingTime;
			queue2Length--;
		}
		steps++;
	}

	while (queue1Length > 0 || queue2Length > 0) {
		if (exponentialGenerator(mu, timestep) && queue1Length > 0) {
			totalWaitingTime += timestep * steps - deleteFromQueue(&queue1Ptr);
			queue1Length--;
		}
		if (exponentialGenerator(mu, timestep) && queue2Length > 0) {
			totalWaitingTime += timestep * steps - deleteFromQueue(&queue2Ptr);
			queue2Length--;
		}
		steps++;
	}

	avgQueueLength = ((double) totalQueueLength) / arrivals;
	printf("\nArrivals = %d\nTotal queue length = %d, TotalWaitingTime = %lf, TotalPacketsDropped = %d\n\n", arrivals, totalQueueLength, totalWaitingTime, totalPacketsDropped);
	avgWaitingTime = ((double) totalWaitingTime) / (arrivals - totalPacketsDropped);
	blockingProbability = ((double) totalPacketsDropped) / arrivals;
}


int exponentialGenerator(double lambda, double time) {
	// cdf = F(t) = 1 - exp(-lambda*time)
	double probability = 1 - exp(-lambda * time);
	double draw = ((double) (rand() % 1000000)) / 1000000;

	return draw <= probability ? 1 : 0;
}

int main(int args, char** argv) {
	int i = 0;
	int lambda, mu;

	printf("Please enter lambda (arrival rate): ");
	scanf("%d", &lambda);
	printf("Please enter mu (service rate): ");
	scanf("%d", &mu);
	printf("\n");

	double totalWaitingTime = 0, totalQueueLength = 0, totalBlockingProbability = 0;
	for (i = 0; i < ITERATIONS; i++) {
		srand((unsigned) time(NULL) + i * (rand() % ITERATIONS));
		simulate(lambda, mu);

		totalWaitingTime += avgWaitingTime;
		totalQueueLength += avgQueueLength;
		totalBlockingProbability += blockingProbability;
	}

	printf("\n--------------------------------------------------------------------\n");
	printf("Results are averaged over %d iterations:\n", ITERATIONS);
	printf("Average queue length: %lf\n", totalQueueLength / ITERATIONS);
	printf("Average waiting time: %lf\n", totalWaitingTime / ITERATIONS);
	printf("Blocking probability: %lf\n", totalBlockingProbability / ITERATIONS);
	printf("\n--------------------------------------------------------------------\n");
}
