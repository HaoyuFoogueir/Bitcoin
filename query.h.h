//ini update
#ifndef QUERY_H
#define QUERY_H

typedef struct Minor{
	char ip[20];
	int port;
	char name[20];
	struct Minor *next;
};

typedef struct Transaction{
	struct Minor* receipient;
	struct Minor* sender;
	double amount;
	struct Transaction* next, *prev;
};

typedef struct Ledger{
	int minor_number;
	struct Minor* minors;
	struct Transaction *head, *tail;
};

#endif