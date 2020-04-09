#include "Coin.h"
#include <iostream>
Coin::Coin(){
	this->m_AreaMAX =0;
	this->m_AreaMIN =0;
    this->m_Value=0;
}
Coin::~Coin(){
}

Coin::Coin(double m_AreaMAX,double m_AreaMIN, double m_Value)
{
	
	this->m_AreaMAX = m_AreaMAX;
	this->m_AreaMIN =m_AreaMIN;
    this->m_Value= m_Value;
}


double Coin::getAreaMAX(){
	return this->m_AreaMAX;
}
double Coin::getAreaMIN(){
	return this->m_AreaMIN;
}
double Coin::getValue(){
	return this->m_Value;
}

bool Coin::isTheCoin( double areaImage){
	std::cout<<" Area "<<areaImage<<" max "<<this->m_AreaMAX <<"min "<<this->m_AreaMIN<<std::endl;
	if( areaImage <this->m_AreaMAX && areaImage>=this->m_AreaMIN)
        return true;
    else
        return false;
}



