
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <queue>
#include <random>
using namespace std;


const int x_[]={0,0,1,-1}, y_[]={1,-1,0,0}; //ovo je da dfs izgleda ljepse
int view_x_pos = 32*250, view_y_pos= 32*250, view_x_size = 32*30, view_y_size = 32*30; // ovo je za upravljanje viewom
int mapa[500][500]= {{0}};
int pomak_x = 250;
int pomak_y = 250;

bool sijece(int x, int y){ // ovo provjerava kad se postavlja plocica preklapa li se s nekom drugom i jeli susjedna nekoj drugoj
    for(int i=0; i<4; i++){
        for(int j=0; j<4;j++){
            if(mapa[y+i][x+j])
                return 1;
        }
    }
    if(x <0 || x>496 || y<0 || x>496) return 1;
    for(int i=0; i<4; i++)
        if(mapa[y-1][x+i] >0 || mapa[y+4][x+i] >0 || mapa[y+i][x-1] >0 || mapa[y+i][x+4] > 0) return 0;

    return 1;
}

int dfs(int x,int y,int (&priv)[500][500], int k){ // dfs za bodovanje
    int izlaz = 0;
    queue<pair<int,int>> red;
    red.push(make_pair(x,y));
    while(!red.empty()){
        int a = red.front().first, b = red.front().second; red.pop();
        if(mapa[b][a] == k && priv[b][a] == 0){
            izlaz++;
            priv[b][a] =1;
            for(int i=0; i<4; i++){
                red.push(make_pair(a+x_[i],b+y_[i]));
            }
        }
    }
    return izlaz;
}


class Tile : public sf::Drawable, public sf::Transformable // klasa za plocice
{
public:
    string tileset, string_dobre, string_prozirne;
    sf::Vector2u tileSize;
    int*tiles;
    unsigned int width = 4;
    unsigned int height = 4;

    

    Tile(const std::string& name, sf::Vector2u tileSize, const int* n_tiles){
        string_prozirne = "boje_svijetle.png";
        string_dobre = name;
        tileset = name;
        Tile::tileSize = tileSize;
        tiles = (int*)malloc(sizeof(int)*width*height);
        for(int i=0; i<width*height; i++)
            tiles[i] = n_tiles[i];
    }



    bool in(){
        sf::Vector2f poz = this->getPosition();
        int x = poz.x; /// provjera je li plocica unutar granica viewa
        int y = poz.y;
        if(view_x_pos-view_x_size/2 <= x && view_x_pos+view_x_size/2>= x && view_y_pos-view_y_size/2 <= y && view_y_pos+view_y_size/2 >= y)
            return 1;
        if(view_x_pos-view_x_size/2 <= x+32*4 && view_x_pos+view_x_size/2>= x+32*4 && view_y_pos-view_y_size/2 <= y && view_y_pos+view_y_size/2 >= y)
            return 1;
        if(view_x_pos-view_x_size/2 <= x && view_x_pos+view_x_size/2>= x && view_y_pos-view_y_size/2 <= y+32*4 && view_y_pos+view_y_size/2 >= y+32*4)
            return 1;
        if(view_x_pos-view_x_size/2 <= x+32*4 && view_x_pos+view_x_size/2>= x+32*4 && view_y_pos-view_y_size/2 <= y+32*4 && view_y_pos+view_y_size/2 >= 32*4+y)
            return 1;
        if(view_x_pos-view_x_size/2 >= x && view_x_pos-view_x_size/2<= x+32*4 && view_y_pos-view_y_size/2 >= y && view_y_pos-view_y_size/2 <= y+32*4)
            return 1;
        if(view_x_pos+view_x_size/2 >= x && view_x_pos+view_x_size/2<= x+32*4 && view_y_pos-view_y_size/2 >= y && view_y_pos-view_y_size/2 <= y+32*4)
            return 1;
        if(view_x_pos-view_x_size/2 >= x && view_x_pos-view_x_size/2<= x+32*4 && view_y_pos+view_y_size/2 >= y && view_y_pos+view_y_size/2 <= y+32*4)
            return 1;
        if(view_x_pos+view_x_size/2 >= x && view_x_pos+view_x_size/2<= x+32*4 && view_y_pos+view_y_size/2 >= y && view_y_pos+view_y_size/2 <= y+32*4)
            return 1;

        return 0;
    }

    bool load() // ovo je funkcija iz tutorijala za slicnu upotrebu
    {
        if (!m_tileset.loadFromFile(tileset))
            return false;

        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                int tileNumber = tiles[i + j * width];

                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }
    
    void prozirne(){ //postavlja na set boja koje su za preview plocice 
        tileset = string_prozirne;
        this->load();
    }


    void dobre(){ // postavlja na dobar set boja
        tileset = string_dobre;
        this->load();
    }

    bool rotate(){ // rotira plocicu
        int* priv = (int*)malloc(sizeof(int)*16);
        for(int i = 0; i<width; i++){
            for(int j=0; j< height; j++){
                priv[i*width+j] = tiles[i+width*(height-1-j)];
            }
        }
        free(tiles);
        tiles = priv;
        return load();
    }

    

    int postavi( int x,int y){ // postavlja plocicu u globalno ploje mapa i vraca broj bodova
        int a = x;
        int b= y;
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                mapa[b+i][a+j] = tiles[i+j*4]+1;
            }
        }
        int priv[500][500] = {{0}};
        int rez = 0;
        for(int i = 0; i<4; i++){
            if(mapa[b-1][a+i] == mapa[b][a+i]){
                rez+=dfs(a+i,b,priv, mapa[b][a+i]);
            }
            if(mapa[b+4][a+i] == mapa[b+3][a+i]){
                rez+=dfs(a+i,b+3,priv,mapa[b+3][a+i]);
            }
            if(mapa[b+i][a-1] == mapa[b+i][a]){
                rez+=dfs(a,b+i,priv, mapa[b+i][a]);
            }
            if(mapa[b+i][a+4] == mapa[b+i][a+3]){
                rez+=dfs(a+3,b+i,priv,mapa[b+i][a+3]);
            }
        }
        return rez;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const // ovo je opet iz tutoraiala
    {
        states.transform *= getTransform();

        states.texture = &m_tileset;

        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<Tile*> plocice;

void generiraj(){ // generira set od 42 plocice za igru

    for(int i = 0; i<4; i++){
        for(int j=i+1; j<4; j++){
            const int level[] = {
            i,j,i,j,
            j,j,i,i,
            i,i,j,j,
            j,i,j,i
            };
            Tile* nova = new Tile("boje.png",sf::Vector2u(32, 32),level);
            nova->load();
            plocice.push_back(nova);
        }
    }
    for(int i = 0; i<4; i++){
        for(int j=0; j<4; j++){
            for(int k=j+1; k<4; k++){
            if(j==i || k == i) continue;
            const int level[] = {
            i,j,k,i,
            j,j,k,k,
            k,k,j,j,
            i,k,j,i
            };
            Tile* nova = new Tile("boje.png",sf::Vector2u(32, 32),level);
            nova->load();
            plocice.push_back(nova);
            }
        }
    }
    for(int i=0; i<4; i++){
        for(int j=i+1; j<4; j++){
            for(int k=0; k<4; k++){
                if(k == j || k == i) continue;
                const int level1[] = {
                i,j,k,j,
                j,j,k,k,
                k,k,j,j,
                j,k,j,i
                };
                Tile* nova1 = new Tile("boje.png",sf::Vector2u(32, 32),level1);
                nova1->load();
                plocice.push_back(nova1);

                const int level2[] = {
                i,k,i,j,
                k,k,i,i,
                i,i,k,k,
                j,i,k,i
                };
                Tile* nova2 = new Tile("boje.png",sf::Vector2u(32, 32),level2);
                nova2->load();
                plocice.push_back(nova2);
            }
        }
    }
}

class Screen: public sf::View{
    public:
        void R(){
            if(view_x_pos+32*4+view_x_size/2 > 32*500) return;
            view_x_pos+=32*4;
            this->move(128.f,0.f);
        }
        void L(){
            if(view_x_pos-32*4-view_x_size/2 <0) return;
            view_x_pos-=32*4;
            this->move(-128.f,0.f);
        }
        void U(){
            if(view_y_pos-32*4-view_y_size/2 < 0) return;
            view_y_pos-=32*4;
            this->move(0.f,-128.f);
        }
        void D(){
            if(view_x_pos+32*4+view_x_size/2 > 32*500) return;
            view_y_pos+=32*4;
            this->move(0.f,128.f);
        }
        void zoom_in(){
            if(view_x_size < 32*20) return;
            int privx = view_x_size, privy = view_y_size;
            view_x_size -=32*4;
            view_y_size -=32*4;
            this->setSize(sf::Vector2f(view_x_size, view_y_size));
        }
        void zoom_out(){
            if(view_y_pos+ view_y_size/2 + 32*2 > 32*500 || view_y_pos- 32*2 -view_y_size/2 < 0) return;
            if(view_x_pos + 32*2 + view_x_size/2 > 32*500 || view_x_pos - 32*2 - view_x_size/2 < 0) return;
            float privx = view_x_size, privy = view_y_size;
            view_y_size +=32*4;
            view_x_size +=32*4;
            this->setSize(sf::Vector2f(view_x_size, view_y_size));
            
        }


};

void greedy_algo(int *x, int *y, Tile * next){ // greedy algoritam kako racunalo igra, može proći sve mogućnosti postavljanja plocica jer je polje maleno
    int max= 0;
    int* tiles;
    for(int t=0; t<2;t++){ // for 2 jer mora rotirat plocicu
        next->rotate();
        tiles = next->tiles;
        int prov = 1;
        for(int a=0; a<497; a++){
            for(int b=0; b<497; b++){
                if(sijece(a,b)) continue;
                if(max==0 ) {*x= a,*y =b;}
                for(int i=0; i<4; i++){
                    for(int j=0; j<4; j++){
                        mapa[b+i][a+j] = tiles[i+j*4]+1;
                    }
                }
                int priv[500][500] = {{0}};
                int rez = 0;
                for(int i = 0; i<4; i++){
                    if(mapa[b-1][a+i] == mapa[b][a+i]){
                        rez+=dfs(a+i,b,priv, mapa[b][a+i]);
                    }
                    if(mapa[b+4][a+i] == mapa[b+3][a+i]){
                        rez+=dfs(a+i,b+3,priv,mapa[b+3][a+i]);
                    }
                    if(mapa[b+i][a-1] == mapa[b+i][a]){
                        rez+=dfs(a,b+i,priv, mapa[b+i][a]);
                    }
                    if(mapa[b+i][a+4] == mapa[b+i][a+3]){
                        rez+=dfs(a+3,b+i,priv,mapa[b+i][a+3]);
                    }
                }
                if(rez > max){
                    max= rez;
                    *x=a;
                    *y=b;
                    prov = 0;
                }
                for(int i=0; i<4; i++){
                    for(int j=0; j<4; j++){
                        mapa[b+i][a+j] = 0;
                    }
                }
            }

        } // vraca na proslu rotaciju ako je tamo optimalno
        if(prov && t){
            next->rotate();
        }
    }
}




////////////////////////////////main //////////////////////////////////
int main()
{ 

    sf::Font font;
    font.loadFromFile("font.ttf");
    


    vector<Tile*> postavljene;

    
    int a= 0;
    generiraj();
    { //// randomizira poredak plocica
        
        vector<Tile*> priv;
        while(!plocice.empty()){
            random_device rd;
            mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0,plocice.size()-1);
            int indeks = dis(gen);
            priv.push_back(plocice[indeks]);
            plocice.erase(plocice.begin()+indeks);
        }
        for(int i=0; i<priv.size(); i++)
            plocice.push_back(priv[i]);
    }

    Tile * next= plocice[a]; /// postavlja prvu plocicu i setupira next i a 
    next->setPosition(sf::Vector2f(250*32,250*32));
    next->postavi(250,250);
    postavljene.push_back(next);
    next= plocice[++a];

    const int windowHeight = 1500, windowWidth = 1500, gridSize = 32;
    
    sf::Color lineColor(255, 255, 255);

    // create the window
    sf::RenderWindow window(sf::VideoMode(windowHeight, windowWidth), "Tilemap");
    
    Screen view; // glavni view
    view.setCenter(8000.f,8000.f);
    view.setSize(sf::Vector2f(view_x_size, view_y_size));


    sf::View view2; // view za scoreboard i sledeci tile
    view2.setCenter(-1000.f,-1000.f);
    view2.setSize(213.3333f,256.f);
    view2.setViewport(sf::FloatRect(0.75f,0.f,0.25f,0.3f));

    string komp_string="racunalo", komp_string_rez = "0", igrac_string="igrac", igrac_string_rez ="0" ; /// tekst za scoreboard
    sf::Text komp_tekst, komp_tekst_rez, igrac_tekst, igrac_tekst_rez, sljedeci_tekst, kraj; 
    komp_tekst.setCharacterSize(22); igrac_tekst.setCharacterSize(22); komp_tekst_rez.setCharacterSize(22); igrac_tekst_rez.setCharacterSize(22);
    komp_tekst.setFont(font); komp_tekst_rez.setFont(font); igrac_tekst.setFont(font); igrac_tekst_rez.setFont(font); sljedeci_tekst.setFont(font); 
    //////
    view.L();
    view.zoom_out();

    ////////////////////////// main loop
    while (window.isOpen())
    {
        window.setView(view);
        

        
        sf::Event event;
        
        while (window.pollEvent(event))
        {   
            switch (event.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                
                case sf::Event::KeyPressed:
                    switch(event.key.code){
                        case sf::Keyboard::Left:
                            view.L();
                            break;
                        
                        case sf::Keyboard::Right:
                            view.R();
                            break;

                        case sf::Keyboard::Up:
                            view.U();
                            break;

                        case sf::Keyboard::Down:
                            view.D();
                            break;
                        
                        case sf::Keyboard::Add:
                            view.zoom_in();
                            break;
                        
                        case sf::Keyboard::Subtract:
                            view.zoom_out();
                            break;
                        
                        case sf::Keyboard::Enter:
                            next->rotate();
                            break;
                    }
                    break;
                
                case sf::Event::MouseButtonPressed:
                    if(event.mouseButton.button == sf::Mouse::Left){
                        if (a==42 ) break; /// kraj igre

                        int x=event.mouseButton.x, y =event.mouseButton.y;
                        sf::Vector2f world_pos = window.mapPixelToCoords(sf::Vector2i(x,y)); // prebacujem iz koordinata windowa u koordinate screena
                        x = world_pos.x; y= world_pos.y;
                        x/=32;x*=32;y/=32;y*=32; // ovo namjesti da plocice pasu na grid kao mod 32 sto je velicina idnok kvadrata grida
                        if(sijece(x/32,y/32)) break;

                        next->dobre();
                        next->setPosition(sf::Vector2f(x,y)); ///postavljanje igraca
                        postavljene.push_back(next);
                        igrac_string_rez = to_string( stoi(igrac_string_rez) + next->postavi(x/32,y/32) );
                        
                        next = plocice[++a];
                        if(a==42) break;

                        greedy_algo(&x,&y,next);
                        next->setPosition(sf::Vector2f(x*32,y*32)); // postavljanje kompa
                        postavljene.push_back(next);
                        komp_string_rez = to_string( stoi(komp_string_rez) + next->postavi(x,y) );

                        next = plocice[++a];
                    }
                    break;


            }
        }


        window.clear();
        
        

        window.setView(view);
        
        if(a<42){
            sf::Vector2f mis_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // postavljanje tile previewa
            int x = mis_pos.x;
            int y = mis_pos.y;
            x/=32;x*=32;
            y/=32;y*=32;

        
            next->setPosition(sf::Vector2f(x,y)); 
            next->prozirne();
            window.draw(*next);
        }
        
        for(int i=0; i<postavljene.size(); i++){
            if(postavljene[i]->in())
                window.draw(*postavljene[i]);
        }

        

        for (int x = view_x_pos-view_x_size/2; x <= view_x_pos + view_x_size/2; x += gridSize) {// printa grid
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, view_y_pos-view_y_size/2), lineColor),  
                sf::Vertex(sf::Vector2f(x, view_y_pos+view_y_size/2), lineColor)
            };
            window.draw(line, 2, sf::Lines);
        }

        for (int y = view_y_pos-view_y_size; y <=  view_y_pos + view_y_size/2; y += gridSize) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(view_x_pos-view_x_size/2, y), lineColor),
                sf::Vertex(sf::Vector2f(view_x_pos+view_x_size/2, y), lineColor)
            };
            window.draw(line, 2, sf::Lines);
        }

        
        

        ////////////////////scoreboard
        window.setView(view2);
        sf::RectangleShape rect(sf::Vector2f(view2.getSize().x, view2.getSize().y));
        rect.setFillColor(sf::Color(128,128,128));
        rect.setPosition(view2.getCenter().x - view2.getSize().x / 2, view2.getCenter().y - view2.getSize().y / 2);
        window.draw(rect);

        
        if(a<42){ // crta sljedeci tile
            next->dobre();
            next->setPosition(sf::Vector2f(view2.getCenter().x-64,view2.getCenter().y-10 ));
            window.draw(*next);
        }
        else{ ////end screeen
            kraj.setCharacterSize(30);
            kraj.setFont(font);
            kraj.setPosition(view2.getCenter().x - 90 , view2.getCenter().y - view2.getSize().y / 2+ 130);
            if(stoi(komp_string_rez) > stoi(igrac_string_rez)){
                kraj.setString("POBJEDILO JE:\n  racunalo");
            }
            else{
                kraj.setString("POBJEDIO JE: \n   igrac");
            }
            window.draw(kraj);
        }

        // crtanje granica za scoreboard
        sf::Vertex line1[] = {// printa najgornju liniju 
            sf::Vertex(sf::Vector2f(view2.getCenter().x - view2.getSize().x / 2, view2.getCenter().y - view2.getSize().y / 2+30), lineColor),  
            sf::Vertex(sf::Vector2f(view2.getCenter().x + view2.getSize().x / 2, view2.getCenter().y - view2.getSize().y / 2 +30), lineColor)
        };
        igrac_tekst.setPosition(view2.getCenter().x - view2.getSize().x / 2 +10, view2.getCenter().y - view2.getSize().y / 2+1); // namjestava koordinate imena
        komp_tekst.setPosition(view2.getCenter().x +10, view2.getCenter().y - view2.getSize().y / 2+1);
        window.draw(line1, 2, sf::Lines);
        sf::Vertex line2[] = { //printa sljedecu liniju
            sf::Vertex(sf::Vector2f(view2.getCenter().x - view2.getSize().x / 2, view2.getCenter().y - view2.getSize().y / 2 +70), lineColor), 
            sf::Vertex(sf::Vector2f(view2.getCenter().x + view2.getSize().x / 2, view2.getCenter().y - view2.getSize().y / 2 +70), lineColor)
        };
        igrac_tekst_rez.setPosition(view2.getCenter().x - view2.getSize().x / 2 +10, view2.getCenter().y - view2.getSize().y / 2+37); ////namjestava koordinate rez
        komp_tekst_rez.setPosition(view2.getCenter().x +10, view2.getCenter().y - view2.getSize().y / 2+37);
        
        window.draw(line2, 2, sf::Lines);//printa sljedecu linju
        sf::Vertex line3[] = {
            sf::Vertex(sf::Vector2f(view2.getCenter().x , view2.getCenter().y - view2.getSize().y / 2), lineColor),  
            sf::Vertex(sf::Vector2f(view2.getCenter().x , view2.getCenter().y - view2.getSize().y / 2 +70), lineColor)
        };
        window.draw(line3, 2, sf::Lines);// printa granice
        sf::Vertex line4[] = {
            sf::Vertex(sf::Vector2f(view2.getCenter().x - view2.getSize().x / 2+1, view2.getCenter().y - view2.getSize().y / 2 ), lineColor),  
            sf::Vertex(sf::Vector2f(view2.getCenter().x - view2.getSize().x / 2+1, view2.getCenter().y + view2.getSize().y / 2 ), lineColor)
        };
        window.draw(line4, 2, sf::Lines);
        sf::Vertex line5[] = {//printa granice
            sf::Vertex(sf::Vector2f(view2.getCenter().x - view2.getSize().x / 2, view2.getCenter().y + view2.getSize().y / 2 -1), lineColor),  
            sf::Vertex(sf::Vector2f(view2.getCenter().x + view2.getSize().x / 2, view2.getCenter().y + view2.getSize().y / 2 -1), lineColor)
        };
        window.draw(line5, 2, sf::Lines);

        if(a<42){
            int x  = view2.getCenter().x-64, y= view2.getCenter().y-10;// printa grid na sljedeci tile
            for(int i=0; i<5; i++){
                sf::Vertex line1[] = {
                    sf::Vertex(sf::Vector2f(x+i*32, y), lineColor),  
                    sf::Vertex(sf::Vector2f(x+i*32, y+32*4), lineColor)
                };
                window.draw(line1, 2, sf::Lines);
                sf::Vertex line2[] = {
                    sf::Vertex(sf::Vector2f(x, y+i*32), lineColor),  
                    sf::Vertex(sf::Vector2f(x+4*32, y+i*32), lineColor)
                };
                window.draw(line2, 2, sf::Lines);
            }
        }



        //////
        komp_tekst.setString(komp_string); window.draw(komp_tekst);
        komp_tekst_rez.setString(komp_string_rez);  window.draw(komp_tekst_rez);
        igrac_tekst.setString(igrac_string);   window.draw(igrac_tekst);
        igrac_tekst_rez.setString(igrac_string_rez);    window.draw(igrac_tekst_rez);
        
        window.display();
    }

    return 0;
}