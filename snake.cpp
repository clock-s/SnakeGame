//Libraries used
#include <iostream> 
#include <time.h>
#include <cstdlib>
#include <vector>
#include <ncurses.h>
#include <unistd.h> 
using namespace std;

//Tools facilits
#define initSize 1
#define berry '#'
#define head '@'
#define body 'o'
#define background ' '
#define wall 'X'
#define mapSize 30
#define jMapSize mapSize*2
int iBeforeCoord = -1;
int jBeforeCoord = -1;
char direction = 'p';
int delay = 110000;

//Functions

//Useless in this mode
void display(char map[][jMapSize]){
    for (int i = 0 ; i < mapSize ; i++){
            for (int j = 0 ; j < mapSize ; j++){
                cout << map[i][j] << background;
            }
            cout << "\n";
        }
}

void score(int points) {

    points = (points - initSize) * 10 ;
    move(mapSize,0);
    printw("Score: %d", points);
    move(mapSize+1,0);
    
}

//Condition of Game Over
bool gameOver(vector<pair<int,int>> nextPosition, char map[][jMapSize]){
    if(map[nextPosition[0].first][nextPosition[0].second] == wall || 
        map[nextPosition[0].first][nextPosition[0].second] == body) return 1;
    else return 0;
}


//Moviments control
void move(vector<pair<int,int>> &nextPosition, int i){
    char key;
    key = getch();


    switch(key){
            case 's':
                if(direction != 'w') direction = 's';
                break;
            case 'w':
                if(direction != 's') direction = 'w';
                break;
            case 'a':
                if(direction != 'd') direction = 'a';
                break;
            case 'd':
                if(direction != 'a') direction = 'd';
                break;
    }


    switch(direction){
            case 's':
                nextPosition[i].first++;
                break;
            case 'w':
                nextPosition[i].first--;
                break;
            case 'a':
                nextPosition[i].second--;
                break;
            case 'd':
                nextPosition[i].second++;
                break;
    }

}

void makeBerry(char map[][jMapSize],int& ibefore, int& jbefore){
    int i = rand();
    int j = rand();

    i = i%mapSize;
    j = j%jMapSize;


    if(map[i][j] == body || map[i][j] == head || i <= 0 || j <= 0 
        || i == (mapSize-1) || j == (jMapSize-1) || i == ibefore || j == jbefore){

        makeBerry(map,ibefore,jbefore);

    }else{
        ibefore = i;
        jbefore = j;

        map[i][j] = berry;

        mvprintw(i,j,"%c",berry);
    }

}

void snake(char map[][jMapSize],int parte, int& size,vector<pair<int,int>> &beforePosition, 
    vector<pair<int,int>> &Position, vector<pair<int,int>> &nextPosition){

    //Creating a new body if eat a berry
    if(Position.size() < size){
        Position.resize(size);
        beforePosition.resize(size);
        nextPosition.resize(size);
    }
    

    //Coordenadas da parte posterior indo para a anterior
    if(parte > 0 && parte < size){
        nextPosition[parte].first = Position[parte-1].first;
        nextPosition[parte].second = Position[parte-1].second;

        Position[parte].first = beforePosition[parte-1].first;
        Position[parte].second = beforePosition[parte-1].second;
        
    }


    //Head's moviment
    if(parte == 0){
        move(nextPosition,parte);
    }

    map[Position[parte].first][Position[parte].second] = background;
    mvprintw(Position[parte].first, Position[parte].second, "%c", background);


    if(parte >= 0 && parte < size-1){
        snake(map,parte+1,size,beforePosition,Position,nextPosition);
    }

    beforePosition[parte].first = Position[parte].first;
    beforePosition[parte].second = Position[parte].second;


    Position[parte].first = nextPosition[parte].first;
    Position[parte].second = nextPosition[parte].second;

    if(parte == 0){
        //Condições: Check conditions before put head in next place
            //Berrys
            if(map[nextPosition[parte].first][nextPosition[parte].second] == berry){
                makeBerry(map,iBeforeCoord,jBeforeCoord);
                size++;
            }
            //Game over
            if(gameOver(nextPosition,map)){
                mvprintw(nextPosition[parte].first, nextPosition[parte].second, "%c", head);

                mvprintw(mapSize/2,(jMapSize/3)-1, "Game Over!!!");
                refresh();
                while(getch() != '\n');

                endwin();
                exit(0);
            }

        map[nextPosition[parte].first][nextPosition[parte].second] = head;

        mvprintw(nextPosition[parte].first, nextPosition[parte].second, "%c", head);

    }else{
        map[nextPosition[parte].first][nextPosition[parte].second] = body;

        mvprintw(nextPosition[parte].first, nextPosition[parte].second, "%c", body);

    }
    
    

}

void initCoord(int& i, int& j){
    i = rand()%mapSize;
    j = rand()%jMapSize;

    if(i == 0 || i == (mapSize-1) || j == 0 || j == (jMapSize-1)) initCoord(i,j);
}


int main(){
    srand(time(NULL));

//Setting coordinates
    int initI,initJ;
    initCoord(initI,initJ);

//Parts of snake
    vector<pair<int,int>> Position = {{initI,initJ}};
    vector<pair<int,int>> nextPosition = {{initI,initJ}};
    vector<pair<int,int>> beforePosition = {{initI,initJ}};

//Setting map's and snake's initial size
    char map[mapSize][jMapSize]; int size = initSize;
    initscr();
    raw();
    noecho();
    scrollok(stdscr, TRUE);


//Setting map
    for (int i = 0 ; i < mapSize ; i++){
        for (int j = 0 ; j < jMapSize ; j++){
            if(j == 0 || j == (jMapSize-1)) map[i][j] = wall;
            else if(i == 0 || i == (mapSize-1)) map[i][j] = wall;
            else map[i][j] = background;
        }
    }


//Setting first berry and Snake's head
    makeBerry(map,iBeforeCoord,iBeforeCoord);
    map[nextPosition[0].first][nextPosition[0].second] = head;

//Map
    for(int i = 0 ; i < mapSize ; i++){
        for(int j = 0 ; j < jMapSize ; j++){
            mvprintw(i,j,"%c",map[i][j]);
        }
    }


   // getch();
    nodelay(stdscr,TRUE);
//Game
    while(true){
        score(size);

        snake(map,0,size,beforePosition,Position,nextPosition);

        refresh();

        usleep(delay);

    }

    endwin();


    return 0;
}

