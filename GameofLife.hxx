#include <iostream>
#include <ncurses.h>

constexpr int max_timeout = 4096;
constexpr int min_timeout = 64;

class Pointer{
    public:
    int x;
    int y;
    Pointer() = default;
    ~Pointer() = default;
    Pointer(int _x,int _y): x(_x),y(_y) {}
};

template<int X,int Y>
class GameofLife{
    public:
    int width = X;
    int height = Y;
    Pointer point;
    bool grid[Y][X];
    bool running;
    int tick;
    GameofLife() : running(false), point(Pointer(0,0)), tick(256){
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                grid[i][j]=false;
            }
        }
        initscr();            
        noecho();
        nodelay(stdscr, TRUE);
        keypad(stdscr, TRUE);  
        timeout(tick);
        start();
    }
    ~GameofLife()=default;

    void drawboard(){
        for(int i=0;i<Y;i++){
            for(int j=0;j<X;j++){
                if(grid[i][j]){
                    move(i,j);
                    printw("O");
                }
            }
        }
        move(point.y,point.x);
    }

    void compute_next_grid(bool current_grid[][X]){
        bool new_grid[Y][X];
        for(int i=0;i<Y;i++){
            for(int j=0;j<X;j++){
                new_grid[i][j]=current_grid[i][j];
            }
        }

        int live_neighbours;
        bool check_borders;

        for(int i=1;i<Y-1;i++){
            for(int j=1;j<X-1;j++){
                live_neighbours = 0; 
                for(int y=-1;y<=1;y++){
                    for(int x=-1;x<=1;x++){
                        live_neighbours += current_grid[i+y][j+x];
                    }
                }
                live_neighbours-=new_grid[i][j];

                if((new_grid[i][j]) && (live_neighbours < 2)){
                    new_grid[i][j] = 0;
                }
                else if((new_grid[i][j]) && (live_neighbours > 3)){
                    new_grid[i][j] = 0;
                }
                else if((!new_grid[i][j]) && (live_neighbours == 3)){
                    new_grid[i][j] = 1;
                }
            }
        }

        for(int i=0;i<X;i++){
            new_grid[0][i]=0;
            new_grid[Y-2][i]=0;
        }
        for(int i=0;i<Y;i++){
            new_grid[i][0]=0;
            new_grid[i][X-2]=0;
        }

        for(int i=0;i<Y;i++){
            for(int j=0;j<X;j++){
                current_grid[i][j]=new_grid[i][j];
            }
        }

    }

    void nextGeneration(){
        if(running){
            clear();
            compute_next_grid(grid);
        }
        refresh();
    }

    void start(){
        while(true){
            drawboard();
            auto input = getch();
            if(input!=ERR){
                if(input=='q'){
                    break;
                    clear();
                }

                if(input==KEY_DOWN){
                    clear();
                    (point.y<height-2)?move(++point.y,point.x):move(point.y,point.x);
                }

                if(input==KEY_UP){
                    clear();
                    (point.y>0)?move(--point.y,point.x):move(point.y,point.x);
                }

                if(input==KEY_LEFT){
                    clear();
                    (point.x>0)?move(point.y,--point.x):move(point.y,point.x);
                }

                if(input==KEY_RIGHT){
                    clear();
                    (point.x<width-1)?move(point.y,++point.x):move(point.y,point.x);  
                }

                if(input=='p'){
                    running=!running;
                }

                if(input==' '){
                    grid[point.y][point.x] = (grid[point.y][point.x])?false:true;
                }

                if(input=='-'){
                    if(tick>=min_timeout){
                        tick/=2;
                        timeout(tick);
                    } 
                }

                if(input=='='){
                    if(tick<=max_timeout){
                        tick*=2;
                        timeout(tick);
                    } 
                }
            }
            nextGeneration();
        
        }
        endwin();
    }

};
