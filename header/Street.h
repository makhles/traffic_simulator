#ifndef STREET_H
#define STREET_H

//=====================
// Forward declarations

//=====================
// Includes
#include <string>
#include "FilaEnc.hpp"
#include "Vehicle.h"

class Street : private FilaEnc<Vehicle*>
{
 public:
    Street(std::string name, int period, int p_var,
		   int speed, int length, bool drain);

    ~Street();
    void addVehicle(Vehicle* vehicle);
	void removeVehicle();
	Vehicle* peek();

	// Getters & setters
    std::string getName();
    int getPeriod();
    int getPeriodVar();
    int getSpeed();
    int getLength();
	int getFreeSpace();
    bool isDrain();
	bool isGreenLight();
	void switchGreenLight();
	Street* getDestinationStreet(int& pos);
    void setEfferents(Street* eff_prob[10]);
	bool isEmpty();
	int getInflow() const;
	int getOutflow() const;


 private:
    std::string m_name;      //< The name of the street.
    int m_period;            //< Period at which cars are created.
    int m_periodVar;         //< Period variance.
    int m_speed;             //< The constant speed in km/h
    int m_length;            //< The length in meters.
	int m_freeSpace;         //< The available space in meters.
    bool m_drain;            //< Is it a drain for vehicles?
	bool m_greenLight;       //< Is the traffic light green?
	Street* m_effProb[10];   //< List of pointers to efferents based on probability.
	int m_inflow;            //< Number of vehicles that entered this street.
	int m_outflow;           //< Number of vehicles that left this street.
};
#endif // STREET_H
