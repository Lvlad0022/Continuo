#include <iostream>
#include <queue>

using namespace std;

const int x_[]={0,0,1,-1}, y_[]={1,-1,0,0};
int mapa[5][5]{
    {1,1,2,3,1},
    {1,5,5,3,2},
    {1,1,5,5,5},
    {2,3,5,5,5},
    {7,7,7,7,7}
};


int dfs(int x,int y,int (&priv)[5][5], int k){
    int izlaz = 0;
    queue<pair<int,int>> red;
    red.push(make_pair(x,y));
    while(!red.empty()){
        int a = red.front().first, b = red.front().second; red.pop();
        cout << a << ' ' << b << ' ' << mapa[b][a] << ' '<< k<< endl;
        if(mapa[b][a] == k && priv[b][a] == 0){
            izlaz++;
            priv[b][a] =1;
            for(int i=0; i<4; i++){
                if(0<=a+x_[i] && a+x_[i] < 5 && 0<=b+y_[i] && b+y_[i]<5)
                    red.push(make_pair(a+x_[i],b+y_[i]));
            }
        }
    }
    return izlaz;
}

int main(void){
    int priv[5][5]= {{0}};
    cout << dfs(1,1, priv, mapa[1][1])<< ' ' <<dfs(1,1, priv, mapa[1][1]) <<  endl;

}



#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <queue>
using namespace std;


const int x_[]={0,0,1,-1}, y_[]={1,-1,0,0}; //ovo je da dfs izgleda ljepse
int view_x_pos = 32*250, view_y_pos= 32*250, view_x_size = 32*30, view_y_size = 32*30; // ovo je za upravljanje viewom
int mapa[500][500]= {{0}};
int pomak_x = 250;
int pomak_y = 250;


int dfs(int x,int y,int (&priv)[500][500], int k){
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

class Tile : public sf::Drawable, public sf::Transformable
{
public:
    string tileset;
    sf::Vector2u tileSize;
    int*tiles;
    unsigned int width = 4;
    unsigned int height = 4;

    

    Tile(const std::string& name, sf::Vector2u tileSize, const int* n_tiles){
        tileset = name;
        Tile::tileSize = tileSize;
        tiles = (int*)malloc(sizeof(int)*width*height);
        for(int i=0; i<width*height; i++)
            tiles[i] = n_tiles[i];
    }

    bool in(){
        sf::Vector2f poz = this->getPosition();
        int x = poz.x; /// provjera pasu li granice
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

    bool load()
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }
    
    bool rotate(){
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

    

    int postavi( int x,int y){
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

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<Tile*> plocice;

void generiraj(){

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
            view_x_pos+=32*4;
            this->move(128.f,0.f);
        }
        void L(){
            view_x_pos-=32*4;
            this->move(-128.f,0.f);
        }
        void U(){
            view_y_pos-=32*4;
            this->move(0.f,-128.f);
        }
        void D(){
            view_y_pos+=32*4;
            this->move(0.f,128.f);
        }
        void zoom_in(){
            int privx = view_x_size, privy = view_y_size;
            view_x_size -=32*4;
            view_y_size -=32*4;
            this->setSize(sf::Vector2f(view_x_size, view_y_size));
        }
        void zoom_out(){
            float privx = view_x_size, privy = view_y_size;
            view_y_size +=32*4;
            view_x_size +=32*4;
            this->setSize(sf::Vector2f(view_x_size, view_y_size));
            
        }// treba dodati da se zapravo i view uveca kao sto smo i za pomak


};



////////////////////////////////main //////////////////////////////////
int main()
{ string s= "";

    sf::Font font;
    sf::Text text;
    text.setFont(font);
    string ispis;

    font.loadFromFile("font.ttf");


    vector<Tile*> postavljene;

    
    int a= 0;
    generiraj();
    plocice[32]->setPosition(sf::Vector2f(view_x_pos - view_x_size/2 ,view_y_pos - view_y_size/2));  // zasto tu dijelimo s dva
    Tile * next= plocice[a];

    const int windowHeight = 1500, windowWidth = 1500, gridSize = 32;
    
    sf::Color lineColor(255, 255, 255);

    // create the window
    sf::RenderWindow window(sf::VideoMode(windowHeight, windowWidth), "Tilemap");
    
    Screen view; // glavni view
    view.setCenter(8000.f,8000.f);
    view.setSize(sf::Vector2f(view_x_size, view_y_size));


    sf::View view2; // view za scoreboard i sledeci tile
    view2.setCenter(-1000.f,-1000.f);
    view2.setSize(500.f,500.f);
    view2.setViewport(sf::FloatRect(0.5f,0.f,0.5f,1.f));

    

    // run the main loop
    while (window.isOpen())
    {
        window.setView(view);
        // handle events
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
                        int x=event.mouseButton.x, y =event.mouseButton.y;
                        sf::Vector2f world_pos = window.mapPixelToCoords(sf::Vector2i(x,y)); // prebacujem iz koordinata windowa u koordinate screena
                        x = world_pos.x; y= world_pos.y;
                        x/=32;x*=32;y/=32;y*=32;

                        next->setPosition(sf::Vector2f(x,y));
                        postavljene.push_back(next);
                        int e = next->postavi(x/32,y/32); // to postavlja u internu matricu koja prati pozicije svih plocica 
                                        // te racuna koliko bodova dobije igrac

                        next = plocice[++a];
                        s= to_string(x/32)+" "+to_string(y/32);
                        
                        
                        text.setPosition(sf::Vector2f(view2.getCenter().x-view2.getSize().x/2,view2.getCenter().y));
                        
                        text.setString(to_string(e));
                    }


            }
        }
        // draw the map
        window.clear();
        
        window.setView(view);
        
        window.draw(*plocice[32]);
        
        for(int i=0; i<postavljene.size(); i++){
            if(postavljene[i]->in())
                window.draw(*postavljene[i]);
        }

        for (int x = view_x_pos-view_x_size/2; x <= view_x_pos + view_x_size/2; x += gridSize) {
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

        text.setFillColor(sf::Color::Blue );
        //window.draw(text);

        ////////////////////scoreboard
        window.setView(view2);
        sf::RectangleShape rect(sf::Vector2f(view2.getSize().x, view2.getSize().y));
        rect.setFillColor(sf::Color(128,128,128));
        rect.setPosition(view2.getCenter().x - view2.getSize().x / 2, view2.getCenter().y - view2.getSize().y / 2);
        window.draw(rect);
        // tu treba dodati if postoji sljedeci ugl treba neki end screen
        
      

        // crtanje granica za scoreboard
   
        //////
        
        text.setPosition(sf::Vector2f(view2.getCenter().x-view2.getSize().x/2,view2.getCenter().y-view2.getSize().y/2));
        for(int i=0; i<30; i++){
            for(int j=0; j<30;j++){
                s = s+to_string(mapa[235+i][235+j])+" ";
            }
            s+="\n";
        }
        
        text.setString(s);
        window.draw(text);
        
        window.display();
    }

    return 0;
}