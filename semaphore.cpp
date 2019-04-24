
#include <vector>
#include <queue>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/* A Operating system structure for the critical section
 * Authors: Christian Jiminez, Richard Williams, Stephanie Van Dyke
 */




 //use process in all queues
struct process
{
	string id;
	int num;
};

struct semaphore {
	int count;
	queue<process> buffer;
};

//all global variables---------------------------
queue<process> critical;
process node;
semaphore majorLock;

void semSignal(semaphore *s)
{
	s->count++;
	if (s->count <= 0)
	{
			node = s->buffer.front();
			s->buffer.pop();
			critical.push(node);
	}
}

void semWait(semaphore *s)
{
	s->count--;
	if (s->count < 0)
	{
		s->buffer.push(node);
	}
}

//allows for easy making of a process
process pMaker(int c)
{
	stringstream s;
	s << "P" << c << "";
	string a = s.str();
	int b = (rand() % 50 + 1);
	process p = { a,b };
	return  p;
};


// Wraps the priority-queue in a container to iterate over later 
template <class T, class S, class C>
S& Container(priority_queue<T, S, C>& q) {
	struct PQueue : private priority_queue<T, S, C> {
		static S& Container(priority_queue<T, S, C>& q) {
			return q.*&PQueue::c;
		}
	};
	return PQueue::Container(q);
}

//used to compare processess
struct compareProcess {
	bool operator()(process const& a, process const& b)
	{
		// return "true" if "p1" is ordered  
		// before "p2", for example: 
		return a.num > b.num;
	}
};

// print queues
void printQueue(queue<process> q)
{
	while (!q.empty())
	{
		process a = q.front();
		cout << " " << a.id << ":" << a.num << " ";
		q.pop();
	}
	cout << endl;
}

int main()
{
	//variable set up----------------------------------------------------------------
	priority_queue<process, vector<process>, compareProcess> pq;	// priority queue
	vector<process> &tasks = Container(pq);	// priority queue in container
	int counter = 1;

	majorLock.count = 0;

	//fills the priority queue
	for (int i = 0; i < 9; i++)
	{
		pq.push(pMaker(counter));
		counter++;
	}

	int step = 0;


	//while loop steps through all queues and decrements process in critical section until all queues are empty
	while (pq.size() > 0 || critical.size() > 0 || majorLock.buffer.size()>0)
	{
		cout << "step: " << step << endl;
		cout << endl;
		step++;

		//process generator creates 25 process
		if (pq.size() < 10 && counter <= 25)
		{
			process a = pMaker(counter);
			counter++;

			cout << "the new process generated is: ";
			cout << a.id << " with priority " << a.num << endl;
			cout << endl;
			pq.push(a);
		}


		cout << "The processes in the priority queue are: ";
		if (pq.size() > 0)
		{
			for (vector<process>::iterator i = tasks.begin(); i != tasks.end(); i++)
			{
				cout << i->id << ":" << i->num << " ";
			}
			cout << endl;
		}else
		{
			cout << endl;
		}
		cout << endl;


		//semaphore section-----------------------------------------------
		if (pq.size() > 0)
		{
			node = pq.top(); //sets global process node
			pq.pop();
			semWait(&majorLock); //takes node to buffer
		}

		if (critical.size()<= 0)
		{
			semSignal(&majorLock); //brings front of buffer to critical section if available
		}


		//print section---------------------------------------------------
		cout << "the process in the critical section is: ";
		if (critical.size() > 0)
		{
			printQueue(critical);
		}else
		{
			cout << endl;
		}
		cout << endl;

		cout << "the processes in the buffer are: ";
		if (majorLock.buffer.size() > 0)
		{
			printQueue(majorLock.buffer);
		}
		else
		{
			cout << endl;
		}
		cout << endl;

		//decriments the number in the process of the critical section
		if (critical.size() > 0)
		{
			critical.front().num -= 2;

			if (critical.front().num <= 0)
			{
				critical.pop(); //removes finished processes
			}
		}
	}
	system("pause");
} 