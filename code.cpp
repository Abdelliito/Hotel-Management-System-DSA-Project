#include<iostream>
#include<string>
#include<conio.h>
using namespace std;

class room 
{
public:
	int id;
	string type;
	string status;
	room* next;

	room(int roomid,string roomtype,string roomstatus) {
		id = roomid;
		type = roomtype;
		status = roomstatus;
		next = nullptr;
	}
};

class Floor 
{
public:
	int floornumber;
	room* roomlist;
	Floor* next;

	Floor(int floornum) {
		floornumber = floornum;
		roomlist = nullptr;
		next = nullptr;
	}
};

class bookingrequest {
public:
	string costumername;
	string roomtype;
	int nights;
	bool highpriority;
	bookingrequest* next;

	bookingrequest(string costname, string roomty, int numnights, bool highpr) {
		costumername = costname;
		roomtype = roomty;
		nights = numnights;
		highpriority = highpr;
		next = nullptr;
	}
};

class bookinghistory {
public:
	string costumername;
	int roomid;
	int nights;
	bookinghistory* next;

	bookinghistory(string costname, int id, int numnights) {
		costumername = costname;
		roomid = id;
		nights = numnights;
		next = nullptr;
	}
};

class hotelmanagement
{
private:
	Floor* floors;
	bookingrequest* requestqueue;
	bookinghistory* historystack;
	int maxfloors;
	int roomsperfloor;
	string roomstype;
	int requestqueuesize;

public:
	hotelmanagement(int f, int n,string t) {
		floors = nullptr;
		requestqueue = nullptr;
		historystack = nullptr;
		maxfloors = f;
		roomsperfloor = n;
		roomstype = t;
		requestqueuesize = 0;
		initializefloor();
	}

	void initializefloor()
	{
		if (roomstype == "Single")
		{
			for (int i = 1; i <= maxfloors; i++)
			{
				Floor* newFloor = new Floor(i);
				newFloor->next = floors;
				floors = newFloor;

				for (int j = 1; j <= roomsperfloor; j++)
				{
					room* newroom = new room((i * 100) + j, "Single", "Ready");
					newroom->next = newFloor->roomlist;
					newFloor->roomlist = newroom;
				}
			}
		}

		else if (roomstype == "Double")
		{
			for (int i = 1; i <= maxfloors; i++)
			{
				Floor* newFloor = new Floor(i);
				newFloor->next = floors;
				floors = newFloor;

				for (int j = 1; j <= roomsperfloor; j++)
				{
					room* newroom = new room((i * 100) + j, "Double", "Ready");
					newroom->next = newFloor->roomlist;
					newFloor->roomlist = newroom;
				}
			}
		}

		else if (roomstype == "Suite")
		{
			for (int i = 1; i <= maxfloors; i++)
			{
				Floor* newFloor = new Floor(i);
				newFloor->next = floors;
				floors = newFloor;

				for (int j = 1; j <= roomsperfloor; j++)
				{
					room* newroom = new room((i * 100) + j, "Suite", "Ready");
					newroom->next = newFloor->roomlist;
					newFloor->roomlist = newroom;
				}
			}
		}
		else
		{
			cout << "you entered wrong type of room" << endl;
		}
	}

	void displayrooms()
	{
		Floor* currentFloor = floors;
		while (currentFloor)
		{
			cout << "Floor " << currentFloor->floornumber << ":\n";
			room* currentRoom = currentFloor->roomlist;
			while (currentRoom)
			{
				cout << "  Room ID: " << currentRoom->id
					<< ", Type: " << currentRoom->type
					<< ", Status: " << currentRoom->status << endl;
				currentRoom = currentRoom->next;
			}
			currentFloor = currentFloor->next;
		}
	}

	void addbookingrequest(string customerName, string roomType, int nights, bool highPriority)
	{
		bookingrequest* newRequest = new bookingrequest(customerName, roomType, nights, highPriority);
		if (!requestqueue || highPriority)
		{
			newRequest->next = requestqueue;
			requestqueue = newRequest;
		}
		else
		{
			bookingrequest* temp = requestqueue;
			while (temp->next) temp = temp->next;
			temp->next = newRequest;
		}
		requestqueuesize++;

		if (requestqueuesize == 10)
			processBookingRequests();
	}

	// Process booking requests
	void processBookingRequests()
	{
		while (requestqueue)
		{
			bookingrequest* request = requestqueue;
			requestqueue = requestqueue->next;

			room* availableRoom = findAvailableRoom(request->roomtype);
			if (availableRoom)
			{
				availableRoom->status = "Occupied";
				cout << "Booking successful for " << request->costumername << " in Room " << availableRoom->id << " for " << request->nights << " nights.\n";
				addBookingToHistory(request->costumername, availableRoom->id, request->nights);
			}
			else
			{
				cout << "No available rooms for " << request->roomtype << " at the moment.\n";
			}
			delete request;
			requestqueuesize--;
		}
	}

	// Find an available room
	room* findAvailableRoom(string roomType)
	{
		Floor* currentFloor = floors;
		while (currentFloor)
		{
			room* currentroom = currentFloor->roomlist;
			while (currentroom)
			{
				if (currentroom->type == roomType && currentroom->status == "Ready")
				{
					return currentroom;
				}
				currentroom = currentroom->next;
			}
			currentFloor = currentFloor->next;
		}
		return nullptr;
	}

	// Add booking to history
	void addBookingToHistory(string customerName, int roomID, int nights)
	{
		bookinghistory* newHistory = new bookinghistory(customerName, roomID, nights);
		newHistory->next = historystack;
		historystack = newHistory;
	}

	// Display booking history
	void displayBookingHistory()
	{
		bookinghistory* current = historystack;
		while (current)
		{
			cout << "Customer: " << current->costumername
				<< ", Room ID: " << current->roomid
				<< ", Nights: " << current->nights << endl;
			current = current->next;
		}
	}

	// Destructor
	~hotelmanagement()
	{
		while (floors)
		{
			Floor* tempFloor = floors;
			floors = floors->next;

			room* currentRoom = tempFloor->roomlist;
			while (currentRoom)
			{
				room* tempRoom = currentRoom;
				currentRoom = currentRoom->next;
				delete tempRoom;
			}
			delete tempFloor;
		}

		while (requestqueue)
		{
			bookingrequest* tempRequest = requestqueue;
			requestqueue = requestqueue->next;
			delete tempRequest;
		}

		while (historystack)
		{
			bookinghistory* tempHistory = historystack;
			historystack = historystack->next;
			delete tempHistory;
		}
	}
};

int main()
{
	

	int floors, roomsPerFloor;
	string types;

	cout << "Enter the number of floors: ";
	cin >> floors;
	cout << "Enter the number of rooms per floor: ";
	cin >> roomsPerFloor;
	cout << "Enter the types of room in your Hotel(Single/Double/Suite): ";
	cin >> types;
	system("cls");

	hotelmanagement hotel(floors, roomsPerFloor,types);

	cout << endl;
	cout << "\t ===============================" << endl;
	cout << "\t |                             |" << endl;
	cout << "\t |                             |" << endl;
	cout << "\t |         Welcome to          |" << endl;
	cout << "\t |      Hotel Management       |" << endl;
	cout << "\t |           System            | " << endl;
	cout << "\t |                             |" << endl;
	cout << "\t ===============================" << endl;
	cout << endl;
	cout << "\t Press any key to continue..." << endl;
	_getch();
	system("cls");

	int choice;
	do
	{
		cout << endl;
		cout << "1. Display Rooms" << endl;
		cout << "2. Add Booking Request" << endl;
		cout << "3. Process Booking Requests" << endl;
		cout << "4. Display Booking History" << endl;
		cout << "5. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice)
		{
		case 1:
			hotel.displayrooms();
			break;
		case 2:
		{
			string name, roomType;
			int nights;
			bool highPriority;
			cout << "Enter customer name: ";
			cin >> name;
			cout << "Enter room type (Single/Double/Suite): ";
			cin >> roomType;
			cout << "Enter number of nights: ";
			cin >> nights;
			cout << "Is this a high-priority request? (1 for Yes, 0 for No): ";
			cin >> highPriority;
			hotel.addbookingrequest(name, roomType, nights, highPriority);
			break;
		}
		case 3:
			hotel.processBookingRequests();
			break;
		case 4:
			hotel.displayBookingHistory();
			break;
		case 5:
			cout << "Exiting the system.\n";
			break;
		default:
			cout << "Invalid choice. Try again.\n";
		}
	} while (choice != 5);

	return 0;
}
