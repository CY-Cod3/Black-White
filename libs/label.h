#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED

#include <SFML/Graphics.hpp>

class Label : public sf::Drawable {

    public:

        void create ( const int posX , const int posY , const unsigned int Csize , const std::string& title , const sf::Font& font );  //Create a new label


    private:

        virtual void draw( sf::RenderTarget& target , sf::RenderStates states ) const;  //Overload of the draw function implemented in Drawable

        sf::Text text;  //This is what the window will draw
};

#endif // LABEL_H_INCLUDED
