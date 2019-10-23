// kk4036_hw15_Employee.cpp 

#include "pch.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

void open_inFile(ifstream &inFile);

template <class T>
class LList;

class Employee 
{
	int ID;
	double wage;
	string name;
	double numHrs;

public:
	// Constructor
	Employee(int newID = 0, double newWage = 0.00, string newName = "-----", double newHrs = 0.0) :
		ID(newID), wage(newWage), name(newName), numHrs(newHrs) {}

	// Destructor
	~Employee() {}

	// Methods (Setter Functions)
	void set_ID(int newID) { ID = newID; }
	void set_wage(double newWage) { wage = newWage; }
	void set_name(string newName) { name = newName; }
	void set_numHrs(double newHrs) { numHrs = newHrs; } 

	// Methods (Getter Functions)
	int get_ID() const { return ID; }
	double get_wage() const { return wage; }
	string get_name() const {return name; }	
	double get_numHrs() const {return numHrs; }

	// Additional Member Functions
	double get_pay() { return wage * numHrs; }
	void add_numHrs(double extra) { numHrs += extra; }

	// Overloading Operators
	friend ostream& operator<<(ostream& out, const Employee& emp);

	friend class Payroll;
	/*friend istream& operator>>(istream& in, Employee& emp) {
		int ID;
		double wage;
		string name;

		in >> ID; emp.set_ID(ID);
		in >> wage; emp.set_wage(wage);
		getline(in, name); emp.set_name(name);

		return in;
	}

	friend bool operator==(const Employee& emp1, const Employee& emp2) { return emp1.get_ID() == emp2.get_ID(); }

	friend bool operator!=(const Employee& emp1, const Employee& emp2) { return emp1.get_ID() != emp2.get_ID(); }

	friend bool operator<(Employee& emp1, Employee& emp2) { return emp1.get_pay() < emp2.get_pay(); }
	*/
};

ostream& operator<<(ostream& out, const Employee& emp) {
	out << '[' << emp.ID << "]  " << emp.name << "  $" << setprecision(2) << fixed << emp.wage << "  " << emp.numHrs << " hours";

	return out;
}

class Payroll
{
	string name;
	double payment;

public:
	// Constructor
	Payroll(string newName = "-----", double newPayment = 0.00) :
		name(newName), payment(newPayment) {}

	Payroll(const Employee& emp) : name(emp.name), payment(emp.wage*emp.numHrs) {}


	// Methods (getter functions)
	string get_name() const { return name; }
	double get_payment() const { return payment; }
	friend ostream &operator<<(ostream& out, const Payroll& pay);
};

ostream &operator<<(ostream& out, const Payroll& pay) {
	out << pay.name << "\t $" << setprecision(2) << fixed << pay.payment << endl;
	return out;
}


template <class T>
class Node 
{
	T info;
	Node<T>* prev;
	Node<T>* next;

public:
	// Constructor
	Node(T newInfo = T(), Node<T>* newPrev = NULL, Node<T>* newNext = NULL):
		info(newInfo), prev(newPrev), next(newNext) {}

	// Destructor
	~Node() {}

	// Methods (Setter Functions)
	void set_info(T newInfo) { info = newInfo; }
	void set_prev(Node<T>* newPrev) { prev = newPrev; }
	void set_next(Node<T>* newNext) { next = newNext; }

	// Methods (Getter Functions)
	T get_info() const { return info; }
	Node<T>* get_prev() const { return prev; }
	Node<T>* get_next() const { return next; }

	// friend functions / class
	friend class LList <T>;

	friend void update_empList(LList<Employee>& emp);

};


template <class T>
class LList
{
	Node<T>* head;

	// No longer need recursiveCopy
	//Node<T>* recursiveCopy(Node<T>* rhs) {
	//	//Base Case
	//	if (rhs == NULL) { return NULL; }
	//	// Recursive Step - taking an advantage of the Node constructor
	//	return new Node<T>(rhs->info, recursiveCopy(rhs->next));
	//}

public:
	// Constructor
	LList(): head(NULL) {}

	// Destructor
	~LList() { clear(head); }

	// Copy Constructor
	LList(const LList<T>& rhs);

	// Assignment Constructor
	LList<T> &operator=(const LList<T> &rhs);

	// Method (Setter Function)
	void set_head(Node<T>* newHead) { head = newHead; }

	// Method (Getter Function)
	Node<T>* get_head() const { return head; }

	// Additional Member Functions
	bool isEmpty() const { return head == NULL; }
	void clear(Node<T>* nodePtr);
	int size() const;
	void print_list();

	// Insert or Delete
	// 1) New node points to the ex-first node
	// 2) head points to the new node
	void push_head(T newInfo);

	//void push_front(const T& data) { 
	//	head->next = new LListNode<T>(data, head->next, head);  
	//	head->next->next->prev = head->next; 
	//}

	/*void push_any(Node<T>* nodePtr, T newInfo) {
		nodePtr->set_info(newInfo);
	}

	void push_back(T newInfo) {
		if (isEmpty()) {
			push_head(newInfo);
			return;
		}
		else {
			Node<T>* newBack = new Node<T>(newInfo);	
			//newBack->set_info(newInfo);
			// new endPtr pointing to head
			Node<T>* endPtr = head;
			// make ptr point to the last node
			while (endPtr->get_next() != NULL)
				endPtr = endPtr->get_next();	// i.e. end->next;
			endPtr->set_next(newBack);	// endPtr->next = newBack;
		}
	}
	//void push_back(const T& data) { 
	//	tail->prev = new LListNode<T>(data, tail, tail->prev); 
	//	tail->prev->prev->next = tail->prev; 
	//}

	// 1) Make Node* temp and pass the add of the first node
	// 2) Make the seconde node as the head
	// 3) Delete the temp
	void pop_head() {
		if (head == NULL) {
			cout << "Cannot delete the head because the list is empty." << endl;
			return;
		}
		else {
			Node<T>* temp = head;
			head = head->get_next();
			delete temp;
		}
	}

	// 1) Make Node* prev and Node* curr loop until they reach the locIdx
	// 2) Delete curr Node and pass the add of the one after curr to 
	//	  the prev node so Node* prev can point to the one after curr.
	void pop_any(int locIdx) {
		Node<T>* prev = new Node;
		Node<T>* curr = new Node;
		curr = head;

		for (int i = 1; i < locIdx; i++) {
			prev = curr;
			curr = curr->get_next();		//curr = curr->next;
		}
		prev->next = curr->next;
	}*/


};


void update_empList(LList<Employee>& emp) {
	string fileName;
	cout << "Enter the name of the file with working hours: ";
	cin >> fileName;

	ifstream in_hourFile(fileName);
	while (!in_hourFile) {
		cout << "Failed to open " << fileName << ". \n";
		cout << "Please enter the file name again: ";
		cin >> fileName;
		in_hourFile.clear();
		in_hourFile.open(fileName);
	}

	int currID;
	double currWorkHrs;
	while (in_hourFile >> currID) {
		in_hourFile >> currWorkHrs;

		Node<Employee>* iter = emp.get_head();
		while (iter != NULL) {
			if (iter->info.get_ID() == currID) {
				iter->info.add_numHrs(currWorkHrs);
			}
			iter = iter->get_next();
		}
	}
	in_hourFile.close();
}


template <class T>
LList<T>::LList(const LList<T>& rhs) {
	if (rhs.get_head() == NULL) { head = NULL; }

	else {
		head = new Node<T>(*(rhs.get_head()));
		Node<T>* nodePtrRhs = rhs.get_head()->get_next();
		Node<T>* nodePtr = head;
		while (nodePtrRhs != NULL) {
			nodePtr->set_next(new Node<T>(nodePtrRhs->get_info(), nodePtr, NULL));
			nodePtr = nodePtr->get_next();
			nodePtrRhs = nodePtrRhs->get_next();
		}
	}
}


template <class T>
LList<T>& LList<T>::operator=(const LList<T> &rhs) {
	LList<T> res;
	if (rhs.get_head() == NULL) { res.set_head(NULL); }
	else {
		res.set_head(new Node<T>(*(rhs.get_head())));
		Node<T>* nodePtrRhs = rhs.get_head()->get_next();
		Node<T>* nodePtr = head;
		while (nodePtrRhs != NULL) {
			nodePtr->set_next(new Node<T>(nodePtrRhs->get_info(), nodePtr, NULL));
			nodePtr = nodePtr->get_next();
			nodePtrRhs = nodePtrRhs->get_next();
		}
	}
	return res;
}

template <class T>
void LList<T>::clear(Node<T>* nodePtr) {	// while(!isEmpty() pop_head(); 
	if (nodePtr == NULL) { return; }
	else {
		clear(nodePtr->get_next());
		delete nodePtr;
	}
}

template <class T>
int LList<T>::size() const {
	int count = 0;
	Node<T>* temp = head;
	while (temp != NULL) {
		count++;
		temp = temp->next;
	}
	return count;
}

template <class T>
void LList<T>::print_list() {
	Node<T>* iter = head;
	while (iter != NULL) {
		cout << iter->get_info() << ' ';
		iter = iter->get_next();
		cout << endl;
	}
	cout << endl;
}

template <class T>
void LList<T>::push_head(T newInfo) {
	Node<T>* newHead = new Node<T>(newInfo, NULL, head);
	if (!isEmpty()) { head->set_prev(newHead); }
	head = newHead;
}


template <class T>
void swapEmp(T& emp1, T& emp2);
int findMaxIdx(vector<Payroll> payrollVector, int startIdx);
void sort(vector<Payroll>& payrollVector);
vector<Payroll> payrollObj(LList<Employee> emp);
void print_payroll(vector<Payroll> empVec) { for (Payroll pay : empVec) cout << pay; }
	/*for (unsigned int i = 0; i < empVec.size(); i++) {
		cout << empVec[i].get_name() << ", \t" << "$"; cout << setprecision(2) << fixed << empVec[i].get_pay() << endl;
	}*/


int main()
{
	// Opening Employee.txt file
	ifstream empInfo;
	open_inFile(empInfo);
	
	// Whilst reading from Employee.txt file, 
	// insert Emp ID, Pay Wage, and Name into the LList empList
	LList<Employee> empLList;
	Employee currEmp;
	int currID;
	double currWage;
	string currName;
	while (empInfo >> currID) {
		empInfo >> currWage;
		empInfo.ignore(999, ' ');
		getline(empInfo, currName);
		if (currName[currName.length() - 1] == '\r')
			currName.erase(currName.length() - 1, 1);
		currEmp = Employee(currID, currWage, currName);
		empLList.push_head(currEmp);
	}
	empInfo.close();

	cout << "Employee Information: \n";
	empLList.print_list();


	// Opening Payroll.txt file and updating empList w/ working hours
	update_empList(empLList);

	cout << "Payroll Information: \n";
	empLList.print_list();


	vector<Payroll> payrollVec = payrollObj(empLList);


	// Sorting the payrollDoc by pay (highest -> lowest)
	sort(payrollVec);

	//for (unsigned int i = 0; i < payrollDoc.size(); i++) {
	//	int maxIdx = i;
	//	for (unsigned int j = i + 1; j < payrollDoc.size(); j++) {
	//		if (payrollDoc[j].get_pay() > payrollDoc[maxIdx].get_pay())
	//			maxIdx = j;
	//	}
	//	if (i != maxIdx) {
	//		//swap(payrollToPrint[i], payrollToPrint[maxIndex]);
	//		//void swap(Employee& i, Employee& j)
	//		Employee temp = i;
	//		i = maxIdx;
	//		maxIdx = temp;
	//	}
	//}


	// Printing payrollDoc
	cout << "***** Payroll Information ********** \n";
	print_payroll(payrollVec);
	cout << "***** End Payroll ******************" << endl;

	return 0;
}

void open_inFile(ifstream &inFile) {
	string filename;
	cout << "Enter the name of the Employee file: ";
	cin >> filename;
	inFile.open(filename);

	while (!inFile) {
		cout << "Failed to open " << filename << ". \n";
		cout << "Please enter the file name again: ";
		cin >> filename;
		inFile.clear();
		inFile.open(filename);
	}
}


/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// class Employee /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// class LList ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Other Functions /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

template <class T>
void swapEmp(T& emp1, T& emp2) {
	T temp = emp1;
	emp1 = emp2;
	emp2 = temp;
}

int findMaxIdx(vector<Payroll> payrollVector, int startIdx) {
	int n = payrollVector.size();
	if (n == 0) { return -1; }
	else if (n == 1) { return 0; }
	else {
		double max = payrollVector[startIdx].get_payment();
		int maxIdx = startIdx;
		for (int i = startIdx + 1; i < n; i++) {
			if (payrollVector[i].get_payment() > max) {
				max = payrollVector[i].get_payment();
				maxIdx = i;
			}
		}
		return maxIdx;
	}
}

void sort(vector<Payroll>& payrollVector) {
	int n = payrollVector.size();
	int swapIdx;
	for (int i = 0; i < n - 1; i++) {
		swapIdx = findMaxIdx(payrollVector, i);
		swapEmp(payrollVector[i], payrollVector[swapIdx]);
	}
}

vector<Payroll> payrollObj(LList<Employee> emp) {
	vector<Payroll> payrollVec;
	Node<Employee>* iter = emp.get_head();
	while (iter != NULL) {
		payrollVec.push_back(Payroll(iter->get_info()));
		iter = iter->get_next();
	}
	return payrollVec;
}