#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QThread> //for putting pause
#include <random>
#include <iostream>

//int turn=0; //global variable to call on

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //importing background page labels; must change file direcotry here!
    QPixmap choosing_player, starting_page, rules_page, ending_page, playing_page, choose_mode_page;
    choosing_player.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/choose_player.png"); //CHANGE IMAGE DIRECTORY HERE!
    starting_page.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/start_page.png");
    rules_page.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/rules_page.png");
    playing_page.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/playing_background.png");
    ending_page.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/ending_page.png");
    choose_mode_page.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/choose_mode_page.png");

    //setting background page labels
    ui->choose_player_label->setPixmap(choosing_player);
    ui->start_page_label->setPixmap(starting_page);
    ui->rule_page_label->setPixmap(rules_page);
    ui->playing_background->setPixmap(playing_page);
    ui->ending_page_label->setPixmap(ending_page);
    ui->choose_mode_background->setPixmap(choose_mode_page);

    QPushButton *button_slots_connect[8][8]; //creating slot names as array to connect the board buttons
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; ++j){
            QString butName="button"+QString::number(i)+QString::number(j);
            button_slots_connect[i][j]=MainWindow::findChild<QPushButton *>(butName);
            connect(button_slots_connect[i][j], SIGNAL(released()), this, SLOT(button_slots_pressed())); //connecting buttons
        }
    }

}

MainWindow::~MainWindow() //destructor for ui
{
    delete ui;
}



void MainWindow::on_start_button_clicked() //when clicking start button
{
    ui->stackedWidget->setCurrentIndex(5); //go to choose mode
}

//updates entire board icons
void MainWindow::update_board_icons(){ //change icons depending on value of the array
    QPixmap player1_icon("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/player1_icon"); //create button icons
    QPixmap player2_icon("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/player2_icon");
    QIcon icon1(player1_icon);
    QIcon icon2(player2_icon);

    for(int i=0; i<8; ++i){ //update icons
        for(int j=0; j<8; ++j){
            QString butName="button"+QString::number(i)+QString::number(j);

            if(button_slots[i][j]==1){
                MainWindow::findChild<QPushButton *>(butName)->setIcon(icon1);
            }
            else if(button_slots[i][j]==2){
                MainWindow::findChild<QPushButton*>(butName)->setIcon(icon2);
            }
            else{ //get rid of button icon if value is not for players 1 or 2
                MainWindow::findChild<QPushButton*>(butName)->setIcon(QIcon());
            }

        }
    }
}

void MainWindow::end_the_game() //shows final results of the game
{
    update_board_icons();
    //QThread::sleep(1); //put pause before ending
    int winner;
    ui->stackedWidget->setCurrentIndex(4);
    if(player1_score>player2_score){ //decide winner
        winner=1;
    }
    else if(player2_score>player1_score){
        winner=2;
    }
    else{
        winner=3;

    }

    if(winner==3){ //when tied
        ui->player1_final->setText(QString::number(player1_score));
        ui->player2_final->setText(QString::number(player2_score));
        ui->final_winner->setText("Both");

    }
    else{ //show scores for each player
        ui->player1_final->setText(QString::number(player1_score));
        ui->player2_final->setText(QString::number(player2_score));
        ui->final_winner->setText(QString::number(winner));
    }

    ai_mode=false; //set all values to initial value when ending game
    limit_mode=false;
    hard_mode=false;
    turn=0;
    starting_player=0;



}

void MainWindow::update_board_score() //updating score based on the button_slots array
{
    player1_score=0;
    player2_score=0;
    int end_game=0;

    for(int i=0; i<8; ++i){
        for(int j=0; j<8; ++j){
            if(button_slots[i][j]==1)
                player1_score++;
            else if(button_slots[i][j]==2)
                player2_score++;
            else if(button_slots[i][j]==0)
                end_game++;
        }
    }

    ui->score_player1->setText(QString::number(player1_score)); //show scores on playing page
    ui->score_player2->setText(QString::number(player2_score));

    if(end_game==0){ //end game if all boards are filled

        end_the_game();
    }

}

//changes the values of the array according to the results of what must change
void MainWindow::change_pieces(int start_idx, int end_idx, int current_player, int direction, int row_idx, int column_idx){ //change value of board
    if(direction==1){ //when row needs to change; check up and down
        for(int i=start_idx; i<end_idx+1; ++i){
            button_slots[i][column_idx]=current_player;
        }


    }else{ //when col needs to change; check sideways
        for(int i=start_idx; i<end_idx+1; ++i){
            button_slots[row_idx][i]=current_player;
        }
    }

    update_board_icons(); //final update based on value of boxes; update ui
    update_board_score(); //update score
}


//changes the array values when diagonal pieces must change; just values of array are changing
void MainWindow::change_pieces_diag(int start_row_idx, int end_row_idx, int start_col_idx, int end_col_idx, int current_player, int direction){
    if(direction==1){ //when right bottom or left top is changing
        while(start_row_idx<=end_row_idx){
            button_slots[start_row_idx][start_col_idx]=current_player;
            start_row_idx++;
            start_col_idx++;

        }

    }

    else if(direction==2){ //when right top is changing
        while(start_col_idx<=end_col_idx){
            button_slots[start_row_idx][start_col_idx]=current_player;
            start_row_idx-=1;
            start_col_idx++;

        }

    }
    else{ //when left bottom is changing; direction 3
        while(start_row_idx<=end_row_idx){
            button_slots[start_row_idx][start_col_idx]=current_player;
            start_row_idx+=1;
            start_col_idx-=1;

        }

    }

    update_board_icons(); //update icons and score
    update_board_score();
}




//updating board after piece is down; determine what needs to change
void MainWindow::update_board(int row_idx, int column_idx)
{
    int current_player=button_slots[row_idx][column_idx];
    int opponent_player=0;

    int start_idx=0; //starting and ending indexes
    int end_idx=0;

    int start_row_idx=0;
    int end_row_idx=0;
    int start_col_idx=0;
    int end_col_idx=0;

    if(current_player==1) //finding opponent and current player value
        opponent_player=2;
    else
        opponent_player=1;


    //=================================================================================
    //FOR CROSS PATHS
    //=================================================================================
    if(button_slots[row_idx+1][column_idx]==opponent_player){ //case for when bottom part is true;

        start_idx=row_idx+1;
        end_idx=start_idx;

        while(button_slots[end_idx][column_idx]==opponent_player){

            end_idx+=1; //find when stops
        }


        if(button_slots[end_idx][column_idx]==current_player && end_idx<8 && end_idx>=0){

            change_pieces(start_idx, end_idx, current_player, 1, row_idx, column_idx); //1 means row needs to change, 2 means col needs to change
        }
    }
    //================================================================
    if(button_slots[row_idx-1][column_idx]==opponent_player){ //case for when top part is true

        start_idx=row_idx-1;
        end_idx=start_idx;

        while(button_slots[end_idx][column_idx]==opponent_player){

            end_idx-=1; //find when stops
        }


        if(button_slots[end_idx][column_idx]==current_player && end_idx<8 && end_idx>=0){

            change_pieces(end_idx, start_idx, current_player, 1, row_idx, column_idx); //1 means row needs to change, 2 means col needs to change
        }

    }
    //================================================================
    if(button_slots[row_idx][column_idx+1]==opponent_player){ //case for when right part is true
        //works until here
        start_idx=column_idx+1;
        end_idx=start_idx;


        while(button_slots[row_idx][end_idx]==opponent_player){

            end_idx+=1; //find when stops
        }

        if(button_slots[row_idx][end_idx]==current_player && end_idx<8 && end_idx>=0){

            change_pieces(start_idx, end_idx, current_player, 2, row_idx, column_idx); //1 means row needs to change, 2 means col needs to change
        }

    }
    //================================================================
    if(button_slots[row_idx][column_idx-1]==opponent_player){ //case for when top part is true

        start_idx=column_idx-1;
        end_idx=start_idx;

        while(button_slots[row_idx][end_idx]==opponent_player){

            end_idx-=1; //find when stops
        }
        if(end_idx<8 && end_idx>=0){

        if(button_slots[row_idx][end_idx]==current_player && end_idx<7){

            change_pieces(end_idx, start_idx, current_player, 2, row_idx, column_idx); //1 means row needs to change, 2 means col needs to change
        }
        }
    }
    //=================================================================================
    //FOR DIAGONAL PATHS
    //=================================================================================


    if(button_slots[row_idx+1][column_idx+1]==opponent_player){ //case for when right bottom part is true

        start_row_idx=row_idx+1;
        start_col_idx=column_idx+1;
        end_row_idx=start_row_idx;
        end_col_idx=start_col_idx;

        while(button_slots[end_row_idx][end_col_idx]==opponent_player){
            end_row_idx+=1; //find when stops
            end_col_idx+=1;
        }

        if(button_slots[end_row_idx][end_col_idx]==current_player && end_row_idx<8 && end_row_idx>=0 && end_col_idx<8 && end_col_idx>=0){
            change_pieces_diag(start_row_idx, end_row_idx, start_col_idx, end_col_idx, current_player, 1); //1 means row needs to change, 2 means col needs to change
        }
    }

    //================================================================
    if(button_slots[row_idx-1][column_idx+1]==opponent_player){ //case for when right top part is true

        start_row_idx=row_idx-1;
        start_col_idx=column_idx+1;
        end_row_idx=start_row_idx;
        end_col_idx=start_col_idx;

        while(button_slots[end_row_idx][end_col_idx]==opponent_player){
            end_row_idx-=1; //find when stops
            end_col_idx+=1;
        }

        if(button_slots[end_row_idx][end_col_idx]==current_player && end_row_idx<8 && end_row_idx>=0 && end_col_idx<8 && end_col_idx>=0){

            change_pieces_diag(start_row_idx, end_row_idx, start_col_idx, end_col_idx, current_player, 2); //1 means row needs to change, 2 means col needs to change
        }
    }

    //================================================================
    if(button_slots[row_idx-1][column_idx-1]==opponent_player){ //case for when left top part is true

        start_row_idx=row_idx-1;
        start_col_idx=column_idx-1;
        end_row_idx=start_row_idx;
        end_col_idx=start_col_idx;

        while(button_slots[end_row_idx][end_col_idx]==opponent_player){
            end_row_idx-=1; //find when stops
            end_col_idx-=1;
        }

        if(button_slots[end_row_idx][end_col_idx]==current_player && end_row_idx<8 && end_row_idx>=0 && end_col_idx<8 && end_col_idx>=0){
            change_pieces_diag(end_row_idx, start_row_idx, end_col_idx, start_col_idx, current_player, 1); //1 means row needs to change, 2 means col needs to change
        }
    }


    //================================================================
    if(button_slots[row_idx+1][column_idx-1]==opponent_player){ //case for when left bottom part is true

        start_row_idx=row_idx+1;
        start_col_idx=column_idx-1;
        end_row_idx=start_row_idx;
        end_col_idx=start_col_idx;

        while(button_slots[end_row_idx][end_col_idx]==opponent_player){
            end_row_idx+=1; //find when stops
            end_col_idx-=1;
        }

        if(button_slots[end_row_idx][end_col_idx]==current_player && end_row_idx<8 && end_row_idx>=0 && end_col_idx<8 && end_col_idx>=0){

            change_pieces_diag(start_row_idx, end_row_idx, start_col_idx, end_col_idx, current_player, 3); //1 means row needs to change, 2 means col needs to change
        }
    }







}

bool MainWindow::check_in_slot(int row_idx, int column_idx){ //check whether in slot
    if (button_slots[row_idx][column_idx]==1 ||button_slots[row_idx][column_idx]==2 )
        return true;
    else
        return false;
}


bool MainWindow::check_adjacency(int row_idx, int column_idx){ //check adjacent places and see if piece can be put down


    if((button_slots[row_idx-1][column_idx]==1 && row_idx-1>=0 && row_idx<8)||(button_slots[row_idx-1][column_idx]==2&& row_idx-1>=0 && row_idx<8)){//vertical and horizontal check
        if(row_idx>=0 && row_idx<8 && column_idx>=0 && column_idx<8)
            return false;
        else
            return true;
    }
    else if((button_slots[row_idx+1][column_idx]==1 && row_idx+1>=0 && row_idx+1<8)||(button_slots[row_idx+1][column_idx]==2 && row_idx+1>=0 && row_idx+1<8)){
        if(row_idx>=0 && row_idx<8 && column_idx>=0 && column_idx<8)
            return false;
        else
            return true;
    }
    else if((button_slots[row_idx][column_idx+1]==1 && column_idx+1>=0 && column_idx+1<8)||(button_slots[row_idx][column_idx+1]==2&& column_idx+1>=0 && column_idx+1<8)){
        if(row_idx>=0 && row_idx<8 && column_idx>=0 && column_idx<8)
            return false;
        else
            return true;
    }
    else if((button_slots[row_idx][column_idx-1]==1 && column_idx-1>=0 && column_idx-1<8)||(button_slots[row_idx][column_idx-1]==2 && column_idx-1>=0 && column_idx-1<8)){
        if(row_idx>=0 && row_idx<8 && column_idx>=0 && column_idx<8)
            return false;
        else
            return true;
    }
    else if((button_slots[row_idx-1][column_idx-1]==1 && row_idx-1>=0 && row_idx-1<8 && column_idx-1>=0 && column_idx-1<8)||(button_slots[row_idx-1][column_idx-1]==2 && row_idx-1>=0 && row_idx-1<8 && column_idx-1>=0 && column_idx-1<8)){ //diagonal check
        if(row_idx>=0 && row_idx<8 && column_idx>=0 && column_idx<8)
            return false;
        else
            return true;
    }

    else if((button_slots[row_idx-1][column_idx+1]==1 && row_idx-1>=0 && row_idx-1<8 && column_idx+1>=0 && column_idx+1<8)||(button_slots[row_idx-1][column_idx+1]==2 && row_idx-1>=0 && row_idx-1<8 && column_idx+1>=0 && column_idx+1<8)){
        if(row_idx>=0 && row_idx<8 && column_idx>=0 && column_idx<8)
            return false;
        else
            return true;
    }
    else if((button_slots[row_idx+1][column_idx-1]==1 && row_idx+1>=0 && row_idx+1<8 && column_idx-1>=0 && column_idx-1<8)||(button_slots[row_idx+1][column_idx-1]==2 && row_idx+1>=0 && row_idx+1<8 && column_idx-1>=0 && column_idx-1<8)){
        if(row_idx>=0 && row_idx<8 && column_idx>=0 && column_idx<8)
            return false;
        else
            return true;
    }
    else if((button_slots[row_idx+1][column_idx+1]==1 && row_idx+1>=0 && row_idx+1<8 && column_idx+1>=0 && column_idx+1<8)||(button_slots[row_idx+1][column_idx+1]==2 && row_idx+1>=0 && row_idx+1<8 && column_idx+1>=0 && column_idx+1<8)){
        if(row_idx>=0 && row_idx<8 && column_idx>=0 && column_idx<8)
            return false;
        else
            return true;
    }
    else
        return true; //true means cannot place there
}

void MainWindow::button_slots_pressed(){ //event when board is clicked

    QPushButton *button = (QPushButton *)sender();

    QString button_name=button->objectName();

    //CHANGE IMAGE DIRECTORY HERE!
    QPixmap player1_icon("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/player1_icon"); //create button icons
    QPixmap player2_icon("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/player2_icon");
    QIcon icon1(player1_icon);
    QIcon icon2(player2_icon);

    int row_idx; //find rows and columns of button
    int column_idx;

    QString just_row_idx=button_name.mid(6, 1); //take just numbers of the value
    QString just_column_idx=button_name.mid(7, 1);

    row_idx=just_row_idx.toInt(); //convert values to int to find row and col index
    column_idx=just_column_idx.toInt();


    QPixmap warning1, good; //set warning / good signs
    warning1.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/error_message.png"); //CHANGE IMAGE DIRECTORY HERE!
    good.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/good_message.png"); //CHANGE IMAGE DIRECTORY HERE!



    //==================================================================================================
    //NON-AI MODE
    //==================================================================================================



    if(check_in_slot(row_idx, column_idx)){ //check whether button has been placed with something already
        //set error message when trying to place on same spot

        ui->warning_1->setPixmap(warning1);
        ui->warning_1->show();

    }
    else if(check_adjacency(row_idx, column_idx)){ //check if piece is adjacent to it
        //if no adjacency, set error message
        //check if can put down piece

        ui->warning_1->setPixmap(warning1);
        ui->warning_1->show();
    }

    else{

        ui->warning_1->setPixmap(good); //show good sign when placed correctly
        ui->warning_1->show();



        if(turn%2==0){ //for when player 1 turn
            button->setIcon(icon1);
            button_slots[row_idx][column_idx]=1; //send value of icon

            if(ai_mode==false){ //change player status for non ai mode
                QPixmap player2; //changing signs of player status
                player2.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/player2_status.png"); //CHANGE IMAGE DIRECTORY HERE!

                ui->player_status->setPixmap(player2);
                ui->player_status->show();
            }



        }
        else{ //when player 2 turn
            button->setIcon(icon2);
            button_slots[row_idx][column_idx]=2; //send value of icon

            if(ai_mode==false){ //change player status for non ai mode
                QPixmap player1; //changing signs of player status
                player1.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/player1_status.png"); //CHANGE IMAGE DIRECTORY HERE!

                ui->player_status->setPixmap(player1);
                ui->player_status->show();
            }



        }


        update_board(row_idx, column_idx); //put down or reverse values depending on button just clicked
        update_board_score(); //update scores
        turn+=1; //changing turns



        if(ai_mode) //ai's turn
            //QThread::sleep(1); //put pause before ending
            ai_turn();

    }


    if(limit_mode==true){ //for when limit mode is played
        if(starting_player==1){ //if starting player is 1
            if(turn==10) //end after 10 turns
                end_the_game();

        }

        else{ //if starting player is 2
            if(turn==11)
                end_the_game();
        }
    }


}


//==================================================================================================
//AI MODE
//==================================================================================================




void MainWindow::ai_turn() //action when ai is playing
{



    int trial_row=0;
    int trial_column=0;
    int trial_value=0;

    int final_row=0;
    int final_column=0;
    int final_value=0;
    int value=3;

    if(hard_mode) //checks more value to find optimal spot
        value=10;



    //CHANGE IMAGE DIRECTORY HERE!
    QPixmap player1_icon("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/player1_icon"); //create button icons
    QPixmap player2_icon("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/player2_icon");
    QIcon icon1(player1_icon);
    QIcon icon2(player2_icon);


    for(int i=0; i<value; ++i) //checks 5 different spots to find optimal spot
    {
        while(true) //continue to find place where piece can be placed
        {
            trial_row=rand()%8;
            trial_column=rand()%8;
            if(check_in_slot(trial_row, trial_column)==false && check_adjacency(trial_row, trial_column)==false)
            { //can put down piece
                break;
            }
        }

        trial_value=check_value(trial_row, trial_column); //checks what if value

        if(trial_value>=final_value){ //finds best value of the 5 tries
            final_value=trial_value;
            final_row=trial_row;
            final_column=trial_column;
        }


    }


    QString butName="button"+QString::number(final_row)+QString::number(final_column);
    if(computer_player==1){
        MainWindow::findChild<QPushButton *>(butName)->setIcon(icon1);
    }else{
        MainWindow::findChild<QPushButton *>(butName)->setIcon(icon2);
    }

    button_slots[final_row][final_column]=computer_player; //change to final later

    update_board(final_row, final_column); //change to final later
    update_board_score();

    turn+=1;

}


int MainWindow::check_value(int trial_row, int trial_column) //predicts value for ai
{

    int current_player=computer_player;
    int opponent_player=0;
    int total_score=0;

    int start_idx=0; //starting and ending indexes
    int end_idx=0;

    int start_row_idx=0;
    int end_row_idx=0;
    int start_col_idx=0;
    int end_col_idx=0;

    if(current_player==1) //finding opponent and current player value
        opponent_player=2;
    else
        opponent_player=1;


    //=================================================================================
    //FOR CROSS PATHS
    //=================================================================================
    if(button_slots[trial_row+1][trial_column]==opponent_player){ //case for when bottom part is true;
        int insert_value=0;
        start_idx=trial_row+1;
        end_idx=start_idx;

        while(button_slots[end_idx][trial_column]==opponent_player){
            insert_value++;
            end_idx+=1; //find when stops
        }


        if(button_slots[end_idx][trial_column]==current_player && end_idx<8 && end_idx>=0){

            total_score=total_score+insert_value; //adding to total score
        }
    }
    //================================================================
    if(button_slots[trial_row-1][trial_column]==opponent_player){ //case for when top part is true
        //works until here
        start_idx=trial_row-1;
        end_idx=start_idx;
        int insert_value=0;
        while(button_slots[end_idx][trial_column]==opponent_player){

            end_idx-=1; //find when stops
            insert_value++;
        }


        if(button_slots[end_idx][trial_column]==current_player && end_idx<8 && end_idx>=0){

            total_score=total_score+insert_value; //adding to total score
        }

    }
    //================================================================
    if(button_slots[trial_row][trial_column+1]==opponent_player){ //case for when right part is true
        //works until here
        start_idx=trial_column+1;
        end_idx=start_idx;
        int insert_value=0;

        while(button_slots[trial_row][end_idx]==opponent_player){

            end_idx+=1; //find when stops
            insert_value++;
        }

        if(button_slots[trial_row][end_idx]==current_player && end_idx<8 && end_idx>=0){

            total_score=total_score+insert_value; //adding to total score
        }

    }
    //================================================================
    if(button_slots[trial_row][trial_column-1]==opponent_player){ //case for when top part is true

        start_idx=trial_column-1;
        end_idx=start_idx;
        int insert_value=0;

        while(button_slots[trial_row][end_idx]==opponent_player){

            end_idx-=1; //find when stops
            insert_value++;
        }
        if(end_idx<8 && end_idx>=0){

        if(button_slots[trial_row][end_idx]==current_player && end_idx<7){

            total_score=total_score+insert_value; //adding to total score
        }
        }
    }

    //=================================================================================
    //FOR DIAGONAL PATHS
    //=================================================================================


    if(button_slots[trial_row][trial_column+1]==opponent_player){ //case for when right bottom part is true

        start_row_idx=trial_row+1;
        start_col_idx=trial_column+1;
        end_row_idx=start_row_idx;
        end_col_idx=start_col_idx;

        int insert_value=0;

        while(button_slots[end_row_idx][end_col_idx]==opponent_player){
            end_row_idx+=1; //find when stops
            end_col_idx+=1;
            insert_value++;
        }

        if(button_slots[end_row_idx][end_col_idx]==current_player && end_row_idx<8 && end_row_idx>=0 && end_col_idx<8 && end_col_idx>=0){
            total_score=total_score+insert_value; //adding to total score
        }
    }

    //================================================================
    if(button_slots[trial_row-1][trial_row+1]==opponent_player){ //case for when right top part is true

        start_row_idx=trial_row-1;
        start_col_idx=trial_column+1;
        end_row_idx=start_row_idx;
        end_col_idx=start_col_idx;

        int insert_value=0;

        while(button_slots[end_row_idx][end_col_idx]==opponent_player){
            end_row_idx-=1; //find when stops
            end_col_idx+=1;
            insert_value++;
        }

        if(button_slots[end_row_idx][end_col_idx]==current_player && end_row_idx<8 && end_row_idx>=0 && end_col_idx<8 && end_col_idx>=0){

            total_score=total_score+insert_value; //adding to total score
        }
    }

    //================================================================
    if(button_slots[trial_row-1][trial_column-1]==opponent_player){ //case for when left top part is true

        start_row_idx=trial_row-1;
        start_col_idx=trial_column-1;
        end_row_idx=start_row_idx;
        end_col_idx=start_col_idx;

        int insert_value=0;

        while(button_slots[end_row_idx][end_col_idx]==opponent_player){
            end_row_idx-=1; //find when stops
            end_col_idx-=1;
            insert_value++;
        }

        if(button_slots[end_row_idx][end_col_idx]==current_player && end_row_idx<8 && end_row_idx>=0 && end_col_idx<8 && end_col_idx>=0){
            total_score=total_score+insert_value; //adding to total score
        }
    }


    //================================================================
    if(button_slots[trial_row+1][trial_column-1]==opponent_player){ //case for when left bottom part is true

        start_row_idx=trial_row+1;
        start_col_idx=trial_column-1;
        end_row_idx=start_row_idx;
        end_col_idx=start_col_idx;

        int insert_value=0;

        while(button_slots[end_row_idx][end_col_idx]==opponent_player){
            end_row_idx+=1; //find when stops
            end_col_idx-=1;
            insert_value++;
        }

        if(button_slots[end_row_idx][end_col_idx]==current_player && end_row_idx<8 && end_row_idx>=0 && end_col_idx<8 && end_col_idx>=0){

            total_score=total_score+insert_value; //adding to total score
        }
    }

    return total_score;

}



void MainWindow::on_rules_button_clicked() //show rules page
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_return_button_clicked() //return to starting page
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::initialize_board() //initialize the board
{

    for (int i = 0; i < 8; i++) //set all board values to 0
      for (int j = 0; j < 8; j++)
        button_slots[i][j] = 0;

    button_slots[3][3]=1; //initialize the middle part of the board
    button_slots[3][4]=2;
    button_slots[4][3]=2;
    button_slots[4][4]=1;

    update_board_icons(); //update the board and score
    update_board_score();

}

void MainWindow::on_choose_player1_clicked() //when player 1 is chosen to go first
{
    if(ai_mode){
        computer_player=2;
    }
    starting_player=1;
    ui->stackedWidget->setCurrentIndex(3);
    QPixmap player1;
    player1.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/player1_status.png"); //CHANGE IMAGE DIRECTORY HERE!

    ui->player_status->setPixmap(player1);
    ui->player_status->show();

    initialize_board();


}

void MainWindow::on_choose_player2_clicked() //when player 2 is chosen to go first
{
    if(ai_mode){
        computer_player=1;
    }
    starting_player=2;
    ui->stackedWidget->setCurrentIndex(3);
    QPixmap player2;
    player2.load("C:/HGU/Grade4_Sem2/Programming2/reversiiii/tosize_2/player2_status.png"); //CHANGE IMAGE DIRECTORY HERE!

    ui->player_status->setPixmap(player2);
    ui->player_status->show();
    turn+=1;

    initialize_board();

}


void MainWindow::on_ending_button_clicked() //end the game no matter what when end button clicked
{
    end_the_game();
}


void MainWindow::on_normal_mode_clicked() //start normal mode
{
    ai_mode=false;
    hard_mode=false;
    limit_mode=false;

    ui->stackedWidget->setCurrentIndex(2);
    ui->mode_name->setText("Normal Mode");
}


void MainWindow::on_limited_mode_clicked() //start limited turn mode
{
    ai_mode=false;
    hard_mode=false;
    limit_mode=true;

    ui->stackedWidget->setCurrentIndex(2);
    ui->mode_name->setText("10 Moves");

}


void MainWindow::on_ai_versus_clicked() //easy ai mode
{
    limit_mode=false;
    ai_mode=true;
    hard_mode=false;

    ui->stackedWidget->setCurrentIndex(2);
    ui->mode_name->setText("Easy Mode");

}


void MainWindow::on_hard_mode_clicked() //hard ai mode
{
    limit_mode=false;
    ai_mode=true;
    hard_mode=true;

    ui->stackedWidget->setCurrentIndex(2);
    ui->mode_name->setText("Hard Mode");

}


void MainWindow::on_retry_button_clicked() //redo the game
{
    ui->stackedWidget->setCurrentIndex(0);

}


void MainWindow::on_quit_button_clicked() //terminate application
{
    QApplication::quit();
}


