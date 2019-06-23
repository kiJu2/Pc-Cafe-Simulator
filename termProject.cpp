#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <typeinfo>
#include <fstream>
#include <time.h>
#include <ctype.h>

//Color표
#define BLACK 0 
#define BLUE 1 
#define GREEN 2 
#define CYAN 3 
#define RED 4 
#define MAGENTA 5 
#define BROWN 6 
#define LIGHTGRAY 7 
#define DARKGRAY 8 
#define LIGHTBLUE 9 
#define LIGHTGREEN 10 
#define LIGHTCYAN 11 
#define LIGHTRED 12 
#define LIGHTMAGENTA 13 
#define YELLOW 14 
#define WHITE 15

using namespace std;
#define MAX_Y 13
#define MAX_X 13

#define DOS_WIDTH 150
#define DOS_HEIGHT 40

#define PC_MAP_AREA_WIDTH DOS_WIDTH / 2 + 1
#define PC_MAP_AREA_HEIGHT DOS_HEIGHT * 0.75
#define PC_MAP_AREA_X 0
#define PC_MAP_AREA_Y 0

#define INFO_AREA_WIDTH DOS_WIDTH - PC_MAP_AREA_WIDTH
#define INFO_AREA_HEIGHT PC_MAP_AREA_HEIGHT
#define INFO_AREA_X PC_MAP_AREA_WIDTH + 2
#define INFO_AREA_Y 0

#define INFO_REMAINING_AREA_X DOS_WIDTH - 23
#define INFO_REMAINING_AREA_Y INFO_AREA_Y + 4
#define INFO_REMAINING_TEXT_AREA_X INFO_REMAINING_AREA_X + 3
#define INFO_REMAINING_TEXT_AREA_Y INFO_REMAINING_AREA_Y + 1

#define INPUT_AREA_X 0
#define INPUT_AREA_Y PC_MAP_AREA_HEIGHT + 1
const int INPUT_AREA_HEIGHT = DOS_HEIGHT - INPUT_AREA_Y - 2;

#define _INPUT_AREA_X 0
#define _INPUT_AREA_Y DOS_HEIGHT - 1

#define MAX_PC_NUM_X 19
#define MAX_PC_NUM_Y 15

#define X_WIDTH 4
#define Y_HEIGHT 2

class SelectArea;
//현재 도스 커서 좌표값 가져오기
COORD getXY() {
	COORD Cur;
	CONSOLE_SCREEN_BUFFER_INFO a;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &a);
	Cur.X = a.dwCursorPosition.X;
	Cur.Y = a.dwCursorPosition.Y;
	return Cur;
	//출처 : https://wowan.tistory.com/5
}
//글자색 입히기
void textColor(int foreground, int background){
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
//콘솔 커서 좌표 이동
void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//콘솔 창의 크기 조절
//system("mode ")를 사용하여 스크린 크기를 조절하려 하였지만 환경변수 중 path를 설정하여야 하기에 타 사용자 OS 환경에서 원활한 실행이 되지 않을 수 있다고 판단하여
//새롭게 코딩을 하려 하였습니다 하지만 저의 기술력이 부족하기에 외부 플랫폼에서 함수를 가지고 왔습니다.
void SetWindow(int Width, int Height)
{
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;

	// Get handle of the standard output 
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (Handle == NULL)
	{
		cout << "Failure in getting the handle\n" << GetLastError();
	}

	// Set screen buffer size to that specified in coord 
	if (!SetConsoleScreenBufferSize(Handle, coord))
	{
		cout << "Failure in setting buffer size\n" << GetLastError();
	}

	// Set the window size to that specified in Rect 
	if (!SetConsoleWindowInfo(Handle, TRUE, &Rect))
	{
		cout << "Failure in setting window size\n" << GetLastError();
	}
	//출처 : https://www.3dbuzz.com/forum/threads/102195-How-do-you-resize-a-console-window-in-C
}

//-----------------------------Print Display---------------------------------
//하단부에 텍스트를 출력.
void printInputArea(string state) {
	COORD currentCursor = getXY();

	gotoxy(INPUT_AREA_X, INPUT_AREA_Y);
	cout << state <<  endl;

	gotoxy(currentCursor.X, currentCursor.Y);
}
//현재 값을 InfoArea에 출력(테스트 용도임)
char saveY = 0;
void printTestInInfoArea(string state, int info) {
	COORD currentCursor = getXY();

	gotoxy(INFO_AREA_X, saveY);
	cout << state  << " : " << info << endl;
	saveY++;

	gotoxy(currentCursor.X, currentCursor.Y);
}

void printTestInInfoArea(string state) {
	COORD currentCursor = getXY();

	gotoxy(INFO_AREA_X, saveY);
	cout << state << endl;
	saveY++;

	gotoxy(currentCursor.X, currentCursor.Y);
}

void printTestInInfoArea(int state) {
	COORD currentCursor = getXY();

	gotoxy(INFO_AREA_X, saveY);
	cout << state << endl;
	saveY++;

	gotoxy(currentCursor.X, currentCursor.Y);
}

//InputArea의 화면을 지움
void printClearInputArea() {
	gotoxy(INPUT_AREA_X, INPUT_AREA_Y);
	for (int y = 0; y < DOS_WIDTH; y++) {
		for (int x = 0; x < INPUT_AREA_HEIGHT - 2; x++) {
			cout << ' ';
		}
	}
	gotoxy(INPUT_AREA_X, INPUT_AREA_Y);
}
//IA의 화면을 지움
void printClearIA() {
	for (int line = 0; line <= 23; line++) {
		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + 3 + line);
		for (int i = 0; i < 47; i++) {
			cout << ' ';
		}
	}
	
}
//pcMapArea의 화면을 지움
void printClearPcMapArea() {
	gotoxy(PC_MAP_AREA_X, PC_MAP_AREA_Y);
	for (int y = 0; y <= PC_MAP_AREA_HEIGHT; y++) {
		for (int x = 0; x < PC_MAP_AREA_WIDTH; x++) {
			cout << ' ';
		}
		gotoxy(PC_MAP_AREA_X, y);
	}
	gotoxy(PC_MAP_AREA_X, PC_MAP_AREA_Y);
}
//도스의 레이아웃을 그려줌
string infoLayout[] = {"▒▒▒▒▒▒▒▒▒▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒                ▒",
					"▒▒▒▒▒▒▒▒▒▒"};
void clearLayout() {
	int lineInc = 0;
	for (auto line : infoLayout) {
		gotoxy(INFO_REMAINING_AREA_X, INFO_REMAINING_AREA_Y + lineInc);
		cout << line;
		lineInc++;
	}
}
void printLayout() {
	string myGit = "git : github.com/kiJu2";

	gotoxy(DOS_WIDTH - myGit.length() - 1, INPUT_AREA_Y - 2);
	cout << myGit;
	int lineInc = 0;
	for (auto line : infoLayout) {
		gotoxy(INFO_REMAINING_AREA_X, INFO_REMAINING_AREA_Y + lineInc);
		cout << line;
		lineInc++;
	}
	gotoxy(INPUT_AREA_X, DOS_HEIGHT - 2);
	for (int dosX = INPUT_AREA_X; dosX < DOS_WIDTH; dosX++) {
		cout << "_";
	}
	for (int dosY = 0; dosY < PC_MAP_AREA_HEIGHT; dosY++) {
		gotoxy(PC_MAP_AREA_WIDTH, dosY);
		cout << "│";
	}
	for (int dosX = 0; dosX < DOS_WIDTH; dosX++) {
		gotoxy(dosX, PC_MAP_AREA_HEIGHT);
		cout << "─";
	}
	
	gotoxy(PC_MAP_AREA_WIDTH, PC_MAP_AREA_HEIGHT);
	cout << "┴";
}

//-----------------------------library tool----------------------------------
int sepsNextValue(string str, char seps) {
	int value;
	int cnt = 0;
	
	//일부 데이터 값을 불러옴.
	for (char ch : str) {
		if (ch == seps) {
			value =  stoi(str.substr(cnt + 1, str.size()));
			return value;
		}
		cnt++;
	}
	return -1;
}
//-----------------------------Setting---------------------------------------
enum { EMPTY, USING, WALL, SELECT_EMPTY, SELECT_USING, WARNING };
bool timerOneSecond() {
	static time_t currentSeconds, conditionSeconds;
	static bool flag = false;

	currentSeconds = time(NULL);
	if (!flag) {
		conditionSeconds = currentSeconds + 1;
		flag = true;
	}
	if (currentSeconds >= conditionSeconds) {
		flag = false;
		return true;
	}
	return false;
}
class SelectArea;
class User {
private:
	int fare;
	time_t startTime;
	time_t endTime;
	int remainingTime;
public:
	User() { 
		startTime = 0;
		endTime = 0;
		remainingTime = 0;
		fare = 0; 
	}
	friend int toMinute(int time);
	friend int toHour(int time);

	void reset(int _fare, time_t _startTime, time_t _endTime, int _remainingTime) {
		fare = _fare;
		startTime = _startTime;
		endTime = _endTime;
		remainingTime = _remainingTime;
	}

	void reset() { 
		fare = 0; 
		startTime = 0;
		endTime = 0;
		remainingTime = 0;
	}
	void setFare(int _fare) { fare += _fare; }
	void setStartTime(int _startTime) { startTime = _startTime; }
	void setEndTime(int _endTime) { endTime = _endTime; }
	void addEndTime(int _endTime) { endTime += _endTime; }

	int getFare() { return fare; }
	int getRemainingTime() { return endTime - time(NULL); }
	time_t getStartTime() { return startTime; }
	time_t getEndTime() { return endTime; }

	int totalTime() { return endTime - startTime; }

	void setRemainingTime() { remainingTime = endTime - time(NULL); }
};
class PcInfo {
protected:
	//PcLocation
	short id;
	short pcX;
	short pcY;
	//State
	int state;
	User user;
public:
	PcInfo(short _id, short _pcX, short _pcY, short _state, int _fare, time_t _startTime, time_t _endTime, int _remainingTime) : id(_id), pcX(_pcX), pcY(_pcY), state(_state) {
		user.reset(_fare, _startTime, _endTime, _remainingTime);
	}
	PcInfo(short _id, short _pcX, short _pcY, short _state) : id(_id), pcX(_pcX), pcY(_pcY), state(_state) {}
	PcInfo() {}
	short getId() { return id; }
	short getX() { return pcX; }
	short getY() { return pcY; }
	int getState() { return state; }

	void reset() { 
		user.reset(); 
		state = EMPTY;
	}

	User* getUser() { return &user; }
	void setState(int _state) { state = _state; }
};
class PcCafe {
private:
	short pcMaxNum;
	short pcMap[MAX_PC_NUM_Y][MAX_PC_NUM_X];
	vector<PcInfo> pc;
	friend class SelectArea;
	//IA
	enum { INDEX, IS_EMPTY, MANAGE_STORE, IS_USING};
	short optionPage;
	//시간
	int year, month, days, hour, minute, seconds;
	//기본 요금 및 매출
	int charge;
	long long sales;
	int visited;
public:
	PcCafe() {
		charge = 1000;
		visited = 0;
	}
	~PcCafe() {
		cout << "종료";
	}

	//-------------------------------Information Right---------------------------
	//시간 조정
	void setCurrentTime() {
		SYSTEMTIME lst;
		GetLocalTime(&lst);

		year = lst.wYear;
		month = lst.wMonth;
		days = lst.wDay;
		hour = lst.wHour;
		minute = lst.wMinute;
		seconds = lst.wSecond;
	}
	//시간 표시
	void printCurrentTime() {
		SYSTEMTIME lst;
		GetLocalTime(&lst);

		gotoxy(DOS_WIDTH - 21, INFO_AREA_Y);
		textColor(YELLOW, BLACK);
		printf("%04d-%02d-%02d %02d:%02d:%02d",
			year, month, days, hour, minute, seconds);
		textColor(LIGHTGRAY, BLACK);
	}
	struct timeStruct {
		int time;
		short id;
	};
	void printRemainingTime() {
		vector<timeStruct> timeArray;
		timeStruct timeStr;
		vector<string> remainingTimes;
		string element = "";
		int minute;
		int hour;
		int lineLoc = 0;

		//전원이 켜진 PC의 정보를 모두 담음
		for (auto info : pc) {
			if (info.getState() == USING || info.getState() == SELECT_USING || info.getState() == WARNING) {
				timeStr.time = (*(info.getUser())).getRemainingTime();
				timeStr.id = info.getId();
				timeArray.push_back(timeStr);
			}
		}
		//timeArray 정렬
		if (timeArray.size() > 0)
			for (int i = 0; i < timeArray.size() - 1; i++) {
				for (int j = i + 1; j < timeArray.size(); j++) {
					if (timeArray[i].time > timeArray[j].time) {
						timeStr = timeArray[i];
						timeArray[i] = timeArray[j];
						timeArray[j] = timeStr;
					}
				}
			}
		//구조체를 string화 시킴
		int cnt = 0;
		for (auto info : timeArray) {
			minute = toMinute(info.time);
			hour = toHour(info.time);

			element = string("PC") + to_string(info.id) + " - " + to_string(hour) + ":" + to_string(minute);
			remainingTimes.push_back(element);
			
		}

		for (auto line : remainingTimes) {
			gotoxy(INFO_REMAINING_TEXT_AREA_X, INFO_REMAINING_TEXT_AREA_Y + lineLoc);
			cout << line;
			lineLoc++;
			if (cnt > 20) {
				break;
			}
			cnt++;
		}

		remainingTimes.~vector();
	}
	//-------------------------------Pc Cafe------------------------------------
	//Charge 조정
	void setCharge(int _charge) { charge = _charge; }
	int getCharge() { return charge; }
	//피시 로그아웃
	void logoutPc() {
		for (auto &info : pc) {
			if (info.getState() == USING || info.getState() == SELECT_USING || info.getState() == WARNING)
				if ((*(info.getUser())).getRemainingTime() <= 0) {
					clearLayout();
					log.push_back(to_string(info.getId()) + "번 PC가 로그아웃 하였습니다.");
					info.reset();
					//printTestInInfoArea((*(info.getUser())).getRemainingTime());
				}
		}
	}
	//피시 로그인
	void loginPc(int optionPc, int fare) {
		time_t cuurrentTime = time(NULL);
		int playMinute = (double(fare) / double(charge)) * 60;
		playMinute *= 60; // 초로 환산

		//현재 시간 대입
		(*(pc[optionPc].getUser())).setStartTime(time(NULL));
		//끝날 시간 대입
		(*(pc[optionPc].getUser())).setEndTime(time(NULL) + playMinute);
		//돈 추가
		(*(pc[optionPc].getUser())).setFare(fare);
		sales += fare;
		visited++;
		pc[optionPc].setState(USING);
	}

	void addPc(int optionPc, int fare) {
		time_t cuurrentTime = time(NULL);
		int playMinute = (double(fare) / double(charge)) * 60;
		playMinute *= 60; // 초로 환산

		//끝날 시간 대입
		(*(pc[optionPc].getUser())).addEndTime(playMinute);
		//돈 추가
		(*(pc[optionPc].getUser())).setFare(fare);
		sales += fare;
	}
	vector<PcInfo>* getPc() { return &pc; }
	//pcMap 표시
	void showPcMap() {
		//printClearPcMapArea();
		for (auto pcInfo : pc) {
			//상태 표시
			gotoxy(pcInfo.getX() * X_WIDTH, pcInfo.getY() * Y_HEIGHT);
			if (pcInfo.getState() == EMPTY) cout << " □ ";
			else if (pcInfo.getState() == USING) cout << " ■ ";
			else if (pcInfo.getState() == WALL) cout << " 　 ";
			else if (pcInfo.getState() == SELECT_EMPTY) {
				textColor(BLUE, BLACK);
				cout << " □ ";
				textColor(LIGHTGRAY, BLACK);
			}
			else if (pcInfo.getState() == SELECT_USING) {
				textColor(BLUE, BLACK);
				cout << " ■ ";
				textColor(LIGHTGRAY, BLACK);
			}
			//id 표시
			gotoxy(pcInfo.getX() * X_WIDTH, pcInfo.getY() * Y_HEIGHT + 1);
			printf(" %-3d", pcInfo.getId());
		}
	}

	//---------------------------------IA------------------------------------
	vector<string> log;

	//배열의 크기만큼 가운데 정렬을 함
	void printPage(vector<string> showMeOnCenter) {
		for (char showLocation = 0; showLocation < showMeOnCenter.size(); showLocation++) {
			short locX = (INPUT_AREA_X + (DOS_WIDTH / (showMeOnCenter.size() + 1)) * (showLocation + 1)) - (showMeOnCenter[showLocation].length() / 2);
			short locY = INPUT_AREA_Y + (INPUT_AREA_HEIGHT / 2);
			gotoxy(locX, locY);
			cout << showMeOnCenter[showLocation];
		}
	}
	//화면 제거
	void pageLineClear() {
		short locX = INPUT_AREA_X;
		short locY = INPUT_AREA_Y + (INPUT_AREA_HEIGHT / 2);

		gotoxy(locX, locY);
		for (int i = 0; i < DOS_WIDTH; i++) cout << " ";
	}
	//입력값 확인
	char pushKey() {
		if (_kbhit()) {
			return _getch();
		}
		return NULL;
	}
	//페이지를 현재 상황과 맞게 출력해줌.
	void selectPage() {
		switch (optionPage) {
		case INDEX:
			printPage({ "1. 자리선택", "2. 매장 관리", "3. 저장", "4. 종료" });
			break;
		case IS_EMPTY:
			printPage({ "1. 로그인", "x. 초기 화면" });
			break;
		case MANAGE_STORE:
			printPage({ "1. 요금 변경", "x. 초기 화면" });
			break;
		case IS_USING:
			printPage({ "1. 요금 추가", "x. 초기 화면" });
			break;
		}
	}
	//입력창으로 커서를 이동.
	inline void gotoInput() {
		gotoxy(_INPUT_AREA_X, _INPUT_AREA_Y);
	}
	//화면 재구성
	void replace(PcCafe &pcCafe) {
		system("cls");
		
		pcCafe.showPcMap();
		selectPage();
		printLayout();
	}
	//입력창 제어
	void inputNumber(int& number, PcCafe& pcCafe) {
		gotoInput();
		cin >> number;
		replace(pcCafe);
	}
	//enum { INDEX, IS_EMPTY, MANAGE_STORE, FOOD_MANAGE};
	void printPage(int optionPc) {
		if (optionPage == INDEX || optionPage == IS_EMPTY) printIndex();
		else if (optionPage == MANAGE_STORE) printManage();
		else if (optionPage == IS_USING) printIsusing(optionPc);
	}
	//페이지를 넘길 것인지 여부 결정, 인터페이스
	void nextPage(PcCafe &pcCafe) {
		char key = pushKey();
		int number;
		static int optionPc;
		printPage(optionPc);
		if (key != NULL) {
		next:;
			SetWindow(150, 40);
			printClearInputArea();
			selectPage();
			switch (optionPage) {
				//매장 관리
			case MANAGE_STORE:
				printClearIA();
				//요금 변동
				if (key == '1') {
					printInputArea(string("변경할 요금을 입력해주세요(현재 요금 : ") + to_string(pcCafe.getCharge()) + string(")"));
					inputNumber(number, pcCafe);
					pcCafe.setCharge(number);
					printInputArea("변경이 완료되었습니다.");
					log.push_back(to_string(number) + "원으로 요금을 변경하였습니다.");
					break;
				}
				//초기 화면 이동
				if (key == 'x') {
					optionPage = INDEX;
					replace(pcCafe);
					break;
				}
				break;
				//선택 - 빈 자리
			case IS_USING:
				//요금 추가
				if (key == '1') {
					printInputArea("추가하실 요금을 입력해주세요.");
					inputNumber(number, pcCafe);

					pcCafe.addPc(optionPc, number);
					optionPage = INDEX;
					replace(pcCafe);
					log.push_back(to_string(optionPc) + "번 손님이 " + to_string(number) + "원 추가하였습니다.");
					(*(pcCafe.getPc()))[optionPc].setState(USING);
					break;
				}
				//초기 화면
				if (key == 'x') {
					optionPage = INDEX;
					//이전의 지정된 PC의 상태를 되돌려 줌.
					if ((*(pcCafe.getPc()))[optionPc].getState() == SELECT_EMPTY) (*(pcCafe.getPc()))[optionPc].setState(EMPTY);
					else if ((*(pcCafe.getPc()))[optionPc].getState() == SELECT_USING) (*(pcCafe.getPc()))[optionPc].setState(USING);
					//재구성
					replace(pcCafe);
					break;
				}
			case IS_EMPTY:
				//로그인
				if (key == '1') {
					printInputArea("요금을 입력해주세요.");
					inputNumber(number, pcCafe);

					pcCafe.loginPc(optionPc, number);
					optionPage = INDEX;
					replace(pcCafe);
					log.push_back(to_string(optionPc) + "번 손님이 " + to_string(number) + "원 충전하였습니다.");
					break;
				}
				//초기 화면으로 가기
				if (key == 'x') {
					optionPage = INDEX;
					//이전의 지정된 PC의 상태를 되돌려 줌.
					if ((*(pcCafe.getPc()))[optionPc].getState() == SELECT_EMPTY) (*(pcCafe.getPc()))[optionPc].setState(EMPTY);
					else if ((*(pcCafe.getPc()))[optionPc].getState() == SELECT_USING) (*(pcCafe.getPc()))[optionPc].setState(USING);
					//재구성
					replace(pcCafe);
					break;
				}
				break;
				//초기 화면
			case INDEX:
				//pc선택
				if (key == '1') {
					printInputArea("PC번호를 입력하세요.");
					inputNumber(number, pcCafe);
					optionPc = number;

					//pc 상태 조정
					if (optionPc < pcCafe.getPc()->size() && optionPc >= 0) {
						if ((*(pcCafe.getPc()))[optionPc].getState() == EMPTY) {
							(*(pcCafe.getPc()))[optionPc].setState(SELECT_EMPTY);
							pcCafe.showPcMap();
							optionPage = IS_EMPTY;
							key = -1;
							goto next;
						}
						else if ((*(pcCafe.getPc()))[optionPc].getState() == USING) {
							(*(pcCafe.getPc()))[optionPc].setState(SELECT_USING);
							pcCafe.showPcMap();
							optionPage = IS_USING;
							key = -1;
							goto next;
						}
					}
					else {
						printInputArea("잘못 입력하였습니다..");
						optionPage = INDEX;
						break;
					}
				}
				//매장 관리
				else if (key == '2') {
					optionPage = MANAGE_STORE;
					goto next;
				}
				//저장할 경우
				else if (key == '3') {
					pcCafe.fileSave();
					printInputArea("저장을 완료하였습니다.");
					log.push_back("정보를 저장하였습니다.");
				}
				//종료할 경우
				else if (key == '4')
					exit(1);
				break;
			}
		}
	}
	//-------------------------------Information Left---------------------------
	//enum {INDEX, IS_EMPTY, MANAGE_STORE};
	//로그 관리 24이상 넘어가지 않도록 방지
	void logLimite() {
		const int limiteLine = 24;
		if (log.size() > limiteLine)
			while (log.size() > limiteLine)
				log.erase(log.begin());
	}
	//INDEX 일 경우
	void printIndex() {
		logLimite();
		for (int y = 0; y < log.size(); y++) {
			gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + 3 + y);
			cout << log[y];
		}
	}
	// IS_USING일 경우
	void printIsusing(int optionPc) {
		const int initY = 11;
		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + initY + 0);
		cout << "충전 시간 : " << toHour((*(pc[optionPc].getUser())).totalTime()) << "시간 " << toMinute((*(pc[optionPc].getUser())).totalTime()) << "분";

		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + initY + 1);
		cout << "사용 금액 : " << (*(pc[optionPc].getUser())).getFare() << "원";

		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + initY + 2);
		cout << "이용 시간 : " << toHour(time(NULL) - (*(pc[optionPc].getUser())).getStartTime()) << "시간 " << toMinute(time(NULL) - (*(pc[optionPc].getUser())).getStartTime()) << "분";
		int use = 0;
		for (auto info : pc) {
			if (info.getState() == USING) use++;
		}
		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + initY + 4);
		cout << "방문 시간 : " << (toHour((*(pc[optionPc].getUser())).getStartTime()) + 9) % 24 << "시 " << toMinute((*(pc[optionPc].getUser())).getStartTime()) << "분";

		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + initY + 5);
		cout << "종료 시간 : " << (toHour((*(pc[optionPc].getUser())).getEndTime()) + 9) % 24 << "시 " << toMinute((*(pc[optionPc].getUser())).getEndTime()) << "분";
	}
	//Manage 창
	void printManage() {
		const int initY = 11;
		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + initY + 0);
		cout << "매출 : " << sales;
		
		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + initY + 1);
		cout << "기본 요금 : " << charge;

		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + initY + 2);
		cout << "방문자 수 : " << visited;
		int use = 0;
		for (auto info : pc) {
			if (info.getState() == USING) use++;
		}
		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + initY + 4);
		cout << "사용중인 PC : " << use;

		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + initY + 5);
		cout << "잔여석 : " << pcMaxNum - use;

		gotoxy(INFO_AREA_X + 1, INFO_AREA_Y + initY + 6);
		cout << "총 석 : " << pcMaxNum;
	}

	//-----------------------------Initiate----------------------------------
	//Pc의 정보를 설정함
	void initPcClassSetting(short pcCnt) {
		pc.clear();

		//pcMap을 탐색 후 정보를 설정
		short pcId = 0;
		for (short y = 0; y < MAX_PC_NUM_Y; y++) {
			for (short x = 0; x < MAX_PC_NUM_X; x++) {
				if (pcMap[y][x] == EMPTY) {
					pc.push_back(PcInfo::PcInfo(pcId, x, y, EMPTY));
					pcId++;
				}
			}
		}
	}
	//파일 세이브 및 로드
	void fileSave() {
		ofstream save;
		save.open("Pc Cafe Information.txt");

		save << pcMaxNum << endl;
		save << charge << endl;
		save << sales << endl;
		save << visited << endl;

		//데이터 저장
		for (auto &pcInfo : pc) {
			save << "ID:" << pcInfo.getId() << endl;
			save << "X:" << pcInfo.getX() << endl;
			save << "Y:" << pcInfo.getY() << endl;
			save << "State:" << pcInfo.getState() << endl;
			save << "Fare:" << (*(pcInfo.getUser())).getFare() << endl;
			save << "TartTime:" << (*(pcInfo.getUser())).getStartTime() << endl;
			save << "EndTime:" << (*(pcInfo.getUser())).getEndTime() << endl;
			save << "RemainingTime:" << (*(pcInfo.getUser())).getRemainingTime() << endl;
			save << endl;
		}

		save.close();
	}
	//로드
	void fileLoad() {
		ifstream loader;
		string str;
		string data;

		loader.open("Pc Cafe Information.txt");
		//데이터 로드
		short pcId, pcX, pcY, state;
		int fare, remainingTime;
		time_t startTime, endTime;

		bool isData = false;
		if (loader.is_open() == false) initPcMap();
		else {
			loader >> pcMaxNum;
			loader >> charge;
			loader >> sales;
			loader >> visited;
			while (loader.peek() != EOF) {
				getline(loader, str);
				if (str[0] == 'I') {
					pcId = sepsNextValue(str, ':');
				}
				else if (str[0] == 'X') pcX = sepsNextValue(str, ':');
				else if (str[0] == 'Y') pcY = sepsNextValue(str, ':');
				else if (str[0] == 'S') state = sepsNextValue(str, ':');
				else if (str[0] == 'F') fare = sepsNextValue(str, ':');
				else if (str[0] == 'T') startTime = sepsNextValue(str, ':');
				else if (str[0] == 'E') endTime = sepsNextValue(str, ':');
				else if (str[0] == 'R') { remainingTime = sepsNextValue(str, ':'); isData = true;}
				if (isData) {
					//PcInfo(short _id, short _pcX, short _pcY, short _state, int _fare, time_t _startTime, time_t _endTime, int _remainingTime)
					pc.push_back(PcInfo::PcInfo(pcId, pcX, pcY, state, fare, startTime, endTime, remainingTime));
					isData = false;
				}
			}
		}
		loader.close();
	}
	//초기 PcMap 설정
	void initPcMap() {
		//초기 PC 상태 변경
		for (short y = 0; y < MAX_PC_NUM_Y; y++) {
			for (short x = 0; x < MAX_PC_NUM_X; x++) {
				pcMap[y][x] = WALL;
			}
		}
		initShowPcMap();
		//PC위치 입력
		gotoxy(INPUT_AREA_X, INPUT_AREA_Y);
		short optionNumber = 0;
		short optionX, optionY;
		short pcCnt = 0;
		while (true) {
			printClearInputArea();
			cout << "설치할 PC의 위치를 입력하세요(입력 종료 : -1) : ";
			cin >> optionNumber;
			//예외처리, 범위가 넘어설 경우
			if (optionNumber < -1 || optionNumber > 284) {
				optionNumber = 0;
				continue;
			}
			if (!cin) {
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				optionNumber = 0;
				continue;
			}
			if (optionNumber == -1) break;
			optionX = optionNumber % MAX_PC_NUM_X;
			optionY = optionNumber / MAX_PC_NUM_X;
			//pc의 수를 셈과 동시에 토글처리.
			if (pcMap[optionY][optionX] == EMPTY) {
				pcMap[optionY][optionX] = WALL;
				pcCnt--;
			}
			else if (pcMap[optionY][optionX] == WALL) {
				pcMap[optionY][optionX] = EMPTY;
				pcCnt++;
			}

			pcMaxNum = pcCnt;
			initShowPcMap();
			printClearInputArea();
		}
		printClearPcMapArea();
		initPcClassSetting(pcCnt);
		pcMaxNum = pcCnt;
	}

	//Pc맵을 보여줌.
	void initShowPcMap() {
		gotoxy(PC_MAP_AREA_X, PC_MAP_AREA_Y);
		//이하 실질적 PC 배치
		int pcNumber = 0;
		//자리 표시
		for (int y = 0; y < MAX_PC_NUM_Y; y++) {
			//자리 상태 표시
			for (int x = 0; x < MAX_PC_NUM_X; x++) {
				if (pcMap[y][x] == EMPTY) cout << " □ ";
				else if (pcMap[y][x] == USING) cout << " ■ ";
				else if (pcMap[y][x] == WALL) cout << " 　 ";
			}
			cout << endl;
			//숫자 표시
			for (int x = 0; x < MAX_PC_NUM_X; x++) {
				printf(" %-3d", pcNumber);
				pcNumber++;
			}
			cout << endl;
		}
	}

};



int toMinute(int time) {
	return (time / 60) % 60;
}
int toHour(int time) {
	return (time / 60) / 60;
}
time_t toHour(time_t time) {
	return ((time / 60) / 60) % 24;
}
void main() {
	SetWindow(150, 40);
	//intro
	PcCafe pcCafe;

	system("cls");
	printLayout();
	pcCafe.fileLoad();
	pcCafe.showPcMap();
	pcCafe.selectPage();
	//end intro
	while(true){
		pcCafe.setCurrentTime();
		if (timerOneSecond()) {
			pcCafe.showPcMap();
			pcCafe.printCurrentTime();
		}
		pcCafe.nextPage(pcCafe);
		pcCafe.printRemainingTime();
		pcCafe.logoutPc();
		Sleep(10);
	}
	pcCafe.fileSave();
	_getch();
}