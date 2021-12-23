#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <math.h>
#include <algorithm>

using namespace std;

const static char* ws = " \t\n\r\f\v";
int typer = 1;
string inputStr;


static inline string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from beginning of string (left)
static inline string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

static bool isNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

static int maxLength(std::vector <std::string> vec) {
	if (vec.size() == 0) return 0;

	int max = vec[0].length();

	for (int i = 1; i < vec.size(); i++)
		if (max < vec[i].length()) max = vec[i].length();

	return max;
}

static vector <std::string> split(std::string s, const char splitter = ' ', bool ignoreQuotes = false);

// trim from both ends of string (right then left)
static inline string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
	return ltrim(rtrim(s, t), t);
}

static string toLower(string s) {
	for_each(s.begin(), s.end(), [](char& c) {
		c = tolower(c);
		});

	return s;
}

static bool replace(string& str, const string& from, const string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

vector <string> split(string s, const char splitter, bool ignoreQuotes) {
	if (s == "")
		return vector<string>();

	bool inQuotes = false;
	int start = 0;
	vector <string> parts;

	for (unsigned int i = 0; i <= s.length(); i++) {
		if (s[i] == splitter || i == s.length()) {
			if (inQuotes && !ignoreQuotes) continue;

			string piece = s.substr(start, i - start);
			parts.push_back(trim(piece));
			start = i + 1;
		}
		if (s[i] == '\"')
			inQuotes = !inQuotes;
	}

	return parts;
}

pair<int, string> getLeftPart(string s, const char splitter, bool ignoreQuotes) {
	bool inQuotes = false;

	if (s.length() == 0)
		return make_pair(0, "");

	for (unsigned int i = 0; i <= s.length(); i++) {
		if (s[i] == splitter || i == s.length()) {
			if (inQuotes && !ignoreQuotes) continue;

			string piece = s.substr(0, i);

			return make_pair(i - 1, piece);
		}
		if (s[i] == '\"')
			inQuotes = !inQuotes;
	}

	return make_pair(0, "");
}
pair<int, string> getRightPart(string s, const char splitter, bool ignoreQuotes) {
	bool inQuotes = false;

	if (s.length() == 0)
		return make_pair(0, "");

	for (unsigned int i = s.length() - 1; i >= 0; i--) {
		if (s[i] == splitter || i == 0) {
			if (inQuotes && !ignoreQuotes) continue;

			string piece = s.substr(i + 1);

			return make_pair(i - 1, piece);
		}
		if (s[i] == '\"')
			inQuotes = !inQuotes;
	}

	return make_pair(0, "");
}

class Table
{

private:
	string name = "";

	map <string, int> titles;
	vector <vector<string>> rows;
	vector <int> ids;

	void drawBorder(int colsN, vector <int> colWidths) {
		for (int i = 0; i < colsN; i++) {
			cout << "+";
			for (int j = 0; j < colWidths[i] + 3; j++)
				cout << "-";
		}
		cout << "+" << endl;
	}
	void drawRow(vector <string> row, vector <int> colWidths) {
		for (int i = 0; i < row.size(); i++) {
			cout << setw(colWidths[i] + 2) << row[i] << " ";
		}
		cout << endl;
	}
	void drawTable(vector <string> columns, vector <vector <string>> values) {
		bool isEmpty = !values.size();

		int colsN = columns.size();

		vector <int> colWidths;

		if (isEmpty) {
			for (int i = 0; i < columns.size(); i++)
				colWidths.push_back(columns[i].length());
		}
		else {
			for (int i = 0; i < columns.size(); i++) {
				if (columns[i] == "id") {
					colWidths.push_back(max<int>(to_string(ids[ids.size() - 1]).length(), 2));
					continue;
				}

				int len = max<int>(maxLength(getColumn(titles[columns[i]])), columns[i].length());
				colWidths.push_back(len);
			}
		}

		// drawBorder(colsN, colWidths);
		drawRow(columns, colWidths);
		// drawBorder(colsN, colWidths);

		for (int i = 0; i < values.size(); i++) {
			drawRow(values[i], colWidths);
			// drawBorder(colsN, colWidths);
		}
	}
	vector <string> getColumn(int colID) {
		vector <string> col;

		for (int i = 0; i < (int)rows.size(); i++)
			col.push_back(rows[i][colID]);

		return col;
	}

public:
	Table(string n = "", vector <string> columns = {}) {
		this->name = n;

		for (int i = 0; i < columns.size(); i++)
			titles[columns[i]] = i;
	}

	string getName() {
		return this->name;
	}
	void setName(string n) {
		this->name = n;
	}
	bool clear() {
		rows = {};
		ids = {};

		return true;
	};
	bool addRow(vector <string> row) {
		rows.push_back(row);
		return true;
	}
	bool removeRow(int id);
	bool showRows(vector <string> columns, int id = 0) {
		std::vector <int> colIndexes;
		std::vector <vector<string>> usedRows;

		for (auto col : columns) {

			if (titles.find(col) != titles.end())
				colIndexes.push_back(titles[col]);

			else {
				cerr << "ERROR: Column \"" << col << "\" not found!" << endl;
				return false;
			}
		}

		columns.insert(columns.begin(), "id");

		for (int i = 0; i < rows.size(); i++) {
			usedRows.push_back(vector<string>());

			usedRows[i].push_back(to_string(ids[i]));

			for (int col : colIndexes)
				usedRows[i].push_back(rows[i][col]);
		}

		drawTable(columns, usedRows);

		return true;
	}

	int execute(string command) {
		command = toLower(trim(command));

		auto firstPart = getLeftPart(command, ' ', true);

		if (firstPart.second == "show") {
			command = command.substr(firstPart.first + 1);
			command = trim(command);

			if (command.length() == 0 || command[0] != '(' || command[command.length() - 1] != ')') {
				cerr << "ERROR: Missing arguments in command SHOW.\n\tIN [table name] SHOW (col1, col2, col3 ...)" << endl;
				return -1;
			}

			command = trim(command, "()");

			vector <string> columns = split(command, ',');

			if (columns.size() == 0)
			{
				cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
				return -1;
			}

			for (auto& col : columns) {
				if (col == "")
				{
					cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
					return -1;
				}
				col = trim(col, "\"");
			}

			if (!showRows(columns)) {
				return -1;
			}
		}
		else if (firstPart.second == "delete") {
			command = command.substr(firstPart.first + 1);
			command = trim(command);

			if (isNumber(command) == false)
			{
				cerr << "ERROR: Incorrect arguments in command DELETE.\n\tIN [table name] DELETE [row id]" << endl;
				return -1;
			}

			int row_id = stoi(command);
			auto ids_iter = find(ids.begin(), ids.end(), row_id);

			if (ids_iter == ids.end()) {
				cerr << "ERROR: Row with id=" << row_id << " doesn't exists!" << endl;
				return -1;
			}

			rows.erase(rows.begin() + (ids_iter - ids.begin()));
			ids.erase(ids_iter);

			cout << "SUCCESS: Row with id \"" << row_id << "\" deleted!" << endl;
		}
		else if (firstPart.second == "add") {
			command = command.substr(firstPart.first + 1);
			command = trim(command);

			command = trim(command, "()");

			if (command == "")
			{
				cerr << "ERROR: Incorrect arguments in command ADD.\n\tIN [table name] ADD (value1, value2, ...)" << endl;
				return -1;
			}

			if (ids.empty()) ids.push_back(1);
			else             ids.push_back(ids[ids.size() - 1] + 1);

			vector <string> values = split(command, ',');

			if (values.size() != titles.size())
			{
				cerr << "ERROR: " << values.size() << " values given, " << titles.size() << " expected" << endl;
				return -1;
			}

			for (auto& val : values) {
				val = trim(val, "\"");
			}

			addRow(values);

			cout << "SUCCESS: Row added to table!" << endl;
		}
		else if (firstPart.second == "edit") {
			command = command.substr(firstPart.first + 1);
			command = trim(command);

			cout << command << endl;

			vector <string> args = split(command, ' ');
			if (args.size() < 3)
			{
				cerr << "ERROR: Incorrect arguments in command EDIT.\n\tIN [table name] EDIT [column] [value] [id]" << endl;
				return -1;
			}

			for (auto& arg : args) {
				arg = trim(arg, "\"");
			}

			if (isNumber(args[2]) == false)
			{
				cerr << "ERROR: Incorrect arguments in command EDIT.\n\tIN [table name] EDIT [column] [value] [id]" << endl;
				return -1;
			}

			int id = stoi(args[2]);
			auto ids_iter = find(ids.begin(), ids.end(), id);

			if (ids_iter == ids.end()) {
				cerr << "ERROR: Row with id=" << id << " doesn't exists!" << endl;
				return -1;
			}

			int index = (ids_iter - ids.begin());

			if (titles.find(args[0]) == titles.end())
			{
				cerr << "ERROR: Column \"" << args[0] << "\" doesn't exists!" << endl;
				return -1;
			}

			rows[index][titles[args[0]]] = args[1];

			cout << "SUCCESS: Value in row \"" << id << "\" and column \"" << args[0] << "\" changed to \"" << args[1] << "\"!" << endl;
		}

		return 1;
	}
};

class Database
{
private:
	vector <Table*> tables;

	Table* findTableByName(string name) {
		for (int i = 0; i < tables.size(); i++)
			if (tables[i]->getName() == name) return tables[i];

		return nullptr;
	}
public:
	Database() {}
	int execute(string command) {
		command = toLower(trim(command));

		if (command[command.length() - 1] == ';')
			command = command.substr(0, command.length() - 1);

		auto firstPart = getLeftPart(command, ' ', true);

		if (firstPart.second == "create") {
			command = command.substr(firstPart.first + 1);
			command = trim(command);

			if (command.length() == 0) {
				cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
				return -1;
			}

			auto namePart = getLeftPart(command, ' ', true);

			string name = namePart.second;
			name = trim(name, "\"");

			if (findTableByName(name) != nullptr) {
				cerr << "ERROR: Table \"" << name << "\" already exists!" << endl;
				return -1;
			}

			command = command.substr(namePart.first + 1);
			command = trim(command);

			if (command.length() == 0 || command[0] != '(' || command[command.length() - 1] != ')') {
				cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
				return -1;
			}

			command = trim(command, "()");

			vector <string> columns = split(command, ',');

			if (columns.size() == 0)
			{
				cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
				return -1;
			}

			for (auto& col : columns) {
				if (count(columns.begin(), columns.end(), col) > 1) {
					cerr << "ERROR: Duplicate column name \"" << col << "\"" << endl;
					return -1;
				}
				if (col == "")
				{
					cerr << "ERROR: Missing arguments in command CREATE.\n\tCREATE [table name] (col1, col2, col3 ...)" << endl;
					return -1;
				}

				col = trim(col, "\"");
			}

			tables.push_back(new Table(name, columns));

			cout << "SUCCESS: Table \"" << name << "\" created with " << columns.size() << " columns!" << endl;
			return 1;

		}
		else if (firstPart.second == "delete") {
			command = command.substr(firstPart.first + 1);
			command = trim(command);

			if (command.length() == 0) {
				cerr << "ERROR: Missing arguments in command DELETE.\n\tDELETE [table]" << endl;
				return -1;
			}

			string name = command;
			name = trim(name);
			name = trim(name, "\"");

			if (name == "")
			{
				cerr << "ERROR: Missing arguments in command DELETE.\n\tDELETE [table]" << endl;
				return -1;
			}

			Table* table = findTableByName(name);

			if (table == nullptr)
			{
				cerr << "ERROR: Table \"" << name << "\" not found!" << endl;
				return -1;
			}

			tables.erase(std::find(tables.begin(), tables.end(), table));
			delete table;
			cout << "SUCCESS: Table \"" << name << "\" deleted!" << endl;
			return 1;
		}
		else if (firstPart.second == "clear") {
			command = command.substr(firstPart.first + 1);
			command = trim(command);

			if (command.length() == 0) {
				cerr << "ERROR: Missing arguments in command CLEAR.\n\tCLEAR [table]" << endl;
				return -1;
			}

			string name = command;
			name = trim(name);
			name = trim(name, "\"");

			if (name == "")
			{
				cerr << "ERROR: Missing arguments in command CLEAR.\n\tCLEAR [table]" << endl;
				return -1;
			}

			Table* table = findTableByName(name);

			if (table == nullptr)
			{
				cerr << "ERROR: Table \"" << name << "\" not found!" << endl;
				return -1;
			}

			table->clear();
			cout << "SUCCESS: Table \"" << name << "\" cleared!" << endl;
		}
		else if (firstPart.second == "rename") {
			command = command.substr(firstPart.first + 1);
			command = trim(command);

			auto names = split(command, ' ');
			if (names.size() < 2) {
				cerr << "ERROR: Missing arguments in command RENAME.\n\tRENAME [old name] [new name]" << endl;
				return -1;
			}

			string oldName = names[0], newName = names[1];
			oldName = trim(trim(oldName), "\"");
			newName = trim(trim(newName), "\"");

			// cout << "Rename " << oldName << " to " << newName << endl;

			Table* table = findTableByName(oldName);

			if (table == nullptr)
			{
				cerr << "ERROR: Table \"" << oldName << "\" not found!" << endl;
				return -1;
			}

			if (findTableByName(newName) != nullptr) {
				cerr << "ERROR: Table \"" << newName << "\" already exists!" << endl;
				return -1;
			}

			table->setName(newName);
			cout << "SUCCESS: Table \"" << oldName << "\" renamed to \"" << newName << "\"!" << endl;
			return 1;
		}
		else if (firstPart.second == "exit") {
			return 0;
		}
		else if (firstPart.second == "list") {
			cout << "Tables: " << endl;

			for (int i = 0; i < tables.size(); i++) {
				cout << "\t" << i + 1 << ". " << tables[i]->getName() << endl;
			}
			if (tables.size() == 0)
				cout << "\tNo tables created yet!" << endl;
		}

		else if (firstPart.second == "in") {
			command = command.substr(firstPart.first + 1);
			command = trim(command);

			auto namePart = getLeftPart(command, ' ', true);
			string name = namePart.second;
			name = trim(name);
			name = trim(name, "\"");

			if (name == "")
			{
				cerr << "ERROR: Missing arguments in command IN.\n\tIN [table] ADD/EDIT/DELETE/SHOW" << endl;
				return -1;
			}

			Table* table = findTableByName(name);

			if (table == nullptr) {
				cerr << "ERROR: Table \"" << name << "\" not found!" << endl;
				return -1;
			}

			command = command = command.substr(namePart.first + 1);
			command = trim(command);

			return table->execute(command);
		}

		else if (command == "") return -1;

		else {
			cerr << "ERROR: Unknown command \"" << firstPart.second << "\"" << endl;
		}

		return 1;
	}
};

int main() {
	Database* baza = new Database;
	while (typer) {
		getline(cin, inputStr);
		typer = baza->execute(inputStr);
	}
}