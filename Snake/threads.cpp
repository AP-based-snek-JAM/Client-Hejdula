#include <iostream>
#include <thread>
#include <termios.h>
#include <stdio.h>
#include <chrono>
#include <mutex>
#include <unistd.h>
#include <vector>
using namespace std;


class Timer {
private:
	// Type aliases to make accessing nested type easier
	using Clock = chrono::steady_clock;
	using Second = chrono::duration<double, ratio<1> >;

	chrono::time_point<Clock> m_beg { Clock::now() };
public:
	void reset() {
		m_beg = Clock::now();
	}

	double elapsed() const {
		return chrono::duration_cast<Second>(Clock::now() - m_beg).count();
	}
};


/*Functions for the keyboard handle thread*/
/* Initialize new terminal i/o settings */
static struct termios old, current;
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}
/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) {return getch_(0);}

/* Read 1 character with echo */
char getche(void) {return getch_(1);}


volatile int direction;//0:up, 1:down, 2:left, 3:right
mutex directionMutex;

// A dummy function
void keyboardHandleFunction(){
    char letter;
    while(1){
        letter = getch();
        
        if(letter == 'q'){
            break;
        }
        
        directionMutex.lock();
        switch(letter){
            case 'w':{
                direction=0;
                break;
            }
            case 's':{
                direction=1;
                break;
            }
            case 'a':{
                direction=2;
                break;
            }
            case 'd':{
                direction=3;
                break;
            }
        }
        directionMutex.unlock();
    }
}



class Field{
    public:
    int heigth;
    int width;
    vector<vector<int>> data{};
    Field(int localWidth,int localHeigth){
        heigth = localHeigth;
        width = localWidth;
        vector<int> vect(width,0);
        for(int i=0; i<heigth;i++){
            data.push_back(vect);
        }
    }
    void printGameField(){
        cout << "╔";
        for (int i = 0; i<width;i++){
            cout << "═";
        }
        cout << "╗\n";
        for (int i{heigth-1}; i >= 0; --i) {
            cout << "║";
            for (int j{0}; j < width; ++j) {
                cout << (data[i][j]?"▒":"░");
            }
            cout << "║\n";
        }
        cout << "╚";
        for (int i = 0; i<width;i++){
            cout << "═";
        }
        cout << "╝\n";
    }
    void printData(){
        for (int row = heigth-1;row>=0; row--) {
                for (int i : data[row]) {
                    cout << i << "\t";
                }
            cout << endl;
        }
    }
    void tick(){
        for(auto& row: data){
            for(auto& i:row){
                i?i--:true;
        }
      }
    }
};

int main(){
    thread keyboardHandleThread(keyboardHandleFunction);
    Field field(50,25);
    int head[2]={15,5};
    int score = 50;
    while(1){
        directionMutex.lock();
        switch(direction){
            case 0:{
                head[1]++;
                break;
            }
            case 1:{
                head[1]--;
                break;
            }
            case 2:{
                head[0]--;
                break;
            }
            case 3:{
                head[0]++;
                break;
            }
        }
        directionMutex.unlock();
        if(field.data[head[1]][head[0]]){
            score=0;
            for (int row = field.heigth-1;row>=0; row--) {
                for (int i : field.data[row]) {
                    i=0;
                }
             }
        } 
        field.data[head[1]][head[0]]=score+1;
        field.printGameField();
        field.tick();
        usleep(200000); 
    }
    keyboardHandleThread.join();
    return 0;
}