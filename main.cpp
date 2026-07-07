#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <xlnt/xlnt.hpp>
/*
int main()
{
    xlnt::workbook wb;
    xlnt::worksheet ws = wb.active_sheet();
    ws.cell("A1").value(5);
    ws.cell(1, 2).value(123.45); // A2
    ws.cell("B2").value("string data");
    ws.cell("C3").formula("=RAND()");
    ws.merge_cells("C3:C4");
    ws.freeze_panes("B2");
    wb.save("example.xlsx");
    return 0;
}
*/

#define window_heith 600
#define window_width 800


using namespace std;
using namespace xlnt;
using namespace sf;
typedef struct PROMETHEE_data{
    vector <string>nams;
    vector <double> scors;
};

vector <vector<double>> metrax_opperation(vector <vector<double>> data_ , double fx(double x));
double oper(double x);

xlnt::workbook save_tabel( int column_start , int line_start ,
                         vector <string> paramatrs , vector <string> objects,
                         vector <vector<double>> data);
PROMETHEE_data PROMETHEE_algo(vector <vector<double>> data , vector<string>objects, vector <char> condistions , vector <double> poids);


int main() {
    cout<<"\n*** Ayadi ©  PROMETHEE program ***\n"
        <<"conditions : \n" 
        <<"\t-The table must start in the cell A1.\n"
        <<"\t-There should be no empty cells in the table.\n"
        <<"\t-Table information must be numbers except for the baselines.\n"
        <<"have fun\n\n"
        <<"load file : ";
    
    string file;
    cin>>file;

    vector <string> line;
    vector <string> row;
    vector <vector<double>>data;
   
    int line_n = 1;
    int row_n  = 2;

    workbook wb;
    wb.load(file);




    worksheet ws = wb.active_sheet();


    for(line_n ; ws.cell(line_n,1).has_value() ; line_n++){
        line.push_back( ws.cell(line_n,1).to_string());
    }
    for(row_n  ; ws.cell(1,row_n).has_value() ; row_n++){
        row.push_back( ws.cell(1,row_n).to_string());
    }
    /* read table data*/
    for(int i = 2 ; i < row_n ; i++){
        vector <double> colon;
        for(int ii = 2 ; ii < line_n ; ii++){
           // cout<<ii<<"\t"<<i<<"\t"<<stod(ws.cell(ii,i).to_string())<<"\n";
            colon.push_back(stod(ws.cell(ii,i).to_string()));
        }
        data.push_back(colon); 
    } 
    /***end***/

    /*chow table*/
    cout<<"\n\n***********THE TABLE*************\n\n";
    for(int i = 0 ; i < line_n-1; i++){
        cout<<line[i]<<"\t";
    }
    cout<<"\n";
    for(int i = 0 ; i < row_n-2; i++){
        cout<<row[i]<<"\t";
        for(int ii=0 ;ii< line_n-2 ; ii++){
            cout<<data[i][ii]<<"\t";
        }
        cout<<"\n";
    }
    cout<<"\n\n*********************************\n\n";
    // تعديل بعض القيم
    //ws.cell("A1").value("Nom");
    //ws.cell("B1").value("Valeur modifiée");
    //cell ss(ws["A5"]);
    
    /*end chow table*/
    vector <string> goals;
    vector <vector<char>>   goals_par;
    vector <vector<double>> poids;

    data=metrax_opperation(data,oper);

    cout<<"\nwill , thay are "<<line_n-2<<" parametar .\n";
    cout<<"How many goals are desired? : ";
    int goals_n = 0;
    cin>>goals_n;
    cout<<"\n********name it*******\n";
    for(int i = 0 ; i < goals_n ; i++){
        cout<<"goul #"<<i+1<<" : ";
        string name;
        cin>>name;
        goals.push_back(name);
    }
    cout<<"\n**********************\n";
    cout<<"conditions2 - you have \n"
        <<"\t - min.\n"
        <<"\t - max.\n"
        <<"\t - no (don't care).\n";
    for(int i = 0 ; i < goals_n ; i++){
        cout<<"*****************"<<goals[i]<<"*****************\n\n";
        vector <char> input;
        for(int ii = 0 ; ii < line_n-2 ; ii++){
            cout<<line[ii+1]<<" : ";
            int min_max;
            string couse;
            cin>>couse;
            while (couse!="min" && couse!="max" && 
                   couse!="no" && couse!="don't care" && 
                   couse!="0" && couse!="1" && couse!="2")
            {
                cout<<"\nERR: this value is not in cnoditions !\n";
                cout<<line[ii+1]<<" : ";
                cin>>couse;
            }
            if(couse == "min" || couse == "0"){
                min_max=0;
            }
            else if(couse == "max" || couse == "1"){
                min_max=1;
            }
            else if(couse == "no" || couse == "2" || couse == "don't care"){
                min_max=2;
            }
            input.push_back((char)min_max);
        }
        goals_par.push_back(input);
    }
    cout<<"\n\n****** now , pleas enter poids ******\n\n";

    for(int i = 0 ; i < goals_n ; i++){
        cout<<"***************** poids of "<<goals[i]<<" *****************\n\n";
        vector <double> input;
        for(int ii = 0 ; ii < line_n-2 ; ii++){
            cout<<line[ii+1]<<" : ";
            double min_max;
            cin>>min_max;
            input.push_back(min_max);
        }
        poids.push_back(input);
    }


    workbook save_wb;
    worksheet save_ws = save_wb.active_sheet();
    vector <vector<string>> final_save;
    vector <vector<double>> final_score;
    font _font_;/*slect _font_ preferaction*/
    _font_.size(22);
    _font_.underline();
    _font_.bold();
    _font_.name("Arial");
    font _font2_;/*slect _font_ preferaction*/
    _font2_.underline();
    _font2_.bold();
    _font2_.name("Arial");
    _font2_.color(xlnt::color::darkblue());

    border bord;/*slect boearder*/
    bord.side(border_side::bottom);
    
    int start_colon_regester = 4;
    int colon_betwine = 4;
    for(int i = 0 ; i < goals_n ; i++){
        cout<<"\n**********  "<<goals[i]<<"  **********\n";
        PROMETHEE_data Promethee = PROMETHEE_algo(data, row ,goals_par[i],poids[i]);
        final_save.push_back(Promethee.nams);
        final_score.push_back(Promethee.scors);

        save_ws.cell(i*colon_betwine+1,start_colon_regester-3).font(_font_);

        save_ws.cell(i*colon_betwine+1,start_colon_regester-3).value(goals[i]);
        
        save_ws.cell(i*colon_betwine+1,start_colon_regester-1).font(_font2_);
        save_ws.cell(i*colon_betwine+2,start_colon_regester-1).font(_font2_);

        save_ws.cell(i*colon_betwine+1,start_colon_regester-1).value("Object");
        save_ws.cell(i*colon_betwine+2,start_colon_regester-1).value("Score");
        
        for(int ii=0; ii<row_n-2 ; ii++){
            save_ws.cell(i*colon_betwine+1,ii+start_colon_regester).border(bord);
            save_ws.cell(i*colon_betwine+1,ii+start_colon_regester).value(final_save[i][ii]);
            save_ws.cell(i*colon_betwine+2,ii+start_colon_regester).value(final_score[i][ii]);
            cout<<final_save[i][ii]<<"\t"<<final_score[i][ii]<<"\t"<<ii+1<<"\n";

        }
    }
    cout<<"\nchose a name of your result file : ";
    string final_file;
    cin>>final_file;
    final_file.append(".xlsx");
    save_wb.save(final_file);
    RenderWindow window(VideoMode(window_width/*width*/, window_heith/*hight*/), "graph");
    window.setFramerateLimit(60);
    sf::Text text;
    sf::Font font;
    font.loadFromFile("ARIALUNI.TTF");
    text.setFont(font);
    text.setString("hello , this is a text !");
    //text.setScale(20,20);
    text.setColor(Color::White);
    text.move(20,20);
    text.setCharacterSize(20);
    
    vector<Text> txt;
    int size = 10;//window_width/line.size();
    int name_y=0;

    for(int i = 0 ; i < row.size() ; i++){
        
        Text txt2;
        txt2.setFont(font);
        txt2.setString(final_save[0][i]);
        txt2.setColor(Color::White);
	    if(row[i].size()*row.size()*size>window_width){
		    static int ii = 0;
            ii++;
            ii%=(row[i].size()*row.size()*size)/window_width;

            txt2.setPosition(i*(window_width/row.size()),window_heith-(size*ii));
            if((size*ii)>name_y){
                name_y=(size*ii);
            }

	    }else{
            txt2.setPosition(i*(window_width/row.size()),window_heith-size);
        }
        txt2.setCharacterSize(size);

        txt.push_back(txt2);
    }
    //RectangleShape draw(Vector2f(width, hight));
    vector <RectangleShape> lins;
    
    for(int i = 0 ; i < final_score[0].size();i++){
        int lin_width = window_width/(final_score[0].size()*(final_score.size()+1));
        static int row_x = 0;
        int clors[3] = {255,255,255};
        for(int ii = 0 ;  ii<final_score.size() ; ii++){
            RectangleShape lin;
            int lin_heith =(int)abs(final_score[ii][i]);
            cout<<final_score.size();
            //clors[ii%3]=(255/(final_score.size()/3));
            //(Uint32)(((clors[0]&0xff)<<24)|((clors[1]&0xff)<<16)|(((clors[2]&0xff)<<8)|0xff))
            sf::Color cl_((Uint8)clors[0],(Uint8)clors[1],(Uint8)clors[2],0xff);
            row_x+=lin_width;
            lin.setPosition(row_x , window_heith-name_y-lin_heith*10);
            lin.setSize(sf::Vector2f{lin_width,lin_heith*10});
            lin.setFillColor(cl_);
            lins.push_back(lin);
        }
        row_x+=lin_width;
        
    }
    while(window.isOpen()){
        Event event;
        if (window.pollEvent(event)) {/*procec evants(key , mo*/
        	
            if (event.type == Event::Closed){
                window.close();//close the window
            }
        }


        for(int i = 0 ; i < txt.size() ; i++){
            window.draw(txt[i]);
        }
       for(int i = 0 ; i < lins.size() ; i++){
            window.draw(lins[i]);
        }
      
        window.draw(text);
        window.display();
        window.requestFocus();/* no "non requst from this window" , this line make requst , soo , good line */
        sleep(milliseconds(20));
        window.clear(Color(0,0,0,255));
    }


    




    //save_tabel(1,90,line,row,data).save("donnees_modifiees.xlsx");

    //wb.save("donnees_modifiees.xlsx");
    return 0;
}


long P(double a , double b , char max_min){
    if(max_min!=2){
        long i = (a-b)>0.0?1:-1;
        if(max_min==0){
            i*=-1;
        }
        return i;
    }
    return 0;
}
struct first_table{
    vector <string> name;
    vector <double> score;
};

PROMETHEE_data PROMETHEE_algo(vector <vector<double>> data , vector<string>objects, vector <char> condistions , vector <double> poids){
    cout<<"PROMETHEE calculing ....\n\n";

    
    first_table ftable;
    for(int i = 0 ; i < data.size() ; i++ ){
        double win_colon_n=0.0;
        double lost_colon_n=0.0;
        for(int ii = 0 ; ii < objects.size() ; ii++ ){
            if(i==ii)continue;
            double coulun_w_score=0.0;
            double coulun_l_score=0.0;
            for(int iii = 0 ; iii < data[ii].size() ; iii++ ){
                double result =(double)P(data[i][iii],data[ii][iii],condistions[iii]);
                if(result>0.0){
                    coulun_w_score+=(double)(result*poids[iii]);
                }else{
                    coulun_l_score+=(double)(result*poids[iii]*-1.0);
                }
            }
            /*if(coulun_score>0){
                win_colon_n++;
            }
            if(coulun_score<0){
                lost_colon_n++;
            }*/
           win_colon_n+=coulun_w_score;
           lost_colon_n+=coulun_l_score;
        }
        double noote = win_colon_n-lost_colon_n;
        ftable.name.push_back(objects[i]);
        ftable.score.push_back(noote);

    }
    /*for(int i = 0 ; i<ftable.name.size() ; i++){
       cout<<ftable.name[i]<<"\t"<<ftable.score[i]<<"\n";
    }
    double big_pois = 0.0;
    for(int i = 0 ; i < poids.size( ) ; i++){
        if(poids[i]>big_pois){
            big_pois=poids[i];
        }
    }*/
    
    first_table midtable;
    double last_number =(double)(0xffffffff);//(double)data.size()*condistions.size()*big_pois;

    for(int i = 0 ; i<data.size() ; i ++){
        vector<string> nams;
        string name ="N/D";
        double number=-((double)0xffffffff);//(-data.size()*data[i].size()*big_pois);
        
        for(int ii = 0 ; ii<data.size() ; ii ++){
            if(ftable.score[ii]>number && ftable.score[ii]<last_number){
                name=ftable.name[ii];
                number=ftable.score[ii];
            }
        }
        midtable.name.push_back(name);
        midtable.score.push_back(number);
        for(int ii = 0 ; ii<data.size() ; ii ++){
            if(ftable.score[ii]==number && ftable.score[ii]<last_number && ftable.name[ii]!=name){
                midtable.name.push_back(ftable.name[ii]);
                midtable.score.push_back(number);
            }
        }
        last_number=number;

    }
    PROMETHEE_data ltable;
    /*for(int i = 0 ; i<data.size() ; i ++){
        ltable.nams.push_back("      ");
        ltable.scors.push_back(0.0);
    }*/
    for(int i = 0 ; i<data.size() ; i ++){
        ltable.nams.push_back(midtable.name[i]);
        ltable.scors.push_back(midtable.score[i]);
        //ltable.scors[i]=midtable.score[i];
    }
    cout<<"Calculing end .\n\n";
    return ltable;
}


vector <vector<double>> metrax_opperation(vector <vector<double>> data_ , double fx(double x)){
    for(int i = 0 ; i < data_.size() ; i++){
        for(int ii = 0 ; ii < data_[i].size() ; ii++){
            data_[i][ii]=fx(data_[i][ii]);
        }
    }
    return data_;
}



double oper(double x){
    return x*1;
}

xlnt::workbook save_tabel( int column_start , int line_start ,
                         vector <string> paramatrs , vector <string> objects,
                         vector <vector<double>> data)
{
    xlnt::workbook  wb;
    xlnt::worksheet ws = wb.active_sheet();
    for(int i = 0 ; i < paramatrs.size() ; i++){
        ws.cell(column_start+i,line_start).value(paramatrs[i]);
    }
    for(int i = 0 ; i < objects.size() ; i++){
        ws.cell(column_start,line_start+i+1).value(objects[i]);
    }
    for(int i = 0 ; i <data.size();i++){
        for(int ii = 0 ; ii <data[i].size();ii++){
            ws.cell(column_start+ii+1,line_start+i+1).value(data[i][ii]);
        }   
    }
    return wb;

}
// compile with -std=c++14 -Ixlnt/include -lxlnt
