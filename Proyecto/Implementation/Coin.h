#ifndef Coin__H
#define Coin__H

class Coin
{

private:
double m_AreaMAX;
double m_AreaMIN;
double m_Value;

public:
Coin();
Coin(double m_AreaMAX,double m_AreaMIN, double m_Value);
virtual ~Coin();
double getAreaMAX();
double getAreaMIN();
double getValue();
bool isTheCoin( double m_AreaImage);
	
};

#endif
