#include <iostream>
#include <vector>
#include <chrono> // for std::chrono functions
#include <ncurses.h>
using namespace std;


class Timer {
private:
	// Type aliases to make accessing nested type easier
	using Clock = chrono::steady_clock;
	using Second = chrono::duration<double, ratio<1> >;

	chrono::time_point<Clock> m_beg { Clock::now() };

public:
	void reset()
	{
		m_beg = Clock::now();
	}

	double elapsed() const
	{
		return chrono::duration_cast<Second>(Clock::now() - m_beg).count();
	}
};


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
    int head[2]={0,0};  
    int score = 0;
    int direction = 0; //0:up, 1:down, 2:left, 3:right
    Field field(15,7);
    field.printGameField();
    field.tick();
    Timer time;
    /*
    while(true){
        cout << time.elapsed()<<"\n";
    }
    */
}