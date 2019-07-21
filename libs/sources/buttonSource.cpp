#include "../button.h"

void Button::create( const int posX , const int posY , const int sizeX , const int sizeY ) {

    Button::shape.setPosition( sf::Vector2f( posX , posY ) );
    Button::shape.setSize( sf::Vector2f( sizeX , sizeY ) );

}

void Button::update( bool hover ) {

    if( hover )
        Button::shape.setFillColor( sf::Color::Red );
    else
        Button::shape.setFillColor( sf::Color(168,168,57) );
}

void Button::draw( sf::RenderTarget& target , sf::RenderStates states ) const {

    target.draw(Button::shape , states);
}
